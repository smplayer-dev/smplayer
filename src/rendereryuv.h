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

#ifndef RENDERERYUV_H
#define RENDERERYUV_H

#include "renderer.h"
#include <QOpenGLBuffer>

class QOpenGLShaderProgram;
class QOpenGLTexture;

class RendererYUV : public Renderer
{
public:
	RendererYUV(QObject * parent);
	~RendererYUV();

	virtual void initializeGL(int window_width, int window_height);
	virtual void paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * buffer);
	virtual void resizeGL(int w, int h);

protected:
	QOpenGLShaderProgram * program;
	QOpenGLBuffer vbo;
	GLuint textureUniformY, textureUniformU, textureUniformV;
	QOpenGLTexture * textureY = nullptr, * textureU = nullptr, * textureV = nullptr;
	GLuint idY,idU,idV;
};

#endif

