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
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QDebug>

FloatingWidget2::FloatingWidget2(QWidget * parent)
	: QWidget(parent)
	, turned_on(false)
	, auto_hide(false)
	, use_animation(false)
	, spacing(0)
	, perc_width(100)
	, internal_widget(0)
	, timer(0)
	, animation(0)
{
	setAutoFillBackground(true);
	parent->installEventFilter(this);
	installFilter(parent);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUnderMouse()));
	timer->setInterval(5000);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}

FloatingWidget2::~FloatingWidget2() {
	if (animation) delete animation;
}

void FloatingWidget2::setInternalWidget(QWidget * w) {
	layout()->addWidget(w);
	internal_widget = w;
}

void FloatingWidget2::installFilter(QObject *o) {
	QObjectList children = o->children();
	for (int n=0; n < children.count(); n++) {
		if (children[n]->isWidgetType()) {
			qDebug() << "FloatingWidget2::installFilter: child name:" << children[n]->objectName();
			QWidget *w = static_cast<QWidget *>(children[n]);
			w->setMouseTracking(true);
			w->installEventFilter(this);
			installFilter(children[n]);
		}
	}
}

void FloatingWidget2::activate() {
	turned_on = true;
	timer->start();
}

void FloatingWidget2::deactivate() {
	turned_on = false;
	timer->stop();
	hide();
}

void FloatingWidget2::show() {
	qDebug() << "FloatingWidget2::show";
	resizeAndMove();

	if (use_animation) {
		showAnimated();
	} else {
		QWidget::show();
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
	if (turned_on) {
		//qDebug() << "FloatingWidget2::eventFilter: obj:" << obj << "type:" << event->type();
		if (event->type() == QEvent::MouseMove) {
			qDebug() << "FloatingWidget2::eventFilter: mouse move" << obj;
			if (!isVisible()) show();
		}
	}

	return QWidget::eventFilter(obj, event);
}

void FloatingWidget2::showAnimated() {
	if (!animation) {
		animation = new QPropertyAnimation(this, "pos");
	}

	QPoint initial_position = QPoint(pos().x(), parentWidget()->size().height());
	QPoint final_position = pos();
	move(initial_position);

	QWidget::show();

	animation->setDuration(300);
	animation->setEasingCurve(QEasingCurve::OutBounce);
	animation->setEndValue(final_position);
	animation->setStartValue(initial_position);
	animation->start();
}

#include "moc_floatingwidget2.cpp"

