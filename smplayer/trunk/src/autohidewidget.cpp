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

#include "autohidewidget.h"
#include <QTimer>
#include <QEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QDebug>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

#define HANDLE_TAP_EVENT

#ifdef HANDLE_TAP_EVENT
#include <QGestureEvent>
#endif

AutohideWidget::AutohideWidget(QWidget * parent)
	: QWidget(parent)
	, turned_on(false)
	, auto_hide(false)
	, use_animation(false)
	, spacing(0)
	, perc_width(100)
	, activation_area(Anywhere)
	, internal_widget(0)
	, timer(0)
#if QT_VERSION >= 0x040600
	, animation(0)
#endif
{
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	setLayoutDirection(Qt::LeftToRight);

	QWidget * widget_to_watch = parent;
	widget_to_watch->installEventFilter(this);
	installFilter(widget_to_watch);

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
	//qDebug() << "AutohideWidget::setInternalWidget:" << w;
	layout()->addWidget(w);
	internal_widget = w;
}

void AutohideWidget::setHideDelay(int ms) {
	timer->setInterval(ms);
}

int AutohideWidget::hideDelay() {
	return timer->interval();
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
			#ifdef HANDLE_TAP_EVENT
			//w->grabGesture(Qt::TapGesture);
			#endif
		}
	}
}

bool AutohideWidget::visiblePopups() {
	//qDebug() << "AutohideWidget::visiblePopups: internal_widget:" << internal_widget;
	if (!internal_widget) return false;

	// Check if any of the menus in the internal widget is visible
	QObjectList children = internal_widget->children();
	foreach(QObject * child, children) {
		if (child->isWidgetType()) {
			//qDebug() << "AutohideWidget::visiblePopups:" << child << "child name:" << child->objectName();
			QWidget *w = static_cast<QWidget *>(child);

			QList<QAction *> actions = w->actions();
			foreach(QAction * action, actions) {
				//qDebug() << "AutohideWidget::visiblePopups: action:" << action;

				QList<QWidget *> aw = action->associatedWidgets();
				//qDebug() << "AutohideWidget::visiblePopups: aw:" << aw;

				QMenu * menu = 0;
				foreach(QWidget * widget, aw) {
					//qDebug() << "AutohideWidget::visiblePopups: widget:" << widget;
					if ((menu = qobject_cast<QMenu *>(widget))) {
						//qDebug() << "AutohideWidget::visiblePopups: menu:" << menu << "visible:" << menu->isVisible();
						if (menu->isVisible()) return true;
					}
				}

				menu = action->menu();
				if (menu) {
					//qDebug() << "AutohideWidget::visiblePopups: menu:" << menu << "visible:" << menu->isVisible();
					if (menu->isVisible()) return true;
				}
			}
		}
	}
	return false;
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
		if (isVisible() && !underMouse() && !visiblePopups()) {
			hide();
		}
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
		#ifdef HANDLE_TAP_EVENT
		if (event->type() == QEvent::Gesture) {
			qDebug() << "AutohideWidget::eventFilter: obj:" << obj << "gesture:" << event;
			QGestureEvent * gesture_event = static_cast<QGestureEvent*>(event);
			if (gesture_event->gesture(Qt::TapGesture)) {
				qDebug() << "AutohideWidget::eventFilter: tap event detected";
				if (!isVisible()) show(); //else hide();
				event->setAccepted(true);
				return true;
			} 
		}
		else
		#endif
		if (event->type() == QEvent::MouseMove) {
			//qDebug() << "AutohideWidget::eventFilter: mouse move" << obj;
			if (!isVisible()) {
				if (activation_area == Anywhere) {
					show();
				} else {
					QMouseEvent * mouse_event = dynamic_cast<QMouseEvent*>(event);
					QWidget * parent = parentWidget();
					QPoint p = parent->mapFromGlobal(mouse_event->globalPos());
					//qDebug() << "AutohideWidget::eventFilter: y:" << p.y();
					if (p.y() > (parent->height() - height() - spacing)) {
						show();
					}
				}
			}
		}
		else
		if (event->type() == QEvent::MouseButtonRelease && obj == this) {
			event->setAccepted(true);
			return true;
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

