/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "version.h"
#include <QSettings>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>

UpdateChecker::UpdateChecker(QObject * parent, QSettings * settings) : QObject(parent)
{
	set = settings;

	net_manager = new QNetworkAccessManager();
	QUrl url("http://smplayer.sourceforge.net/current_version");

	QNetworkRequest req(url);
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
				set->beginGroup("update_checker");
				QString last_known_version = set->value("last_known_version", stableVersion()).toString();
				set->endGroup();
				if (last_known_version != version) {
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
	set->beginGroup("update_checker");
	set->setValue("last_known_version", v);
	set->endGroup();
}

#include "moc_updatechecker.cpp"

