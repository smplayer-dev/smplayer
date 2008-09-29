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

#ifndef _AUDIOTRACKS_H_
#define _AUDIOTRACKS_H_

#include "audiodata.h"
#include <QMap>
#include <QSettings>

class AudioTracks {

public:

	AudioTracks();
	~AudioTracks();

	void clear();
	void list();

	void addLang(int ID, QString lang);
	void addName(int ID, QString name);
	void addID(int ID);

	int numItems();
	bool existsItemAt(int n);

	AudioData itemAt(int n);
	AudioData item(int ID);
	int find(int ID);

	int findLang(QString expr);

/*
#ifndef NO_USE_INI_FILES
	void save(QSettings & set);
	void load(QSettings & set);
#endif
*/

protected:
	typedef QMap <int, AudioData> AudioMap;
	AudioMap tm;
};

#endif
