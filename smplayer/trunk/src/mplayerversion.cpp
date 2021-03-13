/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "global.h"
#include "preferences.h"

#include <QRegExp>
#include <QDebug>

using namespace Global;

QString MplayerVersion::mpv_version;
bool MplayerVersion::is_mpv = false;

#ifdef MPLAYER2_SUPPORT
QString MplayerVersion::mplayer2_version;
bool MplayerVersion::is_mplayer2 = false;
#endif

int MplayerVersion::mplayerVersion(QString string) {
	//static QRegExp rx_mplayer_revision("^MPlayer (\\S+)-SVN-r(\\d+)-(.*)");
	static QRegExp rx_mplayer_revision("^MPlayer (.*)[-\\.]r(\\d+)(.*)");
	static QRegExp rx_mplayer_version("^MPlayer ([a-z0-9.]+)[-(.*)| (.*)]");
	static QRegExp rx_mplayer_git("^MPlayer GIT(.*)", Qt::CaseInsensitive);
	static QRegExp rx_mplayer_version_final("1.0rc([0-9])");
#ifdef MPLAYER2_SUPPORT
	static QRegExp rx_mplayer2_version("^MPlayer2 (.*) \\(C\\).*", Qt::CaseInsensitive);
#endif
	static QRegExp rx_mpv_version("^mpv (.*) (\\(C\\)|Copyright).*", Qt::CaseInsensitive);
#ifndef Q_OS_WIN
	static QRegExp rx_mplayer_version_ubuntu("^MPlayer (\\d):(\\d)\\.(\\d)~(.*)");
	static QRegExp rx_mplayer_revision_ubuntu("^MPlayer svn r(\\d+) (.*)");
	static QRegExp rx_mplayer_version_mandriva("^MPlayer ([a-z0-9\\.]+)-\\d+\\.([a-z0-9]+)\\.[\\d\\.]+[a-z]+[\\d\\.]+-(.*)");
#endif

	int mplayer_svn = 0;
	is_mpv = false;
#ifdef MPLAYER2_SUPPORT
	mplayer2_version = QString::null;
	is_mplayer2 = false;
#endif

#ifdef Q_OS_WIN
	// Hack to recognize mplayer 1.0rc2 from CCCP:
	if (string.startsWith("MPlayer CCCP ")) { 
		string.remove("CCCP "); 
		qDebug() << "MplayerVersion::mplayerVersion: removing CCCP:" << string;
	}
#else
	// Hack to recognize mplayer 1.0rc1 from Ubuntu:
	if (rx_mplayer_version_ubuntu.indexIn(string) > -1) {
		int v1 = rx_mplayer_version_ubuntu.cap(2).toInt();
		int v2 = rx_mplayer_version_ubuntu.cap(3).toInt();
		QString rest = rx_mplayer_version_ubuntu.cap(4);
		//qDebug("%d - %d - %d", rx_mplayer_version_ubuntu.cap(1).toInt(), v1 , v2);
		string = QString("MPlayer %1.%2%3").arg(v1).arg(v2).arg(rest);
		qDebug() << "MplayerVersion::mplayerVersion: line converted to" << string;
	}
	else
	if (rx_mplayer_revision_ubuntu.indexIn(string) > -1) {
		int svn = rx_mplayer_revision_ubuntu.cap(1).toInt();
		QString rest = rx_mplayer_revision_ubuntu.cap(2);
		string = QString("MPlayer SVN-r%1-%2").arg(svn).arg(rest);
		qDebug() << "MplayerVersion::mplayerVersion: line converted to" << string;
	}

	// Hack to recognize mplayer version from Mandriva:
	if (rx_mplayer_version_mandriva.indexIn(string) > -1) {
		QString v1 = rx_mplayer_version_mandriva.cap(1);
		QString v2 = rx_mplayer_version_mandriva.cap(2);
		QString rest = rx_mplayer_version_mandriva.cap(3);
		string = QString("MPlayer %1%2-%3").arg(v1).arg(v2).arg(rest);
		qDebug() << "MplayerVersion::mplayerVersion: line converted to" << string;
	}
#endif

	if (rx_mplayer_git.indexIn(string) > -1) {
		qDebug("MplayerVersion::mplayerVersion: MPlayer from git. Assuming >= 1.0rc3");
		mplayer_svn = MPLAYER_1_0_RC3_SVN;
	}
	else
	if (rx_mplayer_revision.indexIn(string) > -1) {
		mplayer_svn = rx_mplayer_revision.cap(2).toInt();
		qDebug() << "MplayerVersion::mplayerVersion: MPlayer SVN revision found:" << mplayer_svn;
	} 
	else
	if (rx_mplayer_version.indexIn(string) > -1) {
		QString version = rx_mplayer_version.cap(1);
		qDebug() << "MplayerVersion::mplayerVersion: MPlayer version found:" << version;
		mplayer_svn = 0;

		if (version == "1.2") mplayer_svn = MPLAYER_1_2;
		else
		if (version == "1.1") mplayer_svn = MPLAYER_1_1;
		else
		if (version == "1.0rc4") mplayer_svn = MPLAYER_1_0_RC4_SVN;
		else
		if (version == "1.0rc3") mplayer_svn = MPLAYER_1_0_RC3_SVN;
		else
		if (version == "1.0rc2") mplayer_svn = MPLAYER_1_0_RC2_SVN;
		else
		if (version == "1.0rc1") mplayer_svn = MPLAYER_1_0_RC1_SVN;
		else
		/*
		if(rx_mplayer_version_final.indexIn(version) > -1 && rx_mplayer_version_final.cap(1).toInt() > 3)
			mplayer_svn = MPLAYER_1_0_RC3_SVN; //version is > 1.0rc3, so treat as 1.0rc3 since support for later versions is not yet implemented
		else
			qWarning("MplayerVersion::mplayerVersion: unknown MPlayer version");
		*/
		// Assume it's at least mplayer 1.2
		mplayer_svn = MPLAYER_1_2;
	}
	else
#ifdef MPLAYER2_SUPPORT
	if (rx_mplayer2_version.indexIn(string) > -1) {
		mplayer2_version = rx_mplayer2_version.cap(1);
		qDebug() << "MplayerVersion::mplayerVersion: MPlayer2 version found:" << mplayer2_version;
		is_mplayer2 = true;
		mplayer_svn = MPLAYER_1_0_RC4_SVN; // simulates mplayer 1.0rc4
	}
	else
#endif
	if (rx_mpv_version.indexIn(string) > -1) {
		mpv_version = rx_mpv_version.cap(1);
		qDebug() << "MplayerVersion::mplayerVersion: mpv version found:" << mpv_version;
		is_mpv = true;
		#ifdef MPLAYER2_SUPPORT
		is_mplayer2 = true;
		#endif
		mplayer_svn = MPLAYER_1_0_RC4_SVN; // simulates mplayer 1.0rc4
	}

	if (pref) {
		pref->mplayer_detected_version = mplayer_svn;
		#ifdef MPLAYER2_SUPPORT
		pref->mplayer_is_mplayer2 = is_mplayer2;
		pref->mplayer2_detected_version = mplayer2_version;
		#endif
	}

	qDebug() << "MplayerVersion::mplayerVersion: mplayer_svn:" << mplayer_svn;

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
		qWarning("MplayerVersion::isMplayerAtLeast: assuming that the mplayer version is less than %d", svn_revision);
		return false;
	}

	return (mplayer_svn >= svn_revision);
}

bool MplayerVersion::isMplayerAtLeast(int svn_revision) {
	if (pref->mplayer_detected_version >= MPLAYER_1_0_RC1_SVN) {
		// SVN version seems valid
		if (pref->mplayer_user_supplied_version != -1) {
			qDebug("MplayerVersion::isMplayerAtLeast: using the parsed svn version from mplayer output");
			qDebug("MplayerVersion::isMplayerAtLeast: and clearing the previously user supplied version");
			pref->mplayer_user_supplied_version = -1;
		}
		return isMplayerAtLeast(pref->mplayer_detected_version, svn_revision);
	} 
	else 
	if (pref->mplayer_user_supplied_version != -1) {
		qDebug("MplayerVersion::isMplayerAtLeast: no parsed version, using user supplied version");
		return isMplayerAtLeast(pref->mplayer_user_supplied_version, svn_revision);
	}
	else {
		qWarning("MplayerVersion::isMplayerAtLeast: there's no parsed version nor user supplied version!");
		return isMplayerAtLeast(pref->mplayer_detected_version, svn_revision);
	}
}

QString MplayerVersion::toString(int svn_revision) {
	QString version;

	switch (svn_revision) {
		case MPLAYER_1_0_RC1_SVN: version = QString("1.0rc1"); break;
		case MPLAYER_1_0_RC2_SVN: version = QString("1.0rc2"); break;
		case MPLAYER_1_0_RC3_SVN: version = QString("1.0rc3"); break;
		case MPLAYER_1_0_RC4_SVN: version = QString("1.0rc4"); break;
		case MPLAYER_1_1: version = QString("1.1"); break;
		default : version =  QString("SVN r%1").arg(svn_revision);
	}

	return version;
}

