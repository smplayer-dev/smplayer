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

#include "shutdown.h"
#include <QProcess>

#ifdef Q_OS_LINUX
#include <QtDBus>
#include <QDebug>
#endif

void Shutdown::shutdown() {
#ifdef Q_OS_WIN
	QProcess::startDetached("shutdown -s -f -t 10");
#endif

#ifdef Q_OS_LINUX
	bool works = false;

	QDBusMessage response;

	QDBusInterface gnomeSessionManager("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
	response = gnomeSessionManager.call("RequestShutdown");
	if (response.type() == QDBusMessage::ErrorMessage) {
		qDebug() << "Shutdown::shutdown: error:" << response.errorName() << ":" << response.errorMessage();
	} else {
		works = true;
	}

	if (!works) {
		QDBusInterface kdeSessionManager("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
		response = kdeSessionManager.call("logout", 0, 2, 2);
		if (response.type() == QDBusMessage::ErrorMessage) {
			qDebug() << "Shutdown::shutdown: error:" << response.errorName() << ":" << response.errorMessage();
		} else {
			works = true;
		}
	}

	if (!works) {
		QDBusInterface powermanagement("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", 
			"org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
		response = powermanagement.call("Shutdown");
		if (response.type() == QDBusMessage::ErrorMessage) {
			qDebug() << "Shutdown::shutdown: error:" << response.errorName() << ":" << response.errorMessage();
		} else {
			works = true;
		}
	}

	if (!works) {
		QDBusInterface powermanagement("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager",
			QDBusConnection::systemBus());
		response = powermanagement.call("Stop");
		if (response.type() == QDBusMessage::ErrorMessage) {
			qDebug() << "Shutdown::shutdown: error:" << response.errorName() << ":" << response.errorMessage();
		} else {
			works = true;
		}
	}

	if (!works) {
		qDebug("Shutdown::shutdown: shutdown failed");

		QProcess::startDetached("xmessage", QStringList() << "-buttons" << "Accept:0" << "-center" <<
			"This is a message from SMPlayer\n"
			"The computer should shut down now.\n"
			"However shutdown failed.");
	}
#endif
}

