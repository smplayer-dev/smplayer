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


#ifndef _AUDIODATA_H_
#define _AUDIODATA_H_

#include <QString>
#include <QSettings>

/* Class to store info about audio tracks */

class AudioData {

public:

	AudioData();
	~AudioData();

	void setLang( const QString & l ) { _lang = l; };
	void setName( const QString & n ) { _name = n; };
	void setID( int id ) { _ID = id; };

	QString lang() const { return _lang; };
	QString name() const { return _name; };
	int ID() const { return _ID; };

	QString displayName() const;

/*
#ifndef NO_USE_INI_FILES
	void save(QSettings & set);
	void load(QSettings & set);
#endif
*/

protected:

	/* Language code: es, en, etc. */
	QString _lang;

	/* spanish, english... */
	QString _name;

	int _ID;
};

#endif
