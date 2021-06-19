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

#include "rendereryuy2.h"
#include "videolayerrender.h"

#include <QGLShaderProgram>
#include <QDebug>

RendererYUY2::RendererYUY2(QObject * parent) : Renderer(parent)
{
	program = new QGLShaderProgram(this);
}

RendererYUY2::~RendererYUY2() {
}

void RendererYUY2::initializeGL(int window_width, int window_height) {
	qDebug("RendererYUY2::initializeGL");
	initializeOpenGLFunctions();

	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glBlendFunc(GL_ONE, GL_ZERO);

	glGenTextures(1, screen_texture);

	glBindTexture(GL_TEXTURE_2D, screen_texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	QString code = QString("uniform sampler2D tex;"
				   "uniform float texl_w;"
				   "uniform float tex_w;"
				   "uniform float tex_h;"
				   "void main()"
				   "{"
				   "   float y, u, v;"
				   "   vec4 luma_chroma;"
				   "   vec2 xy = vec2(gl_TexCoord[0].xy);"
				   "   float xcoord = floor(xy.x * tex_w);"
				   "   float ycoord = floor(xy.y * tex_h);");

	code += QString("if (mod(xcoord, 2.0) == 0.0) {"
			       "   luma_chroma = texture2D(tex, xy);"
			       "   y = luma_chroma.r;"
			       "} else {"
			       "   luma_chroma = texture2D(tex, vec2(xy.x - texl_w, xy.y));"
			       "   y = luma_chroma.b;"
			       "}"
			       "u = luma_chroma.g - 0.5;"
			       "v = luma_chroma.a - 0.5;"
			       );

	code += QString("   y = (255.0 / 219.0) * (y - (16.0 / 255.0));"
			       "   u = (255.0 / 224.0) * u;"
			       "   v = (255.0 / 224.0) * v;"
			       );

	code += QString("   float r = y + 1.5701 * v;"
			       "   float g = y - 0.1870 * u - 0.4664 * v;"
			       "   float b = y + 1.8556 * u;"
			       );

	code += QString("   r = (r <= -0.081) ? -pow((r - 0.099) / -1.099, 1.0 / 0.45) : "
			       "        ((r < 0.081) ? r / 4.5 : pow((r + 0.099) / 1.099, 1.0 / 0.45));"
			       "   g = (g <= -0.081) ? -pow((g - 0.099) / -1.099, 1.0 / 0.45) : "
			       "        ((g < 0.081) ? g / 4.5 : pow((g + 0.099) / 1.099, 1.0 / 0.45));"
			       "   b = (b <= -0.081) ? -pow((b - 0.099) / -1.099, 1.0 / 0.45) : "
			       "        ((b < 0.081) ? b / 4.5 : pow((b + 0.099) / 1.099, 1.0 / 0.45));"
			       );

	code += QString("   r = (r < -0.0031308) ? -1.055 * pow(-r, 1.0 / 2.4) + 0.055 : "
		       "        ((r <= 0.0031308) ? r * 12.92 : 1.055 * pow(r, 1.0 / 2.4) - 0.055);"
		       "   g = (g < -0.0031308) ? -1.055 * pow(-g, 1.0 / 2.4) + 0.055 : "
		       "        ((g <= 0.0031308) ? g * 12.92 : 1.055 * pow(g, 1.0 / 2.4) - 0.055);"
		       "   b = (b < -0.0031308) ? -1.055 * pow(-b, 1.0 / 2.4) + 0.055 : "
		       "        ((b <= 0.0031308) ? b * 12.92 : 1.055 * pow(b, 1.0 / 2.4) - 0.055);"
		       );

	code += QString ("   gl_FragColor = vec4(r, g, b, 0.0);"
			  "}");

	bool src_ok = program->addShaderFromSourceCode(QGLShader::Fragment, code);

	if (!src_ok) {
		qDebug("RendererYUY2::initializeGL: YUY2 shader compilation failed.\n");
	}

	program->bind();
}

void RendererYUY2::paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer) {
	//qDebug("RendererYUY2::paintGL");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width / 2, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	GLenum error_code = glGetError();
	if (error_code != GL_NO_ERROR) {
		qDebug() << "RendererYUY2::paintGL: glTexImage2D: error:" << error_code;
	}

	int idx;
	idx = glGetUniformLocation(program->programId(), "texl_w"); // Texel width
	glUniform1f(idx, 1.0 / image_width);
	idx = glGetUniformLocation(program->programId(), "tex_w"); // Texture width
	glUniform1f(idx, image_width);
	idx = glGetUniformLocation(program->programId(), "tex_h"); // Texture height
	glUniform1f(idx, image_height);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screen_texture[0]);
	GLint Y = glGetUniformLocation(program->programId(), "tex");
	glUniform1i(Y, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width / 2, image_height, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);

	error_code = glGetError();
	if (error_code != GL_NO_ERROR) {
		qDebug() << "RendererYUY2::paintGL: glTexSubImage2D: error:" << error_code;
	}

	glBegin(GL_QUADS);
	glTexCoord2i(0,0); glVertex2i(0, window_height);
	glTexCoord2i(0,1); glVertex2i(0,0);
	glTexCoord2i(1,1); glVertex2i(window_width, 0);
	glTexCoord2i(1,0); glVertex2i(window_width, window_height);
	glEnd();
}

void RendererYUY2::resizeGL(int w, int h) {
	qDebug("RendererYUY2::resizeGL");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
}

