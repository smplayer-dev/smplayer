/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "deviceinfo.h"
#include <QProcess>
#include <QFile>

InfoList DeviceInfo::alsaDevices() {
	qDebug("DeviceInfo::alsaDevices");

	InfoList l;
	QRegExp rx_device("^card\\s([0-9]+).*\\[(.*)\\],\\sdevice\\s([0-9]+):");

	if (QFile::exists("/usr/bin/aplay")) {
		QProcess p;
		p.setProcessChannelMode( QProcess::MergedChannels );
		p.start("/usr/bin/aplay", QStringList() << "-l");

		if (p.waitForFinished()) {
			QByteArray line;
			while (p.canReadLine()) {
				line = p.readLine();
				qDebug("DeviceInfo::alsaDevices: '%s'", line.constData());
				if ( rx_device.indexIn(line) > -1 ) {
					QString name = rx_device.cap(1);
					name.append(".");
					name.append(rx_device.cap(3));
					QString desc = rx_device.cap(2);
					qDebug("DeviceInfo::alsaDevices: found device: '%s' '%s'", name.toUtf8().constData(), desc.toUtf8().constData());
					l.append( InfoData(name, desc) );
				}
			}
		}
	}

	return l;
}

