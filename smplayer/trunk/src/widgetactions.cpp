/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "widgetactions.h"
#include "helper.h"
#include <QLabel>

#if MINI_ARROW_BUTTONS
#include <QToolButton>
#endif

MyWidgetAction::MyWidgetAction( QWidget * parent )
	: QWidgetAction(parent)
{
}

MyWidgetAction::~MyWidgetAction() {
}

void MyWidgetAction::enable() {
	propagate_enabled(true);
}

void MyWidgetAction::disable() {
	propagate_enabled(false);
}

void MyWidgetAction::propagate_enabled(bool b) {
	QList<QWidget *> l = createdWidgets();
	for (int n=0; n < l.count(); n++) {
		TimeSlider *s = (TimeSlider*) l[n];
		s->setEnabled(b);;
	}
	setEnabled(b);
}


TimeSliderAction::TimeSliderAction( QWidget * parent )
	: MyWidgetAction(parent)
{
#if ENABLE_DELAYED_DRAGGING
	drag_delay = 200;
#endif
}

TimeSliderAction::~TimeSliderAction() {
}

void TimeSliderAction::setPos(int v) {
	QList<QWidget *> l = createdWidgets();
	for (int n=0; n < l.count(); n++) {
		TimeSlider *s = (TimeSlider*) l[n];
		s->setPos(v);
	}
}

int TimeSliderAction::pos() {
	QList<QWidget *> l = createdWidgets();
	if (l.count() >= 1) {
		TimeSlider *s = (TimeSlider*) l[0];
		return s->pos();
	} else {
		return -1;
	}
}

#if ENABLE_DELAYED_DRAGGING
void TimeSliderAction::setDragDelay(int d) {
	drag_delay = d;

	QList<QWidget *> l = createdWidgets();
	for (int n=0; n < l.count(); n++) {
		TimeSlider *s = (TimeSlider*) l[n];
		s->setDragDelay(drag_delay);
	}
}

int TimeSliderAction::dragDelay() {
	return drag_delay;
}
#endif

QWidget * TimeSliderAction::createWidget ( QWidget * parent ) {
	TimeSlider *t = new TimeSlider(parent);
	t->setEnabled( isEnabled() );

	connect( t,    SIGNAL(posChanged(int)), 
             this, SIGNAL(posChanged(int)) );
	connect( t,    SIGNAL(draggingPos(int)),
             this, SIGNAL(draggingPos(int)) );
#if ENABLE_DELAYED_DRAGGING
	t->setDragDelay(drag_delay);

	connect( t,    SIGNAL(delayedDraggingPos(int)),
	         this, SIGNAL(delayedDraggingPos(int)) );
#endif

	return t;
}


VolumeSliderAction::VolumeSliderAction( QWidget * parent )
	: MyWidgetAction(parent)
{
}

VolumeSliderAction::~VolumeSliderAction() {
}

void VolumeSliderAction::setValue(int v) {
	QList<QWidget *> l = createdWidgets();
	for (int n=0; n < l.count(); n++) {
		MySlider *s = (MySlider*) l[n];
		s->setValue(v);
	}
}

int VolumeSliderAction::value() {
	QList<QWidget *> l = createdWidgets();
	if (l.count() >= 1) {
		MySlider *s = (MySlider*) l[0];
		return s->value();
	} else {
		return -1;
	}
}

QWidget * VolumeSliderAction::createWidget ( QWidget * parent ) {
	MySlider *t = new MySlider(parent);
	t->setMinimum(0);
	t->setMaximum(100);
	t->setValue(50);
	t->setOrientation( Qt::Horizontal );
	t->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	t->setFocusPolicy( Qt::NoFocus );
	t->setTickPosition( QSlider::TicksBelow );
	t->setTickInterval( 10 );
	t->setSingleStep( 1 );
	t->setPageStep( 10 );
	t->setToolTip( tr("Volume") );
	t->setEnabled( isEnabled() );
	t->setAttribute(Qt::WA_NoMousePropagation);

	connect( t,    SIGNAL(valueChanged(int)), 
             this, SIGNAL(valueChanged(int)) );
	return t;
}


TimeLabelAction::TimeLabelAction( QWidget * parent )
	: MyWidgetAction(parent)
{
}

TimeLabelAction::~TimeLabelAction() {
}

void TimeLabelAction::setText(QString s) {
	_text = s;
	emit newText(s);
}

QWidget * TimeLabelAction::createWidget ( QWidget * parent ) {
	QLabel * time_label = new QLabel(parent);
    time_label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    time_label->setAutoFillBackground(true);

    Helper::setBackgroundColor( time_label, QColor(0,0,0) );
    Helper::setForegroundColor( time_label, QColor(255,255,255) );
    time_label->setText( "00:00:00 / 00:00:00" );
    time_label->setFrameShape( QFrame::Panel );
    time_label->setFrameShadow( QFrame::Sunken );

	connect( this, SIGNAL(newText(QString)), 
             time_label, SLOT(setText(QString)) );

	return time_label;
}

#if MINI_ARROW_BUTTONS
SeekingButton::SeekingButton( QList<QAction*> actions, QWidget * parent ) 
	: QWidgetAction(parent)
{
	_actions = actions;
}

SeekingButton::~SeekingButton() {
}

QWidget * SeekingButton::createWidget( QWidget * parent ) {
	QToolButton * button = new QToolButton(parent);
	button->setPopupMode(QToolButton::MenuButtonPopup);

	if (_actions.count() > 0 ) {
		button->setDefaultAction( _actions[0] );
	}
	for (int n = 1; n < _actions.count(); n++) {
		button->addAction( _actions[n] );
	}

	return button;
}
#endif

#include "moc_widgetactions.cpp"
