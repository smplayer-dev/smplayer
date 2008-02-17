/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#include <QApplication>
#include "smplayer.h"
#include "global.h"
#include "helper.h"

using namespace Global;

void myMessageOutput( QtMsgType type, const char *msg ) {
	static QRegExp rx_log;

	if ( (!pref) || (!pref->log_smplayer) ) return;

	rx_log.setPattern(pref->log_filter);

	QString line = QString::fromUtf8(msg);
	switch ( type ) {
		case QtDebugMsg:
			if (rx_log.indexIn(line) > -1) {
				#ifndef NO_DEBUG_ON_CONSOLE
				fprintf( stderr, "Debug: %s\n", line.toLocal8Bit().data() );
				#endif
				Helper::addLog( line );
			}
			break;
		case QtWarningMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Warning: %s\n", line.toLocal8Bit().data() );
			#endif
			Helper::addLog( "WARNING: " + line );
			break;
		case QtFatalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Fatal: %s\n", line.toLocal8Bit().data() );
			#endif
			Helper::addLog( "FATAL: " + line );
			abort();                    // deliberately core dump
		case QtCriticalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Critical: %s\n", line.toLocal8Bit().data() );
			#endif
			Helper::addLog( "CRITICAL: " + line );
			break;
	}
}

int main( int argc, char ** argv ) 
{
	QApplication a( argc, argv );
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	// Sets the ini_path
	QString ini_path;
	if (QFile::exists( a.applicationDirPath() + "/smplayer.ini" ) ) {
        ini_path = a.applicationDirPath();
        qDebug("main: using existing %s", QString(ini_path + "/smplayer.ini").toUtf8().data());
    }
	QStringList args = a.arguments();
	int pos = args.indexOf("-ini-path");
	if ( pos != -1) {
		if (pos+1 < args.count()) {
			pos++;
			ini_path = args[pos];
			// Delete from list
			args.removeAt(pos);
			args.removeAt(pos-1);
		} else {
			printf("Error: expected parameter for -ini-path\r\n");
			return SMPlayer::ErrorArgument;
		}
	}

    qInstallMsgHandler( myMessageOutput );

	SMPlayer * smplayer = new SMPlayer(ini_path);
	SMPlayer::ExitCode c = smplayer->processArgs( args );
	if (c != SMPlayer::NoExit) {
		return c;
	}

	smplayer->start();

	int r = a.exec();

	delete smplayer;

	return r;
}

