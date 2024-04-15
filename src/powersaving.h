/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2024 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef POWER_SAVING_H
#define POWER_SAVING_H

#include <QObject>
#include <QList>

class QDBusInterface;

class PowerSaving : public QObject
{
	Q_OBJECT

public:
	PowerSaving(QObject * parent = 0);
	~PowerSaving();

public slots:
	void inhibit();
	void uninhibit();

protected:
	QDBusInterface * interface;
	QList<uint> cookies;
};

#endif
