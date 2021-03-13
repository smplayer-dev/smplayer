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

#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include "myslider.h"
#include "config.h"

class TimeSlider : public MySlider 
{
	Q_OBJECT

public:
	TimeSlider( QWidget * parent );
	~TimeSlider();

public slots:
	virtual void setPos(int); // Don't use setValue!
	virtual int pos();
	virtual void setDuration(double t) { total_time = t; };
	virtual double duration() { return total_time; };
#if ENABLE_DELAYED_DRAGGING
	void setDragDelay(int);
	int dragDelay();
#endif

signals:
	void posChanged(int);
	void draggingPos(int);
#if ENABLE_DELAYED_DRAGGING
	//! Emitted with a few ms of delay
	void delayedDraggingPos(int);
#endif
	void wheelUp();
	void wheelDown();

protected slots:
	void stopUpdate();
	void resumeUpdate();
	void mouseReleased();
	void valueChanged_slot(int);
#if ENABLE_DELAYED_DRAGGING
	void checkDragging(int);
	void sendDelayedPos();
#endif

protected:
	virtual void wheelEvent(QWheelEvent * e);
	virtual bool event(QEvent *event);

private:
	bool dont_update;
	int position;
	double total_time;

#if ENABLE_DELAYED_DRAGGING
	int last_pos_to_send;
	QTimer * timer;
#endif
};

#endif

