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

#define APPCOMMAND_MEDIA_PLAY 46
#define APPCOMMAND_MEDIA_STOP 13

bool MyApplication::winEventFilter ( MSG * msg, long * result ) {
	if (msg->message == WM_APPCOMMAND) {
		qDebug() << "MyApplication::winEventFilter" << msg->message;
		int cmd  = GET_APPCOMMAND_LPARAM(msg->lParam);
		qDebug() << "MyApplication::winEventFilter: cmd" << cmd;
		if (cmd == APPCOMMAND_MEDIA_PLAY) {
			QKeyEvent event(QEvent::KeyPress, Qt::Key_MediaPlay, Qt::NoModifier, "Media Play");
			QCoreApplication::sendEvent(QApplication::focusWidget(), &event);
		}
		else
		if (cmd == APPCOMMAND_MEDIA_STOP) {
			QKeyEvent event(QEvent::KeyPress, Qt::Key_MediaStop, Qt::NoModifier, "Media Stop");
			QCoreApplication::sendEvent(QApplication::focusWidget(), &event);
		}	
	}
	return false;
}
#endif

#include "moc_myapplication.cpp"

