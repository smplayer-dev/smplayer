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

#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include "videolayerrender.h"
#include <QString>

class ConnectionBase : public QObject
{
	Q_OBJECT

public:
	enum Format { RGB24 = 1380401688, RGB16 = 1380401680, I420 = 808596553,
                  YUY2 = 844715353, UYVY = 1498831189,
                  MP_YUV420P = 1002, MP_UYVY422 = 1005, MP_RGB24 = 1013 };

	ConnectionBase(VideoLayerRender * parent) : QObject(parent), video_window(parent) {}
	~ConnectionBase() {}

	virtual bool isVOSupported(const QString & vo) = 0;

	static QString formatToString(uint32_t f);

public slots:
	virtual void start() = 0;
	virtual void stop() = 0;

protected:
	VideoLayerRender * video_window;
};

#endif
