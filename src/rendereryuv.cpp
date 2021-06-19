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

#include "rendereryuv.h"
#include "videolayerrender.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>

#define VERTEXIN 0
#define TEXTUREIN 1

RendererYUV::RendererYUV(QObject * parent) : Renderer(parent)
{
}

RendererYUV::~RendererYUV() {
}

void RendererYUV::initializeGL(int window_width, int window_height) {
	initializeOpenGLFunctions();

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

	//QString rgb_conv = rgb_bt_601;
	//QString rgb_conv = rgb_bt_709;
	QString rgb_conv = rgb_jpeg;

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
}

void RendererYUV::paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * image_buffer) {
#if defined(USE_RGB) || defined(USE_YUY2)
	program->bind();
#endif

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, idY);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE1, idU);

	uint32_t plane_size[2];
	plane_size[0] = image_width * image_height;
	plane_size[1] = (image_width * image_height) / 2;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width >> 1, image_height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer + plane_size[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, idV);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width >> 1, image_height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer + plane_size[0] + plane_size[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glUniform1i(textureUniformY, 0);
	glUniform1i(textureUniformU, 1);
	glUniform1i(textureUniformV, 2);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RendererYUV::resizeGL(int w, int h) {
}
