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

#ifndef AUTOHIDEWIDGET_H
#define AUTOHIDEWIDGET_H

#include <QWidget>

class QTimer;
class QPropertyAnimation;

class AutohideWidget : public QWidget
{
	Q_OBJECT

public:
	enum Activation { Anywhere = 1, Bottom = 2 };

	AutohideWidget(QWidget * parent = 0);
	~AutohideWidget();

	void setInternalWidget(QWidget * w);
	QWidget * internalWidget() { return internal_widget; };

public slots:
	void show();
	void activate();
	void deactivate();
	void setAutoHide(bool b);
	void setAnimated(bool b) { use_animation = b; };
	void setMargin(int margin) { spacing = margin; };
	void setPercWidth(int s) { perc_width = s;}
	void setActivationArea(Activation m) { activation_area = m; }
	void setHideDelay(int ms);

public:
	bool isActive() { return turned_on; };
	bool autoHide() { return auto_hide; };
	bool isAnimated() { return use_animation; };
	int margin() { return spacing; };
	int percWidth() { return perc_width; };
	Activation activationArea() { return activation_area; }
	int hideDelay();

protected:
	bool eventFilter(QObject * obj, QEvent * event);

private slots:
	void checkUnderMouse();
	void showAnimated();

private:
	void installFilter(QObject *o);
	void resizeAndMove();
	bool visiblePopups();

private:
	bool turned_on;
	bool auto_hide;
	bool use_animation;
	int spacing;
	int perc_width;
	Activation activation_area;
	QWidget * internal_widget;
	QTimer * timer;
#if QT_VERSION >= 0x040600
	QPropertyAnimation * animation;
#endif
};

#endif

