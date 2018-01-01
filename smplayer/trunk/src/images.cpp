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

#include "images.h"
#include <QFile>
#include <QDebug>

#ifdef USE_RESOURCES
#include <QResource>
#endif

#ifdef SMCODE
#include "global.h"
#include "preferences.h"
#include "paths.h"
using namespace Global;
#endif

QString Images::current_theme;
QString Images::themes_path;

bool Images::is_internal = false;

#ifdef USE_RESOURCES
QString Images::last_resource_loaded;
bool Images::has_rcc = false;

QString Images::resourceFilename() {
	QString filename = QString::null;

	if ((!themes_path.isEmpty()) && (!current_theme.isEmpty())) {
		filename = themes_path +"/"+ current_theme +"/"+ current_theme +".rcc";
	}

	qDebug() << "Images::resourceFilename:" << filename;

	return filename;
}
#endif

void Images::setTheme(const QString & name) {
	current_theme = name;

	is_internal = (QFile::exists(":/" + current_theme + "/style.qss"));
	if (is_internal) {
		qDebug() << "Images::setTheme:" << current_theme << "is an internal theme";
		setThemesPath("");
		#ifdef USE_RESOURCES
		has_rcc = false;
		#endif
		return;
	}

#ifdef SMCODE
	QString dir = Paths::configPath() + "/themes/" + name;
	if (QFile::exists(dir)) {
		setThemesPath(Paths::configPath() + "/themes/");
	} else {
		setThemesPath(Paths::themesPath());
	}
#endif

#ifdef USE_RESOURCES
	if (!last_resource_loaded.isEmpty()) {
		qDebug() << "Images::setTheme: unloading" << last_resource_loaded;
		QResource::unregisterResource(last_resource_loaded);
		last_resource_loaded = QString::null;
	}

	QString rs_file = resourceFilename();
	if ((!rs_file.isEmpty()) && (QFile::exists(rs_file))) {
		qDebug() << "Images::setTheme: loading" << rs_file;
		QResource::registerResource(rs_file);
		last_resource_loaded = rs_file;
		has_rcc = true;
	} else {
		has_rcc = false;
	}
	qDebug() << "Images::setTheme: has_rcc:" << has_rcc;
#endif
}

void Images::setThemesPath(const QString & folder) {
	themes_path = folder;
	qDebug() << "Images::setThemesPath:" << themes_path;
}

QString Images::file(const QString & name) {
#ifdef SMCODE
	if (current_theme != pref->iconset) {
		setTheme(pref->iconset);
	}
#endif

	QString icon_name;
	if (!current_theme.isEmpty()) {
	#ifdef USE_RESOURCES
		if (has_rcc || is_internal) {
			icon_name = ":/" + current_theme + "/"+ name;
		} else {
			icon_name = themes_path +"/"+ current_theme + "/"+ name;
		}
	#else
		if (is_internal) {
			icon_name = ":/" + current_theme + "/"+ name;
		} else {
			icon_name = themes_path +"/"+ current_theme + "/"+ name;
		}
	#endif
	}

	bool has_extension = name.contains(".");
	if (!has_extension) icon_name += ".png";

	//qDebug() << "Images::file:" << icon_name;
	if ((icon_name.isEmpty()) || (!QFile::exists(icon_name))) {
		icon_name = ":/default-theme/" + name;
		if (!has_extension) icon_name += ".png";
	}

	//qDebug() << "Images::file:" << icon_name;
	return icon_name;
}


QPixmap Images::icon(QString name, int size) {
	QString icon_name = file(name);
	QPixmap p(icon_name);

	if (!p.isNull()) {
		if (size != -1) {
			p = resize(&p, size);
		}
	}

	return p;
}

QPixmap Images::resize(QPixmap *p, int size) {
	return QPixmap::fromImage( (*p).toImage().scaled(size,size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation) );
}

QPixmap Images::flip(QPixmap *p) {
	return QPixmap::fromImage( (*p).toImage().mirrored(true, false) );
}

QPixmap Images::flippedIcon(QString name, int size) {
	QPixmap p = icon(name, size);
	p = flip(&p);
	return p;
}

#ifdef SMCODE
QString Images::styleSheet(){
	QString filename;
	filename = themesDirectory() + "/main.css";
	QFile file(filename);
	if (file.exists()) {
		file.open(QFile::ReadOnly | QFile::Text);
		QString css = QString::fromUtf8(file.readAll().constData());
		return css;
	}
	else
		return "";
}

QString Images::themesDirectory(){
	QString skin = pref->iconset;
	QString dirname;
	if (!skin.isEmpty()) {
		dirname = Paths::configPath() + "/themes/" + skin;
		if (!QFile::exists(dirname)) {
			dirname = Paths::themesPath() + "/" + skin ;
		}
	}
	return dirname;
}
#endif
