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

#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#include <qpa/qplatformnativeinterface.h>
#include <xcb/xcb.h>

class X11Data {
public:
	X11Data() {
		QPlatformNativeInterface *native = qApp->platformNativeInterface();
		void *display = native->nativeResourceForScreen(QByteArray("display"),
                                                        QGuiApplication::primaryScreen());
		disp = reinterpret_cast<Display *>(display);
	}

	Display *display() {
		return disp;
	}

	Window rootWindow() {
		return DefaultRootWindow(display());
	}

	bool isValid() { return disp != 0; }

	quint32 keysymToKeycode(KeySym keysym) {
		if (isValid()) {
			return XKeysymToKeycode(display(), keysym);
		}
		return 0;
	}

private:
	Display * disp;
};


bool GlobalShortcuts::nativeEventFilter(const QByteArray & eventType, void * message, long * /*result*/) {
	//qDebug() << "GlobalShortcuts::nativeEventFilter";

	if (!isEnabled()) return false;

	xcb_key_press_event_t *kev = 0;
	if (eventType == "xcb_generic_event_t") {
		xcb_generic_event_t *ev = static_cast<xcb_generic_event_t *>(message);
		if ((ev->response_type & 127) == XCB_KEY_PRESS) {
			kev = static_cast<xcb_key_press_event_t *>(message);
		}

		if (kev != 0) {
			unsigned int keycode = kev->detail;
			qDebug() << "GlobalShortcuts::nativeEventFilter: keycode:" << keycode;

			if (key_list.contains(keycode)) {
				activateShortcut(key_list[keycode]);
				return true;
			}
		}
	}

	return false;
}

bool GlobalShortcuts::registerShortcut(quint32 nativeKey, quint32 nativeMods) {
	qDebug() << "GlobalShortcuts::registerShortcut: nativeKey:" << nativeKey;

	X11Data x11;
	if (x11.isValid()) {
		Display * display = x11.display();
		Window window = x11.rootWindow();

		Bool owner = True;
		int pointer = GrabModeAsync;
		int keyboard = GrabModeAsync;

		XGrabKey(display, nativeKey, nativeMods, window, owner, pointer, keyboard);
		XGrabKey(display, nativeKey, nativeMods | Mod2Mask, window, owner, pointer, keyboard);
		XSync(display, False);

		return true;
	}
	return false;
}

bool GlobalShortcuts::unregisterShortcut(quint32 nativeKey, quint32 nativeMods) {
	qDebug() << "GlobalShortcuts::unregisterShortcut: nativeKey:" << nativeKey;

	X11Data x11;
	if (x11.isValid()) {
		Display * display = x11.display();
		Window window = x11.rootWindow();

		XUngrabKey(display, nativeKey, nativeMods, window);
		XUngrabKey(display, nativeKey, nativeMods | Mod2Mask, window);
		XSync(display, False);
		return true;
	}
	return false;
}

void GlobalShortcuts::createKeysList() {
	X11Data x11;
	key_list.clear();
	if (grabbed_keys.testFlag(VolumeDown)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioLowerVolume)] = Qt::Key_VolumeDown;
	}
	if (grabbed_keys.testFlag(VolumeUp)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioRaiseVolume)] = Qt::Key_VolumeUp;
	}
	if (grabbed_keys.testFlag(VolumeMute)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioMute)] = Qt::Key_VolumeMute;
	}
	if (grabbed_keys.testFlag(MediaPlay)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioPlay)] = Qt::Key_MediaPlay;
	}
	if (grabbed_keys.testFlag(MediaStop)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioStop)] = Qt::Key_MediaStop;
	}
	if (grabbed_keys.testFlag(MediaPrevious)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioPrev)] = Qt::Key_MediaPrevious;
	}
	if (grabbed_keys.testFlag(MediaNext)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioNext)] = Qt::Key_MediaNext;
	}
	if (grabbed_keys.testFlag(MediaPause)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioPause)] = Qt::Key_MediaPause;
	}
	if (grabbed_keys.testFlag(MediaRecord)) {
		key_list[x11.keysymToKeycode(XF86XK_AudioRecord)] = Qt::Key_MediaRecord;
	}
	//key_list[x11.keysymToKeycode(0xffc8)] = Qt::Key_F11; // Test
	qDebug() << "GlobalShortcuts::createKeysList:" << key_list;
}
