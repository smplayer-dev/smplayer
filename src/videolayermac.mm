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

#include "videolayermac.h"
#include <QDebug>
#include <QTimer>

#include "mac/objc_bridge.h"
#include "mac/globaldataclass.h"
GlobalDataClass globaldata;

VideoLayerMac::VideoLayerMac(QWidget* parent, Qt::WindowFlags f)
	: VideoLayer(parent, f)
{
	globaldata.gl = this;
}

VideoLayerMac::~VideoLayerMac() {
}

void VideoLayerMac::cleararea_slot() {
	cleararea_bridge();
}

void VideoLayerMac::updateView()
{
	updateGL();
}

void VideoLayerMac::setSharedMemory(QString memoryName)
{
	qDebug() << "VideoLayerMac::setSharedMemory:" << memoryName;
	makeCurrent();
	startObjcFunction(memoryName.toLatin1().data());
	//cleararea_bridge();
	QTimer::singleShot(0, this, SLOT(cleararea_slot()));
}

void VideoLayerMac::stopOpengl()
{
	stopObjcFunction();
}

#include "moc_videolayermac.cpp"
