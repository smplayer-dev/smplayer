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

#include "autohidewidget.h"
#include <QTimer>
#include <QEvent>
#include <QVBoxLayout>
#include <QDebug>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

AutohideWidget::AutohideWidget(QWidget * parent)
	: QWidget(parent)
	, turned_on(false)
	, auto_hide(false)
	, use_animation(false)
	, spacing(0)
	, perc_width(100)
	, internal_widget(0)
	, timer(0)
#if QT_VERSION >= 0x040600
	, animation(0)
#endif
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	setLayoutDirection(Qt::LeftToRight);

	parent->installEventFilter(this);
	installFilter(parent);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkUnderMouse()));
	timer->setInterval(3000);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}

AutohideWidget::~AutohideWidget() {
#if QT_VERSION >= 0x040600
	if (animation) delete animation;
#endif
}

void AutohideWidget::setInternalWidget(QWidget * w) {
	layout()->addWidget(w);
	internal_widget = w;
}

void AutohideWidget::installFilter(QObject *o) {
	QObjectList children = o->children();
	for (int n=0; n < children.count(); n++) {
		if (children[n]->isWidgetType()) {
			qDebug() << "AutohideWidget::installFilter: child name:" << children[n]->objectName();
			QWidget *w = static_cast<QWidget *>(children[n]);
			w->setMouseTracking(true);
			w->installEventFilter(this);
			installFilter(children[n]);
		}
	}
}

void AutohideWidget::activate() {
	turned_on = true;
	timer->start();
}

void AutohideWidget::deactivate() {
	turned_on = false;
	timer->stop();
	hide();
}

void AutohideWidget::show() {
	qDebug() << "AutohideWidget::show";
	resizeAndMove();

	if (use_animation) {
		showAnimated();
	} else {
		QWidget::show();
	}

	// Restart timer
	if (timer->isActive()) timer->start();
}

void AutohideWidget::setAutoHide(bool b) {
	auto_hide = b;
}

void AutohideWidget::checkUnderMouse() {
	if (auto_hide) {
		//qDebug("AutohideWidget::checkUnderMouse");
		if ((isVisible()) && (!underMouse())) hide();
	}
}

void AutohideWidget::resizeAndMove() {
	QWidget * widget = parentWidget();
	int w = widget->width() * perc_width / 100;
	int h = height();
	resize(w, h);

	int x = (widget->width() - width() ) / 2;
	int y = widget->height() - height() - spacing;
	move(x, y);
}

bool AutohideWidget::eventFilter(QObject * obj, QEvent * event) {
	if (turned_on) {
		//qDebug() << "AutohideWidget::eventFilter: obj:" << obj << "type:" << event->type();
		if (event->type() == QEvent::MouseMove) {
			//qDebug() << "AutohideWidget::eventFilter: mouse move" << obj;
			if (!isVisible()) show();
		}
	}

	return QWidget::eventFilter(obj, event);
}

void AutohideWidget::showAnimated() {
#if QT_VERSION >= 0x040600
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
#else
	QWidget::show();
#endif
}

#include "moc_autohidewidget.cpp"

