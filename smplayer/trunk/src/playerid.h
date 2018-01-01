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

#ifndef PLAYERID_H
#define PLAYERID_H

#include <QString>

#if defined(MPV_SUPPORT) && defined(MPLAYER_SUPPORT)
#define PLAYER_NAME "MPlayer/mpv"
#else
#ifdef MPV_SUPPORT
#define PLAYER_NAME "mpv"
#endif
#ifdef MPLAYER_SUPPORT
#define PLAYER_NAME "MPlayer"
#endif
#endif

class PlayerID
{
public:
	enum Player { MPLAYER = 0, MPV = 1 };
	static Player player(const QString & player_bin);
};

#endif
