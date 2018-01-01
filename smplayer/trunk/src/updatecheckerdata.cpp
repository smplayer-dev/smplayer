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

#include "updatecheckerdata.h"
#include "version.h"
#include <QSettings>

void UpdateCheckerData::save(QSettings * set) {
	set->beginGroup("update_checker");
	set->setValue("checked_date", last_checked);
	set->setValue("enabled", enabled);
	set->setValue("days_to_check", days_to_check);
	set->setValue("last_known_version", last_known_version);
	set->endGroup();
}

void UpdateCheckerData::load(QSettings * set) {
	set->beginGroup("update_checker");
	last_checked = set->value("checked_date", 0).toDate();
	enabled = set->value("enabled", true).toBool();
	days_to_check = set->value("days_to_check", 7).toInt();
	last_known_version = set->value("last_known_version", Version::with_revision()).toString();
	set->endGroup();
}

