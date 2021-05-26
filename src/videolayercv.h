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

#ifndef VIDEOLAYERCV_H
#define VIDEOLAYERCV_H

#include "videolayer.h"

class VideoLayerCV : public VideoLayer
{
	Q_OBJECT

public:
	VideoLayerCV(QWidget* parent = 0, Qt::WindowFlags f = QFlag(0));
	~VideoLayerCV();

public slots:
	virtual void playingStarted();
	virtual void playingStopped();
	virtual void gotVO(QString);

	void init_slot(int width, int height, int bytes, int aspect);
	void render_slot();
	void stop_slot();

protected:
	void start_connection();
	void stop_connection();
	QString buffer_name;
	bool is_corevideo;

	int shm_fd;
	int image_width;
	int image_height;
	int image_bytes;
	unsigned char * image_data;
	unsigned char * image_buffer;

protected:
	virtual void paintEvent(QPaintEvent *event);
	QPixmap frame;
};

#endif
