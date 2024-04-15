/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2024 Ricardo Villalba <ricardo@smplayer.info>

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
#include <QRegularExpression>
#include <QFile>
#include <QDebug>

OpenGLRenderer::OpenGLRenderer(QObject * parent)
	: QObject(parent)
	, vertex_buffer(QOpenGLBuffer::VertexBuffer)
	, vertex_shader(0)
	, fragment_shader(0)
	, current_format(0)
	, current_colorspace(ConnectionBase::CSP_BT_709)
{
	//glsl_version = "130";
	glsl_version = "330";

	for (int n = 0; n < 3; n++) {
		textures[n] = 0;
	}

	GLfloat matrix[] = {
		1.660497, -0.587657, -0.072840,
		-0.124547, 1.132895, -0.008348,
		-0.018154, -0.100597, 1.118751
	};
	cms_matrix = QMatrix3x3(matrix);
	luma = 0.212637;
}

OpenGLRenderer::~OpenGLRenderer() {
}

void OpenGLRenderer::setFormat(ConnectionBase::Format format) {
	qDebug("OpenGLRenderer::setFormat: %s", ConnectionBase::formatToString(format).toLatin1().constData());

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

void OpenGLRenderer::setColorspace(ConnectionBase::Colorspace c, ConnectionBase::Primary primary) {
	bool changed = (current_colorspace != c || current_primary != primary);
	current_colorspace = c;
	current_primary = primary;
	if (changed) {
		if (program.isLinked()) {
			program.release();
			program.removeShader(fragment_shader);
			delete fragment_shader;
			fragment_shader = 0;
		}
	}
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

	if (!fragment_shader->compileSourceCode(glslCode(code))) {
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

	if (QOpenGLContext::currentContext()->isOpenGLES()) {
		glsl_version = "300 es";
	}

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
		"in vec4 vertexIn; \n"
		"in vec2 textureIn; \n"
		"out vec2 textureOut; \n"
		"void main(void) \n"
		"{ \n"
			"gl_Position = vertexIn; \n"
			"textureOut = textureIn; \n"
		"} \n");
	if (!vertex_shader->compileSourceCode(glslCode(code))) {
		qWarning() << "OpenGLRenderer::initializeGL:" << vertex_shader->log();
	}
	program.addShader(vertex_shader);

	if (vao.create()) vao.bind();
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

		//qDebug() << "OpenGLRenderer::paintGL: cms_matrix:" << cms_matrix;
		program.setUniformValue(program.uniformLocation("cms_matrix"), cms_matrix);
		program.setUniformValue(program.uniformLocation("dst_luma"), luma, luma, luma);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLRenderer::resizeGL(int w, int h) {
	Q_UNUSED(w);
	Q_UNUSED(h);

	qDebug("OpenGLRenderer::resizeGL");
}

QString OpenGLRenderer::rgbShader() {
	QString code = QString(
		"out highp vec4 fragColor; \n"
		"in highp vec2 textureOut; \n"
		"uniform sampler2D tex_y; \n"
		"void main(void) \n"
		"{ \n"
		"highp vec3 rgb; \n"
		"rgb = texture(tex_y, textureOut).rgb; \n"
		"fragColor = vec4(rgb, 1.0); \n"
		"} \n"
	);

	//qDebug("OpenGLRenderer::rgbShader: code: %s", code.toLatin1().constData());
	return code;
}

QString OpenGLRenderer::yuv420Shader() {
	QString code = QString(
		"out highp vec4 fragColor; \n"
		"in highp vec2 textureOut; \n"
		"uniform sampler2D tex_y; \n"
		"uniform sampler2D tex_u; \n"
		"uniform sampler2D tex_v; \n"
		"uniform highp vec3 dst_luma; \n"
		"uniform highp mat3 cms_matrix; \n"

		"void main(void) \n"
		"{ \n"
			"highp vec3 yuv; \n"
			"highp vec3 rgb; \n"
		);

	code += colorspaceMat();

	code += QString(
			"yuv.x = texture(tex_y, textureOut).r - 0.063; \n"
			"yuv.y = texture(tex_u, textureOut).r - 0.500; \n"
			"yuv.z = texture(tex_v, textureOut).r - 0.500; \n"
			"rgb = yuv2rgb_mat * yuv; \n");

	qDebug() << "OpenGLRenderer::yuv420Shader: current_primary:" << current_primary;
	if (current_primary == ConnectionBase::PRIM_BT_2020 && glsl_version >= "130") {
		code += HDRColorConversionCode();
	}

	code += QString(
			"fragColor = vec4(rgb, 1.0); \n"
		"}\n"
	);

	//qDebug("OpenGLRenderer::yuv420Shader: code: %s", code.toLatin1().constData());
	return code;
}

QString OpenGLRenderer::packedShader(PackedPattern p) {
	QString code = QString(
		"out highp vec4 fragColor; \n"
		"in highp vec2 textureOut; \n"
		"uniform sampler2D tex_y; \n"
		"uniform highp float tex_stepx; \n"
		"void main(void) \n"
		"{ \n"
	);

	code += colorspaceMat();

	code += QString(
			"highp vec3 yuv2rgb_offset = vec3(0.063, 0.500, 0.500); \n"
			"highp vec2 pos = textureOut; \n"
			"highp float f_x = fract(pos.x / tex_stepx); \n"
			"highp vec4 left = texture(tex_y, vec2(pos.x - f_x * tex_stepx, pos.y)); \n"
			"highp vec4 right = texture(tex_y, vec2(pos.x + (1.0 - f_x) * tex_stepx , pos.y)); \n"
	);

	if (p == YUYV) {
		code += QString(
			"highp float y_left = mix(left.r, left.b, f_x * 2.0); \n"
			"highp float y_right = mix(left.b, right.r, f_x * 2.0 - 1.0); \n"
			"highp vec2 uv = mix(left.ga, right.ga, f_x); \n"
		);
	}
	else
	if (p == UYVY) {
		code += QString(
			"highp float y_left = mix(left.g, left.a, f_x * 2.0); \n"
			"highp float y_right = mix(left.a, right.g, f_x * 2.0 - 1.0); \n"
			"highp vec2 uv = mix(left.rb, right.rb, f_x); \n"
		);
	}

	code += QString(
			"highp float y = mix(y_left, y_right, step(0.5, f_x)); \n"
			"highp vec3 rgb = yuv2rgb_mat * (vec3(y, uv) - yuv2rgb_offset); \n"
			"fragColor = vec4(rgb, 1.0); \n"
		"} \n"
	);

	//qDebug("OpenGLRenderer::packedShader: code: %s", code.toLatin1().constData());
	return code;
}

QString OpenGLRenderer::colorspaceMat() {
	qDebug("OpenGLRenderer::colorspaceMat: %d", current_colorspace);

	QString bt601 = QString(
			"highp mat3 yuv2rgb_mat = mat3( \n"
				"vec3(1.164,  1.164, 1.164), \n"
				"vec3(0.000, -0.392, 2.017), \n"
				"vec3(1.596, -0.813, 0.000) \n"
			"); \n");

	QString bt709 = QString(
			"highp mat3 yuv2rgb_mat = mat3( \n"
				"vec3(1.164,  1.164, 1.164), \n"
				"vec3(0.000, -0.213, 2.112), \n"
				"vec3(1.793, -0.533, 0.000) \n"
			"); \n");

	QString jpeg = QString(
			"highp mat3 yuv2rgb_mat = mat3( \n"
				"vec3(1.000,  1.000, 1.000), \n"
				"vec3(0.000, -0.343, 1.765), \n"
				"vec3(1.400, -0.711, 0.000) \n"
			"); \n");

	QString bt2020ncl = QString(
			"highp mat3 yuv2rgb_mat = mat3( \n"
				"vec3(1.164,  1.164, 1.164), \n"
				"vec3(0.000, -0.187, 2.141), \n"
				"vec3(1.679, -0.650, 0.000) \n"
			"); \n");

	QString bt2020cl = QString(
			"highp mat3 yuv2rgb_mat = mat3( \n"
				"vec3(0.000, 1.164, 0.000), \n"
				"vec3(0.000, 0.000, 1.138), \n"
				"vec3(1.138, 0.000, 0.000) \n"
			"); \n");

	switch (current_colorspace) {
		case ConnectionBase::CSP_BT_601: return bt601;
		case ConnectionBase::CSP_BT_2020_NC: return bt2020ncl;
	}
	return bt709;
}

QString OpenGLRenderer::HDRColorConversionCode() {
	QString code;

	QString filename = ":/glsl/hdrcolorconversion-330.glsl";
	QFile f(filename);
	if (f.open(QIODevice::ReadOnly)) {
		code = f.readAll();
		f.close();
	} else {
		qWarning() << "OpenGLRenderer::HDRColorConversionCode: failed to load" << filename;
	}

	return code;
}

QString OpenGLRenderer::glslCode(QString code) {
	code = "#version "+ glsl_version +" \n" + code;

	//qDebug("OpenGLRenderer::glslCode: code IN: %s", code.toLatin1().constData());

	if (glsl_version < "200") {
		code = code.replace("highp ", "");
		code = code.replace("out vec4 fragColor;", "");
		code = code.replace("out vec2 textureOut", "varying vec2 textureOut");
		code = code.replace("in vec2 textureOut", "varying vec2 textureOut");
		code = code.replace("fragColor", "gl_FragColor");
		code = code.replace("out ", "varying ");
		QRegularExpression rx("^in ");
		rx.setPatternOptions(QRegularExpression::MultilineOption);
		code = code.replace(rx, "attribute ");
		code = code.replace("texture(", "texture2D(");
		//qDebug("OpenGLRenderer::glslCode: code OUT: %s", code.toLatin1().constData());
	}
	return code;
}

#include "moc_openglrenderer.cpp"
