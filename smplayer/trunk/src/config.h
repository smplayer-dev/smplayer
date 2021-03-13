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

#ifndef CONFIG_H
#define CONFIG_H

#include <Qt>

#define NOTIFY_SUB_CHANGES 1
#define NOTIFY_AUDIO_CHANGES 1
#define NOTIFY_VIDEO_CHANGES 1
#define NOTIFY_CHAPTER_CHANGES 1

#define SIMPLE_TRACK_SELECTION 1
#define SELECT_TRACKS_ON_STARTUP 1



// STYLE_SWITCHING
// if 1, the preferences dialog will have an option to switch
// the Qt style

#define STYLE_SWITCHING 1


// EXTERNAL_SLEEP
// if 1, it will be used the function usleep() from unistd.h
// instead of QThread::msleep()
// It can be useful if your Qt doesn't have QThread support.
// Note: not much test it
// Note 2: not used anymore

#define EXTERNAL_SLEEP 0


// ENABLE_DELAYED_DRAGGING
// if 1, sends the dragging position of the time slider
// some ms later

#define ENABLE_DELAYED_DRAGGING 1


// SEEKBAR_RESOLUTION
// if SEEKBAR_RESOLUTION is defined, it specified the
// maximum value of the time slider

#define SEEKBAR_RESOLUTION 1000


// SMART_DVD_CHAPTERS
// if set to 1, the slave command "chapter" will use if not using a cache,
// otherwise mplayer will be restarted and -chapter will be used.

#define SMART_DVD_CHAPTERS 1


// ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
// if 1, the video window may be hidden when playing audio files
// depending on the hide_video_window_on_audio_files option in
// the config file

#define ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES 1


// DELAYED_AUDIO_SETUP_ON_STARTUP
// if 1, the audio track will be initialized later once the file
// has begun to play

#define DELAYED_AUDIO_SETUP_ON_STARTUP 0


// CHECK_VIDEO_CODEC_FOR_NO_VIDEO
// if 1, the video codec will be checked to decide if the file
// has video or not. If it's empty it has no video.
// If 0, it will check for the line "Video: no video"

#define CHECK_VIDEO_CODEC_FOR_NO_VIDEO 1


// Just for testing, possibility to disable the use of the colorkey

#define USE_COLORKEY 0


// USE_MINIMUMSIZE
// if 1, the main window will not be smaller than the control widget 
// size hint or pref->gui_minimum_width.

#define USE_MINIMUMSIZE 1


// DVDNAV_SUPPORT
// if 1, smplayer will be compiled with support for mplayer's dvdnav
#ifdef MPLAYER_SUPPORT
#define DVDNAV_SUPPORT 0
#endif


// PROGRAM_SWITCH
// support for program switch in ts files

#define PROGRAM_SWITCH 0


// ALLOW_DEMUXER_CODE_CHANGE
// support changing of demuxer and video and audio codecs

#define ALLOW_DEMUXER_CODEC_CHANGE 1


// Adds or not the "Repaint the background of the video window" option.
#ifndef Q_OS_WIN
#define REPAINT_BACKGROUND_OPTION 1
#endif


// Enables/disables the use of -adapter
#ifdef Q_OS_WIN
#define USE_ADAPTER 0
#define OVERLAY_VO "directx"
//#define OVERLAY_VO "xv"
#endif


// If 1, smplayer will check if mplayer is old
// and in that case it will report to the user
#if !defined(Q_OS_WIN) && !defined(Q_OS_OS2)
#define REPORT_OLD_MPLAYER 1
#endif


// If 1, the background logo will be animated
#if QT_VERSION >= 0x040600
/* #define LOGO_ANIMATION 1 */
#endif

#endif
