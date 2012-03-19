/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#ifdef SINGLE_INSTANCE
#include "QtSingleApplication"

class MyApplication : public QtSingleApplication
{
	Q_OBJECT

public:
	MyApplication ( const QString & appId, int & argc, char ** argv ) 
		: QtSingleApplication(appId, argc, argv) {};

	virtual void commitData ( QSessionManager & /*manager*/ ) {
		// Nothing to do, let the application to close
	}

	inline static MyApplication * instance() {
		return qobject_cast<MyApplication*>(QApplication::instance());
	}
};

#else
#include <QApplication>

class MyApplication : public QApplication
{
	Q_OBJECT

public:
	MyApplication ( const QString & appId, int & argc, char ** argv ) : QApplication(argc, argv) {};

	virtual void commitData ( QSessionManager & /*manager*/ ) {
		// Nothing to do, let the application to close
	}
};

#endif

#endif

