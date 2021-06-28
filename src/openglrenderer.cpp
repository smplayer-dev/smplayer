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

#include "openglrenderer.h"

#include <QGLShaderProgram>
#include <QDebug>

OpenGLRenderer::OpenGLRenderer(QObject * parent)
	: QObject(parent)
	, vertex_buffer(QOpenGLBuffer::VertexBuffer)
	, vertex_shader(0)
	, fragment_shader(0)
	, current_format(0)
{
	for (int n = 0; n < 3; n++) {
		textures[n] = 0;
	}
}

OpenGLRenderer::~OpenGLRenderer() {
}

void OpenGLRenderer::setFormat(ConnectionBase::Format format) {
	qDebug() << "OpenGLRenderer::setFormat:" << ConnectionBase::formatToString(format);

	if (format != current_format) {
		if (program.isLinked()) {
			program.release();
			program.removeShader(fragment_shader);
			delete fragment_shader;
			fragment_shader = 0;
		}
	}
	current_format = format;
}

void OpenGLRenderer::createFragmentShader() {
	int attribute_vertex;
	int attribute_texture;

	fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);

	QString code;

	switch(current_format) {
		case ConnectionBase::RGB24:
		case ConnectionBase::RGB16: code = rgbShader(); break;
		case ConnectionBase::YUY2:  code = packedShader(YUYV); break;
		case ConnectionBase::UYVY:  code = packedShader(UYVY); break;
		case ConnectionBase::I420:  code = yuv420Shader(); break;
	}

	//qDebug() << "OpenGLRenderer::createFragmentShader: code:" << code;

	if (!fragment_shader->compileSourceCode(code)) {
		qWarning() << "OpenGLRenderer::createFragmentShader:" << fragment_shader->log();
	}

	program.addShader(fragment_shader);
	program.link();
	program.bind();

	attribute_vertex = program.attributeLocation("vertexIn");
	attribute_texture = program.attributeLocation("textureIn");

	program.enableAttributeArray(attribute_vertex);
	program.setAttributeBuffer(attribute_vertex, GL_FLOAT, 0, 2, 2 * sizeof(GLfloat));

	program.enableAttributeArray(attribute_texture);
	program.setAttributeBuffer(attribute_texture, GL_FLOAT, 8 * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));

	textureUniformY = program.uniformLocation("tex_y");
	textureUniformU = program.uniformLocation("tex_u");
	textureUniformV = program.uniformLocation("tex_v");
	textureUniformStepX = program.uniformLocation("tex_stepx");

	for (int n = 0; n < 3; n++) {
		if (textures[n] == 0) {
			textures[n] = new QOpenGLTexture(QOpenGLTexture::Target2D);
			textures[n]->create();
		}
	}
}

void OpenGLRenderer::initializeGL(int window_width, int window_height) {
	Q_UNUSED(window_width);
	Q_UNUSED(window_height);

	qDebug("OpenGLRenderer::initializeGL");
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
		qWarning() << "OpenGLRenderer::initializeGL:" << vertex_shader->log();
	}
	program.addShader(vertex_shader);
}

void OpenGLRenderer::configureTexture(QOpenGLTexture &texture) {
	glBindTexture(GL_TEXTURE_2D, texture.textureId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void OpenGLRenderer::paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer) {
	Q_UNUSED(window_width);
	Q_UNUSED(window_height);
	Q_UNUSED(image_format);

	//qDebug("OpenGLRenderer::paintGL: current_format: %d image_format: %d", current_format, image_format);

	if (fragment_shader == 0) {
		createFragmentShader();
	}

	//glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render
	if (current_format == ConnectionBase::YUY2 || current_format == ConnectionBase::UYVY) {
		glActiveTexture(GL_TEXTURE0);
		configureTexture(*textures[0]);

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
		program.setUniformValue(textureUniformStepX, 1.0f / (image_width / 2 - 1));
	}
	else
	if (current_format == ConnectionBase::RGB24) {
		glActiveTexture(GL_TEXTURE0);
		configureTexture(*textures[0]);
		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RGB,
			     image_width,
			     image_height,
			     0,
			     GL_RGB,
			     GL_UNSIGNED_BYTE,
			     image_buffer);
		program.setUniformValue(textureUniformY, 0);
	}
	else
	if (current_format == ConnectionBase::RGB16) {
		glActiveTexture(GL_TEXTURE0);
		configureTexture(*textures[0]);
		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RGB565,
			     image_width,
			     image_height,
			     0,
			     GL_RGB,
			     GL_UNSIGNED_SHORT_5_6_5_REV,
			     image_buffer);
		program.setUniformValue(textureUniformY, 0);
	}
	else
	if (current_format == ConnectionBase::I420) {
		uint32_t plane_size[2];
		plane_size[0] = image_width * image_height;
		plane_size[1] = (image_width * image_height) / 2;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Activate texture Y */
		glActiveTexture(GL_TEXTURE0);
		configureTexture(*textures[0]);
		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RED,
			     image_width,
			     image_height,
			     0,
			     GL_RED,
			     GL_UNSIGNED_BYTE,
			     image_buffer);

		/* Activate texture U */
		glActiveTexture(GL_TEXTURE1);
		configureTexture(*textures[1]);
		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RED,
			     image_width / 2,
			     image_height / 2,
			     0,
			     GL_RED,
			     GL_UNSIGNED_BYTE,
			     image_buffer + plane_size[0]);

		/* Activate texture V */
		glActiveTexture(GL_TEXTURE2);
		configureTexture(*textures[2]);
		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RED,
			     image_width / 2,
			     image_height / 2,
			     0,
			     GL_RED,
			     GL_UNSIGNED_BYTE,
			     image_buffer + plane_size[0] + plane_size[1]);

		program.setUniformValue(textureUniformY, 0);
		program.setUniformValue(textureUniformU, 1);
		program.setUniformValue(textureUniformV, 2);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLRenderer::resizeGL(int w, int h) {
	Q_UNUSED(w);
	Q_UNUSED(h);

	qDebug("OpenGLRenderer::resizeGL");
}

QString OpenGLRenderer::rgbShader() {
	return QString(
		"varying vec2 textureOut;"
		"uniform sampler2D tex_y;"
		"void main(void)"
		"{"
		"vec3 rgb;"
		"rgb = texture2D(tex_y, textureOut).rgb;"
		"gl_FragColor = vec4(rgb, 1.0);"
		"}"
	);
}

QString OpenGLRenderer::yuv420Shader() {
	return QString(
		"varying vec2 textureOut;"
		"uniform sampler2D tex_y;"
		"uniform sampler2D tex_u;"
		"uniform sampler2D tex_v;"
		"void main(void)"
		"{"
			"vec3 yuv;"
			"vec3 rgb;"
			"mat3 yuv2rgb_bt601_mat = mat3("
				"vec3(1.164,  1.164, 1.164),"
				"vec3(0.000, -0.392, 2.017),"
				"vec3(1.596, -0.813, 0.000)"
			");"
			"yuv.x = texture2D(tex_y, textureOut).r - 0.063;"
			"yuv.y = texture2D(tex_u, textureOut).r - 0.500;"
			"yuv.z = texture2D(tex_v, textureOut).r - 0.500;"
			"rgb = yuv2rgb_bt601_mat * yuv;"
			"gl_FragColor = vec4(rgb, 1.0);"
		"}"
	);
}

QString OpenGLRenderer::packedShader(PackedPattern p) {
	QString code = QString(
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

	if (p == YUYV) {
		code += QString(
			"float y_left = mix(left.r, left.b, f_x * 2.0);"
			"float y_right = mix(left.b, right.r, f_x * 2.0 - 1.0);"
			"vec2 uv = mix(left.ga, right.ga, f_x);"
		);
	}
	else
	if (p == UYVY) {
		code += QString(
			"float y_left = mix(left.g, left.a, f_x * 2.0);"
			"float y_right = mix(left.a, right.g, f_x * 2.0 - 1.0);"
			"vec2 uv = mix(left.rb, right.rb, f_x);"
		);
	}

	code += QString(
			"float y = mix(y_left, y_right, step(0.5, f_x));"
			"vec3 rgb = yuv2rgb_bt601_mat * (vec3(y, uv) - yuv2rgb_bt601_offset);"
			"gl_FragColor = vec4(rgb, 1.0);"
		"}"
	);

	return code;
}

#include "moc_openglrenderer.cpp"
