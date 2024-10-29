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

#include "screensaver.h"

#ifdef Q_OS_WIN
#include "winscreensaver.h"
#endif
#ifdef OS_UNIX_NOT_MAC
#include "powersaving.h"
#endif
#ifdef Q_OS_MACX
#include "powersaving_mac.h"
#endif

ScreenSaver::ScreenSaver(QObject * parent) : QObject(parent) {
#ifdef Q_OS_WIN
	win_screensaver = new WinScreenSaver();
#else
	power_saving = new PowerSaving(this);
#endif
}

ScreenSaver::~ScreenSaver() {
#ifdef Q_OS_WIN
	delete win_screensaver;
#endif
}

bool ScreenSaver::isValid() {
#ifdef Q_OS_WIN
	return win_screensaver->isValid();
#else
	return power_saving->isValid();
#endif
}

void ScreenSaver::enable() {
#ifdef Q_OS_WIN
	win_screensaver->enable();
#else
	power_saving->uninhibit();
#endif
}

void ScreenSaver::disable() {
#ifdef Q_OS_WIN
	win_screensaver->disable();
#else
	power_saving->inhibit();
#endif
}

#include "moc_screensaver.cpp"
