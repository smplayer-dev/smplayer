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

#include "playlistpreferences.h"

PlaylistPreferences::PlaylistPreferences( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
    setupUi(this);
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	//layout()->setSizeConstraint(QLayout::SetFixedSize);
	adjustSize();
}

PlaylistPreferences::~PlaylistPreferences() {
}

void PlaylistPreferences::setDirectoryRecursion(bool b) {
	recursive_check->setChecked(b);
}

bool PlaylistPreferences::directoryRecursion() {
	return recursive_check->isChecked();
}

void PlaylistPreferences::setAutoGetInfo(bool b) {
	getinfo_check->setChecked(b);
}

bool PlaylistPreferences::autoGetInfo() {
	return getinfo_check->isChecked();
}

QSize PlaylistPreferences::sizeHint () const {
	return QSize(580,200);
}

#include "moc_playlistpreferences.cpp"

