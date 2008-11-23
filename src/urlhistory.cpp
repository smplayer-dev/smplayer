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

#include "urlhistory.h"
#include "constants.h"

URLHistory::URLHistory() : Recents() 
{
}

URLHistory::~URLHistory() {
}

void URLHistory::addUrl(QString url, bool is_playlist) {
	qDebug("Recents::addItem: '%s'", url.toUtf8().data());

	// Delete duplicates
	QStringList::iterator iterator = l.begin();
	while (iterator != l.end()) {
		QString s = (*iterator);
		if (s.endsWith(IS_PLAYLIST_TAG)) {
			s = s.remove( QRegExp(IS_PLAYLIST_TAG_RX) );
		}
		if (s == url) 
			iterator = l.erase(iterator);
		else
			iterator++;
	}

	// Add new item to list
	if (is_playlist) url = url + IS_PLAYLIST_TAG;
	l.prepend(url);

	if (l.count() > max_items) l.removeLast();
}

QString URLHistory::url(int n) {
	QString s = l[n];
	if (isPlaylist(n)) s = s.remove( QRegExp(IS_PLAYLIST_TAG_RX) );
	return s;
}

bool URLHistory::isPlaylist(int n) {
	return l[n].endsWith(IS_PLAYLIST_TAG);
}

