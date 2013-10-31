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

#include "floatingwidget2.h"
#include <QTimer>
#include <QEvent>
#include <QDebug>

FloatingWidget2::FloatingWidget2(QWidget * parent)
	: EditableToolbar(parent)
	, auto_hide(false)
	, spacing(0)
	, perc_width(100)
{
	setAutoFillBackground(true);
	parent->installEventFilter(this);
	installFilter(parent);

	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUnderMouse()));
	timer->setInterval(5000);
	timer->start();
}

FloatingWidget2::~FloatingWidget2() {
}

void FloatingWidget2::installFilter(QObject *o) {
	QObjectList children = o->children();
	for (int n=0; n < children.count(); n++) {
		if (children[n]->isWidgetType()) {
			qDebug() << "FloatingWidget2::installFilter: child name:" << children[n]->objectName();
			children[n]->installEventFilter(this);
			installFilter(children[n]);
		}
	}
}

void FloatingWidget2::setAutoHide(bool b) {
	auto_hide = b;
}

void FloatingWidget2::checkUnderMouse() {
	if (auto_hide) {
		qDebug("FloatingWidget2::checkUnderMouse");
		if ((isVisible()) && (!underMouse())) hide();
	}
}

void FloatingWidget2::resizeAndMove() {
	QWidget * widget = parentWidget();
	int w = widget->width() * perc_width / 100;
	int h = height();
	resize(w, h);

	int x = (widget->width() - width() ) / 2;
	int y = widget->height() - height() - spacing;
	move(x, y);
}

bool FloatingWidget2::eventFilter(QObject * obj, QEvent * event) {
	//qDebug() << "FloatingWidget2::eventFilter: obj:" << obj << "type:" << event->type();
	if (event->type() == QEvent::Resize) {
		qDebug() << "FloatingWidget2::eventFilter: resize";
		resizeAndMove();
	}
	else
	if (event->type() == QEvent::MouseMove) {
		qDebug() << "FloatingWidget2::eventFilter: move";
	}
	return EditableToolbar::eventFilter(obj, event);
}

#include "moc_floatingwidget2.cpp"

