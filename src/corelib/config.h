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

#ifndef _CONFIG_H_
#define _CONFIG_H_


// Activate the new code for aspect ratio

#define NEW_ASPECT_CODE 1


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


// SCALE_ASS_SUBS
// MPlayer r25843 adds the possibility to change the
// size of the subtitles, when using -ass, with the
// sub_scale slave command. Unfortunately this require
// a different code, which also makes the size of the
// subtitles to be very different when using -ass or not.
// Setting SCALE_ASS_SUBS to 1 activates this code.

#define SCALE_ASS_SUBS 1


// SMART_DVD_CHAPTERS
// if set to 1, the slave command "chapter" will use if not using a cache,
// otherwise mplayer will be restarted and -chapter will be used.

#define SMART_DVD_CHAPTERS 1


// Just for testing, possibility to disable the use of the colorkey

#define USE_COLORKEY 1


#endif
