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

#include "videolayershm.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

static struct header_t {
	uint32_t header_size;
	uint32_t video_buffer_size;
	uint32_t width;
	uint32_t height;
	uint32_t bytes;
	uint32_t stride;
	uint32_t planes;
	uint32_t format;
	uint32_t frame_count;
	uint32_t busy;
	float fps;
	//unsigned char * image_buffer;
} * header;


VideoLayerShm::VideoLayerShm(QWidget* parent, Qt::WindowFlags f)
	: VideoLayerRender(parent, f)
	, buffer_size(0)
{
	buffer_name = QString("smplayer-%1").arg(QCoreApplication::applicationPid());

	render_timer = new QTimer(this);
	connect(render_timer, SIGNAL(timeout()), this, SLOT(render_slot()));
	render_timer->setInterval(1000 / 60);

	connect_timer = new QTimer(this);
	connect(connect_timer, SIGNAL(timeout()), this, SLOT(start_connection()));
	connect_timer->setInterval(10);

	setVOToRender("shm");
}

VideoLayerShm::~VideoLayerShm() {
}

void VideoLayerShm::playingStopped() {
	qDebug("VideoLayerShm::playingStopped");
	VideoLayerRender::playingStopped();
	stop_connection();
}

void VideoLayerShm::gotVO(QString vo) {
	qDebug() << "VideoLayerShm::gotVO:" << vo;
	VideoLayerRender::gotVO(vo);
	if (vo == "shm") connect_timer->start(); else stop_connection();
}

void VideoLayerShm::render_slot() {
	static uint32_t last_frame = 0;
	//qDebug("VideoLayerShm::render_slot");

	if (header->frame_count == last_frame) return;
	last_frame = header->frame_count;

	//qDebug("VideoLayerShm::render_slot: frame_count: %d", header->frame_count);
	//qDebug("VideoLayerShm::render_slot: %d %d", header->width, header->height);
	//qDebug("VideoLayerShm::render_slot: header size: %d busy: %d", header->header_size, header->busy);
	//qDebug("VideoLayerShm::render_slot: %p %p", header, image_data);

	uint32_t count = 0;
	while (header->busy == 1 && count < 1000000) { count++; }
	/*
	if (count != 0) {
		qDebug("VideoLayerShm::render_slot: count: %d", count);
	}
	*/

	render();
}

void VideoLayerShm::stop_slot() {
	qDebug("VideoLayerShm::stop_slot");
}

void VideoLayerShm::start_connection() {
	qDebug("VideoLayerShm::start_connection");

	shm_fd = shm_open(buffer_name.toLatin1().constData(), O_RDONLY, S_IRUSR);

	if (shm_fd == -1) {
		qDebug("VideoLayerShm::start_connection: shm_open failed");
		return;
	}

	header = (header_t *) mmap(NULL, sizeof(header), PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("VideoLayerShm::start_connection: mmap failed");
		return;
	}

	qDebug("VideoLayerShm::start_connection: bytes: %d stride: %d planes: %d", header->bytes, header->stride, header->planes);
	qDebug("VideoLayerShm::start_connection: header size: %d videobuffer size: %d", header->header_size, header->video_buffer_size);

	int image_width = header->width;
	int image_height = header->height;
	int image_bytes = header->bytes;
	//buffer_size = sizeof(header) + image_width * image_height * image_bytes;
	buffer_size =  header->header_size + header->video_buffer_size;
	qDebug("VideoLayerShm::start_connection: %d %d %d %f", image_width, image_height, image_bytes, header->fps);

	header = (header_t *) mmap(NULL, buffer_size, PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("VideoLayerShm::start_connection: mmap failed");
		return;
	}

	if (image_width == 0 || image_height == 0) {
		qDebug("VideoLayerShm::start_connection: wrong image size");
		return;
	}

	image_data = (unsigned char*) header + header->header_size;
	qDebug("VideoLayerShm::start_connection: header: %p image_data: %p", header, image_data);

	init(image_width, image_height, image_bytes, header->format, image_data);
	qDebug("VideoLayerShm::start_connection: header_size: %d format: %d", header->header_size, header->format);

	connect_timer->stop();
	render_timer->setInterval(1000 / ceil(header->fps));
	render_timer->start();
	qDebug("VideoLayerShm::start_connection: timer interval: %d", render_timer->interval());
}

void VideoLayerShm::stop_connection() {
	render_timer->stop();
	connect_timer->stop();

	// Destroy the shared buffer
	if (munmap(header, buffer_size) == -1) {
		qDebug("VideoLayerShm::stop_connection: munmap failed");
	}
}

#include "moc_videolayershm.cpp"
