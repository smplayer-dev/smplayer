/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <ricardo@smplayer.info>

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

static IOPMAssertionID assertion_id = 0;

PowerSaving::PowerSaving(QObject * parent)
	: QObject(parent)
{
}

PowerSaving::~PowerSaving() {
}

void PowerSaving::inhibit() {
	qDebug("PowerSaving (mac)::inhibit");

	CFStringRef assertion_type = kIOPMAssertionTypeNoDisplaySleep;
	IOPMAssertionCreateWithName(assertion_type,
								kIOPMAssertionLevelOn,
								CFSTR("Playing media content"),
								&assertion_id);
}

void PowerSaving::uninhibit() {
	qDebug("PowerSaving (mac)::uninhibit");

	IOPMAssertionRelease(assertion_id);
}

#include "moc_powersaving_mac.cpp"
