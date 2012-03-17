/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _MEDIASETTINGS_H_
#define _MEDIASETTINGS_H_


/* Settings the user has set for this file, and that we need to */
/* restore the video after a restart */

#include <QString>
#include <QSize>
#include "config.h"
#include "audioequalizerlist.h"

class QSettings;

class MediaSettings {

public:
	enum Denoise { NoDenoise = 0, DenoiseNormal = 1, DenoiseSoft = 2 };
	enum Aspect { AspectAuto = 1, Aspect43 = 2, Aspect54 = 3, Aspect149 = 4,
                  Aspect169 = 5, Aspect1610 = 6, Aspect235 = 7, Aspect11 = 8, 
                  Aspect32 = 9, Aspect1410 = 10, 
                  AspectNone = 0 };
	enum Deinterlace { NoDeinterlace = 0, L5 = 1, Yadif = 2, LB = 3, 
                       Yadif_1 = 4, Kerndeint = 5 };
	enum AudioChannels { ChDefault = 0, ChStereo = 2, ChSurround = 4, 
                         ChFull51 = 6 };
	enum StereoMode { Stereo = 0, Left = 1, Right = 2 };

	enum Rotate { NoRotate = -1, Clockwise_flip = 0, Clockwise = 1, 
                  Counterclockwise = 2, Counterclockwise_flip = 3 };

	enum IDs { NoneSelected = -1000, SubNone = 90000 };

	MediaSettings();
	virtual ~MediaSettings();

	virtual void reset();

	double current_sec;
	int current_sub_id;

#if PROGRAM_SWITCH
	int current_program_id;
#endif

	int current_video_id;
	int current_audio_id;

	int current_title_id;
	int current_chapter_id;
	int current_angle_id;

	int aspect_ratio_id;

	//bool fullscreen;

	int volume;
	bool mute;

	int brightness, contrast, gamma, hue, saturation;

	AudioEqualizerList audio_equalizer;

	QString external_subtitles;
	QString external_audio; // external audio file

	int sub_delay;
	int audio_delay;

	// Subtitles position (0-100)
	int sub_pos;
	double sub_scale;
	double sub_scale_ass;

	int closed_caption_channel; // 0 = disabled

	double speed; // Speed of playback: 1.0 = normal speed

	int current_deinterlacer;

	bool add_letterbox;

	// Filters in menu
	bool phase_filter;
	bool deblock_filter;
	bool dering_filter;
	bool gradfun_filter;
	bool noise_filter;
	bool postprocessing_filter;
	bool upscaling_filter; //!< Software scaling

	int current_denoiser;
	int current_unsharp;

	bool karaoke_filter;
	bool extrastereo_filter;
	bool volnorm_filter;

	int audio_use_channels;
	int stereo_mode;

	double zoom_factor; // mplayerwindow zoom

#if USE_MPLAYER_PANSCAN
	double panscan_factor;
#endif

	int rotate;
	bool flip; //!< Flip image
	bool mirror; //!< Mirrors the image on the Y axis.

	bool loop; //!< Loop. If true repeat the file
	int A_marker;
	int B_marker;

	// This a property of the video and it should be
    // in mediadata, but we have to save it to preserve 
	// this data among restarts.
	double starting_time; // Some videos don't start at 0

	//! The codec of the video is ffh264 and it's high definition
	bool is264andHD;

	// Advanced settings
	QString forced_demuxer;
	QString forced_video_codec;
	QString forced_audio_codec;

	// A copy of the original values, so we can restore them.
	QString original_demuxer;
	QString original_video_codec;
	QString original_audio_codec;

	// Options to mplayer (for this file only)
	QString mplayer_additional_options;
	QString mplayer_additional_video_filters;
	QString mplayer_additional_audio_filters;

	// Some things that were before in mediadata
	// They can vary, because of filters, so better here

	//Resolution used by mplayer
    //Can be bigger that video resolution
    //because of the aspect ratio or expand filter
    int win_width;
    int win_height;
    double win_aspect();

	//! Returns the aspect as a double. Returns 0 if aspect == AspectNone.
	double aspectToNum(Aspect aspect);
	static QString aspectToString(Aspect aspect);

	void list();

#ifndef NO_USE_INI_FILES
	void save(QSettings * set);
	void load(QSettings * set);
#endif
};

#endif
