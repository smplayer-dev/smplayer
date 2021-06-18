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

#include "rendererrgb.h"
#include "videolayerrender.h"
#include <QDebug>

RendererRGB::RendererRGB(QObject * parent) : Renderer(parent)
{
	format_to_gl[VideoLayerRender::RGB24] = { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
	format_to_gl[VideoLayerRender::RGB16] = { GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV };
}

RendererRGB::~RendererRGB() {
}

void RendererRGB::initializeGL(int window_width, int window_height) {
	initializeOpenGLFunctions();

	textureRGB = 0;

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1.0, 1.0);
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

void RendererRGB::paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer) {
#ifdef USE_YUV
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
#endif

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Gformat f = format_to_gl[image_format];
	glBindTexture(GL_TEXTURE_2D, textureRGB);
	glTexImage2D(GL_TEXTURE_2D, 0, f.internal_format, image_width, image_height, 0, f.format, f.type, image_buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, f.format, f.type, image_buffer);

	GLenum error_code = glGetError();
	if (error_code != GL_NO_ERROR) {
		qDebug() << "RendererRGB::paintGL: glTexSubImage2D: error:" << error_code;
	}

	if (error_code == GL_NO_ERROR) {
		glBegin(GL_QUADS);
		glTexCoord2i(0,0); glVertex2i(0, window_height);
		glTexCoord2i(0,1); glVertex2i(0,0);
		glTexCoord2i(1,1); glVertex2i(window_width, 0);
		glTexCoord2i(1,0); glVertex2i(window_width, window_height);
		glEnd();
	}
	glFlush();

}

void RendererRGB::resizeGL(int w, int h) {
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
}

