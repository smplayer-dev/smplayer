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


#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QStringList>
#include "audioequalizerlist.h"
#include "preferences.h"

#ifdef Q_OS_WIN
#include "config.h"
#endif

class Helper {

public:

	// Format a time (hh:mm:ss)
	static QString formatTime(int secs);

	// Format a time (hh:mm:ss.cc)
	static QString formatTime2(double secs);

	static QString timeForJumps(int secs);

	// Give a name for config (group name) based on dvd id
	/* static QString dvdForPref(const QString & dvd_id, int title); */

#ifdef Q_OS_WIN
	static QString shortPathName(QString long_path);

	//! Enable or disables the screensaver
	/* static void setScreensaverEnabled(bool b); */
#endif

	static void msleep(int ms);

	//! Change filenames like "C:/Program Files/" to "C:\Program Files\"
	static QString changeSlashes(QString filename);

	static bool directoryContainsDVD(QString directory);

	//! Returns an int with the version number of Qt at run-time.
    //! If version is 4.3.2 it returns 40302.
	static int qtVersion();

	static QStringList filesForPlaylist(const QString & initial_file, Preferences::AutoAddToPlaylistFilter filter);

#ifdef Q_OS_WIN
	static QStringList resolveSymlinks(const QStringList & files);
#endif

#ifndef Q_OS_WIN
	//! Tries to find the executable in the path.
	//! Returns the path if found or QString::null if not.
	static QString findExecutable(const QString & name);
#endif

private:
	static QStringList searchForConsecutiveFiles(const QString & initial_file);
	static QStringList filesInDirectory(const QString & initial_file, const QStringList & filter);
};

#endif
