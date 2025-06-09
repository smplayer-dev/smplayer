/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef VIDEOLAYERRENDER_H
#define VIDEOLAYERRENDER_H

#include "videolayer.h"
#include <QList>
#include <QMap>
#include <stdint.h>

#define USE_YUV

#ifdef USE_GL_WINDOW
#include <QOpenGLFunctions>
class OpenGLRenderer;
#endif

class QTimer;
class ConnectionBase;

//#define COUNT_FPS

class VideoLayerRender : public VideoLayer
#ifdef USE_GL_WINDOW
	, protected QOpenGLFunctions
#endif
{
	Q_OBJECT

public:
	VideoLayerRender(QWidget* parent = 0, Qt::WindowFlags f = QFlag(0));
	~VideoLayerRender();

	void init(int width, int height, int bytes_per_pixel, uint32_t format, unsigned char* buffer);
	void setImageBuffer(unsigned char* buffer) { image_buffer = buffer; }
	void setColorspace(uint32_t space, uint32_t primary);

	bool isFormatSupported(uint32_t format) {
		return supported_formats.contains(format);
	}

public slots:
	virtual void playingStarted();
	virtual void playingStopped();
	virtual void gotVO(QString);

	virtual void render();

protected:
	QList<ConnectionBase *> connections;
	QList<uint32_t> supported_formats;
	bool is_vo_to_render;

	int image_width;
	int image_height;
	int image_bytes;
	uint32_t image_format;
	unsigned char* image_buffer;
#if !defined(USE_GL_WINDOW) && defined(USE_YUV)
	unsigned char* conv_buffer;
#endif

protected:
#ifdef USE_GL_WINDOW
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int w, int h);

	OpenGLRenderer * renderer;
#else
	virtual void paintEvent(QPaintEvent *event);
	#ifdef USE_YUV
	void YUV420PtoRGB24(unsigned char* yuv_src, unsigned char* rgb_dst, int w, int h);
	void YUY2toRGB24(unsigned char* yuv_src, unsigned char* rgb_dst, int w, int h);
	#endif
	QPixmap frame;
	QMap<uint32_t, int> format_to_image;
#endif

#ifdef COUNT_FPS
signals:
	void renderedFps(int);

protected slots:
	void updateFps();

protected:
	QTimer * fps_timer;
	int current_fps;
#endif
};

#endif
