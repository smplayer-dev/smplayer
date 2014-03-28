/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "myapplication.h"

#ifdef Q_OS_WIN
#include <QKeyEvent>
#include <QEvent>
#include <QWidget>
#include <windows.h>
#include <QDebug>

#ifndef WM_APPCOMMAND
#define WM_APPCOMMAND 0x0319
#endif

#define FAPPCOMMAND_MASK  0xF000
#define GET_APPCOMMAND_LPARAM(lParam) ((short)(HIWORD(lParam) & ~FAPPCOMMAND_MASK))
#define GET_FLAGS_LPARAM(lParam)      (LOWORD(lParam))
#define GET_KEYSTATE_LPARAM(lParam)   GET_FLAGS_LPARAM(lParam)

#define APPCOMMAND_MEDIA_PLAY 46
#define APPCOMMAND_MEDIA_STOP 13
#define APPCOMMAND_MEDIA_NEXTTRACK 11
#define APPCOMMAND_MEDIA_PREVIOUSTRACK 12


bool MyApplication::winEventFilter ( MSG * msg, long * result ) {
#if 0
	if (msg->message == WM_APPCOMMAND) {
		qDebug() << "MyApplication::winEventFilter" << msg->message << "lParam:" << msg->lParam;
		int cmd  = GET_APPCOMMAND_LPARAM(msg->lParam);
		int dwKeys = GET_KEYSTATE_LPARAM(msg->lParam);
		qDebug() << "MyApplication::winEventFilter: cmd:" << cmd << "dwKeys:" << dwKeys;
		int key = 0;
		QString name;
		switch (cmd) {
			case APPCOMMAND_MEDIA_PLAY: key = Qt::Key_MediaPlay; name = "Media Play"; break;
			case APPCOMMAND_MEDIA_STOP: key = Qt::Key_MediaStop; name = "Media Stop"; break;
			/*
			case APPCOMMAND_MEDIA_NEXTTRACK: key = Qt::Key_MediaNext; name = "Media Next"; break;
			case APPCOMMAND_MEDIA_PREVIOUSTRACK: key = Qt::Key_MediaPrevious; name = "Media Previous"; break;
			*/
		}
		if (key != 0) {
			QKeyEvent event(QEvent::KeyPress, key, Qt::NoModifier, name);
			QCoreApplication::sendEvent(QApplication::focusWidget(), &event);
			*result = true;
			return true;
		}
	}
#endif
	return false;
}
#endif

#include "moc_myapplication.cpp"

