/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef GUICONFIG_H
#define GUICONFIG_H

#include <QtGlobal>

// AUTODISABLE_ACTIONS
// if set to 1, some actions will be disabled if they are not needed

#define AUTODISABLE_ACTIONS 1


// MINI_ARROW_BUTTONS
// if set to 1, the GUI will use a popup menu for arrow buttons

#define MINI_ARROW_BUTTONS 1


// ALLOW_CHANGE_STYLESHEET
// if 1, the app stylesheet can be changed

#define ALLOW_CHANGE_STYLESHEET 1


// Allow to use multiple shortcuts for actions

#define USE_MULTIPLE_SHORTCUTS 1


// USE_SHORTCUTGETTER
// if 1, a new dialog will be used to ask the user for a
// keyshortcut.

#define USE_SHORTCUTGETTER 1


// USE_INFOPROVIDER
// if 1, the playlist will read info about the files when they are added
// to the list.
// It's slow but allows the user to see the length and even the name of
// a mp3 song.

#define USE_INFOPROVIDER 1


// USE_CONFIGURABLE_TOOLBARS
// if 1, the toolbars (and controlbars) are saved to the config file
// so the user can modify them.

#define USE_CONFIGURABLE_TOOLBARS 1


// USE_DOCK_TOPLEVEL_EVENT
// if 1, the topLevelChanged from QDockWidget will be use to know
// if the playlist has been docked or undocked

#define USE_DOCK_TOPLEVEL_EVENT 0


#endif

