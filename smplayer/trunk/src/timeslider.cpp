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

#include "timeslider.h"

#include <stdlib.h>
#include <QApplication>
#include <QStyle>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QTimer>

#if NEW_CODE
#include <QStyle>
#include <QStyleOption>
#endif

#define DEBUG 0

MySlider::MySlider( QWidget * parent ) : QSlider(parent)
{
	setOrientation( Qt::Horizontal );
}

MySlider::~MySlider() {
}

#if NEW_CODE
inline int MySlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

#if QT_VERSION < 0x040300
void MySlider::initStyleOption(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->subControls = QStyle::SC_None;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = orientation();
    option->maximum = maximum();
    option->minimum = minimum();
    option->tickPosition = (QSlider::TickPosition) tickPosition();
    option->tickInterval = tickInterval();
    option->upsideDown = (orientation() == Qt::Horizontal) ?
                     (invertedAppearance() != (option->direction == Qt::RightToLeft))
                     : (!invertedAppearance());
    option->direction = Qt::LeftToRight; // we use the upsideDown option instead
    option->sliderPosition = sliderPosition();
    option->sliderValue = value();
    option->singleStep = singleStep();
    option->pageStep = pageStep();
    if (orientation() == Qt::Horizontal)
        option->state |= QStyle::State_Horizontal;
}
#endif

int MySlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void MySlider::mousePressEvent( QMouseEvent * e ) {
	if (e->button() == Qt::LeftButton) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();
        // to take half of the slider off for the setSliderPosition call we use the center - topLeft

        setSliderPosition(pixelPosToRangeValue(pick(e->pos() - center)));
        triggerAction(SliderMove);
        setRepeatAction(SliderNoAction);
	} else {
		QSlider::mousePressEvent(e);
	}
}

#else

void MySlider::mousePressEvent( QMouseEvent * e ) {
	// FIXME:
	// The code doesn't work well with right to left layout,
	// so it's disabled.
	if (qApp->isRightToLeft()) {
		QSlider::mousePressEvent(e);
		return;
	}

	int range = maximum()-minimum();
	int pos = (e->x() * range) / width();
	//qDebug( "width: %d x: %d", width(), e->x());
	//qDebug( "range: %d pos: %d value: %d", range, pos, value());

	// Calculate how many positions takes the slider handle
	int metric = qApp->style()->pixelMetric( QStyle::PM_SliderLength );
	double one_tick_pixels = (double) width() / range;
	int slider_handle_positions = (int) (metric / one_tick_pixels);

	/*
	qDebug("metric: %d", metric );
	qDebug("one_tick_pixels :%f", one_tick_pixels);
	qDebug("width() :%d", width());
	qDebug("slider_handle_positions: %d", slider_handle_positions);
	*/

	if (abs(pos - value()) > slider_handle_positions) { 
		setValue(pos);
		emit sliderMoved( pos );
	} else {
		QSlider::mousePressEvent(e);
	}
}
#endif


TimeSlider::TimeSlider( QWidget * parent ) : MySlider(parent)
{
	dont_update = FALSE;
	setMinimum(0);
	setMaximum(100);

	setFocusPolicy( Qt::NoFocus );
	setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Fixed );

	connect( this, SIGNAL( sliderPressed() ), this, SLOT( stopUpdate() ) );
	connect( this, SIGNAL( sliderReleased() ), this, SLOT( resumeUpdate() ) );
	connect( this, SIGNAL( sliderReleased() ), this, SLOT( mouseReleased() ) );
	connect( this, SIGNAL( valueChanged(int) ), this, SLOT( valueChanged_slot(int) ) );
#if ENABLE_DELAYED_DRAGGING
	connect( this, SIGNAL(draggingPos(int) ), this, SLOT(checkDragging(int)) );
	
	last_pos_to_send = -1;
	timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), this, SLOT(sendDelayedPos()) );
	timer->start(200);
#endif
}

TimeSlider::~TimeSlider() {
}

void TimeSlider::stopUpdate() {
	#if DEBUG
	qDebug("TimeSlider::stopUpdate");
	#endif
	dont_update = TRUE;
}

void TimeSlider::resumeUpdate() {
	#if DEBUG
	qDebug("TimeSlider::resumeUpdate");
	#endif
	dont_update = FALSE;
}

void TimeSlider::mouseReleased() {
	#if DEBUG
	qDebug("TimeSlider::mouseReleased");
	#endif
	emit posChanged( value() );
}

void TimeSlider::valueChanged_slot(int v) {
	#if DEBUG
	qDebug("TimeSlider::changedValue_slot: %d", v);
	#endif

	// Only to make things clear:
	bool dragging = dont_update;
	if (!dragging) {
		if (v!=position) {
			#if DEBUG
			qDebug(" emitting posChanged");
			#endif
			emit posChanged(v);
		}
	} else {
		#if DEBUG
		qDebug(" emitting draggingPos");
		#endif
		emit draggingPos(v);
	}
}

#if ENABLE_DELAYED_DRAGGING
void TimeSlider::setDragDelay(int d) {
	qDebug("TimeSlider::setDragDelay: %d", d);
	timer->setInterval(d);
}

int TimeSlider::dragDelay() {
	return timer->interval();
}

void TimeSlider::checkDragging(int v) {
	qDebug("TimeSlider::checkDragging: %d", v);
	last_pos_to_send = v;
}

void TimeSlider::sendDelayedPos() {
	if (last_pos_to_send != -1) {
		qDebug("TimeSlider::sendDelayedPos: %d", last_pos_to_send);
		emit delayedDraggingPos(last_pos_to_send);
		last_pos_to_send = -1;
	}
}
#endif

void TimeSlider::setPos(int v) {
	#if DEBUG
	qDebug("TimeSlider::setPos: %d", v);
	qDebug(" dont_update: %d", dont_update);
	#endif

	if (v!=pos()) {
		if (!dont_update) {
			position = v;
			setValue(v);
		}
	}
}

int TimeSlider::pos() {
	return position;
}

void TimeSlider::wheelEvent( QWheelEvent * e ) {
	e->ignore();
}


#include "moc_timeslider.cpp"
