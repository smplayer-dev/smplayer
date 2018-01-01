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

#include "prefupdates.h"
#include "preferences.h"
#include "images.h"
#include "updatecheckerdata.h"

PrefUpdates::PrefUpdates(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	createHelp();

#ifndef UPDATE_CHECKER
	updates_check->hide();
	days_frame->hide();
#endif

#ifndef CHECK_UPGRADED
	open_page_check->hide();
#endif
}

PrefUpdates::~PrefUpdates()
{
}

QString PrefUpdates::sectionName() {
	return tr("Updates");
}

QPixmap PrefUpdates::sectionIcon() {
	return Images::icon("pref_updates");
}

void PrefUpdates::retranslateStrings() {
	retranslateUi(this);
	createHelp();
}

void PrefUpdates::setData(Preferences * pref) {
#ifdef UPDATE_CHECKER
	updates_check->setChecked(pref->update_checker_data.enabled);
	days_spin->setValue(pref->update_checker_data.days_to_check);
#endif

#ifdef CHECK_UPGRADED
	open_page_check->setChecked(pref->check_if_upgraded);
#endif
}

void PrefUpdates::getData(Preferences * pref) {
	requires_restart = false;

#ifdef UPDATE_CHECKER
	pref->update_checker_data.enabled = updates_check->isChecked();
	pref->update_checker_data.days_to_check = days_spin->value();
#endif

#ifdef CHECK_UPGRADED
	pref->check_if_upgraded = open_page_check->isChecked();
#endif
}

void PrefUpdates::createHelp() {
	clearHelp();

#ifdef UPDATE_CHECKER
	setWhatsThis(updates_check, tr("Check for updates"),
		tr("If this option is enabled, SMPlayer will check for updates "
           "and display a notification if a new version is available.") );

	setWhatsThis(days_frame, tr("Check interval"),
		tr("You can enter here the interval (in days) for the update checks.") );
#endif

#ifdef CHECK_UPGRADED
	setWhatsThis(open_page_check, tr("Open an informative page after an upgrade"),
		tr("If this option is enabled, an informative page about SMPlayer "
           "will be opened after an upgrade.") );
#endif
}

#include "moc_prefupdates.cpp"
