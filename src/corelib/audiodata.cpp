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

#include "audiodata.h"
#include "helper.h"

AudioData::AudioData() {
	_lang = "";
	_name = "";
	_ID = -1;
}

AudioData::~AudioData() {
}

QString AudioData::displayName() const {
	//qDebug("AudioData::displayName");

	QString dname="";

    if (!_name.isEmpty()) {
        dname = _name;
		if (!_lang.isEmpty()) {
			dname += " ["+ _lang + "]";
		}
	}
    else
    if (!_lang.isEmpty()) {
        dname = _lang;
	}
    else
    dname = QString::number(_ID);

	return dname;
}

/*
#ifndef NO_USE_INI_FILES
void AudioData::save(QSettings & set) {
	//qDebug("AudioData::save");

	set.setValue( "lang", _lang );
	set.setValue( "name", _name );
	set.setValue( "filename", _filename );
	set.setValue( "ID", _ID );
}

void AudioData::load(QSettings & set) {
	//qDebug("AudioData::load");

	_lang = set.value( "lang", _lang ).toString();
	_name = set.value( "name", _name ).toString();
	_filename = set.value( "filename", _filename ).toString();
	_ID = set.value( "ID", _ID ).toInt();
}

#endif // NO_USE_INI_FILES
*/
