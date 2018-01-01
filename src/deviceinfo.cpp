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

#include "deviceinfo.h"
#include "paths.h"
#include <QProcess>
#include <QFile>
#include <QSettings>
#include <QDebug>

#ifdef Q_OS_WIN

#define USE_DIRECTX

#ifdef USE_DIRECTX
#define DIRECTSOUND_VERSION 5
#include <dsound.h>
#include <ddraw.h>

QStringList dsound_device_list;
QStringList ddraw_device_list;

BOOL CALLBACK DirectSoundEnum(LPGUID guid, LPCSTR desc, LPCSTR module, LPVOID context)
{
	Q_UNUSED(guid);
	Q_UNUSED(module);
	Q_UNUSED(context);

	dsound_device_list << QString(desc);
	return TRUE;
}

BOOL WINAPI DirectDrawEnum(GUID FAR *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm)
{
	Q_UNUSED(lpDriverName);
	Q_UNUSED(lpContext);
	Q_UNUSED(hm);

	if (!lpGUID) {
		ddraw_device_list << "Primary Display Adapter";
	} else {
		ddraw_device_list << QString(lpDriverDescription);
	}
	return TRUE;
}


DeviceList DeviceInfo::retrieveDevices(DeviceType type) {
	qDebug("DeviceInfo::retrieveDevices: %d", type);

	DeviceList l;

	dsound_device_list.clear();
	ddraw_device_list.clear();

	if (type == Sound) {
		DirectSoundEnumerateA(DirectSoundEnum, NULL);
		for (int n = 0; n < dsound_device_list.count(); n++) {
			QString desc = dsound_device_list[n];
			qDebug() << "DeviceInfo::retrieveDevices: audio:" << n << desc;
			l.append( DeviceData(n, desc) );
		}
	}
	else
	if (type == Display) {
		DirectDrawEnumerateExA(DirectDrawEnum, NULL, DDENUM_ATTACHEDSECONDARYDEVICES);
		for (int n = 0; n < ddraw_device_list.count(); n++) {
			QString desc = ddraw_device_list[n];
			qDebug() << "DeviceInfo::retrieveDevices: display:" << n << desc;
			l.append( DeviceData(n, desc) );
		}
	}

	return l;
}

#else

DeviceList DeviceInfo::retrieveDevices(DeviceType type) {
	qDebug("DeviceInfo::retrieveDevices: %d", type);

	DeviceList l;

	#ifdef CACHE_DEVICE_INFO
	QString inifile = Paths::configPath() + "/device_info.ini";
	QSettings set(inifile, QSettings::IniFormat);
	QString section_name = "display-devices";
	if (type == Sound) section_name = "dsound-devices";

	// Check if we already have the list stored in the INI file
	l = loadList(&set, section_name);
	if (l.count() > 0) return l;
	#endif

	QRegExp rx_device("^(\\d+): (.*)");

	if (QFile::exists("dxlist.exe")) {
		QProcess p;
		p.setProcessChannelMode( QProcess::MergedChannels );
		QStringList arg;
		if (type == Sound) arg << "-s"; else arg << "-d";
		p.start("dxlist", arg);

		if (p.waitForFinished()) {
			QByteArray line;
			while (p.canReadLine()) {
				line = p.readLine().trimmed();
				qDebug("DeviceInfo::retrieveDevices: '%s'", line.constData());
				if ( rx_device.indexIn(line) > -1 ) {
					int id = rx_device.cap(1).toInt();
					QString desc = rx_device.cap(2);
					qDebug("DeviceInfo::retrieveDevices: found device: '%d' '%s'", id, desc.toUtf8().constData());
					l.append( DeviceData(id, desc) );
				}
			}
		}
	}

	#ifdef CACHE_DEVICE_INFO
	saveList(&set, section_name, l);
	#endif

	return l;
}
#endif

DeviceList DeviceInfo::dsoundDevices() { 
	return retrieveDevices(Sound);
}

DeviceList DeviceInfo::displayDevices() {
	return retrieveDevices(Display);
}

#else

// Linux

#if USE_PULSEAUDIO_DEVICES
DeviceList DeviceInfo::paDevices() {
	qDebug("DeviceInfo::paDevices");

	static DeviceList l;
	if (!l.isEmpty()) return l;

	QRegExp rx_index("(.*)index: (\\d+)");
	QRegExp rx_name("(.*)name: (.*)");

	QProcess p;
	p.setProcessChannelMode( QProcess::MergedChannels );
	QStringList env = QProcess::systemEnvironment();
	env << "LC_ALL=C";
	p.setEnvironment(env);
	p.start("pacmd list-sinks");

	int index = -1;
	QString name;

	if (p.waitForFinished()) {
		QByteArray line;
		while (p.canReadLine()) {
			line = p.readLine().trimmed();
			//qDebug() << "DeviceInfo::paDevices:" << line;
			if (rx_index.indexIn(line) > -1 ) {
				index = rx_index.cap(2).toInt();
				qDebug() << "DeviceInfo::paDevices: index:" << index;
			}
			if (rx_name.indexIn(line) > -1 ) {
				name = rx_name.cap(2);
				if (name.startsWith('<') && name.endsWith('>')) { name = name.mid(1); name.chop(1); }
				qDebug() << "DeviceInfo::paDevices: name:" << name;
				if (index != -1) {
					l.append( DeviceData(index, name) );
					index = -1;
				}
			}
		}
	}

	return l;
}
#endif // USE_PULSEAUDIO_DEVICES

#if USE_ALSA_DEVICES
DeviceList DeviceInfo::alsaDevices() {
	qDebug("DeviceInfo::alsaDevices");

	DeviceList l;

	#ifdef CACHE_DEVICE_INFO
	QString inifile = Paths::configPath() + "/device_info.ini";
	QSettings set(inifile, QSettings::IniFormat);

	// Check if we already have the list stored in the INI file
	l = loadList(&set, "alsa-devices");
	if (l.count() > 0) return l;
	#endif

	QRegExp rx_device("^card\\s([0-9]+).*\\[(.*)\\],\\sdevice\\s([0-9]+):");

	QProcess p;
	p.setProcessChannelMode( QProcess::MergedChannels );
	p.setEnvironment( QStringList() << "LC_ALL=C" );
	p.start("aplay", QStringList() << "-l");

	if (p.waitForFinished()) {
		QByteArray line;
		while (p.canReadLine()) {
			line = p.readLine();
			qDebug("DeviceInfo::alsaDevices: '%s'", line.constData());
			if ( rx_device.indexIn(line) > -1 ) {
				QString id = rx_device.cap(1);
				id.append(".");
				id.append(rx_device.cap(3));
				QString desc = rx_device.cap(2);
				qDebug("DeviceInfo::alsaDevices: found device: '%s' '%s'", id.toUtf8().constData(), desc.toUtf8().constData());
				l.append( DeviceData(id, desc) );
			}
		}
	} else {
		qDebug("DeviceInfo::alsaDevices: could not start aplay, error %d", p.error());
	}

	#ifdef CACHE_DEVICE_INFO
	saveList(&set, "alsa-devices", l);
	#endif

	return l;
}
#endif

#ifdef USE_XV_ADAPTORS
DeviceList DeviceInfo::xvAdaptors() {
	qDebug("DeviceInfo::xvAdaptors");

	DeviceList l;

	#ifdef CACHE_DEVICE_INFO
	QString inifile = Paths::configPath() + "/device_info.ini";
	QSettings set(inifile, QSettings::IniFormat);

	// Check if we already have the list stored in the INI file
	l = loadList(&set, "xv-adaptors");
	if (l.count() > 0) return l;
	#endif

	QRegExp rx_device("^.*Adaptor #([0-9]+): \"(.*)\"");

	QProcess p;
	p.setProcessChannelMode( QProcess::MergedChannels );
	p.setEnvironment( QProcess::systemEnvironment() << "LC_ALL=C" );
	p.start("xvinfo");

	if (p.waitForFinished()) {
		QByteArray line;
		while (p.canReadLine()) {
			line = p.readLine();
			qDebug("DeviceInfo::xvAdaptors: '%s'", line.constData());
			if ( rx_device.indexIn(line) > -1 ) {
				QString id = rx_device.cap(1);
				QString desc = rx_device.cap(2);
				qDebug("DeviceInfo::xvAdaptors: found adaptor: '%s' '%s'", id.toUtf8().constData(), desc.toUtf8().constData());
				l.append( DeviceData(id, desc) );
			}
		}
	} else {
		qDebug("DeviceInfo::xvAdaptors: could not start xvinfo, error %d", p.error());
	}

	#ifdef CACHE_DEVICE_INFO
	saveList(&set, "xv-adaptors", l);
	#endif

	return l;
}
#endif
#endif

#if MPV_AUDIO_DEVICES
QString DeviceInfo::mpv_bin;

#if USE_MPV_ALSA_DEVICES
DeviceList DeviceInfo::mpvAlsaDevices() {
	static DeviceList l;
	if (!l.isEmpty()) return l;
	l = mpvAudioDevices("alsa");
	return l;
}
#endif

#if USE_MPV_WASAPI_DEVICES
DeviceList DeviceInfo::mpvWasapiDevices() {
	static DeviceList l;
	if (!l.isEmpty()) return l;
	l = mpvAudioDevices("wasapi");
	return l;
}
#endif
	
DeviceList DeviceInfo::mpvAudioDevices(const QString & filter) {
	DeviceList l;
	if (!mpv_bin.isEmpty()) l = mpvAudioDevices(mpv_bin, filter);
	return l;
}

DeviceList DeviceInfo::mpvAudioDevices(const QString & mpv_bin, const QString & filter) {
	qDebug("DeviceInfo::mpvAudioDevices");

	DeviceList l;

	QRegExp rx("'" + filter + "\\/(.*)'\\s+\\((.*)\\)");

	QProcess p;
	p.setProcessChannelMode( QProcess::MergedChannels );

	p.start(mpv_bin, QStringList() << "--audio-device=help");

	QString device;
	QString name;
	//int index = 0;

	if (p.waitForFinished()) {
		QString line;
		while (p.canReadLine()) {
			line = QString::fromUtf8(p.readLine().trimmed());
			qDebug() << "DeviceInfo::mpvAudioDevices:" << line;

			if (rx.indexIn(line) > -1 ) {
				device = rx.cap(1);
				name = rx.cap(2);
				qDebug() << "DeviceInfo::mpvAudioDevices: device:" << device << "name:" << name;
				l.append( DeviceData(device, name) );
				//index++;
			}
		}
	}

	return l;
}
#endif

#ifdef CACHE_DEVICE_INFO
void DeviceInfo::saveList(QSettings * set, const QString & section_name, const DeviceList & list) {
	set->beginWriteArray(section_name);
	for (int i = 0; i < list.count(); ++i) {
		set->setArrayIndex(i);
		set->setValue("ID", list.at(i).ID());
		set->setValue("description", list.at(i).desc());
	}
	set->endArray();
}

DeviceList DeviceInfo::loadList(QSettings * set, const QString & section_name) {
	DeviceList l;

	int count = set->beginReadArray(section_name);
	for (int i = 0; i < count; ++i) {
		set->setArrayIndex(i);
		QVariant id = set->value("ID");
		QString desc = set->value("description", "").toString();
		l.append(DeviceData(id, desc));
	}

	set->endArray();

	return l;
}
#endif

QString DeviceInfo::printableName(const QString & driver_name, const DeviceData & device) {
	return printableName(driver_name, device.ID().toString(), device.desc());
}

QString DeviceInfo::internalName(const QString & driver_name, const DeviceData & device) {
	return internalName(driver_name, device.ID().toString(), device.desc());
}

QString DeviceInfo::printableName(const QString & driver_name, const QString & id, const QString & desc) {
	Q_UNUSED(id);
	return driver_name +" (" + /* id + " - " + */ desc + ")";
}

QString DeviceInfo::internalName(const QString & driver_name, const QString & id, const QString & desc) {
	return driver_name + ":::" + id + ":::" + desc;
}

QStringList DeviceInfo::extractDevice(const QString & internal_name) {
	return internal_name.split(":::");
}
