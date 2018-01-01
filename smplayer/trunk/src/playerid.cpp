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

#include "playerid.h"
#include <QFileInfo>
#include <QDebug>

PlayerID::Player PlayerID::player(const QString & player_bin) {
#if defined(MPV_SUPPORT) && defined(MPLAYER_SUPPORT)
	Player p;
	QString bin_name;

	QFileInfo fi(player_bin);
	if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
		bin_name = fi.fileName();
	} else {
		bin_name = player_bin;
	}

	qDebug() << "PlayerID::Player: player_bin:" << player_bin << "filename:" << bin_name;

	if (bin_name.toLower().startsWith("mplayer")) {
		p = MPLAYER;
	} else {
		p = MPV;
	}

	return p;
#else
	#ifdef MPV_SUPPORT
	return MPV;
	#endif

	#ifdef MPLAYER_SUPPORT
	return MPLAYER;
	#endif
#endif
}
