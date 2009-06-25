/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "tvlist.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

TVList::TVList(QString filename, QObject * parent) : Favorites(filename,parent)
{
	parse_channels_conf();
}

TVList::~TVList() {
}

void TVList::parse_channels_conf() {
	qDebug("TVList::parse_channels_conf");

	QString file = QDir::homePath() + "/.mplayer/channels.conf";
	QFile f( file );
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug("TVList::parse_channels_conf: can't open %s", file.toUtf8().constData());
		return;
	}

	QTextStream in(&f);
	while (!in.atEnd()) {
		QString line = in.readLine();
		qDebug("TVList::parse_channels_conf: '%s'", line.toUtf8().constData());
		QString channel = line.section(':', 0, 0);
		if (!channel.isEmpty()) {
			qDebug("TVList::parse_channels_conf: channel: '%s'", channel.toUtf8().constData());
			QString channel_id = "dvb://"+channel;
			if (findFile(channel_id) == -1) {
				f_list.append( Favorite(channel, channel_id) );
			}
		}
	}
}

#include "moc_tvlist.cpp"
