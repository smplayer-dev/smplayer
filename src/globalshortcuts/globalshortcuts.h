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

#ifndef GLOBALSHORTCUTS_H
#define GLOBALSHORTCUTS_H

#include <QObject>
#include <QMap>
#include <QAbstractNativeEventFilter>

class GlobalShortcuts : public QObject, public QAbstractNativeEventFilter
{
	Q_OBJECT

public:
	enum MediaKey { VolumeMute = 1, VolumeDown = 2, VolumeUp = 4,
                    MediaPlay = 8, MediaStop = 16, MediaPrevious = 32,
                    MediaNext = 64, MediaPause = 128, MediaRecord = 256 };
	Q_DECLARE_FLAGS(MediaKeys, MediaKey)

	GlobalShortcuts( QObject* parent = 0 );
	~GlobalShortcuts();

	bool isEnabled() { return enabled; };

	// It should be called before setEnabled
	void setGrabbedKeys(MediaKeys keys);
	MediaKeys grabbedKeys() { return grabbed_keys; };

	virtual bool nativeEventFilter(const QByteArray & eventType, void * message, long * result);

public slots:
	void setEnabled(bool);

private:
	void registerAll();
	void unregisterAll();

	bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
	bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);

	void activateShortcut(Qt::Key);

	void createKeysList();

	QMap<quint32, Qt::Key> key_list;
	bool enabled;
	MediaKeys grabbed_keys;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalShortcuts::MediaKeys)

#endif
