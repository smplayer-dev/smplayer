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

#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>

class Renderer : protected QOpenGLFunctions
{
public:
	virtual void initializeGL(int window_width, int window_height) = 0;
	virtual void paintGL(int window_width, int window_height, int image_width, int image_height, uint32_t image_format, unsigned char * buffer) = 0;
	virtual void resizeGL(int w, int h) = 0;
};

#endif
