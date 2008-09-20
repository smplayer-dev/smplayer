/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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


#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/* Global settings */

#include <QString>
#include "config.h"
#include "audioequalizerlist.h"

class Preferences {

public:
	enum OSD { None = 0, Seek = 1, SeekTimer = 2, SeekTimerTotal = 3 };
	enum Resize { Never = 0, Always = 1, Afterload = 2 };
	enum Priority { Realtime = 0, High = 1, AboveNormal = 2, Normal = 3,
                    BelowNormal = 4, Idle = 5 };
	enum WheelFunction { Seeking = 0, Volume = 1, Zoom = 2, DoNothing = 3,
                         ChangeSpeed = 4 };
	enum OptionState { Detect = -1, Disabled = 0, Enabled = 1 };
	enum H264LoopFilter { LoopDisabled = 0, LoopEnabled = 1, LoopDisabledOnHD = 2 };

	Preferences();
	virtual ~Preferences();

	virtual void reset();

#ifndef NO_USE_INI_FILES
	void save();
	void load();
#endif

	QString mplayer_bin;
	QString vo; // video output
	QString ao; // audio output

#if USE_ADAPTER
	int adapter; //Screen for overlay. If -1 it won't be used.
#endif

#if USE_COLORKEY
	unsigned int color_key;
#endif


	int osd;

	OptionState fast_audio_change; // If activated, not restart mplayer
#if !SMART_DVD_CHAPTERS
	bool fast_chapter_change;
#endif

	QString dvd_device;
	QString cdrom_device;

	int cache_for_files;
	int cache_for_streams;
	int cache_for_dvds;
	int cache_for_vcds;
	int cache_for_audiocds;

	int threads; //!< number of threads to use for decoding (-lavdopts threads <1-8>)

	bool use_mplayer_window;

	QString monitor_aspect;
	double monitor_aspect_double();

	//! Directory of the latest file loaded
	QString latest_dir;
	QString last_url;
	QString last_dvd_directory;

	QString mplayer_verbose;


	bool disable_screensaver;
	bool use_direct_rendering;
	bool use_double_buffer;

	QString screenshot_directory;
	bool subtitles_on_screenshots;

	bool use_soft_video_eq;
	bool use_soft_vol;

	int softvol_max;
	OptionState use_scaletempo;

	QString audio_lang; 		// Preferred audio language
	QString subtitle_lang;		// Preferred subtitle language

	bool use_idx; //!< Use -idx

	bool dont_change_volume; // Don't change volume on startup

	bool use_hwac3; // -afm hwac3

	bool use_audio_equalizer;

	bool use_slices;


	int vcd_initial_title;


	// Let the user pass options to mplayer
	QString mplayer_additional_options;
	QString mplayer_additional_video_filters;
	QString mplayer_additional_audio_filters;

	// Performance
	int priority;
	bool frame_drop;
	bool hard_frame_drop;
	bool autosync;
	int autosync_factor;

	H264LoopFilter h264_skip_loop_filter;
	//! An HD is a video which height is equal or greater than this.
	int HD_height; 

	// SMPlayer will remember all media settings for all videos.
	// This options allow to disable it:
	bool dont_remember_media_settings; 	// Will not remember anything
	bool dont_remember_time_pos;		// Will not remember time pos



	
	//! Postprocessing quality
	int autoq;

	//! Loop. If true repeat the file
	bool loop;



	// Logs
	bool log_mplayer;
	bool log_smplayer;
	QString log_filter;


#if REPAINT_BACKGROUND_OPTION
	//! If true, mplayerlayer erases its background
	bool always_clear_video_background; 
#endif

	//! Make configurable some of the mplayerprocess regular expressions
	QString rx_endoffile;
	QString rx_novideo;


	//! If false, -brightness, -contrast and so on, won't be passed to
	//! mplayer. It seems that some graphic cards don't support those options.
	bool change_video_equalizer_on_startup;


#ifdef Q_OS_WIN
	bool enable_audiocd_on_windows;
#endif




	//! If true it will autoload edl files with the same name of the file
    //! to play
	bool use_edl_files;


	//! Preferred connection method: ipv4 or ipv6
	bool prefer_ipv4;


    //mplayer log autosaving
    bool autosave_mplayer_log;
    QString mplayer_log_saveto;
    //mplayer log autosaving end


	OptionState use_volume_option2; //!< Use -volume in command line

	//! Windows only. If true, smplayer will pass short filenames to mplayer.
	//! To workaround a bug in mplayer.
	bool use_short_pathnames; 


    /* **************
       Initial values
       ************** */

	double initial_sub_scale;
#if SCALE_ASS_SUBS
	double initial_sub_scale_ass;
#endif
	int initial_volume;
	int initial_contrast;
	int initial_brightness;
	int initial_hue;
	int initial_saturation;
	int initial_gamma;

	AudioEqualizerList initial_audio_equalizer;

	//! Default value for panscan (1.0 = no zoom)
	double initial_panscan_factor;

	//! Default value for position of subtitles on screen
	//! 100 = 100% at the bottom
	int initial_sub_pos;

	bool initial_postprocessing; //!< global postprocessing filter
	bool initial_volnorm;

	int initial_deinterlace;

	int initial_audio_channels;

	int initial_audio_track;
	int initial_subtitle_track;


    /* ************
       MPlayer info
       ************ */

	int mplayer_detected_version; 	//!< Latest version of mplayer parsed
	bool mplayer_has_volume_option;

	//! Version of mplayer supplied by the user which will be used if
	//! the version can't be parsed from mplayer output
	int mplayer_user_supplied_version;


    /* *********
       Instances
       ********* */

	bool use_single_instance;
	int connection_port; // Manual port
	bool use_autoport;
	int autoport; // Port automatically chosen by Qt


    /* ****************
       Floating control
       **************** */

	int floating_control_margin;
	int floating_control_width;
	bool floating_control_animated;


	/* ************
	   Subtitle stuff
	   ************** */

	QString font_file;
	QString font_name;
	bool use_fontconfig;
	QString subcp; // -subcp
	int font_autoscale; // -subfont-autoscale
	int subfuzziness;
	bool autoload_sub;

	bool use_ass_subtitles;
	unsigned int ass_color;
	unsigned int ass_border_color;
	QString ass_styles;

	bool use_closed_caption_subs;
	bool use_forced_subs_only;

	//! Use the new sub_vob, sub_demux and sub_file commands
	//! instead of sub_select
	OptionState use_new_sub_commands; 
	OptionState change_sub_scale_should_restart;

	bool show_font_scale_options_in_preferences;


	/* *********
	   GUI stuff
	   ********* */

	bool fullscreen;
	bool start_in_fullscreen;
	bool compact_mode;
	bool stay_on_top;
	int size_factor;

	int resize_method; 	//!< Mainwindow resize method

#if STYLE_SWITCHING
	QString style; 	//!< SMPlayer look
#endif
	bool show_frame_counter;
	bool show_motion_vectors;

	// Function of mouse buttons:
	QString mouse_left_click_function;
	QString mouse_right_click_function;
	QString mouse_double_click_function;
	QString mouse_middle_click_function;
	QString mouse_xbutton1_click_function;
	QString mouse_xbutton2_click_function;
	int wheel_function;

	int recents_max_items; 	//!< Max items in recent's list

	// Configurable seeking
	int seeking1; // By default 10s
	int seeking2; // By default 1m
	int seeking3; // By default 10m
	int seeking4; // For mouse wheel, by default 30s

	bool update_while_seeking;
#if ENABLE_DELAYED_DRAGGING	
	int time_slider_drag_delay;
#endif

	QString language;
	QString iconset;

	//! Number of times to show the balloon remembering that the program
	//! is still running in the system tray.
	int balloon_count;

	//! If true, the position of the main window will be saved before
	//! entering in fullscreen and will restore when going back to
	//! window mode.
	bool restore_pos_after_fullscreen;

	bool save_window_size_on_exit;

	//! Close the main window when a file or playlist finish
	bool close_on_finish;

	QString default_font;

	//!< Pause the current file when the main window is not visible
	bool pause_when_hidden; 

	//!< Allow frre movement of the video window
	bool allow_video_movement;

	QString gui; //!< The name of the GUI to use

#if USE_MINIMUMSIZE
	int gui_minimum_width;
#endif

#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
	bool hide_video_window_on_audio_files;
#endif

	bool report_mplayer_crashes;

	bool auto_add_to_playlist; //!< Add files to open to playlist
};

#endif
