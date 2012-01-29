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

//! This class disables and restores the windows screensaver 

#ifndef _WINSCREENSAVER_H_
#define _WINSCREENSAVER_H_

#ifdef Q_OS_OS2
#include <QLibrary>
#endif

class WinScreenSaver
{
public:
	WinScreenSaver();
	~WinScreenSaver();

	void disable();
	void enable();

private:
	void retrieveState();
	void restoreState();
#ifdef Q_OS_OS2
	void unload();
#endif

private:
#ifndef Q_OS_OS2
	int lowpower, poweroff, screensaver;
#else
	QLibrary *SSaver;
	typedef int (*FuncPtr) (void);
	FuncPtr SSCore_TempDisable;
	FuncPtr SSCore_TempEnable;
#endif
	bool state_saved, modified;
};

#endif
