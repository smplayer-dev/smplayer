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

#ifndef _TV_SETTINGS_H_
#define _TV_SETTINGS_H_

#include "filesettingsbase.h"

class QSettings;

class TVSettings : public FileSettingsBase
{
public:
	TVSettings(QString directory);
	virtual ~TVSettings();

	virtual bool existSettingsFor(QString filename);

	virtual void loadSettingsFor(QString filename, MediaSettings & mset);

	virtual void saveSettingsFor(QString filename, MediaSettings & mset);

	static QString filenameToGroupname(const QString & filename);

private:
	QSettings * my_settings;
};

#endif

