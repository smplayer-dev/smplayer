/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@escomposlinux.org>

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

#ifndef _CHAPTERS_H_
#define _CHAPTERS_H_

#include <QMap>
#include "helper.h"

/* Class to store info about chapters */

class ChapterData {

public:
	ChapterData() { _name = ""; _start = 0 ; _end = 0 ; _ID = -1; };
	~ChapterData() { };

	void setName( const QString & n ) { _name = n; };
	void setStart( double start ) { _start = start; };
	void setEnd( double end ) { _end = end; };
	void setID( int id ) { _ID = id; };

	QString name() const { return _name; };
	double start() const { return _start; };
	double end() const { return _end; };
	int ID() const { return _ID; };
protected:
	QString _name;
	double _start;
	double _end;

	int _ID;
};

class Chapters {

public:
	Chapters();
	~Chapters();

	void clear();
	void list();

	void addName(int ID, QString name);
	void addStart(int ID, double start);
	void addEnd(int ID, double end);
	void addID(int ID);

	int numItems();
	bool existsItemAt(int n);

	ChapterData itemAt(int n);
	ChapterData item(int ID);
	ChapterData itemFromTime(double sec);
	ChapterData itemAfterTime(double sec);
	ChapterData itemBeforeTime(double sec);
	int find(int ID);

protected:
	typedef QMap <int, ChapterData> ChapterMap;
	ChapterMap cm;
};

#endif
