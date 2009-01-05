/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>

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

	static QString join(int title, const QString & device, bool use_dvdnav = false);
	static QString join(const DiscData & d);
	static DiscData split(const QString & dvd_url, bool *ok = 0);
};

#endif
