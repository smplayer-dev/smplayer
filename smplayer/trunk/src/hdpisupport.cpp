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

#include "hdpisupport.h"
#include <QSettings>
#include <QApplication>

//#define DEBUG

#ifdef DEBUG
#include <QDebug>
#endif

HDPISupport * HDPISupport::instance_obj = 0;

HDPISupport * HDPISupport::instance() {
	if (instance_obj == 0) {
		instance_obj = new HDPISupport();
	}
	return instance_obj;
}

HDPISupport::HDPISupport(const QString & config_path)
#ifdef Q_OS_WIN
	: enabled(true)
#else
	: enabled(false)
#endif
	, auto_scale(true)
	, scale_factor(1)
	, pixel_ratio(2)
{
	instance_obj = this;

#ifdef HDPI_STORE_DATA
	set = 0;
	setConfigPath(config_path);
#else
	apply();
#endif
}

HDPISupport::~HDPISupport() {
#ifdef HDPI_STORE_DATA
	if (set) {
		save();
		delete set;
	}
#endif
	instance_obj = 0;
}

#ifdef HDPI_STORE_DATA
void HDPISupport::setConfigPath(const QString & config_path) {
	#ifdef DEBUG
	qDebug() << "HDPISupport::setConfigPath:" << config_path;
	#endif

	if (set) {
		delete set;
		set = 0;
	}

	if (!config_path.isEmpty()) {
		QString inifile = config_path + "/hdpi.ini";
		#ifdef DEBUG
		qDebug() << "HDPISupport::setConfigPath: ini file:" << inifile;
		#endif
		set = new QSettings(inifile, QSettings::IniFormat);
		load();
	}

	apply();
}

bool HDPISupport::load() {
	#ifdef DEBUG
	qDebug("HDPISupport::load");
	#endif

	if (!set) return false;

	set->beginGroup("hdpisupport");
	enabled = set->value("enabled", enabled).toBool();
	auto_scale = set->value("auto_scale", auto_scale).toBool();
	scale_factor = set->value("scale_factor", scale_factor).toDouble();
	pixel_ratio = set->value("pixel_ratio", pixel_ratio).toInt();
	set->endGroup();

	return true;
}

bool HDPISupport::save() {
	#ifdef DEBUG
	qDebug("HDPISupport::save");
	#endif

	if (!set) return false;

	set->beginGroup("hdpisupport");
	set->setValue("enabled", enabled);
	set->setValue("auto_scale", auto_scale);
	set->setValue("scale_factor", scale_factor);
	set->setValue("pixel_ratio", pixel_ratio);
	set->endGroup();

	return true;
}
#endif

void HDPISupport::apply() {
	#ifdef DEBUG
	qDebug("HDPISupport::apply");
	#endif

	if (enabled) {
		#if QT_VERSION >= 0x050400 && QT_VERSION < 0x050600
		if (qgetenv("QT_DEVICE_PIXEL_RATIO").isEmpty()) {
			qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("auto"));
		}
		if (!auto_scale) {
			qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray::number(pixel_ratio));
		}
		#elif QT_VERSION >= 0x050600
		QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
		if (!auto_scale) {
			qputenv("QT_SCALE_FACTOR", QByteArray::number(scale_factor));
		}
		#endif
	}
}
