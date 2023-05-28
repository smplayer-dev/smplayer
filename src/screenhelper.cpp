/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#include "screenhelper.h"
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

ScreenHelper::ScreenHelper(QWidget* parent)
	: widget(parent)
	, check_mouse_timer(0)
	, mouse_last_position(QPoint(0,0))
	, autohide_cursor(false)
	, autohide_interval(0)
{
	widget->setMouseTracking(true);
	widget->setFocusPolicy( Qt::NoFocus );
	widget->setMinimumSize( QSize(0,0) );
	widget->installEventFilter(this);

	check_mouse_timer = new QTimer(this);
	connect(check_mouse_timer, SIGNAL(timeout()), this, SLOT(checkMousePos())) ;

	setAutoHideInterval(1000);
	setAutoHideCursor(false);
}

ScreenHelper::~ScreenHelper() {
}

void ScreenHelper::setAutoHideCursor(bool b) {
	qDebug("ScreenHelper::setAutoHideCursor: %d", b);

	autohide_cursor = b;
	if (autohide_cursor) {
		check_mouse_timer->setInterval(autohide_interval);
		check_mouse_timer->start();
	} else {
		check_mouse_timer->stop();
	}
}

void ScreenHelper::checkMousePos() {
	//qDebug("ScreenHelper::checkMousePos");

	if (!autohide_cursor) {
		widget->setCursor(QCursor(Qt::ArrowCursor));
		return;
	}

	QPoint pos = widget->mapFromGlobal(QCursor::pos());

	//qDebug("ScreenHelper::checkMousePos: x: %d, y: %d", pos.x(), pos.y());

	if (mouse_last_position != pos) {
		widget->setCursor(QCursor(Qt::ArrowCursor));
	} else {
		widget->setCursor(QCursor(Qt::BlankCursor));
	}
	mouse_last_position = pos;
}

bool ScreenHelper::eventFilter(QObject * obj, QEvent * event) {
	if (obj == widget && event->type() == QEvent::MouseMove) {
		QMouseEvent * mouse_event = static_cast<QMouseEvent*>(event);
		emit mouseMoved(mouse_event->pos());

		if (widget->cursor().shape() != Qt::ArrowCursor) {
			//qDebug(" showing mouse cursor" );
			widget->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
	return QObject::eventFilter(obj, event);
}

void ScreenHelper::playingStarted() {
	qDebug("ScreenHelper::playingStarted");
	setAutoHideCursor(true);
}

void ScreenHelper::playingStopped() {
	qDebug("ScreenHelper::playingStopped");
	setAutoHideCursor(false);

#ifdef Q_OS_WIN
	// For an unknown reason the cursor remains in the wait state after stop
	// this sets it to normal
	widget->unsetCursor();
#endif
}

#include "moc_screenhelper.cpp"
