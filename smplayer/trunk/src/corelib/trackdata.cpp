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

#include "trackdata.h"
#include "helper.h"
#include <QFileInfo>

TrackData::TrackData() {
	_lang = "";
	_name = "";
	_filename = "";
	_duration = 0;
	_ID = -1;
#if !GENERIC_CHAPTER_SUPPORT
	_chapters = 0;
#endif
	_angles = 0;
}

TrackData::~TrackData() {
}

QString TrackData::displayName() const {
	//qDebug("TrackData::displayName");

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
	if (!_filename.isEmpty()) {
		QFileInfo f(_filename);
	    dname = f.fileName();
	}
	else
    dname = QString::number(_ID);

	if (_duration > 0) {
		dname += " ("+ Helper::formatTime( (int) _duration ) +")";
	}

	return dname;
}

#ifndef NO_USE_INI_FILES
void TrackData::save(QSettings & set) {
	//qDebug("TrackData::save");

	set.setValue( "lang", _lang );
	set.setValue( "name", _name );
	set.setValue( "filename", _filename );
	set.setValue( "duration", _duration );
#if !GENERIC_CHAPTER_SUPPORT
	set.setValue( "chapters", _chapters );
#endif
	set.setValue( "angles", _angles );
	set.setValue( "ID", _ID );
}

void TrackData::load(QSettings & set) {
	//qDebug("TrackData::load");

	_lang = set.value( "lang", _lang ).toString();
	_name = set.value( "name", _name ).toString();
	_filename = set.value( "filename", _filename ).toString();
	_duration = set.value( "duration", _duration).toDouble();
#if !GENERIC_CHAPTER_SUPPORT
	_chapters = set.value( "chapters", _chapters ).toInt();
#endif
	_angles =  set.value( "angles", _angles ).toInt();
	_ID = set.value( "ID", _ID ).toInt();
}

#endif // NO_USE_INI_FILES

void TrackData::list() {
	//qDebug("TrackData::list");

	qDebug("     ID: '%d' lang: '%s' name: '%s'", _ID, _lang.toUtf8().data(), _name.toUtf8().data() );
#if GENERIC_CHAPTER_SUPPORT
	qDebug("     filename: '%s' duration: %f angles: %d", 
            _filename.toUtf8().data(), _duration, _angles );
#else
	qDebug("     filename: '%s' duration: %f chapters: %d angles: %d", 
            _filename.toUtf8().data(), _duration, _chapters, _angles );
#endif
}
