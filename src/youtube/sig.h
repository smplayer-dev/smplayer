/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef SIG_H
#define SIG_H

#include <QString>

class QSettings;

class Sig
{
public:
	void clear() { html5_player = ""; sts = ""; decrypt_function = ""; }

	QString html5_player;
	QString sts;
	QString function_name;
	QString decrypt_function;

	QString findFunctions(const QString & text);
	QString aclara(const QString & text);

	void save(QSettings * set);
	void load(QSettings * set);

private:
	static QString findText(const QString & text, const QString & begin, const QString & end);
};

#endif

