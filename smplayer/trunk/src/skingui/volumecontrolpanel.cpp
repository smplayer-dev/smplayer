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

#include "volumecontrolpanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include "widgetactions.h"
#include "myaction.h"
#include "actiontools.h"

VolumeControlPanel::VolumeControlPanel(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);    
    setFixedWidth(108);
    muteButton = new MyButton(this);
    maxButton = new MyButton(this);
    playlistButton = new MyButton(this);    
    equalizerButton = new MyButton(this);    
    fullscreenButton = new MyButton(this);    
    playlistButton->setCheckable(true);
    equalizerButton->setCheckable(true);
    fullscreenButton->setCheckable(true);
    volumeBar = new PanelSeeker(this);
    volumeBar->setLeftRightMargin(8);
    volumeBar->setMinimum(0);
    volumeBar->setMaximum(100);
    volumeBar->setDelayPeriod(1);
    volumeBar->setFrozenPeriod(10);
    QHBoxLayout* upperLayout = new QHBoxLayout;
    QHBoxLayout* lowerLayout = new QHBoxLayout;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    upperLayout->addWidget(muteButton);
    upperLayout->addWidget(volumeBar);
    upperLayout->addWidget(maxButton);
    upperLayout->setContentsMargins(0, 0, 0, 0);
    upperLayout->setSpacing(0);    
    QSpacerItem* sp1 = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Preferred);
    lowerLayout->addSpacerItem(sp1);
    lowerLayout->addWidget(fullscreenButton);
    lowerLayout->addWidget(playlistButton);
    lowerLayout->addWidget(equalizerButton);
    QSpacerItem* sp2 = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Preferred);
    lowerLayout->addSpacerItem(sp2);
    lowerLayout->setContentsMargins(0, 0, 0, 0);
    lowerLayout->setSpacing(0);
    mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(lowerLayout);
    mainLayout->setContentsMargins(0,8,5,8);
    mainLayout->setSpacing(2);
    setLayout(mainLayout);

    connect(muteButton, SIGNAL(clicked()), this, SLOT(setVolumeMin()));
    connect(maxButton, SIGNAL(clicked()), this, SLOT(setVolumeMax()));

	connect(volumeBar, SIGNAL(valueChanged(int)), this, SIGNAL(volumeChanged(int)));
	connect(volumeBar, SIGNAL(sliderMoved(int)), this, SIGNAL(volumeSliderMoved(int)));
}

void VolumeControlPanel::setButtonIcons( MyButton* button, QPixmap pix)
{
    MyIcon icon;
    int w = pix.width();
    int h = pix.height();
    icon.setPixmap(pix.copy(0, 0, w, h/4 ), MyIcon::Normal, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/4, w, h/4 ), MyIcon::MouseOver, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/2, w, h/4 ), MyIcon::MouseDown, MyIcon::Off);
    icon.setPixmap(pix.copy(0, 3*h/4, w, h/4 ), MyIcon::Disabled, MyIcon::Off);
    icon.setPixmap(pix.copy(0, 0, w, h/4 ), MyIcon::Normal, MyIcon::On);
    icon.setPixmap(pix.copy(0, h/4, w, h/4 ), MyIcon::MouseOver, MyIcon::On);
    icon.setPixmap(pix.copy(0, h/2, w, h/4 ), MyIcon::MouseDown, MyIcon::On);
    icon.setPixmap(pix.copy(0, 3*h/4, w, h/4 ), MyIcon::Disabled, MyIcon::On);
    button->setMyIcon(icon);
    button->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));
}

void VolumeControlPanel::setActionCollection(QList<QAction*> actions)
{
	//ActionTools::findAction("aaa", actions);
	volumeBar->setEnabled(true);
	/* volumeSliderAction->installEventFilter(this); */
	SETACTIONTOBUTTON(playlistButton, "show_playlist");
	SETACTIONTOBUTTON(fullscreenButton, "fullscreen");
	SETACTIONTOBUTTON(equalizerButton, "video_equalizer");

	retranslateStrings();
}

void VolumeControlPanel::setVolume(int value)
{
    volumeBar->setSliderValue(value);
}

/*
bool VolumeControlPanel::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == volumeSliderAction && event->type() == QEvent::EnabledChange)
    {
        volumeBar->setEnabled(volumeSliderAction->isEnabled());
    }
    return false;
}
*/

// Language change stuff
void VolumeControlPanel::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

void VolumeControlPanel::retranslateStrings() {
	if (playlistButton) playlistButton->setToolTip(tr("Playlist"));
	if (fullscreenButton) fullscreenButton->setToolTip(tr("Fullscreen on/off"));
	if (equalizerButton) equalizerButton->setToolTip(tr("Video equalizer"));
}

#include "moc_volumecontrolpanel.cpp"
