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

#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <QString>

#ifndef Q_OS_WIN
#define COLOR_OUTPUT_SUPPORT 1
#endif

//#define CHANGE_WIDGET_COLOR

class QWidget;
class QColor;

class ColorUtils {

public:
#if 0
	static QString colorToRGB(unsigned int color);
	static QString colorToRRGGBBAA(unsigned int color);
	static QString colorToRRGGBB(unsigned int color);
	static QString colorToAABBGGRR(unsigned int color);
#endif

	static QString colorToRGB(QColor c);
	static QString colorToRRGGBBAA(QColor c);
	static QString colorToRRGGBB(QColor c);
	static QString colorToAABBGGRR(QColor c);
	static QString colorToAARRGGBB(QColor c);

	static QColor AARRGGBBToColor(const QString & s);

#ifdef CHANGE_WIDGET_COLOR
	//! Changes the foreground color of the specified widget
	static void setForegroundColor(QWidget * w, const QColor & color);

	//! Changes the background color of the specified widget
	static void setBackgroundColor(QWidget * w, const QColor & color);
#endif

    /**
     ** \brief Strip colors and tags from MPlayer output lines
     **
     ** Some MPlayer configurations (configured with --enable-color-console)
     ** use colored/tagged console output. This function removes those colors
     ** and tags.
     **
     ** \param s The string to strip colors and tags from
     ** \return Returns a clean string (no colors, no tags)
     */
#if COLOR_OUTPUT_SUPPORT
    static QString stripColorsTags(QString s);
#endif

};

#endif
