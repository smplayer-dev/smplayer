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

#ifndef WIDGETACTIONS_H
#define WIDGETACTIONS_H

#include <QWidgetAction>
#include "timeslider.h"
#include "config.h"
#include "guiconfig.h"

class QStyle;

class MyWidgetAction : public QWidgetAction
{
	Q_OBJECT

public:
	MyWidgetAction( QWidget * parent );
	~MyWidgetAction();

	void setCustomStyle(QStyle * style) { custom_style = style; };
	QStyle * customStyle() { return custom_style; };

	void setStyleSheet(QString style) { custom_stylesheet = style; };
	QString styleSheet() { return custom_stylesheet; };

public slots:
	virtual void enable(); 	// setEnabled in QAction is not virtual :(
	virtual void disable();

protected:
	virtual void propagate_enabled(bool);

protected:
	QStyle * custom_style;
	QString custom_stylesheet;
};


class TimeSliderAction : public MyWidgetAction 
{
	Q_OBJECT

public:
	TimeSliderAction( QWidget * parent );
	~TimeSliderAction();

public slots:
	virtual void setPos(int);
	virtual int pos();
	virtual void setDuration(double);
	virtual double duration() { return total_time; };
#if ENABLE_DELAYED_DRAGGING
	void setDragDelay(int);
	int dragDelay();

private:
	int drag_delay;
#endif

private:
	double total_time;

signals:
	void posChanged(int value);
	void draggingPos(int value);
#if ENABLE_DELAYED_DRAGGING
	void delayedDraggingPos(int);
#endif
	void wheelUp();
	void wheelDown();

protected:
	virtual QWidget * createWidget ( QWidget * parent );
};


class VolumeSliderAction : public MyWidgetAction 
{
	Q_OBJECT

public:
	VolumeSliderAction( QWidget * parent );
	~VolumeSliderAction();

	void setFixedSize(QSize size) { fixed_size = size; };
	QSize fixedSize() { return fixed_size; };

	void setTickPosition(QSlider::TickPosition position);
	QSlider::TickPosition tickPosition() { return tick_position; };

public slots:
	virtual void setValue(int);
	virtual int value();

signals:
	void valueChanged(int value);

protected:
	virtual QWidget * createWidget ( QWidget * parent );

private:
	QSize fixed_size;
	QSlider::TickPosition tick_position;
};


class TimeLabelAction : public MyWidgetAction 
{
	Q_OBJECT

public:
	enum TimeLabelType { CurrentTime = 0, TotalTime = 1, CurrentAndTotalTime = 2, RemainingTime = 3 };

	TimeLabelAction(TimeLabelType type, QWidget * parent );
	~TimeLabelAction();

	virtual QString text() { return current_text; };

public slots:
	virtual void setText(QString s);
	virtual void setCurrentTime(double);
	virtual void setTotalTime(double);

signals:
	void newText(QString s);

protected:
	virtual QWidget * createWidget ( QWidget * parent );
	virtual void updateText();

private:
	QString current_text;
	double current_time, total_time;
	TimeLabelType label_type;
};


#if MINI_ARROW_BUTTONS
class SeekingButton : public QWidgetAction
{
	Q_OBJECT

public:
	SeekingButton( QList<QAction*> actions, QWidget * parent );
	~SeekingButton();

protected:
	virtual QWidget * createWidget ( QWidget * parent );

	QList<QAction*> _actions;
};
#endif

#endif

