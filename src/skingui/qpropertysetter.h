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

#ifndef QPROPERTYSETTER_H
#define QPROPERTYSETTER_H

#include "myaction.h"
#include <QWidget>
#include <QPixmap>
#include <QMessageBox>
#include "playcontrol.h"
#include "myicon.h"
#include "mediapanel.h"



class IconSetter : public QWidget
{
Q_OBJECT
Q_PROPERTY(QPixmap toolbar READ toolbarIcon WRITE setToolbarIcon)

Q_PROPERTY(QPixmap backward READ backwardIcon WRITE setBackwardIcon)
Q_PROPERTY(QPixmap previous READ previousIcon WRITE setPreviousIcon)
Q_PROPERTY(QPixmap playPause READ playPauseIcon WRITE setPlayPauseIcon)
Q_PROPERTY(QPixmap stop READ stopIcon WRITE setStopIcon )
Q_PROPERTY(QPixmap record READ recordIcon WRITE setRecordIcon )
Q_PROPERTY(QPixmap next READ nextIcon WRITE setNextIcon)
Q_PROPERTY(QPixmap forward READ forwardIcon WRITE setForwardIcon)

Q_PROPERTY(QPixmap shuffle READ shuffleIcon WRITE setShuffleIcon )
Q_PROPERTY(QPixmap repeat READ repeatIcon WRITE setRepeatIcon )

public:

    static IconSetter* instance();
    static void removeInstance();
    ~IconSetter() {}
    PlayControl* playControl;
    MediaPanel* mediaPanel;

    void setToolbarIcon(QPixmap icon) { setActionIcon(icon); }
    QPixmap toolbarIcon() { return QPixmap();}


    void setBackwardIcon(QPixmap icon) { buttonIcon(1, icon); }
    QPixmap backwardIcon() { return playControl->backwardButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setPreviousIcon(QPixmap icon) { buttonIcon(2, icon); }
    QPixmap previousIcon() { return playControl->previousButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setPlayPauseIcon(QPixmap icon) { buttonIcon(3, icon); }
    QPixmap playPauseIcon() { return playControl->playPauseButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setStopIcon(QPixmap icon) { buttonIcon(4, icon); }
    QPixmap stopIcon() { return playControl->stopButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setRecordIcon( QPixmap icon) { buttonIcon(5, icon); }
    QPixmap recordIcon() { return playControl->recordButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setNextIcon( QPixmap icon) { buttonIcon(6, icon); }
    QPixmap nextIcon() { return playControl->nextButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setForwardIcon( QPixmap icon) { buttonIcon(7, icon); }
    QPixmap forwardIcon() { return playControl->forwardButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setShuffleIcon( QPixmap icon) { mediaPanelButtonIcon(1, icon); }
    QPixmap shuffleIcon() { return mediaPanel->shuffleButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

    void setRepeatIcon( QPixmap icon) { mediaPanelButtonIcon(2, icon); }
    QPixmap repeatIcon() { return mediaPanel->repeatButton->myIcon().pixmap(MyIcon::Normal, MyIcon::Off); }

	void setToolbarActions(QList<QAction *>actions) { toolbar_actions = actions; }
	QList<QAction *> toolbarActions() { return toolbar_actions; }


private:    
    IconSetter(QWidget *parent = 0);

    static IconSetter* m_instance;
    void setActionIcon(QPixmap pix );
    void buttonIcon(int n, QPixmap pix );
    void mediaPanelButtonIcon( int n, QPixmap pix);

	QList<QAction *> toolbar_actions;
};

#endif // QPROPERTYSETTER_H
