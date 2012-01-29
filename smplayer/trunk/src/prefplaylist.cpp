/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "prefplaylist.h"
#include "preferences.h"
#include "images.h"

PrefPlaylist::PrefPlaylist(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	createHelp();
}

PrefPlaylist::~PrefPlaylist()
{
}

QString PrefPlaylist::sectionName() {
	return tr("Playlist");
}

QPixmap PrefPlaylist::sectionIcon() {
    return Images::icon("playlist", 22);
}

void PrefPlaylist::retranslateStrings() {
	retranslateUi(this);
	createHelp();
}

void PrefPlaylist::setData(Preferences * pref) {
	setAutoAddFilesToPlaylist( pref->auto_add_to_playlist );
	setAddConsecutiveFiles( pref->add_to_playlist_consecutive_files );
}

void PrefPlaylist::getData(Preferences * pref) {
	requires_restart = false;

	pref->auto_add_to_playlist = autoAddFilesToPlaylist();
	pref->add_to_playlist_consecutive_files = addConsecutiveFiles();
}

void PrefPlaylist::setAutoAddFilesToPlaylist(bool b) {
	auto_add_to_playlist_check->setChecked(b);
}

bool PrefPlaylist::autoAddFilesToPlaylist() {
	return auto_add_to_playlist_check->isChecked();
}

void PrefPlaylist::setAddConsecutiveFiles(bool b) {
	add_consecutive_files_check->setChecked(b);
}

bool PrefPlaylist::addConsecutiveFiles() {
	return add_consecutive_files_check->isChecked();
}

void PrefPlaylist::setDirectoryRecursion(bool b) {
	recursive_check->setChecked(b);
}

bool PrefPlaylist::directoryRecursion() {
	return recursive_check->isChecked();
}

void PrefPlaylist::setAutoGetInfo(bool b) {
	getinfo_check->setChecked(b);
}

bool PrefPlaylist::autoGetInfo() {
	return getinfo_check->isChecked();
}

void PrefPlaylist::setSavePlaylistOnExit(bool b) {
	autosave_on_exit_check->setChecked(b);
}

bool PrefPlaylist::savePlaylistOnExit() {
	return autosave_on_exit_check->isChecked();
}

void PrefPlaylist::setPlayFilesFromStart(bool b) {
	play_from_start_check->setChecked(b);
}

bool PrefPlaylist::playFilesFromStart() {
	return play_from_start_check->isChecked();
}

void PrefPlaylist::createHelp() {
	clearHelp();

	setWhatsThis(auto_add_to_playlist_check, tr("Automatically add files to playlist"),
		tr("If this option is enabled, every time a file is opened, SMPlayer "
           "will first clear the playlist and then add the file to it. In "
           "case of DVDs, CDs and VCDs, all titles in the disc will be added "
           "to the playlist.") );

	setWhatsThis(add_consecutive_files_check, tr("Add consecutive files"),
		tr("If this option is enabled, SMPlayer will look for consecutive "
           "files (e.g. video_1.avi, video_2.avi...) and if found, they'll be "
           "added to the playlist.") );

	setWhatsThis(recursive_check, tr("Add files in directories recursively"),
		tr("Check this option if you want that adding a directory will also "
        "add the files in subdirectories recursively. Otherwise only the "
        "files in the selected directory will be added."));

	setWhatsThis(getinfo_check, tr("Add info automatically about files added"), 
		tr("Check this option to inquire the files to be added to the playlist "
        "for some info. That allows to show the title name (if available) and "
        "length of the files. Otherwise this info won't be available until "
        "the file is actually played. Beware: this option can be slow, "
        "specially if you add many files."));

}

#include "moc_prefplaylist.cpp"
