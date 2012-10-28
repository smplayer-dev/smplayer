/*  umplayer, GUI front-end for mplayer.
    Copyright (C) 2010 Ori Rejwan

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

#ifndef PLAYCONTROL_H
#define PLAYCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPixmap>
#include "myicon.h"
#include "mybutton.h"

class MyAction;
class PlayControl : public QWidget
{
Q_OBJECT
public:
    explicit PlayControl(QWidget *parent = 0);

private:
    MyButton* backwardButton;
    MyButton* previousButton;
    MyButton* playPauseButton;
    MyButton* stopButton;
    MyButton* nextButton;
    MyButton* forwardButton;
    MyButton* recordButton;
    QHBoxLayout* layout;    
    bool playOrPause;
    void updateSize();    
    void updateWidths();

public:
    QPixmap backwardIcon();
    void setRecordEnabled(bool enable) { recordButton->setEnabled(enable); updateWidths();}
    void setPreviousTrackEnabled(bool enable) { previousButton->setEnabled(enable); updateWidths();}
    void setNextTrackEnabled(bool enable) { nextButton->setEnabled(enable); updateWidths();}
    void setPlay(bool on) { playOrPause = on; playPauseButton->setState(on); }

    void setBackwardIcon(MyIcon icon ) { backwardButton->setMyIcon(icon); backwardButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths(); }
    void setForwardIcon(MyIcon icon) { forwardButton->setMyIcon(icon); forwardButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths(); }
    void setPreviousIcon(MyIcon icon) { previousButton->setMyIcon(icon); previousButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths();}
    void setNextIcon(MyIcon icon) { nextButton->setMyIcon(icon); nextButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths();}
    void setPlayPauseIcon (MyIcon icon) { playPauseButton->setMyIcon(icon); playPauseButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));updateWidths();}
    void setStopIcon (MyIcon icon) { stopButton->setMyIcon(icon); stopButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths();}
    void setRecordIcon(MyIcon icon) { recordButton->setMyIcon(icon); recordButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off)); updateWidths();}

    void setActionCollection(QList<QAction*> actions);
    bool eventFilter(QObject *watched, QEvent *event);


protected:
    void resizeEvent(QResizeEvent * e);

signals:

public slots:

friend class IconSetter;

};

#endif // PLAYCONTROL_H
