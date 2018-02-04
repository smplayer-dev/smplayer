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

#include "myapplication.h"
#include <QSessionManager>

#ifdef SINGLE_INSTANCE
MyApplication::MyApplication (const QString & appId, int & argc, char ** argv)
	: QtSingleApplication(appId, argc, argv)
#else
MyApplication::MyApplication (const QString & /*appId*/, int & argc, char ** argv)
	: QApplication(argc, argv)
#endif
{
#if defined(USE_WINEVENTFILTER) && QT_VERSION >= 0x050000
	installNativeEventFilter(this);
#endif

#if QT_VERSION >= 0x050600
	setFallbackSessionManagementEnabled(false);
#endif
#if QT_VERSION >= 0x050000
	connect(this, SIGNAL(commitDataRequest(QSessionManager &)),
            this, SLOT(commitData(QSessionManager &)), Qt::DirectConnection);
#endif
};

void MyApplication::commitData(QSessionManager & manager) {
	manager.release();
}

#if defined(USE_WINEVENTFILTER) && QT_VERSION >= 0x050000
bool MyApplication::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
	//qDebug() << "MyApplication::nativeEventFilter:" <<eventType;
	
	if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
		MSG * m = static_cast<MSG *>(message);
		return winEventFilter(m, result);
	}
	
	return false;
}
#endif

#ifdef USE_WINEVENTFILTER
#include <QKeyEvent>
#include <QEvent>
#include <QWidget>
#include <windows.h>
#include <QDebug>

#ifndef WM_APPCOMMAND
#define WM_APPCOMMAND 0x0319
#endif

#ifndef FAPPCOMMAND_MOUSE
#define FAPPCOMMAND_MOUSE 0x8000
#define FAPPCOMMAND_KEY   0
#define FAPPCOMMAND_OEM   0x1000
#define FAPPCOMMAND_MASK  0xF000
#define GET_APPCOMMAND_LPARAM(lParam) ((short)(HIWORD(lParam) & ~FAPPCOMMAND_MASK))
#define GET_DEVICE_LPARAM(lParam)     ((WORD)(HIWORD(lParam) & FAPPCOMMAND_MASK))
#define GET_MOUSEORKEY_LPARAM         GET_DEVICE_LPARAM
#define GET_FLAGS_LPARAM(lParam)      (LOWORD(lParam))
#define GET_KEYSTATE_LPARAM(lParam)   GET_FLAGS_LPARAM(lParam)

#define APPCOMMAND_BROWSER_BACKWARD       1
#define APPCOMMAND_BROWSER_FORWARD        2
#define APPCOMMAND_BROWSER_REFRESH        3
#define APPCOMMAND_BROWSER_STOP           4
#define APPCOMMAND_BROWSER_SEARCH         5
#define APPCOMMAND_BROWSER_FAVORITES      6
#define APPCOMMAND_BROWSER_HOME           7
#define APPCOMMAND_VOLUME_MUTE            8
#define APPCOMMAND_VOLUME_DOWN            9
#define APPCOMMAND_VOLUME_UP              10
#define APPCOMMAND_MEDIA_NEXTTRACK        11
#define APPCOMMAND_MEDIA_PREVIOUSTRACK    12
#define APPCOMMAND_MEDIA_STOP             13
#define APPCOMMAND_MEDIA_PLAY_PAUSE       14
#define APPCOMMAND_LAUNCH_MAIL            15
#define APPCOMMAND_LAUNCH_MEDIA_SELECT    16
#define APPCOMMAND_LAUNCH_APP1            17
#define APPCOMMAND_LAUNCH_APP2            18
#define APPCOMMAND_BASS_DOWN              19
#define APPCOMMAND_BASS_BOOST             20
#define APPCOMMAND_BASS_UP                21
#define APPCOMMAND_TREBLE_DOWN            22
#define APPCOMMAND_TREBLE_UP              23
#endif // FAPPCOMMAND_MOUSE
 
// New commands from Windows XP (some even Sp1)
#ifndef APPCOMMAND_MICROPHONE_VOLUME_MUTE
#define APPCOMMAND_MICROPHONE_VOLUME_MUTE 24
#define APPCOMMAND_MICROPHONE_VOLUME_DOWN 25
#define APPCOMMAND_MICROPHONE_VOLUME_UP   26
#define APPCOMMAND_HELP                   27
#define APPCOMMAND_FIND                   28
#define APPCOMMAND_NEW                    29
#define APPCOMMAND_OPEN                   30
#define APPCOMMAND_CLOSE                  31
#define APPCOMMAND_SAVE                   32
#define APPCOMMAND_PRINT                  33
#define APPCOMMAND_UNDO                   34
#define APPCOMMAND_REDO                   35
#define APPCOMMAND_COPY                   36
#define APPCOMMAND_CUT                    37
#define APPCOMMAND_PASTE                  38
#define APPCOMMAND_REPLY_TO_MAIL          39
#define APPCOMMAND_FORWARD_MAIL           40
#define APPCOMMAND_SEND_MAIL              41
#define APPCOMMAND_SPELL_CHECK            42
#define APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE    43
#define APPCOMMAND_MIC_ON_OFF_TOGGLE      44
#define APPCOMMAND_CORRECTION_LIST        45
#define APPCOMMAND_MEDIA_PLAY             46
#define APPCOMMAND_MEDIA_PAUSE            47
#define APPCOMMAND_MEDIA_RECORD           48
#define APPCOMMAND_MEDIA_FAST_FORWARD     49
#define APPCOMMAND_MEDIA_REWIND           50
#define APPCOMMAND_MEDIA_CHANNEL_UP       51
#define APPCOMMAND_MEDIA_CHANNEL_DOWN     52
#endif // APPCOMMAND_MICROPHONE_VOLUME_MUTE

#define VK_MEDIA_NEXT_TRACK 0xB0
#define VK_MEDIA_PREV_TRACK 0xB1
#define VK_MEDIA_PLAY_PAUSE 0xB3
#define VK_MEDIA_STOP 0xB2


bool MyApplication::winEventFilter(MSG * msg, long * result) {
	//qDebug() << "MyApplication::winEventFilter" << msg->message << "lParam:" << msg->lParam;

	static uint last_appcommand = 0;
	
	if (msg->message == WM_KEYDOWN) {
		//qDebug("MyApplication::winEventFilter: WM_KEYDOWN: %X", msg->wParam);
		bool eat_key = false;
		if ((last_appcommand == APPCOMMAND_MEDIA_NEXTTRACK) && (msg->wParam == VK_MEDIA_NEXT_TRACK)) eat_key = true;
		else
		if ((last_appcommand == APPCOMMAND_MEDIA_PREVIOUSTRACK) && (msg->wParam == VK_MEDIA_PREV_TRACK)) eat_key = true;
		else
		if ((last_appcommand == APPCOMMAND_MEDIA_PLAY_PAUSE) && (msg->wParam == VK_MEDIA_PLAY_PAUSE)) eat_key = true;
		else
		if ((last_appcommand == APPCOMMAND_MEDIA_STOP) && (msg->wParam == VK_MEDIA_STOP)) eat_key = true;
		
		if (eat_key) { 
			qDebug() << "MyApplication::winEventFilter: ignoring key" << msg->wParam;
			last_appcommand = 0; 
			//*result = true; 
			return true; 
		}
	}
	else
	if (msg->message == WM_APPCOMMAND) {
		/*
		QKeySequence k(Qt::Key_MediaTogglePlayPause);
		qDebug() << "MyApplication::winEventFilter" << k.toString();
		*/

		//qDebug() << "MyApplication::winEventFilter" << msg->message << "lParam:" << msg->lParam;
		uint cmd  = GET_APPCOMMAND_LPARAM(msg->lParam);
		uint uDevice = GET_DEVICE_LPARAM(msg->lParam);
		uint dwKeys = GET_KEYSTATE_LPARAM(msg->lParam);
		qDebug() << "MyApplication::winEventFilter: cmd:" << cmd <<"uDevice:" << uDevice << "dwKeys:" << dwKeys;

		//if (uDevice == FAPPCOMMAND_KEY) {
			int key = 0;
			Qt::KeyboardModifiers modifier = Qt::NoModifier;
			QString name;
			
			switch (cmd) {
				case APPCOMMAND_MEDIA_PAUSE: key = Qt::Key_MediaPause; name = "Media Pause"; break;
				case APPCOMMAND_MEDIA_PLAY: key = Qt::Key_MediaPlay; name = "Media Play"; break;
				case APPCOMMAND_MEDIA_STOP: key = Qt::Key_MediaStop; name = "Media Stop"; break;
				case APPCOMMAND_MEDIA_PLAY_PAUSE: key = Qt::Key_MediaTogglePlayPause; name = "Toggle Media Play/Pause"; break;
			
				case APPCOMMAND_MEDIA_NEXTTRACK: key = Qt::Key_MediaNext; name = "Media Next"; break;
				case APPCOMMAND_MEDIA_PREVIOUSTRACK: key = Qt::Key_MediaPrevious; name = "Media Previous"; break;

				case APPCOMMAND_MEDIA_FAST_FORWARD: key = Qt::Key_F; modifier = Qt::ShiftModifier | Qt::ControlModifier; break;
				case APPCOMMAND_MEDIA_REWIND: key = Qt::Key_B; modifier = Qt::ShiftModifier | Qt::ControlModifier; break;
			}
			
			if (key != 0) {
				last_appcommand = cmd;
				
				QKeyEvent event(QEvent::KeyPress, key, modifier, name);
				QWidget * w = QApplication::focusWidget();
				if (w) QCoreApplication::sendEvent(w, &event);
				//*result = true;
				return true;
			}
		//}
	}

	return false;
}
#endif // USE_WINEVENTFILTER

#include "moc_myapplication.cpp"

