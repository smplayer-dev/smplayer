/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include <windows.h>

bool GlobalShortcuts::nativeEventFilter(const QByteArray & /*eventType*/, void * message, long * /*result*/) {
	//qDebug() << "GlobalShortcuts::nativeEventFilter";

	if (!isEnabled()) return false;

	MSG* msg = static_cast<MSG*>(message);
	if (msg->message == WM_HOTKEY) {
		const quint32 keycode = HIWORD(msg->lParam);
		//const quint32 modifiers = LOWORD(msg->lParam);
		qDebug() << "GlobalShortcuts::nativeEventFilter: keycode:" << keycode;

		if (key_list.contains(keycode)) {
			activateShortcut(key_list[keycode]);
		}
	}

	return false;
}

bool GlobalShortcuts::registerShortcut(quint32 nativeKey, quint32 nativeMods) {
	qDebug() << "GlobalShortcuts::registerShortcut: nativeKey:" << nativeKey;
	return RegisterHotKey(0, nativeMods ^ nativeKey, nativeMods, nativeKey);
}

bool GlobalShortcuts::unregisterShortcut(quint32 nativeKey, quint32 nativeMods) {
	qDebug() << "GlobalShortcuts::unregisterShortcut: nativeKey:" << nativeKey;
	return UnregisterHotKey(0, nativeMods ^ nativeKey);
}

void GlobalShortcuts::createKeysList() {
	key_list.clear();
	if (grabbed_keys.testFlag(VolumeDown)) {
		key_list[VK_VOLUME_DOWN] = Qt::Key_VolumeDown;
	}
	if (grabbed_keys.testFlag(VolumeUp)) {
		key_list[VK_VOLUME_UP] = Qt::Key_VolumeUp;
	}
	if (grabbed_keys.testFlag(VolumeMute)) {
		key_list[VK_VOLUME_MUTE] = Qt::Key_VolumeMute;
	}
	if (grabbed_keys.testFlag(MediaPlay)) {
		key_list[VK_MEDIA_PLAY_PAUSE] = Qt::Key_MediaPlay;
	}
	if (grabbed_keys.testFlag(MediaStop)) {
		key_list[VK_MEDIA_STOP] = Qt::Key_MediaStop;
	}
	if (grabbed_keys.testFlag(MediaPrevious)) {
		key_list[VK_MEDIA_PREV_TRACK] = Qt::Key_MediaPrevious;
	}
	if (grabbed_keys.testFlag(MediaNext)) {
		key_list[VK_MEDIA_NEXT_TRACK] = Qt::Key_MediaNext;
	}
	if (grabbed_keys.testFlag(MediaPlay)) {
		key_list[VK_PLAY] = Qt::Key_MediaPlay;
	}
	//key_list[VK_F11] = Qt::Key_MediaPlay; // TEST
	//key_list[] = Qt::Key_MediaPause;
	//key_list[] = Qt::Key_MediaRecord;
}
