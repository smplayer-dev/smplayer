/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include <pulse/context.h>
#include <pulse/glib-mainloop.h>
#include <pulse/introspect.h>
#include <pulse/operation.h>
#include <unistd.h>
#include <QApplication>

QMap<int, QString> pulse_device_list;
bool pulse_list_done = false;

void sink_cb(pa_context *, const pa_sink_info *i, int eol, void *) {
	if (eol == 0) {
		qDebug() << "DeviceInfo::sink_cb: sink #" << i->index << ":" << i->name;
		pulse_device_list[i->index] = i->name;
	} else {
		pulse_list_done = true;
	}
}

void context_state_cb(pa_context *c, void *) {
	qDebug() << "DeviceInfo::context_state_cb";

	pa_context_state_t state = pa_context_get_state(c);

	if (state == PA_CONTEXT_READY) {
		pa_operation *o;

		if (!(o = pa_context_get_sink_info_list(c, sink_cb, NULL))) {
			qWarning() << "DeviceInfo::context_state_cb: pa_context_get_sink_info_list failed";
			return;
		}
		pa_operation_unref(o);
	}
}

DeviceList DeviceInfo::paDevices() {
	qDebug("DeviceInfo::paDevices");
	DeviceList l;

	if (!pulse_list_done) {
		pa_glib_mainloop *mainloop = pa_glib_mainloop_new(NULL);
		pa_mainloop_api *api = pa_glib_mainloop_get_api(mainloop);
		pa_context *context = pa_context_new(api, "smplayer");

		if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL, 0) < 0) {
			pa_context_unref(context);
			return l;
		}
		pa_context_set_state_callback(context, context_state_cb, NULL);

		// Wait for a while until the list is complete
		for (int n = 0; n < 100; n++) {
			QApplication::processEvents();
			if (pulse_list_done) break;
		}
	}

	if (!pulse_list_done) {
		qWarning("DeviceInfo::paDevices: list is not done yet!");
	}

	QMapIterator<int, QString> i(pulse_device_list);
	while (i.hasNext()) {
		i.next();
		l.append(DeviceData(i.key(), i.value()));
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
