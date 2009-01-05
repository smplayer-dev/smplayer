/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "discname.h"
#include <QRegExp>

QString DiscName::join(int title, const QString & device, bool use_dvdnav) {
	DiscData d(use_dvdnav ? "dvdnav" : "dvd", title, device);
	return join(d);
}

QString DiscName::join(const DiscData & d) {
	QString s = d.protocol + "://";
	if (d.title > 0) s += QString::number(d.title);
	if (!d.device.isEmpty()) s+= "/" + d.device;

	return s;
}

DiscData DiscName::split(const QString & dvd_url, bool * ok) {
	qDebug("DiscName::split: dvd_url: '%s'", dvd_url.toUtf8().constData());

	QRegExp rx1("^(dvd|dvdnav)://(\\d+)/(.*)");
	QRegExp rx2("^(dvd|dvdnav)://(\\d+)");
	QRegExp rx3("^(dvd|dvdnav):///(.*)");

	DiscData d;

	bool success = false;

	if (rx1.indexIn(dvd_url) != -1) {
		d.protocol = rx1.cap(1);
		d.title = rx1.cap(2).toInt();
		d.device = rx1.cap(3);
		success = true;
	} 
	else
	if (rx2.indexIn(dvd_url) != -1) {
		d.protocol = rx2.cap(1);
		d.title = rx2.cap(2).toInt();
		d.device = "";
		success = true;
	} 
	else
	if (rx3.indexIn(dvd_url) != -1) {
		d.protocol = rx3.cap(1);
		d.title = 0;
		d.device = rx3.cap(2);
		success = true;
	} 

	if (success) {
		qDebug("DiscName::split: protocol: '%s'", d.protocol.toUtf8().constData());
		qDebug("DiscName::split: title: '%d'", d.title);
		qDebug("DiscName::split: device: '%s'", d.device.toUtf8().constData());
	} else {
		qWarning("DiscName::split: no match in regular expression");
	}

	if (ok != 0) (*ok) = success;

	return d;
}

