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

#include "connectionshm.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

static struct header_t {
	uint32_t header_size;
	uint32_t video_buffer_size;
	uint32_t width;
	uint32_t height;
	uint32_t bytes;
	uint32_t stride[3];
	uint32_t planes;
	uint32_t format;
	uint32_t frame_count;
	uint32_t busy;
	float fps;
	//unsigned char * image_buffer;
} * header = 0;


ConnectionShm::ConnectionShm(VideoLayerRender * parent)
	: ConnectionBase(parent)
	, buffer_size(0)
	, image_data(0)
{
	buffer_name = QString("smplayer-%1").arg(QCoreApplication::applicationPid());

	render_timer = new QTimer(this);
	connect(render_timer, SIGNAL(timeout()), this, SLOT(render_slot()));
	render_timer->setInterval(1000 / 60);

	connect_timer = new QTimer(this);
	connect(connect_timer, SIGNAL(timeout()), this, SLOT(start_connection()));
	connect_timer->setInterval(10);
}

ConnectionShm::~ConnectionShm() {
}

void ConnectionShm::start() {
	qDebug("ConnectionShm::start");
	connect_timer->start();
}

void ConnectionShm::stop() {
	qDebug("ConnectionShm::stop");
	stop_connection();
}

void ConnectionShm::render_slot() {
	static uint32_t last_frame = 0;
	//qDebug("ConnectionShm::render_slot");

	if (header == 0) return;

	if (header->frame_count == last_frame) return;

	//qDebug("ConnectionShm::render_slot: frame_count: %d", header->frame_count);
	//qDebug("ConnectionShm::render_slot: %d %d", header->width, header->height);
	//qDebug("ConnectionShm::render_slot: header size: %d busy: %d", header->header_size, header->busy);
	//qDebug("ConnectionShm::render_slot: %p %p", header, image_data);

	
	if (header->busy == 1) {
		//qDebug("ConnectionShm::render_slot: busy (f: %d)", header->frame_count);
		return;
	}
	//qDebug("ConnectionShm::render_slot: frame_count: %d", header->frame_count);


	/*
	uint32_t count = 0;
	while (header->busy == 1 && count < 1000000) { count++; }
	if (count != 0) {
		qDebug("ConnectionShm::render_slot: count: %d", count);
	}
	*/

	video_window->render();
	last_frame = header->frame_count;
}

void ConnectionShm::stop_slot() {
	qDebug("ConnectionShm::stop_slot");
}

void ConnectionShm::start_connection() {
	qDebug("ConnectionShm::start_connection");

	shm_fd = shm_open(buffer_name.toLatin1().constData(), O_RDONLY, S_IRUSR);

	if (shm_fd == -1) {
		qDebug("ConnectionShm::start_connection: shm_open failed");
		return;
	}

	header = (header_t *) mmap(NULL, sizeof(header), PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("ConnectionShm::start_connection: mmap failed");
		return;
	}

	qDebug("ConnectionShm::start_connection: bytes: %d stride: %d %d %d planes: %d", header->bytes, header->stride[0], header->stride[1], header->stride[2], header->planes);
	qDebug("ConnectionShm::start_connection: header size: %d videobuffer size: %d", header->header_size, header->video_buffer_size);

	int image_width = header->width;
	int image_height = header->height;
	int image_bytes = header->bytes;

	buffer_size =  header->header_size + header->video_buffer_size;
	qDebug("ConnectionShm::start_connection: %d %d %d %f", image_width, image_height, image_bytes, header->fps);

	header = (header_t *) mmap(NULL, buffer_size, PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("ConnectionShm::start_connection: mmap failed");
		return;
	}

	if (image_width == 0 || image_height == 0) {
		qDebug("ConnectionShm::start_connection: wrong image size");
		return;
	}

	image_data = (unsigned char*) header + header->header_size;
	//qDebug("ConnectionShm::start_connection: header: %p image_data: %p", header, image_data);

	// Convert formats from mpv
	uint32_t format = header->format;
	switch (format) {
		case ConnectionBase::MP_YUV420P: format = ConnectionBase::I420; break;
		case ConnectionBase::MP_UYVY422: format = ConnectionBase::UYVY; break;
		case ConnectionBase::MP_RGB24: format = ConnectionBase::RGB24; break;
	}

	video_window->init(image_width, image_height, image_bytes, format, image_data);
	qDebug("ConnectionShm::start_connection: header_size: %d format: %s", header->header_size,
           formatToString(format).toLatin1().constData());

	connect_timer->stop();

	int fps = header->fps;
	if (fps < 10) fps = 60;
	render_timer->setInterval(1000 / fps / 2);
	render_timer->start();
	qDebug("ConnectionShm::start_connection: timer interval: %d", render_timer->interval());
}

void ConnectionShm::stop_connection() {
	render_timer->stop();
	connect_timer->stop();

	if (header == 0) return;

	// Destroy the shared buffer
	if (munmap(header, buffer_size) == -1) {
		qDebug("ConnectionShm::stop_connection: munmap failed");
	}
	header = 0;
	image_data = 0;
}

#include "moc_connectionshm.cpp"
