/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2011 Ricardo Villalba <rvm@escomposlinux.org>

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

QString DiscName::joinDVD(int title, const QString & device, bool use_dvdnav) {
	return join(use_dvdnav ? DVDNAV : DVD, title, device);
}

QString DiscName::join(const DiscData & d) {
	QString s = d.protocol + "://";
	if (d.title > 0) s += QString::number(d.title);
	if (!d.device.isEmpty()) s+= "/" + removeTrailingSlash(d.device);

	qDebug("DiscName::join: result: '%s'", s.toUtf8().constData());
	return s;
}

QString DiscName::join(Disc type, int title, const QString & device) {
	QString protocol;
	switch (type) {
		case DVD: protocol = "dvd"; break;
		case DVDNAV: protocol = "dvdnav"; break;
		case VCD: protocol = "vcd"; break;
		case CDDA: protocol = "cdda"; break;
	}
	return join( DiscData(protocol, title, device) );
}

DiscData DiscName::split(const QString & disc_url, bool * ok) {
	qDebug("DiscName::split: disc_url: '%s'", disc_url.toUtf8().constData());

	QRegExp rx1("^(dvd|dvdnav|vcd|cdda)://(\\d+)/(.*)");
	QRegExp rx2("^(dvd|dvdnav|vcd|cdda)://(\\d+)");
	QRegExp rx3("^(dvd|dvdnav|vcd|cdda):///(.*)");
	QRegExp rx4("^(dvd|dvdnav|vcd|cdda):(.*)");

	DiscData d;

	bool success = false;

	if (rx1.indexIn(disc_url) != -1) {
		d.protocol = rx1.cap(1);
		d.title = rx1.cap(2).toInt();
		d.device = rx1.cap(3);
		success = true;
	} 
	else
	if (rx2.indexIn(disc_url) != -1) {
		d.protocol = rx2.cap(1);
		d.title = rx2.cap(2).toInt();
		d.device = "";
		success = true;
	} 
	else
	if (rx3.indexIn(disc_url) != -1) {
		d.protocol = rx3.cap(1);
		d.title = 0;
		d.device = rx3.cap(2);
		success = true;
	}
	else
	if (rx4.indexIn(disc_url) != -1) {
		d.protocol = rx4.cap(1);
		d.title = 0;
		d.device ="";
		success = true;
	}

	if (!d.device.isEmpty()) d.device = removeTrailingSlash(d.device);

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

// This functions remove the trailing "/" from the device
// with one exception: from Windows drives letters (D:/ E:/...)
QString DiscName::removeTrailingSlash(const QString & device) {
	QString dev = device;

	if (dev.endsWith("/")) {
#ifdef Q_OS_WIN
		QRegExp r("^[A-Z]:/$");
		int pos = r.indexIn(dev);
		//qDebug("DiscName::removeTrailingSlash: drive check: '%s': regexp: %d", dev.toUtf8().data(), pos);
		if (pos == -1)
#endif
			dev = dev.remove( dev.length()-1, 1);
	}

	return dev;
}

#if DISCNAME_TEST
void DiscName::test() {
	DiscData d;
	d = split( "vcd://1//dev/dvd/" );
	d = split( "vcd://1/E:/" );
	d = split( "vcd://5" );
	d = split( "vcd://" );
	d = split( "vcd:////dev/dvdrecorder" );

	d = split( "dvd://1//dev/dvd" );
	d = split( "dvd://1/E:" );
	d = split( "dvd://5" );
	d = split( "dvd://" );
	d = split( "dvd:" );
	d = split( "dvd:////dev/dvdrecorder" );

	d = split( "cdda://1//dev/dvd" );
	d = split( "cdda://1/E:" );
	d = split( "cdda://5" );
	d = split( "cdda://" );
	d = split( "cdda:////dev/dvdrecorder" );

	d = split( "dvdnav://1//dev/dvd" );
	d = split( "dvdnav://1/D:/" );
	d = split( "dvdnav://5" );
	d = split( "dvdnav://" );
	d = split( "dvdnav:////dev/dvdrecorder/" );

	QString s;
	s = join( DVD, 4, "/dev/dvd/" );
	s = join( DVD, 2, "E:" );
	s = join( DVD, 3, "E:/" );
	s = join( DVDNAV, 5, "/dev/dvdrecorder" );
	s = join( VCD, 1, "/dev/cdrom" );
	s = join( CDDA, 10, "/dev/cdrom" );
	s = joinDVD( 1, "/dev/cdrom", false );
	s = joinDVD( 2, "/dev/cdrom/", true );
	s = joinDVD( 3, "", true );
	s = join( VCD, 3, "" );
}
#endif
