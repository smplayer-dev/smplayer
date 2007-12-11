/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2007 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "mplayerversion.h"
#include <QRegExp>

int MplayerVersion::mplayerVersion(QString string) {
	//static QRegExp rx_mplayer_revision("^MPlayer (\\S+)-SVN-r(\\d+)-(.*)");
	static QRegExp rx_mplayer_revision("^MPlayer (.*)-r(\\d+)(.*)");
	static QRegExp rx_mplayer_version("^MPlayer ([a-z,0-9,.]+)-(.*)");

	int mplayer_svn = 0;

#ifdef Q_OS_WIN
	// Hack to recognize mplayer 1.0rc2 from CCCP:
	if (string.startsWith("MPlayer CCCP ")) { 
		string.remove("CCCP "); 
		qDebug("MplayerVersion::mplayerVersion: removing CCCP: '%s'", string.toUtf8().data()); 
	}
#endif

	if (rx_mplayer_revision.indexIn(string) > -1) {
		mplayer_svn = rx_mplayer_revision.cap(2).toInt();
		qDebug("MplayerVersion::mplayerVersion: MPlayer SVN revision found: %d", mplayer_svn);
	} 
	else
	if (rx_mplayer_version.indexIn(string) > -1) {
		QString version = rx_mplayer_version.cap(1);
		qDebug("MplayerVersion::mplayerVersion: MPlayer version found: %s", version.toUtf8().data());
		mplayer_svn = 0;
		if (version == "1.0rc2") mplayer_svn = 24722;
		else
		if (version == "1.0rc1") mplayer_svn = 20372;
		else qWarning("MplayerVersion::mplayerVersion: unknown MPlayer version");
	}

	return mplayer_svn;
}

bool MplayerVersion::isMplayerAtLeast(int mplayer_svn, int svn_revision) {
	qDebug("MplayerVersion::isMplayerAtLeast: comparing %d with %d", svn_revision, mplayer_svn);

	if (mplayer_svn == -1) {
		qWarning("MplayerVersion::isMplayerAtLeast: no version found!");
	}
	else
	if (mplayer_svn == 0) {
		qWarning("MplayerVersion::isMplayerAtLeast: version couldn't be parsed!");
	}

	if (mplayer_svn <= 0) {
		qWarning("MplayerVersion::isMplayerAtLeast: assuming that the mplayer version is greater than %d", svn_revision);
		return true;
	}

	return (mplayer_svn >= svn_revision);
}
