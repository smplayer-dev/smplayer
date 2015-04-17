/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "sharedata.h"

QUrl ShareData::donateUrl() {
	return QUrl("http://smplayer.sourceforge.net/donate.php");
}

QUrl ShareData::facebookUrl() {
	QString share_url = "http://smplayer.sourceforge.net";
	return QUrl("http://www.facebook.com/sharer.php?u=" + share_url);
}

QUrl ShareData::twitterUrl() {
	QString share_url = "http://smplayer.sourceforge.net";

	QString text = QObject::tr("SMPlayer is a free media player for PC. It plays all formats and can even download Youtube videos.",
					  "This text is to be published on twitter and the translation should not be more than 99 characters long");

	qDebug("ShareData::twitterUrl: length: %d", text.length());
	if (text.length() > 99) {
		qDebug("ShareData::twitterUrl: the translation text is too long (%d), it shouldn't be longer than 99 characters. Using the English text.", text.length());
		text = "SMPlayer is a free media player for PC. It plays all formats and can even download Youtube videos.";
	}
	//text = text.replace("SMPlayer", "#SMPlayer");
	text = QUrl::toPercentEncoding(text);
	QString url = "http://twitter.com/intent/tweet?text=" + text + "&url=" + QUrl::toPercentEncoding(share_url) + "/&via=smplayer_dev"; 
	return QUrl::fromEncoded(url.toLatin1());
}

