/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#include "videolayer.h"
#include <QDebug>

VideoLayer::VideoLayer(QWidget* parent, Qt::WindowFlags f)
	: VIDEOLAYER_PARENT(parent, f)
#if REPAINT_BACKGROUND_OPTION
	, repaint_background(false)
#endif
	, playing(false)
{
}

VideoLayer::~VideoLayer() {
}

#if REPAINT_BACKGROUND_OPTION
void VideoLayer::setRepaintBackground(bool b) {
	qDebug("VideoLayer::setRepaintBackground: %d", b);
	repaint_background = b;
}
#endif

void VideoLayer::playingStarted() {
	qDebug("VideoLayer::playingStarted");
//	repaint();
	playing = true;

#if REPAINT_BACKGROUND_OPTION
	#ifndef Q_OS_WIN
	if (!repaint_background) setUpdatesEnabled(false);
	#endif
#endif

	//Screen::playingStarted();
}

void VideoLayer::playingStopped() {
	qDebug("VideoLayer::playingStopped");
	playing = false;

#ifndef Q_OS_WIN
	setUpdatesEnabled(true);
#endif

//	repaint();
	//Screen::playingStopped();
}

void VideoLayer::gotVO(QString vo) {
	qDebug() << "VideoLayer::gotVO:" << vo;
}

#include "moc_videolayer.cpp"
