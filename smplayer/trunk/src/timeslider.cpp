/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "helper.h"

#include <QWheelEvent>
#include <QTimer>
#include <QToolTip>
#include <QStyleOption>
#include <QDebug>

#define DEBUG 0

TimeSlider::TimeSlider( QWidget * parent ) : MySlider(parent)
	, dont_update(false)
	, position(0)
	, total_time(0)
{
	setMinimum(0);
#ifdef SEEKBAR_RESOLUTION
	setMaximum(SEEKBAR_RESOLUTION);
#else
	setMaximum(100);
#endif

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
	dont_update = true;
}

void TimeSlider::resumeUpdate() {
	#if DEBUG
	qDebug("TimeSlider::resumeUpdate");
	#endif
	dont_update = false;
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

void TimeSlider::wheelEvent(QWheelEvent * e) {
	//e->ignore();

	qDebug("TimeSlider::wheelEvent: delta: %d", e->delta());
	e->accept();

	if (e->orientation() == Qt::Vertical) {
		if (e->delta() >= 0)
			emit wheelUp();
		else
			emit wheelDown();
	} else {
		qDebug("Timeslider::wheelEvent: horizontal event received, doing nothing");
	}
}

bool TimeSlider::event(QEvent *event) {
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent * help_event = static_cast<QHelpEvent *>(event);
		//qDebug() << "TimeSlider::event: x:" << help_event->x() << "maximum:" << maximum() << "width:" << width() << "total_time:" << total_time;

		QStyleOptionSlider opt;
		initStyleOption(&opt);
		const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
		const QPoint center = sliderRect.center() - sliderRect.topLeft();

		int value = pixelPosToRangeValue(help_event->x() - center.x());
		int range = maximum() - minimum();
		qreal time = value * total_time / range;
		//qDebug() << "TimeSlider::event: value:" << value << "range:" << range << "time:" << time;

		if (time >= 0 && time <= total_time) {
			QToolTip::showText(help_event->globalPos(), Helper::formatTime(time), this);
		} else {
			QToolTip::hideText();
			event->ignore();
		}
		return true;
	}
	return QWidget::event(event);
}

#include "moc_timeslider.cpp"
