/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2007 Ricardo Villalba <rvm@escomposlinux.org>

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

#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>

//! Tries to get the revision by running "svn info"
QString get_from_svn_info() {
	QString revision;

	QProcess p;
	p.setEnvironment( QStringList() << "LC_ALL=C" );
	p.setProcessChannelMode(QProcess::MergedChannels);
	p.start( "svn", QStringList() << "info" );
	p.waitForStarted();
	p.waitForFinished();

	QByteArray line;
	QRegExp rx("^Revision: (\\d+)");
	while (p.canReadLine()) {
		line = p.readLine();
		if (rx.indexIn(line)!=-1) {
			revision = rx.cap(1);
			break;
		}
		//qDebug( line.data() );
	}

	return revision;
}

//! Looks for 'revision="number"' in .svn/entries
QString get_from_entries_1() {
	QString revision;

	QFile f(".svn/entries");
	QRegExp rx("revision=\"(\\d+)\"");
	if ( f.open(QIODevice::ReadOnly) ) {
		QByteArray line;
		while (!f.atEnd()) {
			line = f.readLine();
			if (rx.indexIn(line)!=-1) {
				revision = rx.cap(1);
				break;
			}
			//qDebug( line.data() );
		}
	}

	return revision;
}

//! Looks for 'dir' in .svn/entries
//! the revision is in the next line
QString get_from_entries_2() {
	QString revision;

	QFile f(".svn/entries");
	//QFile f("/tmp/temp/entries");
	QRegExp rx("^dir$");
	if ( f.open(QIODevice::ReadOnly) ) {
		QByteArray line;
		while (!f.atEnd()) {
			line = f.readLine();
			line = line.simplified();
			if (rx.indexIn(line)!=-1) {
				// Next line
				line = f.readLine();
				revision = line.simplified();
				break;
			}
			//qDebug( line.data() );
		}
	}

	return revision;
}

int main( int argc, char ** argv )
{
	QCoreApplication a( argc, argv );

	QString revision;

	revision = get_from_svn_info();

	if (revision.isEmpty()) {
		revision = get_from_entries_1();

		if (revision.isEmpty()) {
			revision = get_from_entries_2();

			if (revision.isEmpty()) {
				revision = "UNKNOWN";
			}
		}
	}

	printf("#define SVN_REVISION \"SVN-r%s\"\n", revision.toLatin1().data());

	return 0;
};
