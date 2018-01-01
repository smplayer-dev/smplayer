/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "updatechecker.h"
#include "updatecheckerdata.h"
#include "version.h"
#include "links.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>
#include <QDate>
#include <QDateTime>
#include <QStringList>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTemporaryFile>
#include <QSettings>
#include <QDebug>


UpdateChecker::UpdateChecker(QWidget * parent, UpdateCheckerData * data) : QObject(parent)
	, net_manager(0)
	, d(0)
{
	d = data;

	check_url = URL_VERSION_INFO;
	user_agent = "SMPlayer";

	/*
	It looks like some distro maintainers disable this update checker...
	This is a very very bad idea. Main reason: in order to play Youtube videos
	with SMPlayer a very recent version is required. Distros usually provide
	ancient versions...
	If you remove the possibility to inform the user about new versions they will
	probably use a version with broken support for Youtube like forever.

	If you still want to disable this option, the correct way to do it is by
	removing the UPDATE_CHECKER define in smplayer.pro, and not by removing the
	following code.
	*/

	connect(this, SIGNAL(newVersionFound(const QString &)),
            this, SLOT(reportNewVersionAvailable(const QString &)));

	connect(this, SIGNAL(noNewVersionFound(const QString &)),
            this, SLOT(reportNoNewVersionFound(const QString &)));

	connect(this, SIGNAL(errorOcurred(int, QString)), this, SLOT(reportError(int, QString)));

	net_manager = new QNetworkAccessManager(this);

	QDate now = QDate::currentDate();
	//now = now.addDays(27);
	int days = QDateTime(d->last_checked).daysTo(QDateTime(now));

	qDebug() << "UpdateChecker::UpdateChecker: enabled:" << d->enabled;
	qDebug() << "UpdateChecker::UpdateChecker: days_to_check:" << d->days_to_check;
	qDebug() << "UpdateChecker::UpdateChecker: days since last check:" <<  days;

	if ((!d->enabled) || (days < d->days_to_check)) return;

	QNetworkRequest req(check_url);
	req.setRawHeader("User-Agent", user_agent);
	QNetworkReply *reply = net_manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotReply()));
}

UpdateChecker::~UpdateChecker() {
}

// Force a check, requested by the user
void UpdateChecker::check() {
	qDebug("UpdateChecker::check");

	QNetworkRequest req(check_url);
	req.setRawHeader("User-Agent", user_agent);
	QNetworkReply *reply = net_manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotReplyFromUserRequest()));
}

QString UpdateChecker::parseVersion(const QByteArray & data, const QString & name) {
	QTemporaryFile tf;
	tf.open();
	tf.write(data);
	tf.close();
	QString tfile = tf.fileName();
	qDebug() << "UpdateChecker::parseVersion: tfile:" << tfile;

	#ifdef Q_OS_WIN
	QString grname = "windows";
	#else
	QString grname = "linux";
	#endif
	QSettings set(tfile, QSettings::IniFormat);
	set.beginGroup(grname);
	QString version = set.value(name, "").toString();
	set.endGroup();
	return version;
}

void UpdateChecker::gotReply() {
	qDebug("UpdateChecker::gotReply");

	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QString version = parseVersion(reply->readAll(), "stable");
			if (!version.isEmpty()) {
				d->last_checked = QDate::currentDate();
				qDebug() << "UpdateChecker::gotReply: last known version:" << d->last_known_version << "received version:" << version;
				qDebug() << "UpdateChecker::gotReply: installed version:" << Version::with_revision();
				if ((d->last_known_version != version) && (formattedVersion(version) > formattedVersion(Version::with_revision()))) {
					qDebug() << "UpdateChecker::gotReply: new version found:" << version;
					emit newVersionFound(version);
				}
			}
		} else {
			//get http status code
			int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			qDebug("UpdateChecker::gotReply: status: %d", status);
		}
		reply->deleteLater();
	}
}

void UpdateChecker::gotReplyFromUserRequest() {
	qDebug("UpdateChecker::gotReplyFromUserRequest");

	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QString version = parseVersion(reply->readAll(), "unstable");
			if (!version.isEmpty()) {
				if ((formattedVersion(version) > formattedVersion(Version::with_revision()))) {
					qDebug("UpdateChecker::gotReplyFromUserRequest: new version found: %s", version.toUtf8().constData());
					emit newVersionFound(version);
				} else {
					emit noNewVersionFound(version);
				}
			} else {
				emit errorOcurred(1, tr("Failed to get the latest version number") );
			}
		} else {
			int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			qDebug("UpdateChecker::gotReplyFromUserRequest: status: %d", status);
			emit errorOcurred((int)reply->error(), reply->errorString());
		}
		reply->deleteLater();
	}
}

void UpdateChecker::saveVersion(QString v) {
	d->last_known_version = v;
}

QString UpdateChecker::formattedVersion(const QString & version) {
	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	QStringList l = version.split(".");
	int c = l.count();
	if (c >= 1) n1 = l[0].toInt();
	if (c >= 2) n2 = l[1].toInt();
	if (c >= 3) n3 = l[2].toInt();
	if (c >= 4) n4 = l[3].toInt();

	QString res = QString("%1.%2.%3.%4").arg(n1, 2, 10, QChar('0'))
										.arg(n2, 2, 10, QChar('0'))
										.arg(n3, 2, 10, QChar('0'))
										.arg(n4, 4, 10, QChar('0'));
	//qDebug() << "UpdateChecker::formattedVersion:" << res;
	return res;
}

void UpdateChecker::reportNewVersionAvailable(const QString & new_version) {
	QWidget * p = qobject_cast<QWidget*>(parent());

	QMessageBox::StandardButton button = QMessageBox::information(p, tr("New version available"),
		tr("A new version of SMPlayer is available.") + "<br><br>" +
		tr("Installed version: %1").arg(Version::with_revision()) + "<br>" +
		tr("Available version: %1").arg(new_version) + "<br><br>" +
		tr("Would you like to know more about this new version?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	if (button == QMessageBox::Yes) {
		QDesktopServices::openUrl(QUrl(URL_CHANGES));
	}

	saveVersion(new_version);
}

void UpdateChecker::reportNoNewVersionFound(const QString & version) {
	QWidget * p = qobject_cast<QWidget*>(parent());

	QMessageBox::information(p, tr("Checking for updates"),
		tr("Congratulations, SMPlayer is up to date.") + "<br><br>" +
		tr("Installed version: %1").arg(Version::with_revision()) + "<br>" +
		tr("Available version: %1").arg(version));
}

void UpdateChecker::reportError(int error_number, QString error_str) {
	QWidget * p = qobject_cast<QWidget*>(parent());
	QMessageBox::warning(p, tr("Error"), 
		tr("An error happened while trying to retrieve information about the latest version available.") +
		"<br>" + tr("Error code: %1").arg(error_number) + "<br>" + error_str);
}

#include "moc_updatechecker.cpp"

