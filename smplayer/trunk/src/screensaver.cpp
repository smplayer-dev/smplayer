/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "screensaver.h"
#include <Qt>
#include <QSysInfo>
#include <windows.h>

WinScreenSaver::WinScreenSaver() {
	lowpower = poweroff = screensaver = 0;
	state_saved = false;
	modified = false;
	
	retrieveState();
}

WinScreenSaver::~WinScreenSaver() {
	restoreState();
}

void WinScreenSaver::retrieveState() {
	qDebug("WinScreenSaver::retrieveState");
	
	if (!state_saved) {
		if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {
			// Not supported on Windows Vista
			SystemParametersInfo(SPI_GETLOWPOWERTIMEOUT, 0, &lowpower, 0);
			SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT, 0, &poweroff, 0);
		}
		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &screensaver, 0);
		state_saved = true;
		
		qDebug("WinScreenSaver::retrieveState: lowpower: %d, poweroff: %d, screensaver: %d", lowpower, poweroff, screensaver);
	} else {
		qDebug("WinScreenSaver::retrieveState: state already saved previously, doing nothing");
	}
}

void WinScreenSaver::restoreState() {
	if (!modified) {
		qDebug("WinScreenSaver::restoreState: state did not change, doing nothing");
		return;
	}
	
	if (state_saved) {
		if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {
			// Not supported on Windows Vista
			SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, lowpower, NULL, 0);
			SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, poweroff, NULL, 0);
		}
		SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, screensaver, NULL, 0);
		
		qDebug("WinScreenSaver::restoreState: lowpower: %d, poweroff: %d, screensaver: %d", lowpower, poweroff, screensaver);
	} else {
		qWarning("WinScreenSaver::restoreState: no data, doing nothing");
	}
}
	
void WinScreenSaver::disable() {
	qDebug("WinScreenSaver::disable");

	if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {
		// Not supported on Windows Vista
		SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
		SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	}
	SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	modified = true;
}

void WinScreenSaver::enable() {
	qDebug("WinScreenSaver::enable");

	restoreState();
}

