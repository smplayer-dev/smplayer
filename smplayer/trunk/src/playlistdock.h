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

#ifndef PLAYLIST_DOCK_H
#define PLAYLIST_DOCK_H

#include <QtGlobal>
#include <QDockWidget>
#include "guiconfig.h"

class PlaylistDock : public QDockWidget 
{
	Q_OBJECT

public:
	PlaylistDock ( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	~PlaylistDock();

signals:
	void closed();
/*
#if QT_VERSION < 0x040300
	void visibilityChanged(bool visible);
#endif
*/

protected:
	virtual void closeEvent( QCloseEvent * e );
/*
#if QT_VERSION < 0x040300
	virtual void showEvent ( QShowEvent * event );
	virtual void hideEvent ( QHideEvent * event );
#endif
*/
};

#endif
