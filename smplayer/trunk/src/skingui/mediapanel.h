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


#ifndef MEDIAPANEL_H
#define MEDIAPANEL_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include "ui_mediapanel.h"
#include "mybutton.h"
#include "panelseeker.h"

class QGridLayout;

class ScrollingLabel : public QWidget
{
    Q_OBJECT

public:
    ScrollingLabel(QWidget* parent=0);
    ~ScrollingLabel(){}
    QString text() { return mText; }
    void setText( QString text);

    void setScrollingEnabled(bool b);
    bool scrollingEnabled() { return scrolling_enabled; };

private:
    QString mText;
    void updateLabel();
    int scrollPos;
    int timerId;
    QRect textRect;
    static const int gap = 10;
    bool scrolling_enabled;

protected:
    void paintEvent(QPaintEvent *);
    void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent *);
    QSize sizeHint() const;

private slots:
    void timerEvent(QTimerEvent *);
};

class MediaPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPixmap bgLeft READ bgLeftPix WRITE setBgLeftPix)
    Q_PROPERTY(QPixmap bgRight READ bgRightPix WRITE setBgRightPix)
    Q_PROPERTY(QPixmap bgCenter READ bgCenterPix WRITE setBgCenterPix)

public:
    MediaPanel(QWidget *parent = 0);
    ~MediaPanel();
    QPixmap bgLeftPix() { return leftBackground ;}
    void setBgLeftPix( QPixmap pix){ leftBackground = pix; }
    QPixmap bgRightPix() { return rightBackground ;}
    void setBgRightPix( QPixmap pix){ rightBackground = pix; }
    QPixmap bgCenterPix() { return centerBackground ;}
    void setBgCenterPix( QPixmap pix){ centerBackground = pix; }
    void setShuffleIcon( MyIcon icon );
    void setRepeatIcon(MyIcon icon);
    void setElapsedText(QString text) {
        elapsedLabel->setText(text);
        if(seeker->states().testFlag(PanelSeeker::Buffering))
            setBuffering(false);
        }
    void setTotalText( QString text) { totalLabel->setText(text); }
    void setActionCollection(QList<QAction*> actions);
    void setMplayerState(int state);
    void setDuration(int duration);
    void setMediaLabelText(QString text);
    void setResolutionLabelText(QString text);
    void setStatusText(QString text, int time = 2000);
    void setBuffering(bool enable);
    bool eventFilter(QObject *object, QEvent *event);

public slots:
	void setSeeker(int v);
	void setResolutionVisible(bool b);
	void setScrollingEnabled(bool b);

private:
    Ui::MediaPanelClass ui;
    QGridLayout * layout;
    QPixmap leftBackground;
    QPixmap centerBackground;
    QPixmap rightBackground;
    ScrollingLabel* mediaLabel;
    QLabel *resolutionLabel;
    PanelTimeSeeker* seeker;
    MyButton* repeatButton;
    MyButton* shuffleButton;
    QLabel* elapsedLabel;
    QLabel* totalLabel;
    QString originalTitle;
    QTimer* timer;    
    int duration;

private slots:
    void reverseStatus();
    void rearrangeWidgets(bool resolution_visible);

protected:
    void paintEvent(QPaintEvent *);
    virtual void changeEvent (QEvent * event);
    virtual void retranslateStrings();

signals:
	void seekerChanged(int);
	void seekerWheelUp();
	void seekerWheelDown();

public:
    friend class IconSetter;
};

#endif // MEDIAPANEL_H
