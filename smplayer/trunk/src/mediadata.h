/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef MEDIADATA_H
#define MEDIADATA_H

/* Here we store some volatile info about the file we need to remember */

//#define MD_USE_TRACKS

#include "tracks.h"
#include "subtracks.h"
#include "titletracks.h"
#include "chapters.h"
#include "config.h"

#include <QString>
#include <QSettings>


// Types of media

#define TYPE_UNKNOWN -1
#define TYPE_FILE 0
#define TYPE_DVD 1
#define TYPE_STREAM 2
#define TYPE_VCD 3
#define TYPE_AUDIO_CD 4
#define TYPE_TV 5

#ifdef BLURAY_SUPPORT
#define TYPE_BLURAY 6
#endif

class MediaData {

public:
	MediaData();
	virtual ~MediaData();

	virtual void reset();

	QString filename;
	double duration;

	QStringList extra_params; // For streams

	//Resolution of the video
	int video_width;
	int video_height;
	double video_aspect;

	int type; // file, dvd...
	QString dvd_id;

	bool novideo; // Only audio

	bool initialized;

	void list();

#if PROGRAM_SWITCH
	Tracks programs;
#endif

#ifdef MD_USE_TRACKS
	Tracks tvideos;
	Tracks taudios;
	SubTracks tsubs;
#endif

	TitleTracks titles; // for DVDs
	Chapters chapters;

	int n_chapters;

	// Clip info
	QString clip_name;
	QString clip_artist;
	QString clip_author;
	QString clip_album;
	QString clip_genre;
	QString clip_date;
	QString clip_track;
	QString clip_copyright;
	QString clip_comment;
	QString clip_software;

	QString stream_title;
	QString stream_url;
	QString stream_path; // From mpv


	// Other data not really useful for us,
	// just to show info to the user.
	QString demuxer;
	QString video_format;
	QString audio_format;
	int video_bitrate;
	QString video_fps;
	int audio_bitrate;
	int audio_rate;
	int audio_nch; // channels?
	QString video_codec;
	QString audio_codec;

	/*QString info();*/
	QString displayName(bool show_tag = true);
};

#endif
