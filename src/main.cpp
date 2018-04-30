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

#include "myapplication.h"
#include "smplayer.h"

#include <QDir>

#ifdef HDPI_SUPPORT
#include "paths.h"
#include "hdpisupport.h"

#if defined(PORTABLE_APP) && defined(Q_OS_WIN)
QString windowsApplicationPath() {
	wchar_t my_path[_MAX_PATH+1];
	GetModuleFileName(NULL, my_path,_MAX_PATH);
	QString app_path = QString::fromWCharArray(my_path);
	if (app_path.isEmpty()) return "";
	QFileInfo fi(app_path);
	return fi.absolutePath();
}
#endif

QString hdpiConfig() {
	#ifdef PORTABLE_APP
	return windowsApplicationPath();
	#else
	return Paths::configPath();
	#endif
}
#endif // HDPI_SUPPORT

#ifdef Q_OS_WIN
QStringList winArguments() {
	QString cmdLine = QString::fromWCharArray(GetCommandLine());
    QStringList result;
    int size;
    if (wchar_t **argv = CommandLineToArgvW((const wchar_t *)cmdLine.utf16(), &size)) {
        result.reserve(size);
        wchar_t **argvEnd = argv + size;
        for (wchar_t **a = argv; a < argvEnd; ++a)
            result.append(QString::fromWCharArray(*a));
        LocalFree(argv);
    }
    return result;
}
#endif

int main( int argc, char ** argv )
{
#ifdef HDPI_SUPPORT
	QString hdpi_config_path = hdpiConfig();
	HDPISupport * hdpi = 0;
	if (!hdpi_config_path.isEmpty()) {
		hdpi = new HDPISupport(hdpi_config_path);
	}
#endif

	MyApplication a( "smplayer", argc, argv );
	/*
	if (a.isRunning()) { 
		qDebug("Another instance is running. Exiting.");
		return 0;
	}
	*/

	a.setQuitOnLastWindowClosed(false);
	
#ifdef Q_OS_WIN
	// Change the working directory to the application path
	QDir::setCurrent(a.applicationDirPath());
#endif

#if QT_VERSION >= 0x040400
	// Enable icons in menus
	QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
#endif

	// Sets the config path
	QString config_path;

#ifdef PORTABLE_APP
	config_path = a.applicationDirPath();
#else
	// If a smplayer.ini exists in the app path, will use that path
	// for the config file by default
	if (QFile::exists( a.applicationDirPath() + "/smplayer.ini" ) ) {
		config_path = a.applicationDirPath();
		qDebug("main: using existing %s", QString(config_path + "/smplayer.ini").toUtf8().data());
	}
#endif

#ifdef Q_OS_WIN
	QStringList args = winArguments();
#else
	QStringList args = a.arguments();
#endif
	int pos = args.indexOf("-config-path");
	if ( pos != -1) {
		if (pos+1 < args.count()) {
			pos++;
			config_path = args[pos];
			// Delete from list
			args.removeAt(pos);
			args.removeAt(pos-1);
		} else {
			printf("Error: expected parameter for -config-path\r\n");
			return SMPlayer::ErrorArgument;
		}
	}

	SMPlayer * smplayer = new SMPlayer(config_path);
	#ifdef HDPI_SUPPORT
	qDebug() << "main: hdpi_config_path:" << hdpi_config_path;
	#endif
	SMPlayer::ExitCode c = smplayer->processArgs( args );
	if (c != SMPlayer::NoExit) {
		return c;
	}
	smplayer->start();

	int r = a.exec();

	delete smplayer;

#ifdef HDPI_SUPPORT
	if (hdpi != 0) delete hdpi;
#endif

	return r;
}

