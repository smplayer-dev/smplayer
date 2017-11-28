/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>
    umplayer, Copyright (C) 2010 Ori Rejwan

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

#include "panelseeker.h"
#include <QPainter>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <QToolTip>
#include "global.h"
#include "preferences.h"
#include "config.h"
#include "helper.h"


using namespace Global;


PanelSeeker::PanelSeeker(QWidget *parent) :
    QAbstractSlider(parent), isPressed(false), leftRightMargin(5), bufferingPixShift(0),
    frozen(false), delayPeriod(100), frozenPeriod(500)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_Hover, true);    
    setEnabled(false);
    setState(Stopped, true);     
    setTracking(pref->update_while_seeking);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(moved(int)));
    setRange(0, 100);
    freezeTimer = new QTimer(this);
    freezeTimer->setInterval(frozenPeriod);
    freezeTimer->setSingleShot(true);
    dragDelayTimer = new QTimer(this);
    dragDelayTimer->setInterval(delayPeriod);
    dragDelayTimer->setSingleShot(true);
    connect(freezeTimer, SIGNAL(timeout()), this, SLOT(stopFreeze()));
    connect(dragDelayTimer, SIGNAL(timeout()), this, SLOT(goToSliderPosition()));
}


void PanelSeeker::paintEvent(QPaintEvent * e)
{
    Q_UNUSED(e);

    QPainter p(this);

    p.drawPixmap( leftRightMargin, (height()- leftPix.height())/2 , leftPix  );
    p.drawTiledPixmap(QRect(leftRightMargin + leftPix.width(), (height()- centerPix.height())/2   ,width() - 2*leftRightMargin - leftPix.width() - rightPix.width(),centerPix.height()), centerPix);

    p.drawPixmap( width()-rightPix.width() - leftRightMargin , (height()- rightPix.height())/2 ,  rightPix);

    if(state.testFlag(Buffering))
    {
        p.drawTiledPixmap(QRect(leftRightMargin + leftPix.width(), (height()- bufferingPix.height())/2, width() - 2*leftRightMargin -leftPix.width() - rightPix.width(), bufferingPix.height()), bufferingPix, QPoint(bufferingPixShift, 0)  );
    }
    else
    {
        if(isEnabled())
        {
            p.drawTiledPixmap(QRect(leftRightMargin + leftPix.width(), (height()- progressPix.height())/2 , knobRect.center().x() - leftRightMargin - leftPix.width(), progressPix.height()), progressPix);
        }
        p.drawPixmap(knobRect.toRect(), knobCurrentPix  );
    }


}

void PanelSeeker::setKnobIcon( QPixmap pix )
{
    int w = pix.width();
    int h = pix.height();
    knobPix.setPixmap(pix.copy(0, 0, w, h/4 ), MyIcon::Normal, MyIcon::Off);
    knobPix.setPixmap(pix.copy(0, h/4, w, h/4 ), MyIcon::MouseOver, MyIcon::Off);
    knobPix.setPixmap(pix.copy(0, h/2, w, h/4 ), MyIcon::MouseDown, MyIcon::Off);
    knobPix.setPixmap(pix.copy(0, 3*h/4, w, h/4 ), MyIcon::Disabled, MyIcon::Off);
    knobCurrentPix = knobPix.pixmap(MyIcon::Normal, MyIcon::Off);
    /* setSliderValue(minimum()); */
    setState(Normal, true);
}

/*
void PanelSeeker::setSingleKnobIcon(QPixmap pix)
{
    knobPix.setPixmap(pix, MyIcon::Normal, MyIcon::Off);
    knobPix.setPixmap(pix, MyIcon::MouseOver, MyIcon::Off);
    knobPix.setPixmap(pix, MyIcon::MouseDown, MyIcon::Off);
    knobPix.setPixmap(pix, MyIcon::Disabled, MyIcon::Off);
    setSliderValue(minimum());
    setState(Normal, true);
}
*/

void PanelSeeker::mousePressEvent(QMouseEvent *m)
{
    m->accept();
    setTracking(pref->update_while_seeking);
    if(m->button() == Qt::LeftButton)
    {
        #if QT_VERSION >= 0x050000
        QPointF pos = m->localPos();
        #else
        QPointF pos = m->posF();
        #endif
        if(knobRect.contains(pos))
        {
            isPressed = true;
            mousePressPos = pos;
            mousePressDifference = pos.x() - knobRect.center().x();
            setSliderDown(true);
            setState(Pressed, true);
        }
        else
        {
            isPressed = false;
            #if QT_VERSION >= 0x050000
            knobAdjust( m->localPos().x() - knobRect.center().x(), true);
            #else
            knobAdjust( m->posF().x() - knobRect.center().x(), true);
            #endif
        }
    }
}

void PanelSeeker::mouseMoveEvent(QMouseEvent *m)
{
    m->accept();
    if(isPressed)
    {
        #if QT_VERSION >= 0x050000
        knobAdjust(m->localPos().x() - knobRect.center().x() - mousePressDifference );
        #else
        knobAdjust(m->posF().x() - knobRect.center().x() - mousePressDifference );
        #endif
    }
}

void PanelSeeker::mouseReleaseEvent(QMouseEvent *m)
{
    Q_UNUSED(m);

    setSliderDown(false);
    if(isPressed)
    {
        isPressed = false;
        setState(Pressed, false);
        frozen = true;
        freezeTimer->start();
        /*if(mousePressPos == m->pos())
        {
            knobAdjust( m->posF().x() - knobRect.center().x(), true);
            triggerAction(SliderMove);
        } */
    }
}

void PanelSeeker::resetKnob( bool start)
{
    if(start)
    {
        moved(minimum());
    }
    else
    {
        moved(maximum());
    }    
}

void PanelSeeker::knobAdjust(qreal x, bool isSetValue)
{    
    if(state.testFlag(Buffering)) return;
    qreal value = minimum() + (knobRect.center().x() - (leftRightMargin + knobCurrentPix.width()/2) +x ) * (maximum() - minimum()) /(width() - (leftRightMargin) - (leftRightMargin) - knobCurrentPix.width()) ;
    if(isSetValue)
    {
        frozen = true;
        if( state.testFlag(Stopped) ) value = minimum();
        setValue(qRound(value));
        freezeTimer->start();
    }
    else {
        if(hasTracking())
        {
            blockSignals(true);
            moved(qRound(value));
            blockSignals(false);
            emit sliderMoved(qRound(value));
            if(!dragDelayTimer->isActive())
                dragDelayTimer->start();
        }
        else
            moved(qRound(value));
    }
}

bool PanelSeeker::event(QEvent *e)
{    
    if(e->type() == QEvent::HoverMove || e->type() == QEvent::HoverEnter  )
    {
        QHoverEvent* he = static_cast<QHoverEvent*>(e);
        if( knobRect.contains(he->pos()) )
        {
            setState(Hovered, true);
        }
        else
        {
            setState(Hovered, false);
        }
    }
    if(e->type() == QEvent::HoverLeave)
    {        
        setState(Hovered, false);
    }
    return QAbstractSlider::event(e);
}

qreal PanelSeeker::valueForPos(int pos)
{
        qreal value = (qreal)( pos - (leftRightMargin + knobCurrentPix.width()/2) ) * maximum() /(width() - (leftRightMargin) - (leftRightMargin) - knobCurrentPix.width());
        return value;
}

void PanelSeeker::setState(State st, bool on)
{
    if(on)
    {
        if(state.testFlag(st)) return;
        state |= st;        
    }
    else
    {
        if(!state.testFlag(st)) return;
        state &= ~st;
    }
    if(state.testFlag(Buffering))
    {
        startTimer(100);
    }
    if(state.testFlag(Disabled))
    {
        knobCurrentPix = knobPix.pixmap(MyIcon::Disabled, MyIcon::Off);
    }
    else if(state.testFlag(Pressed))
    {
        knobCurrentPix = knobPix.pixmap(MyIcon::MouseDown, MyIcon::Off);
    }
    else if(state.testFlag(Hovered))
    {
        knobCurrentPix = knobPix.pixmap(MyIcon::MouseOver, MyIcon::Off);
    }
    else
    {
        knobCurrentPix = knobPix.pixmap(MyIcon::Normal, MyIcon::Off);
    }
    update();
}

void PanelSeeker::moved( int value)
{
    if(value > maximum()) value = maximum();
    if(value < minimum()) value = minimum();
    if( state.testFlag(Stopped) ) value = minimum();

    qreal ratio =  (qreal)(value - minimum())/(maximum()-minimum());
    qreal centerPixel = ratio*(width() - (leftRightMargin ) - (leftRightMargin) - knobCurrentPix.width());
    QSize size = knobPix.size(MyIcon::Normal, MyIcon::Off);
    knobRect = QRectF(QPointF(centerPixel + (leftRightMargin + knobCurrentPix.width()/2) - size.width()/2, ( height() - size.height())/2 ), size );
    setSliderPosition(value);
    update();
}

void PanelSeeker::resizeEvent(QResizeEvent *)
{
    setSliderValue(value());
}

void PanelSeeker::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::EnabledChange)
    {
        if(isEnabled())
        {
            setState(Disabled, false);
            setState(Stopped, false);

        }
        else
        {
            setState(Disabled, true);
            setState(Stopped, true);
        }
    }    
}

void PanelSeeker::timerEvent(QTimerEvent *t)
{
    if (bufferingPix.width() < 1) return;

    if(!state.testFlag(Buffering))
    {
        killTimer(t->timerId());
        bufferingPixShift = 0;
    }
    else
    {
        bufferingPixShift +=1;
        bufferingPixShift = qRound(bufferingPixShift)% bufferingPix.width();
        update();
    }
}

void PanelSeeker::setSliderValue(int value)
{
    if(!isPressed && !frozen)
    {
        blockSignals(true);
        moved(value);
        setValue(value);
        blockSignals(false);
    }
}

void PanelSeeker::stopFreeze()
{
    frozen = false;
    update();
}

void PanelSeeker::goToSliderPosition()
{
    emit valueChanged(sliderPosition());
    dragDelayTimer->stop();
}

void PanelSeeker::wheelEvent(QWheelEvent *e)
{
    blockSignals(true);
    QAbstractSlider::wheelEvent(e);
    moved(value());
    blockSignals(false);
    frozen = true;
    freezeTimer->start();
    if(!dragDelayTimer->isActive())
        dragDelayTimer->start();

}

void PanelTimeSeeker::wheelEvent(QWheelEvent *e) {
	qDebug("PanelTimeSeeker::wheelEvent: delta: %d", e->delta());
	e->accept();

	if (e->orientation() == Qt::Vertical) {
		if (e->delta() >= 0)
			emit wheelUp();
		else
			emit wheelDown();
	} else {
		qDebug("PanelTimeSeeker::wheelEvent: horizontal event received, doing nothing");
	}
}

#include "moc_panelseeker.cpp"
