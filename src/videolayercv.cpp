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

#include "videolayercv.h"
#include "mconnection.h"

#include <QCoreApplication>
#include <QDebug>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

VideoLayerCV::VideoLayerCV(QWidget* parent, Qt::WindowFlags f)
	: VideoLayerRender(parent, f)
	, shm_fd(0)
	, image_buffer(0)
	, buffer_size(0)
{
	buffer_name = QString("smplayer-%1").arg(QCoreApplication::applicationPid());
	mconnection = new MConnection(this, buffer_name);
	setVOToRender("corevideo");

//	mconnection->moveToThread(&thread);
//	thread.start();
}

VideoLayerCV::~VideoLayerCV() {
}

void VideoLayerCV::playingStopped() {
	qDebug("VideoLayerCV::playingStopped");
	VideoLayerRender::playingStopped();
}

void VideoLayerCV::gotVO(QString vo) {
	qDebug() << "VideoLayerCV::gotVO:" << vo;
	VideoLayerRender::gotVO(vo);
	if (vo == "corevideo") {
		start_connection();
	} else {
		stop_connection();
	}
}

void VideoLayerCV::init_slot(int width, int height, int bytes, int aspect) {
	qDebug("VideoLayerCV::init_slot: %d %d %d %d", width, height, bytes, aspect);

	int image_width = width;
	int image_height = height;
	int image_bytes = bytes;
	buffer_size = image_width * image_height * image_bytes;

	shm_fd = shm_open(buffer_name.toLatin1().constData(), O_RDONLY, S_IRUSR);
	if (shm_fd == -1) {
		qDebug("VideoLayerCV::init_slot: shm_open failed");
		return;
	}

	image_buffer = (unsigned char*) mmap(NULL, buffer_size, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (image_buffer == MAP_FAILED) {
		qDebug("VideoLayerCV::init_slot: mmap failed");
		return;
	}

	init(image_width, image_height, image_bytes, VideoLayerRender::RGB24, image_buffer);
}

void VideoLayerCV::render_slot() {
	//qDebug("VideoLayerCV::render_slot");
	if (image_buffer) {
		render();
	}
}

void VideoLayerCV::stop_slot() {
	qDebug("VideoLayerCV::stop_slot");
}

void VideoLayerCV::start_connection() {
	mconnection->startConnection();
}

void VideoLayerCV::stop_connection() {
	mconnection->stopConnection();

	// Destroy the shared buffer
	if (munmap(image_buffer, buffer_size) == -1) {
		qDebug("VideoLayerCV::stop_connection: munmap failed");
	}
	image_buffer = 0;
}

#include "moc_videolayercv.cpp"
