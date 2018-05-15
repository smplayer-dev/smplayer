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

#include "globalshortcutsdialog.h"
#include "globalshortcuts/globalshortcuts.h"

GlobalShortcutsDialog::GlobalShortcutsDialog(QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);
	setWindowTitle("SMPlayer - " + tr("Global Shortcuts"));
}

GlobalShortcutsDialog::~GlobalShortcutsDialog() {
}

void GlobalShortcutsDialog::setGrabbedKeys(int keys) {
	GlobalShortcuts::MediaKeys k = (GlobalShortcuts::MediaKeys) keys;

	play_check->setChecked(k.testFlag(GlobalShortcuts::MediaPlay));
	stop_check->setChecked(k.testFlag(GlobalShortcuts::MediaStop));
	previous_check->setChecked(k.testFlag(GlobalShortcuts::MediaPrevious));
	next_check->setChecked(k.testFlag(GlobalShortcuts::MediaNext));
	pause_check->setChecked(k.testFlag(GlobalShortcuts::MediaPause));
	record_check->setChecked(k.testFlag(GlobalShortcuts::MediaRecord));
	mute_check->setChecked(k.testFlag(GlobalShortcuts::VolumeMute));
	voldown_check->setChecked(k.testFlag(GlobalShortcuts::VolumeDown));
	volup_check->setChecked(k.testFlag(GlobalShortcuts::VolumeUp));
}

int GlobalShortcutsDialog::grabbedKeys() {
	GlobalShortcuts::MediaKeys k(QFlag (0));

	if (play_check->isChecked()) k = k | GlobalShortcuts::MediaPlay;
	if (stop_check->isChecked()) k = k | GlobalShortcuts::MediaStop;
	if (previous_check->isChecked()) k = k | GlobalShortcuts::MediaPrevious;
	if (next_check->isChecked()) k = k | GlobalShortcuts::MediaNext;
	if (pause_check->isChecked()) k = k | GlobalShortcuts::MediaPause;
	if (record_check->isChecked()) k = k | GlobalShortcuts::MediaRecord;
	if (mute_check->isChecked()) k = k | GlobalShortcuts::VolumeMute;
	if (voldown_check->isChecked()) k = k | GlobalShortcuts::VolumeDown;
	if (volup_check->isChecked()) k = k | GlobalShortcuts::VolumeUp;

	return k;
}

#include "moc_globalshortcutsdialog.cpp"
