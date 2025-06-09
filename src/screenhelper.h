/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef SCREENHELPER_H
#define SCREENHELPER_H

#include <QObject>
#include <QPoint>

class QWidget;
class QMouseEvent;
class QEvent;
class QTimer;

class ScreenHelper : public QObject
{
	Q_OBJECT

public:
	ScreenHelper(QWidget* parent);
	~ScreenHelper();

	void setAutoHideCursor(bool b);
	bool autoHideCursor() { return autohide_cursor; };

	void setAutoHideInterval(int milliseconds) { autohide_interval = milliseconds; };
	int autoHideInterval() { return autohide_interval; };

public slots:
	//! Should be called when a file has started.
	virtual void playingStarted();

	//! Should be called when a file has stopped.
	virtual void playingStopped();

signals:
	void mouseMoved(QPoint);

protected:
	bool eventFilter(QObject * obj, QEvent * event);

protected slots:
	virtual void checkMousePos();

protected:
	QWidget * widget;
	QTimer * check_mouse_timer;
	QPoint mouse_last_position;
	bool autohide_cursor;
	int autohide_interval;
};

#endif
