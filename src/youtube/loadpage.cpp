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

#include "loadpage.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

QString LoadPage::default_user_agent;

LoadPage::LoadPage(QNetworkAccessManager * man, QObject* parent)
	: QObject(parent)
{
	manager = man;
}

LoadPage::~LoadPage() {
}

void LoadPage::fetchPage(const QString & url) {
	qDebug() << "LoadPage::fetchPage:" << url;
	qDebug() << "LoadPage::fetchPage: user agent:" << userAgent();

	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", userAgent().toLatin1());
	req.setRawHeader("Accept-Language", "en-us,en;q=0.5");
	reply = manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotResponse()));
}

void LoadPage::gotResponse() {
	qDebug() << "LoadPage::gotResponse";

	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply->error() == QNetworkReply::NoError) {
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		qDebug() << "LoadPage::gotResponse: status:" << status;
		QString r_url;
		switch (status) {
			case 301:
			case 302:
			case 307:
				r_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
				qDebug() << "LoadPage::gotResponse: redirected:" << r_url;
				fetchPage(r_url);
				break;
			default:
				qDebug() << "LoadPage::gotResponse: emit pageLoaded";
				emit pageLoaded(reply->readAll());
		}
	} else {
		qDebug() << "LoadPage::gotResponse: error:" << reply->error() << ":" << reply->errorString();
		emit errorOcurred((int)reply->error(), reply->errorString());
	}
	reply->deleteLater();
}

#include "moc_loadpage.cpp"

