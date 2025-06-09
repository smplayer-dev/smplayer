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

#include "timeslider.h"
#include "helper.h"
#include "global.h"
#include "preferences.h"

#include <QWheelEvent>
#include <QTimer>
#include <QToolTip>
#include <QStyleOption>
#include <QDebug>

using Global::pref;

#define DEBUG 0

TimeSlider::TimeSlider( QWidget * parent ) : MySlider(parent)
	, dont_update(false)
	, position(0)
	, start_drag_pos(-1)
	, slider_has_moved(false)
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

	connect( this, SIGNAL( sliderPressed() ), this, SLOT( sliderPressed_slot() ) );
	connect( this, SIGNAL( sliderReleased() ), this, SLOT( sliderReleased_slot() ) );
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

void TimeSlider::sliderPressed_slot() {
	#if DEBUG
	qDebug("TimeSlider::sliderPressed_slot");
	#endif
	dont_update = true;
	start_drag_pos = pos();
	slider_has_moved = false;
}

void TimeSlider::sliderReleased_slot() {
	#if DEBUG
	qDebug("TimeSlider::sliderReleased_slot");
	#endif
	dont_update = false;
	if (slider_has_moved) {
		// Only emit a video seek action when the slider actually
		// moved during mouse drag. Otherwise, on mouse release,
		// we would spuriously seek to the same position we are in,
		// causing seeker judder and video jitter during playback.
		if (!pref->update_while_seeking) {
			// It only makes sense to emit a video seek action when
			// "Seek to position when released" is active. When we
			// "Seek to position while dragging", this event is
			// spurious and should be skipped because the desired
			// video seeking was already done when the slider moved.
			emit posChanged( value() );
		}
	}
	start_drag_pos = -1;
	slider_has_moved = false;
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
		if ( start_drag_pos != -1 && v != start_drag_pos ) {
			slider_has_moved = true;
		}
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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
	qDebug("TimeSlider::wheelEvent: delta: %d", e->angleDelta().y());
	e->accept();
	if (e->angleDelta().y() >= 0) emit wheelUp(); else emit wheelDown();
#else
	qDebug("TimeSlider::wheelEvent: delta: %d", e->delta());
	e->accept();
	if (e->orientation() == Qt::Vertical) {
		if (e->delta() >= 0) emit wheelUp(); else emit wheelDown();
	}
#endif
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
