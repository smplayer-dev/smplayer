/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2024 Ricardo Villalba <ricardo@smplayer.info>

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

#include "powersaving.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QProcess>
#include <QDebug>

PowerSaving::PowerSaving(QObject * parent)
	: QObject(parent),
	is_inhibited(false)
{
	if (!QDBusConnection::sessionBus().isConnected()) {
		qDebug("PowerSaving::PowerSaving: Failed to connect to DBus");
	}
	interface = new QDBusInterface("org.freedesktop.ScreenSaver" ,"/ScreenSaver" , "org.freedesktop.ScreenSaver" , QDBusConnection::sessionBus());
	if (interface->isValid()) {
		qDebug("PowerSaving::PowerSaving: interface is valid");
	}

	#ifdef SWAYIDLE_SUPPORT
	// swayidle (wayland)
	QProcess detect;
	detect.start("pgrep", QStringList() << "-af" << "swayidle");
	detect.waitForFinished();
	QString output = QString::fromUtf8(detect.readAllStandardOutput());
	if (!output.isEmpty()) {
		QString line = output.section('\n', 0, 0);
		QStringList l = line.split(" ");
		if (l.count() > 3 && l[1].contains("sh") && l[2] == "-c") {
			l = l.mid(3);
			swayidle_cmd = l.join(" ");
			qDebug() << "PowerSaving::PowerSaving: swayidle_cmd:" << swayidle_cmd;
		} else {
			qDebug("PowerSaving::PowerSaving: swayidle invalid");
		}
	} else{
		qDebug() << "PowerSaving::PowerSaving: swayidle not running";
	}
	#endif
}

PowerSaving::~PowerSaving() {
	uninhibit();
}

void PowerSaving::inhibit() {
	if (is_inhibited) return;

	qDebug("PowerSaving::inhibit");
	QDBusReply<uint> reply = interface->call("Inhibit", "smplayer", "Playing media");
	if (reply.isValid()) {
		qDebug("PowerSaving::inhibit: valid reply");
		qDebug("PowerSaving::inhibit: cookie: %d", reply.value());
		cookies << reply.value();
		is_inhibited = true;
	} else {
		qDebug() << "PowerSaving::inhibit: error:" << reply.error();
	}

	#ifdef SWAYIDLE_SUPPORT
	// swayidle
	if (!swayidle_cmd.isEmpty()) {
		QProcess::execute("pkill", QStringList() << "swayidle");
		is_inhibited = true;
	}
	#endif
}

void PowerSaving::uninhibit() {
	if (!is_inhibited) return;

	qDebug("PowerSaving::uninhibit");
	foreach(uint cookie, cookies) {
		qDebug("PowerSaving::uninhibit: cookie: %d", cookie);
		QDBusReply<void> reply = interface->call("UnInhibit", cookie);
		if (reply.isValid()) {
			qDebug("PowerSaving::uninhibit: valid reply");
			is_inhibited = false;
		} else {
			qDebug() << "PowerSaving::uninhibit: error:" << reply.error();
		}
	}
	cookies.clear();

	#ifdef SWAYIDLE_SUPPORT
	// swayidle
	if (!swayidle_cmd.isEmpty()) {
		bool ok = QProcess::startDetached("sh", QStringList() << "-c" << swayidle_cmd);
		qDebug() << QString("PowerSaving::uninhibit: running command sh -c %1").arg(swayidle_cmd);
		if (ok) is_inhibited = false;
	}
	#endif
}

#include "moc_powersaving.cpp"
