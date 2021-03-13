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

#ifndef FILESETTINGS_BASE_H
#define FILESETTINGS_BASE_H

#include <QString>

class MediaSettings;

class FileSettingsBase
{
public:
	FileSettingsBase(QString directory) { output_directory = directory; };
	virtual ~FileSettingsBase() {};

	virtual bool existSettingsFor(QString filename, int type) = 0;

	virtual void loadSettingsFor(QString filename, int type, MediaSettings & mset, int player) = 0;

	virtual void saveSettingsFor(QString filename, int type, MediaSettings & mset, int player) = 0;

protected:
	QString output_directory;
};

#endif

