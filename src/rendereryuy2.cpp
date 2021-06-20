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

RendererYUY2::RendererYUY2(QObject * parent)
	: Renderer(parent)
	, vertex_buffer(QOpenGLBuffer::VertexBuffer)
{
}

RendererYUY2::~RendererYUY2() {
}

void RendererYUY2::initializeGL(int window_width, int window_height) {
	qDebug("RendererYUY2::initializeGL");
	initializeOpenGLFunctions();

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	static const GLfloat coordinates[2][4][2]{
		{
			/* Vertex coordinates */
			{ -1.0f, -1.0f },
			{ -1.0f, +1.0f },
			{ +1.0f, +1.0f },
			{ +1.0f, -1.0f },
		},
		{
			/* Texture coordinates */
			{ 0.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
		},
	};

	vertex_buffer.create();
	vertex_buffer.bind();
	vertex_buffer.allocate(coordinates, sizeof(coordinates));

	/* Create Vertex Shader */
	vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	QString code = QString(
		"attribute vec4 vertexIn;"
		"attribute vec2 textureIn;"
		"varying vec2 textureOut;"
		"void main(void)"
		"{"
		"gl_Position = vertexIn;"
		"textureOut = textureIn;"
		"}");
	if (!vertex_shader->compileSourceCode(code)) {
		qWarning() << "RendererYUY2::initializeGL:" << vertex_shader->log();
	}
	program.addShader(vertex_shader);

	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/* Create the fragment shader */
	int attributeVertex;
	int attributeTexture;

	fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);

	code = QString(
		"varying vec2 textureOut;"
		"uniform sampler2D tex_y;"
		"uniform float tex_stepx;"
		"void main(void)"
		"{"
		"mat3 yuv2rgb_bt601_mat = mat3("
		"vec3(1.164,  1.164, 1.164),"
		"vec3(0.000, -0.392, 2.017),"
		"vec3(1.596, -0.813, 0.000)"
		");"
		"vec3 yuv2rgb_bt601_offset = vec3(0.063, 0.500, 0.500);"
		"vec2 pos = textureOut;"
		"float f_x = fract(pos.x / tex_stepx);"
		"vec4 left = texture2D(tex_y, vec2(pos.x - f_x * tex_stepx, pos.y));"
		"vec4 right = texture2D(tex_y, vec2(pos.x + (1.0 - f_x) * tex_stepx , pos.y));"
	);

	// YUV_PATTERN_YUYV
	code += QString(
		"float y_left = mix(left.r, left.b, f_x * 2.0);"
		"float y_right = mix(left.b, right.r, f_x * 2.0 - 1.0);"
		"vec2 uv = mix(left.ga, right.ga, f_x);"
	);

	code += QString(
		"float y = mix(y_left, y_right, step(0.5, f_x));"
		"vec3 rgb = yuv2rgb_bt601_mat * (vec3(y, uv) - yuv2rgb_bt601_offset);"
		"gl_FragColor = vec4(rgb, 1.0);"
		"}"
	);

	if (!fragment_shader->compileSourceCode(code)) {
		qWarning() << "RendererYUY2::initializeGL:" << fragment_shader->log();
	}

	program.addShader(fragment_shader);
	program.link();
	program.bind();

	attributeVertex = program.attributeLocation("vertexIn");
	attributeTexture = program.attributeLocation("textureIn");

	program.enableAttributeArray(attributeVertex);
	program.setAttributeBuffer(attributeVertex,
					  GL_FLOAT,
					  0,
					  2,
					  2 * sizeof(GLfloat));

	program.enableAttributeArray(attributeTexture);
	program.setAttributeBuffer(attributeTexture,
					  GL_FLOAT,
					  8 * sizeof(GLfloat),
					  2,
					  2 * sizeof(GLfloat));

	textureUniformY = program.uniformLocation("tex_y");
	textureUniformU = program.uniformLocation("tex_u");
	textureUniformV = program.uniformLocation("tex_v");
	textureUniformStepX = program.uniformLocation("tex_stepx");

	texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	texture->create();
}

void RendererYUY2::paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer) {
	//qDebug("RendererYUY2::paintGL");

#if defined(USE_YUV) || defined(USE_RGB)
	program.bind();
#endif

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture->textureId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RGBA,
			     image_width / 2,
			     image_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,
			     image_buffer);
	program.setUniformValue(textureUniformY, 0);

	/*
	 * The shader needs the step between two texture pixels in the
	 * horizontal direction, expressed in texture coordinate units
	 * ([0, 1]). There are exactly width - 1 steps between the
	 * leftmost and rightmost texels.
	 */
	program.setUniformValue(textureUniformStepX,
					       1.0f / (image_width / 2 - 1));

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RendererYUY2::resizeGL(int w, int h) {
	qDebug("RendererYUY2::resizeGL");
}

