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

#include "filehash.h"
#include <QFile>
#include <QDataStream>

// From the patch by Kamil Dziobek turbos11(at)gmail.com
// (c) Kamil Dziobek turbos11(at)gmail.com | BSD or GPL or public domain
QString FileHash::calculateHash(QString filename) {
	QFile file(filename);

	if (!file.exists()) {
		qWarning("OSParser:calculateHash: error hashing file. File doesn't exist.");
		return QString();
	}

	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);
	in.setByteOrder(QDataStream::LittleEndian);
	quint64 size=file.size ();
	quint64 hash=size; 
	quint64 a;
	for(int i = 0; i < 8192; i++) {
		in >> a ; hash += a;
	};
	file.seek(size-65536);
	for(int i = 0; i < 8192; i++) {
		in >> a ; hash += a;
	};

	QString hexhash = QString("%1").arg(hash, 16, 16, QChar('0'));

	return hexhash;
}

