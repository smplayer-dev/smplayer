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

#ifndef INFOFILE_H
#define INFOFILE_H

#define INFO_SIMPLE_LAYOUT

#include "mediadata.h"
#include <QString>

class InfoFile : public QObject
{
	Q_OBJECT

public:
	InfoFile(QObject * parent = 0);
	~InfoFile();

	QString getInfo(MediaData md);

protected:
	QString title(QString text, QString icon);
	QString openPar(QString text);
	QString closePar();
	QString openItem();
	QString closeItem();

	QString addTrackColumns(QStringList l);

	QString addItem( QString tag, QString value );
	QString addTrack(int n, QString lang, QString name, int ID, QString type = "");

	QString defaultStyle();
	QString style();

#ifndef INFO_SIMPLE_LAYOUT
	int row;
#endif
};

#endif
