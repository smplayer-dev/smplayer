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
#include "updatechecker.h"

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
	updates_check->setChecked(pref->update_checker_data.enabled);
	days_spin->setValue(pref->update_checker_data.days_to_check);
	open_page_check->setChecked(pref->check_if_upgraded);
}

void PrefUpdates::getData(Preferences * pref) {
	requires_restart = false;

	pref->update_checker_data.enabled = updates_check->isChecked();
	pref->update_checker_data.days_to_check = days_spin->value();
	pref->check_if_upgraded = open_page_check->isChecked();
}

void PrefUpdates::createHelp() {
	clearHelp();

	setWhatsThis(updates_check, tr("Check for updates"),
		tr("If this option is enabled, SMPlayer will check for updates "
           "and display a notification if a new version is available.") );

	setWhatsThis(days_frame, tr("Interval for checks"),
		tr("You can enter here the interval (in days) for the update checks.") );

	setWhatsThis(open_page_check, tr("Open a web page with info and tips after an upgrade"),
		tr("If this option is enabled, every time SMPlayer is upgraded a web page will "
		   "be opened with information and tips about how to use SMPlayer.") );
}

#include "moc_prefupdates.cpp"
