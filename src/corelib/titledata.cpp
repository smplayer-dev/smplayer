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

#include "titledata.h"
#include "helper.h"
#include <QFileInfo>

TitleData::TitleData() {
	_name = "";
	_duration = 0;
	_ID = -1;
	_chapters = 0;
	_angles = 0;
}

TitleData::~TitleData() {
}

QString TitleData::displayName() const {
	//qDebug("TitleData::displayName");

	QString dname="";

    if (!_name.isEmpty()) {
        dname = _name;
	}
	else
    dname = QString::number(_ID);

	if (_duration > 0) {
		dname += " ("+ Helper::formatTime( (int) _duration ) +")";
	}

	return dname;
}

