/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "chromecast.h"
#include "links.h"
#include <QUrl>
#include <QFileInfo>
#include <QDesktopServices>
#include <QNetworkInterface>
#include <QDebug>

Chromecast * Chromecast::instance_obj = 0;

Chromecast * Chromecast::instance() {
	if (instance_obj == 0) {
		instance_obj = new Chromecast();
	}
	return instance_obj;
}

Chromecast::Chromecast() {
}

Chromecast::~Chromecast() {
}

void Chromecast::openStream(const QString & url, const QString & title) {
	QDesktopServices::openUrl(QUrl(URL_CHROMECAST "/?title=" + title.toUtf8().toBase64() +
		"&url=" + url.toUtf8().toBase64()));
}

void Chromecast::openLocal(const QString & file, const QString & title) {
	qDebug() << "Chromecast::openLocal:" << file;

	QFileInfo fi(file);
	QString dir = fi.absolutePath();
	QString filename = fi.fileName();

	qDebug() << "Chromecast::openLocal: dir:" << dir;
	qDebug() << "Chromecast::openLocal: filename:" << filename;

	QString local_address;
	foreach(const QHostAddress &address, QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol) {
			QString s_address = address.toString();
			qDebug() << "Chromecast::openLocal: address:" << s_address;
			if (address != QHostAddress(QHostAddress::LocalHost) && s_address.startsWith("192.") && !s_address.endsWith(".1")) {
				if (local_address.isEmpty()) local_address = s_address;
			}
		}
	}
	qDebug() << "Chromecast::openLocal: chosen address:" << local_address;

	if (!local_address.isEmpty()) {
		QString url = local_address + ":8000/" + filename;
		qDebug() << "Chromecast::openLocal: url:" << url;
		/*
		QDesktopServices::openUrl(QUrl(URL_CHROMECAST "/?title=" + title.toUtf8().toBase64() +
			"&url=" + url.toUtf8().toBase64()));
		*/
	}
}
