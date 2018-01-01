/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>
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

#ifndef PANELSEEKER_H
#define PANELSEEKER_H

#include <QAbstractSlider>
#include <QPixmap>
#include <QTimer>
#include "mybutton.h"

class PanelSeeker : public QAbstractSlider
{
Q_OBJECT

Q_PROPERTY( QPixmap left READ leftIcon WRITE setLeftIcon)
Q_PROPERTY( QPixmap center READ centerIcon WRITE setCenterIcon)
Q_PROPERTY( QPixmap right READ rightIcon WRITE setRightIcon)
Q_PROPERTY( QPixmap progress READ progressIcon WRITE setProgressIcon )
Q_PROPERTY( QPixmap buffering READ bufferingIcon WRITE setBufferingIcon)
Q_PROPERTY( QPixmap knob READ knobIcon WRITE setKnobIcon )

public:

    enum State
    {
        Normal = 2,
        Hovered = 4,
        Pressed = 8,
        Disabled =16,
        Stopped = 32,
        Buffering = 64
    };
    Q_DECLARE_FLAGS(States, State)
    explicit PanelSeeker(QWidget *parent = 0);
    QPixmap leftIcon() { return leftPix; }
    QPixmap centerIcon() { return centerPix; }
    QPixmap rightIcon() { return rightPix; }
    QPixmap progressIcon() { return progressPix; }
    QPixmap bufferingIcon() { return bufferingPix; }
    QPixmap knobIcon() { return knobPix.pixmap(MyIcon::Normal, MyIcon::Off); }
    States states() { return state; }

    void setLeftIcon( QPixmap pix) { leftPix = pix;  }
    void setCenterIcon( QPixmap pix) { centerPix = pix; }
    void setRightIcon( QPixmap pix) { rightPix = pix; }
    void setProgressIcon ( QPixmap pix) { progressPix = pix; }
    void setBufferingIcon( QPixmap pix) { bufferingPix = pix; }
    void setKnobIcon( QPixmap pix );
    /* void setSingleKnobIcon(QPixmap pix); */
    void setState(State st, bool on = true);
    void setLeftRightMargin( int margin) { leftRightMargin = margin; }
    void setDelayPeriod(int period) { delayPeriod = period; }
    void setFrozenPeriod(int period) { frozenPeriod = period; }    
    qreal valueForPos(int pos);



private:
    QPixmap leftPix;
    QPixmap centerPix;
    QPixmap rightPix;
    QPixmap progressPix;
    QPixmap bufferingPix;
    MyIcon  knobPix;
    QRectF knobRect;
    bool isPressed;        
    int leftRightMargin;
    QPointF mousePressPos;
    qreal mousePressDifference;    
    States state;
    qreal bufferingPixShift;
    QPixmap knobCurrentPix;
    // freeze the knob after seeking through mouse for frozenPeriod
    // so that knob is not reset by the signal from timeslider action.
    bool frozen;
    QTimer* freezeTimer;
    // we dont seek the movie immediately, so that mutliple
    // consecutive changes are clustered in one seek signal
    QTimer* dragDelayTimer;
    int delayPeriod;
    int frozenPeriod;


    void resetKnob( bool start = true);
    void knobAdjust(qreal x, bool setValue = false);




signals:

public slots:
    void moved( int value);
    void setSliderValue(int value);
    void stopFreeze();
    void goToSliderPosition();


protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * m);
    void mouseMoveEvent(QMouseEvent * m);
    void mouseReleaseEvent(QMouseEvent * m);
    void resizeEvent(QResizeEvent *);
    bool event(QEvent *e);
    void changeEvent(QEvent *e);
    void timerEvent(QTimerEvent * t);
    void wheelEvent(QWheelEvent *e);


};
Q_DECLARE_OPERATORS_FOR_FLAGS(PanelSeeker::States)


class PanelTimeSeeker : public PanelSeeker {
	Q_OBJECT

signals:
	void wheelUp();
	void wheelDown();

protected:
    void wheelEvent(QWheelEvent *e);
};

#endif // PANELSEEKER_H
