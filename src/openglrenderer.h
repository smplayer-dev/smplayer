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

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "connectionbase.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

class OpenGLRenderer : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	enum PackedPattern { UYVY = 1, YUYV = 3 };

	OpenGLRenderer(QObject * parent = 0);
	~OpenGLRenderer();

	void setFormat(ConnectionBase::Format format);
	int format() { return current_format; };

	void setColorspace(ConnectionBase::Colorspace c, ConnectionBase::Primary primary);
	int colorspace() { return current_colorspace; }
	int colorspacePrimary() { return current_primary; }

	virtual void initializeGL(int window_width, int window_height);
	virtual void paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer);
	virtual void resizeGL(int w, int h);

protected:
	void configureTexture(QOpenGLTexture &texture);
	void createFragmentShader();

	QString rgbShader();
	QString yuv420Shader();
	QString packedShader(PackedPattern p = YUYV);
	QString colorspaceMat();
	QString HDRColorConversionCode();

	QString glslCode(QString code);
	QString glsl_version;

	QOpenGLBuffer vertex_buffer;

	QOpenGLTexture * textures[3];

	QOpenGLShader * vertex_shader;
	QOpenGLShader * fragment_shader;
	QOpenGLShaderProgram program;
	QOpenGLVertexArrayObject vao;

	GLuint textureUniformU;
	GLuint textureUniformV;
	GLuint textureUniformY;
	GLuint textureUniformStepX;

	QMatrix3x3 cms_matrix;
	GLfloat luma;

	int current_format;
	int current_colorspace;
	int current_primary;
};

#endif

