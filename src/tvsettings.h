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

#ifndef TV_SETTINGS_H
#define TV_SETTINGS_H

#include "filesettingsbase.h"

class QSettings;

class TVSettings : public FileSettingsBase
{
public:
	TVSettings(QString directory);
	virtual ~TVSettings();

	virtual bool existSettingsFor(QString filename, int type);

	virtual void loadSettingsFor(QString filename, int type, MediaSettings & mset, int player);

	virtual void saveSettingsFor(QString filename, int type, MediaSettings & mset, int player);

	static QString filenameToGroupname(const QString & filename);

private:
	QSettings * my_settings;
};

#endif

