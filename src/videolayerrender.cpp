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

#include "videolayerrender.h"
#include <QCoreApplication>
#include <QTimer>
#include <QMetaObject>
#include <QPainter>
#include <QDebug>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

#ifdef USE_GL_WINDOW
#include <QSurfaceFormat>
#include "openglrenderer.h"
#endif // USE_GL_WINDOW

#ifdef USE_SHM
#include "connectionshm.h"
#endif
#ifdef USE_COREVIDEO_BUFFER
#include "connectioncv.h"
#endif

VideoLayerRender::VideoLayerRender(QWidget* parent, Qt::WindowFlags f)
	: VideoLayer(parent, f)
	, is_vo_to_render(false)
	, image_width(0)
	, image_height(0)
	, image_bytes(0)
	, image_format(0)
	, image_buffer(0)
#if !defined(USE_GL_WINDOW) && defined(USE_YUV)
	, conv_buffer(0)
#endif
{
#ifdef USE_GL_WINDOW
	QSurfaceFormat fmt;
	fmt.setSwapInterval(1);
	fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	//fmt.setColorSpace(QSurfaceFormat::sRGBColorSpace);
	setFormat(fmt);
	//qDebug() << "VideoLayerRender::VideoLayerRender: format:" << format();

	renderer = new OpenGLRenderer(this);
	supported_formats << RGB24 << RGB16 << I420 << YUY2 << UYVY;
#else
	supported_formats << RGB24 << RGB16;
	#ifdef USE_YUV
	supported_formats << I420 << YUY2;
	#endif
	format_to_image[RGB24] = QImage::Format_RGB888;
	format_to_image[RGB16] = QImage::Format_RGB16;
#endif

#ifdef USE_SHM
	shm = new ConnectionShm(this);
#endif
#ifdef USE_COREVIDEO_BUFFER
	cv = new ConnectionCV(this);
#endif
}

VideoLayerRender::~VideoLayerRender() {
#ifdef USE_GL_WINDOW
	delete renderer;
#endif
}

void VideoLayerRender::init(int width, int height, int bytes_per_pixel, uint32_t format, unsigned char* buffer) {
	qDebug("VideoLayerRender::init: w: %d h: %d bytes: %d format: %d", width, height, bytes_per_pixel, format);

	image_width = width;
	image_height = height;
	image_bytes = bytes_per_pixel;
	image_format = format;
	image_buffer = buffer;

	if (!isFormatSupported(image_format)) {
		qDebug("VideoLayerRender::init: error: format %d is not supported", image_format);
	}
#ifdef USE_GL_WINDOW
	renderer->setFormat((ImageFormat) image_format);
#endif
}

void VideoLayerRender::playingStarted() {
	qDebug("VideoLayerRender::playingStarted");
	VideoLayer::playingStarted();
}

void VideoLayerRender::playingStopped() {
	qDebug("VideoLayerRender::playingStopped");

	is_vo_to_render = false;
	image_buffer = 0;
#if !defined(USE_GL_WINDOW) && defined(USE_YUV)
	if (conv_buffer != 0) {
		free(conv_buffer);
		conv_buffer = 0;
	}
#endif
	VideoLayer::playingStopped();
#ifdef USE_SHM
	shm->stop();
#endif
#ifdef USE_COREVIDEO_BUFFER
	cv->stop();
#endif
	update();
}

void VideoLayerRender::gotVO(QString vo) {
	qDebug() << "VideoLayerRender::gotVO:" << vo;
	is_vo_to_render = (vo == target_vo);
#if REPAINT_BACKGROUND_OPTION
	if (is_vo_to_render) setUpdatesEnabled(true);
#endif
	VideoLayer::gotVO(vo);

	is_vo_to_render = false;
#ifdef USE_SHM
	if (vo == "shm") {
		shm->start();
		is_vo_to_render = true;
	} else {
		shm->stop();
	}
#endif
#ifdef USE_COREVIDEO_BUFFER
	if (vo == "corevideo") {
		cv->start();
		is_vo_to_render = true;
	} else {
		cv->stop();
	}
#endif

}

void VideoLayerRender::render() {
	//qDebug("VideoLayerRender::render: buffer: %p, w: %d h: %d, bytes: %d", image_buffer, image_width, image_width, image_bytes);

	if (image_buffer == 0) return;

	if (!isFormatSupported(image_format)) {
		//qDebug("VideoLayerRender::render: format %d not supported", image_format);
		return;
	}

#ifndef USE_GL_WINDOW
	#ifdef USE_YUV
	if (conv_buffer == 0) {
		conv_buffer = (unsigned char*) malloc(image_width * image_height * 3);
		qDebug("VideoLayerRender::render: %d %d conv_buffer: %p", image_width, image_height, conv_buffer);
	}
	if (image_format == I420) {
		YUV420PtoRGB24(image_buffer, conv_buffer, image_width, image_height);
		QImage i(conv_buffer, image_width, image_height, image_width * 3, QImage::Format_RGB888);
		frame = QPixmap::fromImage(i);
	}
	else
	if (image_format == YUY2) {
		YUY2toRGB24(image_buffer, conv_buffer, image_width, image_height);
		QImage i(conv_buffer, image_width, image_height, image_width * 3, QImage::Format_RGB888);
		frame = QPixmap::fromImage(i);
	}
	else
	#endif
	{
		QImage i(image_buffer, image_width, image_height, image_width * image_bytes, (QImage::Format) format_to_image[image_format]);
		frame = QPixmap::fromImage(i);
	}
#endif
	update();
}

#ifndef USE_GL_WINDOW
void VideoLayerRender::paintEvent(QPaintEvent *event) {
	//qDebug("VideoLayerRender::paintEvent");
	if (playing && is_vo_to_render && !frame.isNull()) {
		QPainter painter(this);
		painter.drawPixmap(0,0,frame.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	} else {
		VideoLayer::paintEvent(event);
	}
}
#endif

#ifdef USE_GL_WINDOW
void VideoLayerRender::paintGL() {
	if (image_buffer && playing && is_vo_to_render) {
		renderer->paintGL(width(), height(), image_width, image_height, image_format, image_buffer);
	} else {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void VideoLayerRender::resizeGL(int w, int h) {
	qDebug("VideoLayerRender::resizeGL: w: %d h: %d", w, h);

	glViewport(0, 0, w, h);
	renderer->resizeGL(w, h);
}

void VideoLayerRender::initializeGL() {
	qDebug("VideoLayerRender::initializeGL: w: %d h: %d", width(), height());

	initializeOpenGLFunctions();
	qDebug("VideoLayerRender::initializeGL: GL_VERSION: %s", glGetString(GL_VERSION));

	glClearColor(0.0, 0.0, 0.0, 1.0);

	renderer->initializeGL(width(), height());
}
#endif // USE_GL_WINDOW

#if !defined(USE_GL_WINDOW) && defined(USE_YUV)
extern "C" {
#include <libswscale/swscale.h>
}
void VideoLayerRender::YUV420PtoRGB24(unsigned char* yuv_src, unsigned char* rgb_dst, int w, int h) {
	//qDebug("VideoLayerRender::YUV420PtoRGB24: %d %d, %p %p", w, h, yuv_src, rgb_dst);

	struct SwsContext * sws_ctx = NULL;
	sws_ctx = sws_getContext(w, h, AV_PIX_FMT_YUV420P, w, h, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

	uint8_t * rgb24[1] = { rgb_dst };
	int rgb24_stride[1] = { w * 3 };

	uint8_t * yuv[3];
	yuv[0] = { yuv_src };
	yuv[1] = { yuv_src + w * h };
	yuv[2] = { yuv_src + (w * h) + ((w * h) / 2) };

	int yuv_stride[3];
	yuv_stride[0] = { w };
	yuv_stride[1] = { w / 2 };
	yuv_stride[2] = { w / 2 };

	sws_scale(sws_ctx, yuv, yuv_stride, 0, h, rgb24, rgb24_stride);
}

void VideoLayerRender::YUY2toRGB24(unsigned char* yuv_src, unsigned char* rgb_dst, int w, int h) {
	//qDebug("VideoLayerRender::YUY2toRGB24: %d %d, %p %p", w, h, yuv_src, rgb_dst);

	struct SwsContext * sws_ctx = NULL;
	sws_ctx = sws_getContext(w, h, AV_PIX_FMT_YUYV422, w, h, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

	uint8_t * rgb24[1] = { rgb_dst };
	int rgb24_stride[1] = { w * 3 };

	uint8_t * yuv[1] = { yuv_src };
	int yuv_stride[1] = { w * 2 };

	sws_scale(sws_ctx, yuv, yuv_stride, 0, h, rgb24, rgb24_stride);
}
#endif

#include "moc_videolayerrender.cpp"
