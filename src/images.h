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

#ifndef IMAGES_H
#define IMAGES_H

#include <QString>
#include <QPixmap>
#include <QIcon>

#define USE_RESOURCES
#define SMCODE

class Images
{

public:
	static void setThemesPath(const QString & folder);
	static void setTheme(const QString & name);

	static QPixmap icon(QString name, int size=-1);
	static QPixmap flippedIcon(QString name, int size=-1);

	//! Returns the filename of the icon
	static QString file(const QString & icon_name);

#ifdef SMCODE
	static QString styleSheet();
	static QString themesDirectory();
#endif

	static bool is_internal;

#ifdef USE_RESOURCES
	static bool has_rcc;
#endif

private:
	static QPixmap resize(QPixmap *p, int size=20);
	static QPixmap flip(QPixmap *p);

	static QString current_theme;
	static QString themes_path;

#ifdef USE_RESOURCES
	static QString resourceFilename();
	static QString last_resource_loaded;
#endif
};

#endif
