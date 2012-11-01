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

#include "playcontrol.h"
#include <QResizeEvent>
#include "myaction.h"
#include "actiontools.h"

PlayControl::PlayControl(QWidget *parent) :
    QWidget(parent), playOrPause(true)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    backwardButton = new MyButton(this);
    previousButton = new MyButton(this);
    playPauseButton = new MyButton(this);
    stopButton = new MyButton(this);
    nextButton = new MyButton(this);
    forwardButton = new MyButton(this);
    recordButton = new MyButton(this);
    layout = new QHBoxLayout;
    QSpacerItem* spacer1 = new QSpacerItem(9, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    QSpacerItem* spacer2 = new QSpacerItem(9, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    layout->addSpacerItem(spacer1);
    layout->addWidget(backwardButton);
    layout->addWidget(previousButton);
    layout->addWidget(playPauseButton);
    layout->addWidget(stopButton);
    layout->addWidget(recordButton);
    layout->addWidget(nextButton);
    layout->addWidget(forwardButton);
    layout->addSpacerItem(spacer2);    
    layout->setSpacing(0);    
    layout->setContentsMargins( 0, 0, 0, 0);
    setRecordEnabled(false);
    setLayout(layout);
    recordButton->installEventFilter(this);
    nextButton->installEventFilter(this);
    previousButton->installEventFilter(this);

}


void PlayControl::resizeEvent(QResizeEvent *e)
{
    updateSize();
}

void PlayControl::updateSize()
{
    backwardButton->hide();
    previousButton->hide();
    playPauseButton->hide();
    stopButton->hide();
    recordButton->hide();
    nextButton->hide();
    forwardButton->hide();
    int totalWidth = 18;
    totalWidth += playPauseButton->minimumWidth();
    playPauseButton->show();
    if(recordButton->isEnabled())
    {
        totalWidth += recordButton->minimumWidth();
        recordButton->show();
    }
    else
    {
        recordButton->hide();
    }
    if(nextButton->isEnabled())
    {
        totalWidth += nextButton->minimumWidth();
        if(width() < totalWidth ) { nextButton->hide(); return; }
        else nextButton->show();
    }
    else
    {
        nextButton->hide();
    }

    if(previousButton->isEnabled())
    {
        totalWidth += previousButton->minimumWidth();
        if(width() < totalWidth ) { previousButton->hide(); return; }
        else previousButton->show();
    }
    else
    {
        previousButton->hide();
    }    
    totalWidth += stopButton->minimumWidth();
    if(width() < totalWidth) { stopButton->hide(); return; }
    else stopButton->show();
    totalWidth += forwardButton->minimumWidth();
    if(width() < totalWidth) { forwardButton->hide(); return; }
    else forwardButton->show();
    totalWidth += backwardButton->minimumWidth();
    if(width() < totalWidth) { backwardButton->hide(); return; }
    else backwardButton->show();
    if(!nextButton->isEnabled()) totalWidth += nextButton->minimumWidth();
    if(width() < totalWidth) { nextButton->hide(); return; }
    else nextButton->show();
    if(!previousButton->isEnabled()) totalWidth += previousButton->minimumWidth();
    if(width() < totalWidth) { previousButton->hide(); return; }    
    else previousButton->show();

}

void PlayControl::updateWidths()
{
    int maxWidth = 18;
    int totalWidth = 18;
    totalWidth += playPauseButton->minimumWidth();
    if(recordButton->isEnabled())
    {
        totalWidth += recordButton->minimumWidth();
    }
    setMinimumWidth(totalWidth);
    maxWidth += backwardButton->minimumWidth();
    maxWidth += previousButton->minimumWidth();
    maxWidth += playPauseButton->minimumWidth();
    maxWidth += stopButton->minimumWidth();
    maxWidth += nextButton->minimumWidth();
    maxWidth += recordButton->minimumWidth();
    maxWidth += forwardButton->minimumWidth();
    if(recordButton->isEnabled())
        setMaximumWidth(maxWidth);
    else
        setMaximumWidth(maxWidth - recordButton->minimumWidth());
    updateSize();
}

void PlayControl::setActionCollection(QList<QAction *> actions)
{
	/*
	MyAction *a = static_cast<MyAction*>(actions.at(2));
	qDebug("*** action: %s", a->objectName().toLatin1().constData());
	*/

	SETACTIONTOBUTTON(backwardButton, "rewind1");
	SETACTIONTOBUTTON(previousButton, "play_prev");
	SETACTIONTOBUTTON(playPauseButton, "play_or_pause");
	SETACTIONTOBUTTON(stopButton, "stop");
	SETACTIONTOBUTTON(recordButton, "record");
	SETACTIONTOBUTTON(nextButton, "play_next");
	SETACTIONTOBUTTON(forwardButton, "forward1");

	if (backwardButton) backwardButton->setToolTip(tr("Rewind"));
	if (forwardButton) forwardButton->setToolTip(tr("Forward"));
	if (playPauseButton) playPauseButton->setToolTip(tr("Play / Pause"));
	if (stopButton) stopButton->setToolTip(tr("Stop"));
	if (recordButton) recordButton->setToolTip(tr("Record"));
	if (nextButton) nextButton->setToolTip(tr("Next file in playlist"));
	if (previousButton) previousButton->setToolTip(tr("Previous file in playlist"));
}

bool PlayControl::eventFilter(QObject *watched, QEvent *event)
{
    if((watched == recordButton || watched == previousButton || watched == nextButton ) && event->type() == QEvent::EnabledChange)
    {
        updateWidths();
    }
    return false;
}

#include "moc_playcontrol.cpp"
