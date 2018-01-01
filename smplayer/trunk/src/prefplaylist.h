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

#ifndef PREFPLAYLIST_H
#define PREFPLAYLIST_H

#include "ui_prefplaylist.h"
#include "prefwidget.h"
#include "playlist.h"

class Preferences;

class PrefPlaylist : public PrefWidget, public Ui::PrefPlaylist
{
	Q_OBJECT

public:
	PrefPlaylist( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PrefPlaylist();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

    // Pass data to the dialog
	void setData(Preferences * pref);

    // Apply changes
	void getData(Preferences * pref);


	void setDirectoryRecursion(bool b);
	bool directoryRecursion();

	void setAutoGetInfo(bool b);
	bool autoGetInfo();

	void setSavePlaylistOnExit(bool b);
	bool savePlaylistOnExit();

	void setPlayFilesFromStart(bool b);
	bool playFilesFromStart();

	void setPlayOnLoad(bool b);
	bool playOnLoad();

	void setPlayNextAutomatically(bool b);
	bool playNextAutomatically();

	void setIgnorePlayerErrors(bool b);
	bool ignorePlayerErrors();

	void setAutoSort(bool b);
	bool autoSort();

	void setFilterCaseSensitive(bool b);
	bool filterCaseSensitive();

#ifdef PLAYLIST_DELETE_FROM_DISK
	void allowDeleteFromDisk(bool b);
	bool isDeleteFromDiskAllowed();
#endif

protected:
	virtual void createHelp();

	void setAutoAddFilesToPlaylist(bool b);
	bool autoAddFilesToPlaylist();

	void setMediaToAdd(int);
	int mediaToAdd();

protected:
	virtual void retranslateStrings();
};

#endif
