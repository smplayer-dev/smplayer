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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QVariant>
#include <QList>

#ifdef Q_OS_WIN
#define USE_DSOUND_DEVICES 1
#define USE_MPV_WASAPI_DEVICES 1
#else
#define USE_ALSA_DEVICES 0
#define USE_MPV_ALSA_DEVICES 0
#define USE_PULSEAUDIO_DEVICES 1
#define USE_XV_ADAPTORS 1
#endif

#if defined(USE_MPV_ALSA_DEVICES) || defined(USE_MPV_WASAPI_DEVICES)
#define MPV_AUDIO_DEVICES 1
#endif

#ifndef Q_OS_WIN
#define CACHE_DEVICE_INFO
#endif

class QSettings;

class DeviceData {

public:
	DeviceData() {};
	DeviceData(QVariant ID, QString desc) { _id = ID; _desc = desc; };
	~DeviceData() {};

	void setID(QVariant ID) { _id = ID; };
	void setDesc(QString desc) { _desc = desc; };

	QVariant ID() const { return _id; };
	QString desc() const { return _desc; };

private:
	QVariant _id;
	QString _desc;
};


typedef QList<DeviceData> DeviceList;


class DeviceInfo {

public:
#ifdef Q_OS_WIN
	static DeviceList dsoundDevices();
	static DeviceList displayDevices();
#else
	#if USE_PULSEAUDIO_DEVICES
	static DeviceList paDevices();
	#endif
	#if USE_ALSA_DEVICES
	static DeviceList alsaDevices();
	#endif
	#if USE_XV_ADAPTORS
	static DeviceList xvAdaptors();
	#endif
#endif

#if MPV_AUDIO_DEVICES
	static void setMpvBin(const QString & bin) { mpv_bin = bin; };

	#if USE_MPV_ALSA_DEVICES
	static DeviceList mpvAlsaDevices();
	#endif
	
	#if USE_MPV_WASAPI_DEVICES
	static DeviceList mpvWasapiDevices();
	#endif

	static DeviceList mpvAudioDevices(const QString & mpv_bin, const QString & filter);
	static DeviceList mpvAudioDevices(const QString & filter);
#endif

	static QString printableName(const QString & driver_name, const DeviceData & device);
	static QString internalName(const QString & driver_name, const DeviceData & device);

	static QString printableName(const QString & driver_name, const QString & id, const QString & desc);
	static QString internalName(const QString & driver_name, const QString & id, const QString & desc);
	static QStringList extractDevice(const QString & internal_name);

protected:
#ifdef CACHE_DEVICE_INFO
	static void saveList(QSettings * set, const QString & section_name, const DeviceList & list);
	static DeviceList loadList(QSettings * set, const QString & section_name);
#endif

#ifdef Q_OS_WIN
	enum DeviceType { Sound = 0, Display = 1 };

	static DeviceList retrieveDevices(DeviceType type);
#endif

#if MPV_AUDIO_DEVICES
	static QString mpv_bin;
#endif
};

#endif

