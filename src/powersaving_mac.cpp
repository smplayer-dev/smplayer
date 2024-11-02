/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2024 Ricardo Villalba <ricardo@smplayer.info>

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

#include "powersaving_mac.h"

#include <IOKit/pwr_mgt/IOPMLib.h>
#include <QDebug>

PowerSaving::PowerSaving(QObject * parent)
	: QObject(parent)
{
}

PowerSaving::~PowerSaving() {
}

bool PowerSaving::isValid() {
	return true;
}

void PowerSaving::inhibit() {
	qDebug("PowerSaving::inhibit");

	IOPMAssertionID assertion_id = 0;
	CFStringRef assertion_type = kIOPMAssertionTypeNoDisplaySleep;
	IOReturn r = IOPMAssertionCreateWithName(assertion_type,
								kIOPMAssertionLevelOn,
								CFSTR("Playing media content"),
								&assertion_id);
	if (r == kIOReturnSuccess) {
		qDebug() << "PowerSaving::inhibit: adding assertion_id:" << assertion_id;
		assertions << assertion_id;
	}

}

void PowerSaving::uninhibit() {
	qDebug("PowerSaving::uninhibit");

	foreach (IOPMAssertionID assertion_id, assertions) {
		qDebug() << "PowerSaving::uninhibit: removing assertion_id:" << assertion_id;
		IOPMAssertionRelease(assertion_id);
	}
	assertions.clear();
}

#include "moc_powersaving_mac.cpp"
