/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef VIDEOLAYER_H
#define VIDEOLAYER_H

#include "config.h"

#ifdef USE_GL_WINDOW
  #include <QOpenGLWidget>
  #define VIDEOLAYER_PARENT QOpenGLWidget
#else
 #include <QWidget>
 #define VIDEOLAYER_PARENT QWidget
#endif

class VideoLayer : public VIDEOLAYER_PARENT
{
	Q_OBJECT

public:
	VideoLayer(QWidget* parent = 0, Qt::WindowFlags f = QFlag(0));
	~VideoLayer();

#if REPAINT_BACKGROUND_OPTION
	//! If b is true, the background of the widget will be repainted as usual.
	/*! Otherwise the background will not repainted when a video is playing. */
	void setRepaintBackground(bool b);

	//! Return true if repainting the background is allowed.
	bool repaintBackground() { return repaint_background; };
#endif

public slots:
	//! Should be called when a file has started.
	/*! It's needed to know if the background has to be cleared or not. */
	virtual void playingStarted();
	//! Should be called when a file has stopped.
	virtual void playingStopped();

	virtual void gotVO(QString);

protected:
#if REPAINT_BACKGROUND_OPTION
	bool repaint_background;
#endif
	bool playing;
};

#endif
