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

#ifndef MEDIABARPANEL_H
#define MEDIABARPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include "core.h"


namespace Ui {
    class MediaBarPanel;
}

class PlayControl;
class MediaPanel;
class VolumeControlPanel;
class MyAction;

class MediaBarPanel : public QWidget {
    Q_OBJECT

public:
    MediaBarPanel(QWidget *parent = 0);
    ~MediaBarPanel();
    void setPlayControlActionCollection(QList<QAction*> actions);
    void setMediaPanelActionCollection(QList<QAction*> actions);
    void setVolumeControlActionCollection(QList<QAction*> actions);
	void setToolbarActionCollection(QList<QAction *>actions);
    void setCore(Core* c);
    void setRecordAvailable(bool av);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MediaBarPanel *ui;
    PlayControl* playControlPanel;
    MediaPanel* mediaPanel;
    VolumeControlPanel* volumeControlPanel;
    Core* core;

    // Play Control
public slots:
    void setMplayerState(Core::State state);
    void setDuration();
    void gotCurrentTime(double time);
    void updateMediaInfo();
    void displayMessage(QString status, int time);
    void displayMessage(QString status);
    void displayPermanentMessage(QString status);
    void setBuffering();
	void setVolume(int v);
	void setSeeker(int v);
	void setResolutionVisible(bool b);
	void setScrollingEnabled(bool b);

signals:
	void volumeChanged(int);
	void volumeSliderMoved(int);
	void seekerChanged(int);
};

#endif // MEDIABARPANEL_H
