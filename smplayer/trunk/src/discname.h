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

#ifndef _DISCNAME_H_
#define _DISCNAME_H_

#include <QString>

#define DISCNAME_TEST 0

class DiscData {

public:

	DiscData() { 
		protocol = ""; 
		device = ""; 
		title = 0; 
	};
	DiscData(const QString & protocol, int title, const QString & device) { 
		this->protocol = protocol; 
		this->title = title; 
		this->device = device; 
	};
	~DiscData() {};

	QString protocol;
	QString device;
	int title;
};

class DiscName {

public:
	enum Disc { DVD = 1, DVDNAV = 2, VCD = 3, CDDA = 4 };

	static QString join(Disc type, int title, const QString & device);
	static QString join(const DiscData & d);
	static QString joinDVD(int title, const QString & device, bool use_dvdnav = false);
	static DiscData split(const QString & disc_url, bool *ok = 0);

#if DISCNAME_TEST
	static void test();
#endif

protected:
	static QString removeTrailingSlash(const QString & device);
};

#endif
