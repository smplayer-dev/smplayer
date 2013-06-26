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

#include "prefupdates.h"
#include "preferences.h"
#include "images.h"

PrefUpdates::PrefUpdates(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	createHelp();
}

PrefUpdates::~PrefUpdates()
{
}

QString PrefUpdates::sectionName() {
	return tr("Updates");
}

QPixmap PrefUpdates::sectionIcon() {
    return Images::icon("playlist", 22);
}

void PrefUpdates::retranslateStrings() {
	retranslateUi(this);
	createHelp();
}

void PrefUpdates::setData(Preferences * pref) {
}

void PrefUpdates::getData(Preferences * pref) {
	requires_restart = false;

}

void PrefUpdates::createHelp() {
	clearHelp();

/*
	setWhatsThis(auto_add_to_playlist_check, tr("Automatically add files to playlist"),
		tr("If this option is enabled, every time a file is opened, SMPlayer "
           "will first clear the playlist and then add the file to it. In "
           "case of DVDs, CDs and VCDs, all titles in the disc will be added "
           "to the playlist.") );
*/
}

#include "moc_prefupdates.cpp"
