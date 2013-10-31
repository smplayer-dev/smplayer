/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef FLOATING_WIDGET2_H
#define FLOATING_WIDGET2_H

#include "editabletoolbar.h"

class FloatingWidget2 : public EditableToolbar
{
	Q_OBJECT

public:
	FloatingWidget2(QWidget * parent = 0);
	~FloatingWidget2();

	//bool isAnimated() { return _animated; };
	bool autoHide() { return auto_hide; };
	int margin() { return spacing; };
	int percWidth() { return perc_width; };

public slots:
	//void setAnimated(bool b) { _animated = b; };
	void setAutoHide(bool b);
	void setMargin(int margin) { spacing = margin; };
	void setPercWidth(int s) { perc_width = s;}

protected:
	bool eventFilter(QObject * obj, QEvent * event);
	void showEvent(QShowEvent * event);

private slots:
	void checkUnderMouse();

private:
	void installFilter(QObject *o);
	void resizeAndMove();

private:
	bool auto_hide;
	int spacing;
	int perc_width;
};

#endif

