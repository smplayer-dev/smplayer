/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#ifdef USE_COREVIDEO_BUFFER
#include "mac/objc_bridge.h"
#include "mac/globaldataclass.h"
GlobalDataClass globaldata;
#endif

VideoLayer::VideoLayer(QWidget* parent, Qt::WindowFlags f)
#ifdef USE_GLWIDGET
	: QGLWidget(parent, 0, f)
#else
	: QWidget(parent, f)
#endif
#if REPAINT_BACKGROUND_OPTION
	, repaint_background(false)
#endif
	, playing(false)
{
#ifdef USE_COREVIDEO_BUFFER
	globaldata.gl = this;
#endif
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

#ifdef USE_COREVIDEO_BUFFER
void VideoLayer::cleararea_slot() {
	cleararea_bridge();
}

void VideoLayer::updateView()
{
	updateGL();
}

void VideoLayer::setSharedMemory(QString memoryName)
{
	qDebug() << "VideoLayer::setSharedMemory:" << memoryName;
	makeCurrent();
	startObjcFunction(memoryName.toLatin1().data());
	//cleararea_bridge();
	QTimer::singleShot(0, this, SLOT(cleararea_slot()));
}

void VideoLayer::stopOpengl()
{
	stopObjcFunction();
}
#endif

#include "moc_videolayer.cpp"
