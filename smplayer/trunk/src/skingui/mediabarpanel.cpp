/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>
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

#include "mediabarpanel.h"
#include "ui_mediabarpanel.h"
#include "playcontrol.h"
#include "mediapanel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "qpropertysetter.h"
#include "colorutils.h"
#include "qpropertysetter.h"
#include "volumecontrolpanel.h"



MediaBarPanel::MediaBarPanel(QWidget *parent) :
    QWidget(parent),ui(new Ui::MediaBarPanel),core(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedHeight(53);
    QHBoxLayout* layout = new QHBoxLayout;
    playControlPanel = new PlayControl(this);
    IconSetter::instance()->playControl = playControlPanel;
    volumeControlPanel = new VolumeControlPanel(this);
    volumeControlPanel->setObjectName("volume-control-panel");
    mediaPanel = new MediaPanel(this);
    mediaPanel->setObjectName("media-panel");
    IconSetter::instance()->mediaPanel = mediaPanel;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(playControlPanel);
    layout->addWidget(mediaPanel);
    layout->addWidget(volumeControlPanel);            
    setLayout(layout);

	connect(volumeControlPanel, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
	connect(mediaPanel, SIGNAL(seekerChanged(int)), this, SIGNAL(seekerChanged(int)));
}

MediaBarPanel::~MediaBarPanel()
{
    delete ui;
}

void MediaBarPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MediaBarPanel::setToolbarActionCollection(QList<QAction *>actions) {
	IconSetter::instance()->setToolbarActions(actions);
}

void MediaBarPanel::setPlayControlActionCollection(QList<QAction *>actions)
{
    playControlPanel->setActionCollection(actions);
}

void MediaBarPanel::setMediaPanelActionCollection(QList<QAction *>actions)
{
    mediaPanel->setActionCollection(actions);
}

void MediaBarPanel::setMplayerState(Core::State state)
{
    mediaPanel->setMplayerState((int)state);    
}
void MediaBarPanel::setCore(Core *c)
{
    core = c;
    connect(core, SIGNAL(mediaStartPlay()), this, SLOT(setDuration()) );
    connect( core, SIGNAL(showTime(double)), this, SLOT(gotCurrentTime(double)) );
    connect( core, SIGNAL(mediaInfoChanged()), this, SLOT(updateMediaInfo()) );    
    connect( core, SIGNAL(buffering()), this, SLOT(setBuffering()) );

}

void MediaBarPanel::setDuration()
{
    mediaPanel->setDuration(core->mdat.duration);
}

void MediaBarPanel::setVolumeControlActionCollection(QList<QAction *>actions)
{
    volumeControlPanel->setActionCollection(actions);
}

void MediaBarPanel::gotCurrentTime(double time)
{
    mediaPanel->setElapsedText(Helper::formatTime((int)time));    
}

void MediaBarPanel::updateMediaInfo()
{
    mediaPanel->setMediaLabelText(core->mdat.displayName());
}

void MediaBarPanel::displayMessage(QString status)
{
    mediaPanel->setStatusText(status);
}

void MediaBarPanel::displayPermanentMessage(QString status)
{
    mediaPanel->setStatusText(status, 0);
}

void MediaBarPanel::setRecordAvailable(bool av)
{
    playControlPanel->setRecordEnabled(av);
}

void MediaBarPanel::setBuffering()
{
    mediaPanel->setBuffering(true);
}

void MediaBarPanel::setVolume(int v) { 
	volumeControlPanel->setVolume(v); 
}

void MediaBarPanel::setSeeker(int v) {
	mediaPanel->setSeeker(v);
}

#include "moc_mediabarpanel.cpp"

