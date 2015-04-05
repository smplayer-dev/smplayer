/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include <QDebug>


UpdateChecker::UpdateChecker(QWidget * parent, UpdateCheckerData * data) : QObject(parent)
{
	d = data;

	connect(this, SIGNAL(newVersionFound(const QString &)),
            this, SLOT(reportNewVersionAvailable(const QString &)));

	QDate now = QDate::currentDate();
	//now = now.addDays(27);
	int days = QDateTime(d->last_checked).daysTo(QDateTime(now));

	qDebug("UpdateChecker::UpdateChecker: enabled: %d", d->enabled);
	qDebug("UpdateChecker::UpdateChecker: days_to_check: %d", d->days_to_check);
	qDebug("UpdateChecker::UpdateChecker: days since last check: %d", days);

	if ((!d->enabled) || (days < d->days_to_check)) return;

	net_manager = new QNetworkAccessManager();
	QUrl url("http://updates.smplayer.info/current_version");

	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", "SMPlayer");
	QNetworkReply *reply = net_manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotReply()));
}

UpdateChecker::~UpdateChecker() {
}

void UpdateChecker::gotReply() {
	qDebug("UpdateChecker::gotReply");

	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			//read data from reply
			QRegExp rx_version("^version=(.*)");
			QString version;
			while (reply->canReadLine()) {
				QByteArray line = reply->readLine().trimmed();
				//qDebug("line: %s", line.constData());
				if (rx_version.indexIn(line) != -1) {
					version = rx_version.cap(1);
					//qDebug("version: %s", version.toUtf8().constData());
					break;
				}
			} 
			if (!version.isEmpty()) {
				d->last_checked = QDate::currentDate();
				//qDebug("last known: %s version: %s", d->last_known_version.toUtf8().constData(), version.toUtf8().constData());
				//qDebug("version_with_revision: %s", Version::with_revision().toUtf8().constData());
				if ((d->last_known_version != version) && (formattedVersion(version) > formattedVersion(Version::with_revision()))) {
					qDebug("UpdateChecker::gotReply: new version found: %s", version.toUtf8().constData());
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
		QDesktopServices::openUrl(QUrl("http://smplayer.sourceforge.net/changes.php"));
	}

	saveVersion(new_version);
}

#include "moc_updatechecker.cpp"

