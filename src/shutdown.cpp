/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "shutdown.h"
#include <QProcess>

void Shutdown::shutdown() {
#ifdef Q_OS_WIN
	QProcess::startDetached("shutdown -s -f -t 10");
#endif

#ifdef Q_OS_LINUX
	QProcess::startDetached("xmessage", QStringList() << "-buttons" << "Accept:0" << "-center" <<
		"This is a message from SMPlayer\n"
		"The computer should shut down now.\n"
		"However shutdown hasn't been implemented yet.");
#endif
}

