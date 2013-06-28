/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "ytsig.h"

QString YTSig::rev(const QString & orig) {
	QString r;
	for (int n = orig.size()-1; n >= 0; n--) {
		r.append(orig.at(n));
	}
	return r;
}

// Algorithms from youtube-dl
QString YTSig::aclara(const QString & text) {
	QString res;

	qDebug("ytsig: length: %d", text.size());

	if (text.size() == 86) {
		res = text.mid(2,61) + text.mid(82,1) + text.mid(64,18) + text.mid(63,1);
	}
	else
	if (text.size() == 87) {
		res = text.mid(62,1) + rev(text.mid(63,20)) + text.mid(83,1) + rev(text.mid(53,9)) + text.mid(0,1) + rev(text.mid(3,49));
	}
	else {
		qDebug("ytsig: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
		return res;
	}

	/*
	qDebug("%d: orig: %s", text.size(), text.toLatin1().constData());
	qDebug("%d: conv: %s", text.size(), res.toLatin1().constData());
	*/

	return res;
}
