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

#ifndef AUDIOEQUALIZERLIST_H
#define AUDIOEQUALIZERLIST_H

#include <QList>
#include <QStringList>
#include <QVariant>

#define SIMPLE_EQUALIZER

typedef QList<QVariant> AudioEqualizerList;

class AudioEqualizerHelper {

public:
	enum AudioEqualizerType { Equalizer = 0, Anequalizer = 1, Firequalizer = 2, Superequalizer = 3, FEqualizer = 4 };

	//! Returns a string to be passed to mplayer/mpv with the audio equalizer values.
	static QString equalizerListToString(AudioEqualizerList values, AudioEqualizerType type = Equalizer);

#ifndef SIMPLE_EQUALIZER
	static QStringList equalizerListToStringList(AudioEqualizerList values, AudioEqualizerType type);
	static QStringList equalizerListForCommand(AudioEqualizerList values, AudioEqualizerList old_values, AudioEqualizerType type);
#endif
};

#endif
