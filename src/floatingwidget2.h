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

class QTimer;
class QPropertyAnimation;

class FloatingWidget2 : public EditableToolbar
{
	Q_OBJECT

public:
	FloatingWidget2(QWidget * parent = 0);
	~FloatingWidget2();

public slots:
	void show();
	void activate();
	void deactivate();
	void setAutoHide(bool b);
	void setAnimated(bool b) { use_animation = b; };
	void setMargin(int margin) { spacing = margin; };
	void setPercWidth(int s) { perc_width = s;}

public:
	bool isActive() { return turned_on; };
	bool autoHide() { return auto_hide; };
	bool isAnimated() { return use_animation; };
	int margin() { return spacing; };
	int percWidth() { return perc_width; };

protected:
	bool eventFilter(QObject * obj, QEvent * event);

private slots:
	void checkUnderMouse();
	void showAnimated();

private:
	void installFilter(QObject *o);
	void resizeAndMove();

private:
	bool turned_on;
	bool auto_hide;
	bool use_animation;
	int spacing;
	int perc_width;
	QTimer * timer;
	QPropertyAnimation * animation;
};

#endif

