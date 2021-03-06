/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef _FLOATING_WIDGET_H_
#define _FLOATING_WIDGET_H_

#include <QWidget>
#include <QTimer>

class EditableToolbar;

#if QT_VERSION < 0x040600
#define OLD_ANIMATION
#endif

#ifndef OLD_ANIMATION
class QPropertyAnimation;
#endif

class FloatingWidget : public QWidget
{
	Q_OBJECT

public:
	enum Place { Top = 0, Bottom = 1 };
	enum Movement { Upward = 0, Downward = 1 };

	FloatingWidget(QWidget * parent = 0);
	~FloatingWidget();

	//! Show the floating widget over the specified widget.
	void showOver(QWidget * widget, int size = 100, Place place = Bottom);

	void showAnimated(QPoint final_position, Movement movement);

	EditableToolbar * toolbar() { return tb; };

	bool isAnimated() { return _animated; };
	bool autoHide() { return auto_hide; };
	int margin() { return _margin; };

public slots:
	void setAnimated(bool b) { _animated = b; };
	void setAutoHide(bool b);
	void setMargin(int margin) { _margin = margin; };
#ifndef Q_OS_WIN
	void setBypassWindowManager(bool b);
#endif

protected:
	EditableToolbar * tb;

private slots:
#ifdef OLD_ANIMATION
	void animate();
#endif
	void checkUnderMouse();

private:
	// Animation variables
	bool _animated;
#ifdef OLD_ANIMATION
	QTimer * animation_timer;
#endif
	int final_y;
	int current_y;
	Movement current_movement;

	bool auto_hide;
	QTimer auto_hide_timer;

	int _margin;

#ifndef OLD_ANIMATION
	QPropertyAnimation * animation;
#endif

};

#endif
