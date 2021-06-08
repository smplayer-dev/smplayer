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

#ifdef USE_YUV
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#define VERTEXIN 0
#define TEXTUREIN 1
#endif // USE_YUV

#endif // USE_GL_WINDOW

VideoLayerRender::VideoLayerRender(QWidget* parent, Qt::WindowFlags f)
	: VideoLayer(parent, f)
	, is_vo_to_render(false)
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
	qDebug() << "VideoLayerRender::VideoLayerRender: format:" << format();

	#ifdef USE_YUV
	supported_formats << I420;
	#endif
	#ifdef USE_RGB
	supported_formats << RGB24 << RGB16;
	format_to_gl[RGB24] = { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
	format_to_gl[RGB16] = { GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV };
	#endif
#else
	supported_formats << RGB24 << RGB16;
	#ifdef USE_YUV
	supported_formats << I420;
	#endif
	format_to_image[RGB24] = QImage::Format_RGB888;
	format_to_image[RGB16] = QImage::Format_RGB16;
#endif
}

VideoLayerRender::~VideoLayerRender() {
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
	update();
}

void VideoLayerRender::gotVO(QString vo) {
	qDebug() << "VideoLayerRender::gotVO:" << vo;
	is_vo_to_render = (vo == target_vo);
#if REPAINT_BACKGROUND_OPTION
	if (is_vo_to_render) setUpdatesEnabled(true);
#endif
	VideoLayer::gotVO(vo);
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
	} else
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
	if (image_buffer == 0) {
		VideoLayer::paintGL();
		return;
	}

	if (playing && is_vo_to_render) {
	#ifdef USE_YUV
		if (image_format == I420) {
			paintYUV();
		}
	#endif
	#ifdef USE_RGB
		if (image_format == RGB24 || image_format == RGB16) {
			paintRGB();
		}
	#endif
	} else {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	glFlush();
}

void VideoLayerRender::resizeGL(int w, int h) {
	qDebug("VideoLayerRender::resizeGL: w: %d h: %d", w, h);

	if (image_buffer == 0) {
		VideoLayer::resizeGL(w, h);
		return;
	}

	glViewport(0, 0, w, h);

#ifdef USE_RGB
//	glClear(GL_COLOR_BUFFER_BIT);

//	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, w, 0, h);
	glOrtho(0, w, 0, h, -1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, texture);
#endif
}

void VideoLayerRender::initializeGL() {
	qDebug("VideoLayerRender::initializeGL: w: %d h: %d", width(), height());

	initializeOpenGLFunctions();

#ifdef USE_YUV
	initializeYUV();
#endif
#ifdef USE_RGB
	initializeRGB();
#endif
}

#ifdef USE_RGB
void VideoLayerRender::initializeRGB() {
	textureRGB = 0;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, width(), 0, height());
	glOrtho(0, width(), 0, height(), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
//	glGenTextures(3, &textureRGB);
	glBindTexture(GL_TEXTURE_2D, textureRGB);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureRGB);
//	glDisable(GL_TEXTURE_2D);
}

void VideoLayerRender::paintRGB() {
	//qDebug("VideoLayerRender::paintRGB");
#ifdef USE_YUV
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
#endif

	Gformat f = format_to_gl[image_format];
	glBindTexture(GL_TEXTURE_2D, textureRGB);
	glTexImage2D(GL_TEXTURE_2D, 0, f.internal_format, image_width, image_height, 0, f.format, f.type, image_buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, f.format, f.type, image_buffer);

	GLenum error_code = glGetError();
	if (error_code != GL_NO_ERROR) {
		qDebug() << "VideoLayerRender::paintRGB: glTexSubImage2D: error:" << error_code;
	}

	if (error_code == GL_NO_ERROR) {
		glBegin(GL_QUADS);
		glTexCoord2i(0,0); glVertex2i(0, height());
		glTexCoord2i(0,1); glVertex2i(0,0);
		glTexCoord2i(1,1); glVertex2i( width(), 0);
		glTexCoord2i(1,0); glVertex2i(width(), height());
		glEnd();
	}
}
#endif

#ifdef USE_YUV
void VideoLayerRender::initializeYUV() {
	glEnable(GL_DEPTH_TEST);

	static const GLfloat vertices[]{
		//Vertex coordinates
		-1.0f,-1.0f,
		-1.0f,+1.0f,
		+1.0f,+1.0f,
		+1.0f,-1.0f,
		//Texture coordinates
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
	};

	vbo.create();
	vbo.bind();
	vbo.allocate(vertices, sizeof(vertices));

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
	const char *vsrc =
   "attribute vec4 vertexIn; \
    attribute vec2 textureIn; \
    varying vec2 textureOut;  \
    void main(void)           \
    {                         \
        gl_Position = vertexIn; \
        textureOut = textureIn; \
    }";
	vshader->compileSourceCode(vsrc);

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
	QString rgb_bt_601 = "mat3(1,1,1, 0, -0.39,2.03, 1.14, -0.58,0) * yuv;";
	QString rgb_bt_709 = "mat3(1,1,1, 0, -0.21,2.13, 1.28, -0.38,0) * yuv;";
	QString rgb_jpeg   = "mat3(1,1,1, 0, -0.34,1.77, 1.40, -0.72,0) * yuv;";
	QString rgb_conv = rgb_bt_709;
	QString fsrc = "varying vec2 textureOut; \
    uniform sampler2D tex_y; \
    uniform sampler2D tex_u; \
    uniform sampler2D tex_v; \
    void main(void) \
    { \
        vec3 yuv; \
        vec3 rgb; \
        yuv.x = texture2D(tex_y, textureOut).r - (16.0/255.0); \
        yuv.y = texture2D(tex_u, textureOut).r - 0.5; \
        yuv.z = texture2D(tex_v, textureOut).r - 0.5; \
        rgb = " + rgb_conv +
       "gl_FragColor = vec4(rgb, 1); \
    }";
	fshader->compileSourceCode(fsrc);

	program = new QOpenGLShaderProgram(this);
	program->addShader(vshader);
	program->addShader(fshader);
	program->bindAttributeLocation("vertexIn", VERTEXIN);
	program->bindAttributeLocation("textureIn", TEXTUREIN);
	program->link();
	program->bind();
	program->enableAttributeArray(VERTEXIN);
	program->enableAttributeArray(TEXTUREIN);
	program->setAttributeBuffer(VERTEXIN, GL_FLOAT, 0, 2, 2 * sizeof(GLfloat));
	program->setAttributeBuffer(TEXTUREIN, GL_FLOAT, 8 * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));

	textureUniformY = program->uniformLocation("tex_y");
	textureUniformU = program->uniformLocation("tex_u");
	textureUniformV = program->uniformLocation("tex_v");
	textureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureV = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureY->create();
	textureU->create();
	textureV->create();
	idY = textureY->textureId();
	idU = textureU->textureId();
	idV = textureV->textureId();
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void VideoLayerRender::paintYUV() {
	//qDebug("VideoLayerRender::paintYUV");

#ifdef USE_RGB
	program->bind();
#endif

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, idY);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE1, idU);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width >> 1, image_height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer + image_width * image_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, idV);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width >> 1, image_height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer + image_width * image_height * 5 /4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glUniform1i(textureUniformY, 0);
	glUniform1i(textureUniformU, 1);
	glUniform1i(textureUniformV, 2);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
#endif
#endif

#if !defined(USE_GL_WINDOW) && defined(USE_YUV)
extern "C" {
#include <libswscale/swscale.h>
}
void VideoLayerRender::YUV420PtoRGB24(unsigned char* yuv_src, unsigned char* rgb_dst, int w, int h) {
	//qDebug("VideoLayerRender::YUV420PtoRGB24: %p %p", yuv_src, rgb_dst);

	struct SwsContext *sws_ctx = NULL;
	sws_ctx = sws_getContext(w, h, AV_PIX_FMT_YUV420P, w, h, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

	uint8_t * rgb24[1] = { rgb_dst };
	int rgb24_stride[1] = { w * 3 };

	uint8_t * yuv[3];
	yuv[0] = { yuv_src };
	yuv[1] = { yuv_src + w * h };
	yuv[2] = { yuv_src + w * h *5 / 4 };

	int yuv_stride[3];
	yuv_stride[0] = { w };
	yuv_stride[1] = { w / 2 };
	yuv_stride[2] = { w / 2 };

	sws_scale(sws_ctx, yuv, yuv_stride, 0, h, rgb24, rgb24_stride);
}
#endif

#include "moc_videolayerrender.cpp"
