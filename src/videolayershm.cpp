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
#include <QMetaObject>
#include <QPainter>
#include <QDebug>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

struct header_t {
	uint32_t width;
	uint32_t height;
	uint32_t bytes;
	uint32_t stride;
	uint32_t format;
	uint32_t frame_count;
	unsigned char * image_buffer;
} * header;

VideoLayerShm::VideoLayerShm(QWidget* parent, Qt::WindowFlags f)
	: VideoLayer(parent, f)
{
	buffer_name = QString("smplayer-%1").arg(QCoreApplication::applicationPid());

	render_timer = new QTimer(this);
	connect(render_timer, SIGNAL(timeout()), this, SLOT(render_slot()));
	render_timer->setInterval(1000 / 60);

	connect_timer = new QTimer(this);
	connect(connect_timer, SIGNAL(timeout()), this, SLOT(start_connection()));
	connect_timer->setInterval(10);
}

VideoLayerShm::~VideoLayerShm() {
}

void VideoLayerShm::playingStarted() {
	qDebug("VideoLayerShm::playingStarted");
	VideoLayer::playingStarted();
}

void VideoLayerShm::playingStopped() {
	qDebug("VideoLayerShm::playingStopped");
	VideoLayer::playingStopped();
	stop_connection();
	update();
}

void VideoLayerShm::gotVO(QString vo) {
	qDebug() << "VideoLayerShm::gotVO:" << vo;
	if (vo == "shm") connect_timer->start(); else stop_connection();
}

void VideoLayerShm::render_slot() {
	static uint32_t last_frame = 0;
	//qDebug("VideoLayerShm::render_slot");

	if (header->frame_count == last_frame) return;

	last_frame = header->frame_count;

	//qDebug("VideoLayerShm::render_slot: frame_count: %d", header->frame_count);
	//qDebug("VideoLayerShm::render_slot: %d %d", header->width, header->height);
	//qDebug("VideoLayerShm::render_slot: %p %p", header, &header->image_buffer);

	memcpy(image_buffer, &header->image_buffer, image_width * image_height * image_bytes);
	//image_buffer = (unsigned char*) &header->image_buffer;

	QImage i(image_buffer, image_width, image_height, image_width * image_bytes, QImage::Format_RGB888);
	frame = QPixmap::fromImage(i);
	update();
}

void VideoLayerShm::stop_slot() {
	qDebug("VideoLayerShm::stop_slot");
}

void VideoLayerShm::paintEvent(QPaintEvent *event) {
	//qDebug("VideoLayerShm::paintEvent");
	if (playing && !frame.isNull()) {
		QPainter painter(this);
		painter.drawPixmap(0,0,frame.scaled(size()));
	} else {
		VIDEOLAYER_PARENT::paintEvent(event);
	}
}

void VideoLayerShm::start_connection() {
	qDebug("VideoLayerShm::start_connection");

	shm_fd = shm_open(buffer_name.toLatin1().constData(), O_RDONLY, S_IRUSR);

	if (shm_fd == -1) {
		qDebug("VideoLayerShm::start_connection: shm_open failed");
		return;
	}

	header = (header_t *) mmap(NULL, sizeof(header),
                                  PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("VideoLayerShm::start_connection: mmap failed");
		return;
	}

	image_width = header->width;
	image_height = header->height;
	image_bytes = header->bytes;
	qDebug("VideoLayerShm::start_connection: %d %d %d", image_width, image_height, image_bytes);

	header = (header_t *) mmap(NULL, image_width * image_height * image_bytes,
                                  PROT_READ, MAP_SHARED, shm_fd, 0);

	if (header == MAP_FAILED) {
		qDebug("VideoLayerShm::start_connection: mmap failed");
		return;
	}

	image_buffer = (unsigned char*) malloc(image_width * image_height * image_bytes);

	connect_timer->stop();
	render_timer->start();
}

void VideoLayerShm::stop_connection() {
	render_timer->stop();
	connect_timer->stop();

}

#include "moc_videolayershm.cpp"
