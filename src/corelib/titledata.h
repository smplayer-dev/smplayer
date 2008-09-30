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


#ifndef _TITLEDATA_H_
#define _TITLEDATA_H_

#include <QString>
#include <QSettings>

/* Class to store info about DVD titles */

class TitleData {

public:

	TitleData();
	~TitleData();

	void setName( const QString & n ) { _name = n; };
	void setDuration( double d ) { _duration = d; };
	void setChapters( int n ) { _chapters = n; };
	void setAngles( int n ) { _angles = n; };
	void setID( int id ) { _ID = id; };

	QString name() const { return _name; };
	double duration() const { return _duration; };
	int chapters() const { return _chapters; };
	int angles() const { return _angles; };
	int ID() const { return _ID; };

	QString displayName() const;

protected:
	QString _name;
	double _duration;
	int _chapters;
	int _angles;

	int _ID;
};


#endif
