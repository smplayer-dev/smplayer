/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include <QDebug>

PowerSaving::PowerSaving(QObject * parent)
	: QObject(parent)
{
	if (!QDBusConnection::sessionBus().isConnected()) {
		qDebug("PowerSaving::PowerSaving: Failed to connect to DBus");
	}
	interface = new QDBusInterface("org.freedesktop.ScreenSaver" ,"/ScreenSaver" , "org.freedesktop.ScreenSaver" , QDBusConnection::sessionBus());
	if (interface->isValid()) {
		qDebug("PowerSaving::PowerSaving: interface is valid");
	}
}

PowerSaving::~PowerSaving() {
}

void PowerSaving::inhibit() {
	qDebug("PowerSaving::inhibit");
	QDBusReply<uint> reply = interface->call("Inhibit", "smplayer", "Playing media");
	if (reply.isValid()) {
		qDebug("PowerSaving::inhibit: valid reply");
		qDebug("PowerSaving::inhibit: cookie: %d", reply.value());
		cookies << reply.value();
	} else {
		qDebug() << "PowerSaving::inhibit: error:" << reply.error();
	}
}

void PowerSaving::uninhibit() {
	qDebug("PowerSaving::uninhibit");
	foreach(uint cookie, cookies) {
		qDebug("PowerSaving::uninhibit: cookie: %d", cookie);
		QDBusReply<void> reply = interface->call("UnInhibit", cookie);
		if (reply.isValid()) {
			qDebug("PowerSaving::uninhibit: valid reply");
		} else {
			qDebug() << "PowerSaving::uninhibit: error:" << reply.error();
		}
	}
	cookies.clear();
}

#include "moc_powersaving.cpp"
