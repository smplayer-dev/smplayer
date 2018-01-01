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

#ifndef YT_SIG_H
#define YT_SIG_H

#include <QString>

class YTSig
{
public:
	static QString aclara(const QString & text, const QString & player = "", const QString & function_name = QString::null);

	static void check(QString & u);

	static void setScriptFile(const QString & f) { script_filename = f; reloadScriptFile(); };

private:
	static QString script;
	static QString default_script;
	static QString script_filename;
	static void reloadScriptFile();

private:
	static QString parsed_ts;
};

#endif

