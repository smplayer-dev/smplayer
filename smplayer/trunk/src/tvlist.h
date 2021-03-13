/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef TVLIST_H
#define TVLIST_H

#include "favorites.h"

class QWidget;

class TVList : public Favorites
{
	Q_OBJECT

public:
	enum Service { TV = 1, Radio = 2, Data = 4 };
	Q_DECLARE_FLAGS(Services, Service);

	TVList(bool check_channels_conf, Services services, QString filename, QWidget * parent = 0);
	~TVList();

#ifndef Q_OS_WIN
	static QString findChannelsFile();
#endif

#ifndef Q_OS_WIN
protected:
	void parse_channels_conf(Services services);
#endif

protected:
	virtual Favorites * createNewObject(QString filename, QWidget * parent);

protected slots:
	virtual void edit();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TVList::Services)

#endif

