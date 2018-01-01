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

#include "smplayer.h"
#include "global.h"
#include "paths.h"
#include "translator.h"
#include "version.h"
#include "config.h"
#include "clhelp.h"
#include "cleanconfig.h"
#include "myapplication.h"
#include "baseguiplus.h"

#ifdef DEFAULTGUI
#include "defaultgui.h"
#endif

#ifdef MINIGUI
#include "minigui.h"
#endif

#ifdef MPCGUI
#include "mpcgui.h"
#endif

#ifdef SKINS
#include "skingui.h"
#endif

#include <QDir>
#include <QUrl>
#include <QTime>
#include <stdio.h>

#ifdef Q_OS_WIN
#if USE_ASSOCIATIONS
#include "extensions.h"
#include "winfileassoc.h"	//required for Uninstall
#endif
#endif

#ifdef FONTCACHE_DIALOG
#include "fontcache.h"
#include "version.h"
#endif

using namespace Global;

BaseGui * SMPlayer::main_window = 0;

SMPlayer::SMPlayer(const QString & config_path, QObject * parent )
	: QObject(parent) 
{
#ifdef LOG_SMPLAYER
	#if QT_VERSION >= 0x050000
	qInstallMessageHandler( SMPlayer::myMessageOutput );
	#else
	qInstallMsgHandler( SMPlayer::myMessageOutput );
	#endif
	allow_to_send_log_to_gui = true;
#endif

	gui_to_use = "DefaultGUI";

	close_at_end = -1; // Not set
	start_in_fullscreen = -1; // Not set

	move_gui = false;
	resize_gui = false;

	Paths::setAppPath( qApp->applicationDirPath() );

#ifndef PORTABLE_APP
	if (config_path.isEmpty()) createConfigDirectory();
#endif
	global_init(config_path);

	// Application translations
	translator->load( pref->language );
	showInfo();

#ifdef FONTS_HACK
	createFontFile();
#endif
}

SMPlayer::~SMPlayer() {
	if (main_window != 0) {
		deleteGUI();
	}
	global_end();

#ifdef LOG_SMPLAYER
	if (output_log.isOpen()) output_log.close();
#endif
}

BaseGui * SMPlayer::gui() {
	if (main_window == 0) {
		// Changes to app path, so smplayer can find a relative mplayer path
		QDir::setCurrent(Paths::appPath());
		qDebug("SMPlayer::gui: changed working directory to app path");
		qDebug("SMPlayer::gui: current directory: %s", QDir::currentPath().toUtf8().data());

#ifdef SKINS
		if (gui_to_use == "SkinGUI") {
			QString theme = pref->iconset;
			if (theme.isEmpty()) theme = "Gonzo";
			QString user_theme_dir = Paths::configPath() + "/themes/" + theme;
			QString theme_dir = Paths::themesPath() + "/" + theme;
			qDebug("SMPlayer::gui: user_theme_dir: %s", user_theme_dir.toUtf8().constData());
			qDebug("SMPlayer::gui: theme_dir: %s", theme_dir.toUtf8().constData());
			if ((QDir(theme_dir).exists()) || (QDir(user_theme_dir).exists())) {
				if (pref->iconset.isEmpty()) pref->iconset = theme;
			} else {
				qDebug("SMPlayer::gui: skin folder doesn't exist. Falling back to default gui.");
				gui_to_use = "DefaultGUI";
				pref->iconset = "";
				pref->gui = gui_to_use;
			}
		}
#endif

		main_window = createGUI(gui_to_use);

		if (move_gui) {
			qDebug("SMPlayer::gui: moving main window to %d %d", gui_position.x(), gui_position.y());
			main_window->move(gui_position);
		}
		if (resize_gui) {
			qDebug("SMPlayer::gui: resizing main window to %dx%d", gui_size.width(), gui_size.height());
			main_window->resize(gui_size);
		}
	}

	return main_window;
}

BaseGui * SMPlayer::createGUI(QString gui_name) {
	BaseGui * gui = 0;

#ifdef SKINS
	if (gui_name.toLower() == "skingui")
		gui = new SkinGui(0);
	else
#endif
#ifdef MINIGUI
	if (gui_name.toLower() == "minigui") 
		gui = new MiniGui(0);
	else
#endif
#ifdef MPCGUI
	if (gui_name.toLower() == "mpcgui")
		gui = new MpcGui(0);
	else
#endif
#ifdef DEFAULTGUI
	if (gui_name.toLower() == "defaultgui")
		gui = new DefaultGui(0);
	else
#endif
	{
		// No GUI
		qWarning() << "SMPlayer::createGUI: there's no GUI available!";
		gui = new BaseGuiPlus(0);
	}

	gui->setForceCloseOnFinish(close_at_end);
	gui->setForceStartInFullscreen(start_in_fullscreen);
	connect(gui, SIGNAL(quitSolicited()), qApp, SLOT(quit()));

#ifdef GUI_CHANGE_ON_RUNTIME
	connect(gui, SIGNAL(guiChanged(QString)), this, SLOT(changeGUI(QString)));
#endif

#if SINGLE_INSTANCE
	MyApplication * app = MyApplication::instance();
	connect(app, SIGNAL(messageReceived(const QString&)),
            gui, SLOT(handleMessageFromOtherInstances(const QString&)));
	app->setActivationWindow(gui);
#endif

	return gui;
}

void SMPlayer::deleteGUI() {
#ifdef LOG_SMPLAYER
	allow_to_send_log_to_gui = false;
#endif

	delete main_window;
	main_window = 0;

#ifdef LOG_SMPLAYER
	allow_to_send_log_to_gui = true;
#endif
}

#ifdef GUI_CHANGE_ON_RUNTIME
void SMPlayer::changeGUI(QString new_gui) {
	qDebug("SMPlayer::changeGUI: '%s'", new_gui.toLatin1().constData());

	deleteGUI();

	main_window = createGUI(new_gui);

	main_window->show();
}
#endif

SMPlayer::ExitCode SMPlayer::processArgs(QStringList args) {
	qDebug("SMPlayer::processArgs: arguments: %d", args.count());
	for (int n = 0; n < args.count(); n++) {
		qDebug("SMPlayer::processArgs: %d = %s", n, args[n].toUtf8().data());
	}


	QString action; // Action to be passed to running instance
	bool show_help = false;

	if (!pref->gui.isEmpty()) gui_to_use = pref->gui;
	bool add_to_playlist = false;

#ifdef Q_OS_WIN
	if (args.contains("-uninstall")) {
		#if USE_ASSOCIATIONS
		//Called by uninstaller. Will restore old associations.
		WinFileAssoc RegAssoc; 
		Extensions exts; 
		QStringList regExts; 
		RegAssoc.GetRegisteredExtensions(exts.multimedia(), regExts); 
		RegAssoc.RestoreFileAssociations(regExts); 
		printf("Restored associations\n");
		#endif
		return NoError;
	}
#endif

	if (args.contains("-delete-config")) {
		CleanConfig::clean(Paths::configPath());
		return NoError;
	}

	for (int n = 1; n < args.count(); n++) {
		QString argument = args[n];

		if (argument == "-send-action") {
			if (n+1 < args.count()) {
				n++;
				action = args[n];
			} else {
				printf("Error: expected parameter for -send-action\r\n");
				return ErrorArgument;
			}
		}
		else
		if (argument == "-actions") {
			if (n+1 < args.count()) {
				n++;
				actions_list = args[n];
			} else {
				printf("Error: expected parameter for -actions\r\n");
				return ErrorArgument;
			}
		}
		else
		if (argument == "-sub") {
			if (n+1 < args.count()) {
				n++;
				QString file = args[n];
				if (QFile::exists(file)) {
					subtitle_file = QFileInfo(file).absoluteFilePath();
				} else {
					printf("Error: file '%s' doesn't exists\r\n", file.toUtf8().constData());
				}
			} else {
				printf("Error: expected parameter for -sub\r\n");
				return ErrorArgument;
			}
		}
		else
		if (argument == "-media-title") {
			if (n+1 < args.count()) {
				n++;
				if (media_title.isEmpty()) media_title = args[n];
			}
		}
		else
		if (argument == "-pos") {
			if (n+2 < args.count()) {
				bool ok_x, ok_y;
				n++;
				gui_position.setX( args[n].toInt(&ok_x) );
				n++;
				gui_position.setY( args[n].toInt(&ok_y) );
				if (ok_x && ok_y) move_gui = true;
			} else {
				printf("Error: expected parameter for -pos\r\n");
				return ErrorArgument;
			}
		}
		else
		if (argument == "-size") {
			if (n+2 < args.count()) {
				bool ok_width, ok_height;
				n++;
				gui_size.setWidth( args[n].toInt(&ok_width) );
				n++;
				gui_size.setHeight( args[n].toInt(&ok_height) );
				if (ok_width && ok_height) resize_gui = true;
			} else {
				printf("Error: expected parameter for -resize\r\n");
				return ErrorArgument;
			}
		}
		else
		if ((argument == "--help") || (argument == "-help") ||
            (argument == "-h") || (argument == "-?") ) 
		{
			show_help = true;
		}
		else
		if (argument == "-close-at-end") {
			close_at_end = 1;
		}
		else
		if (argument == "-no-close-at-end") {
			close_at_end = 0;
		}
		else
		if (argument == "-fullscreen") {
			start_in_fullscreen = 1;
		}
		else
		if (argument == "-no-fullscreen") {
			start_in_fullscreen = 0;
		}
		else
		if (argument == "-add-to-playlist") {
			add_to_playlist = true;
		}
		else
		if (argument == "-mini" || argument == "-minigui") {
			gui_to_use = "MiniGUI";
		}
		else
		if (argument == "-mpcgui") {
			gui_to_use = "MpcGUI";
		}
		else
		if (argument == "-defaultgui") {
			gui_to_use = "DefaultGUI";
		}
		else
		if (argument == "-ontop") {
			pref->stay_on_top = Preferences::AlwaysOnTop;
		}
		else
		if (argument == "-no-ontop") {
			pref->stay_on_top = Preferences::NeverOnTop;
		}
#ifdef SKINS
		else
		if (argument == "-skingui") {
			gui_to_use = "SkinGUI";
		}
#endif
		else {
			// File
			#if QT_VERSION >= 0x040600
			QUrl fUrl = QUrl::fromUserInput(argument);
			if (fUrl.isValid() && fUrl.scheme().toLower() == "file") {
				argument = fUrl.toLocalFile();
			}
			#endif
			if (QFile::exists( argument )) {
				argument = QFileInfo(argument).absoluteFilePath();
			}
			files_to_play.append( argument );
		}
	}

	if (show_help) {
		printf("%s\n", CLHelp::help().toLocal8Bit().data());
		return NoError;
	}

	qDebug("SMPlayer::processArgs: files_to_play: count: %d", files_to_play.count() );
	for (int n=0; n < files_to_play.count(); n++) {
		qDebug("SMPlayer::processArgs: files_to_play[%d]: '%s'", n, files_to_play[n].toUtf8().data());
	}

#ifdef SINGLE_INSTANCE
	if (pref->use_single_instance) {
		// Single instance
		MyApplication * a = MyApplication::instance();
		if (a->isRunning()) {
			a->sendMessage("Hello");

			if (!action.isEmpty()) {
				a->sendMessage("action " + action);
			}
			else {
				if (!subtitle_file.isEmpty()) {
					a->sendMessage("load_sub " + subtitle_file);
				}

				if (!media_title.isEmpty()) {
					a->sendMessage("media_title " + files_to_play[0] + " <<sep>> " + media_title);
				}

				if (!files_to_play.isEmpty()) {
					/* a->sendMessage("open_file " + files_to_play[0]); */
					QString command = "open_files";
					if (add_to_playlist) command = "add_to_playlist";
					a->sendMessage(command +" "+ files_to_play.join(" <<sep>> "));
				}
			}

			return NoError;
		}
	}
#endif

	/*
	if (!pref->default_font.isEmpty()) {
		QFont f;
		f.fromString(pref->default_font);
		qApp->setFont(f);
	}
	*/

	return SMPlayer::NoExit;
}

void SMPlayer::start() {
#ifdef FONTCACHE_DIALOG
#ifndef PORTABLE_APP
	if (Version::with_revision() != pref->smplayer_version) {
		FontCacheDialog d(0);
		d.run(pref->mplayer_bin, "sample.avi");
		pref->smplayer_version = Version::with_revision();
	}
#endif
#endif

	if (!gui()->startHidden() || !files_to_play.isEmpty() ) gui()->show();
	if (!files_to_play.isEmpty()) {
		if (!subtitle_file.isEmpty()) gui()->setInitialSubtitle(subtitle_file);
		if (!media_title.isEmpty()) gui()->getCore()->addForcedTitle(files_to_play[0], media_title);
		gui()->openFiles(files_to_play);
	}

	if (!actions_list.isEmpty()) {
		if (files_to_play.isEmpty()) {
			gui()->runActions(actions_list);
		} else {
			gui()->runActionsLater(actions_list);
		}
	}
}

#ifndef PORTABLE_APP
void SMPlayer::createConfigDirectory() {
	// Create smplayer config directory
	if (!QFile::exists(Paths::configPath())) {
		QDir d;
		if (!d.mkdir(Paths::configPath())) {
			qWarning("SMPlayer::createConfigDirectory: can't create %s", Paths::configPath().toUtf8().data());
		}
		// Screenshot folder already created in preferences.cpp if Qt >= 4.4
		#if QT_VERSION < 0x040400 
		QString s = Paths::configPath() + "/screenshots";
		if (!d.mkdir(s)) {
			qWarning("SMPlayer::createHomeDirectory: can't create %s", s.toUtf8().data());
		}
		#endif
	}
}
#endif

#ifdef FONTS_HACK
void SMPlayer::createFontFile() {
	qDebug("SMPlayer::createFontFile");
	QString output = Paths::configPath() + "/fonts.conf";

	// Check if the file already exists with the modified path
	if (QFile::exists(output)) {
		QFile i(output);
		if (i.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QString text = i.readAll();
			if (text.contains("<dir>" + Paths::fontPath() + "</dir>")) {
				qDebug("SMPlayer::createFontFile: file %s already exists with font path. Doing nothing.", output.toUtf8().constData());
				return;
			}
		}
	}

	QString input = Paths::appPath() + "/mplayer/fonts/fonts.conf";
	if (!QFile::exists(input)) {
		qDebug("SMPlayer::createFontFile: %s doesn't exist", input.toUtf8().constData());
		input = Paths::appPath() + "/mplayer/mpv/fonts.conf";
		if (!QFile::exists(input)) {
			qDebug("SMPlayer::createFontFile: %s doesn't exist", input.toUtf8().constData());
			qWarning("SMPlayer::createFontFile: failed to create fonts.conf");
			return;
		}
	}
	qDebug("SMPlayer::createFontFile: input: %s", input.toUtf8().constData());
	QFile infile(input);
	if (infile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QString text = infile.readAll();
		text = text.replace("<!-- <dir>WINDOWSFONTDIR</dir> -->", "<dir>WINDOWSFONTDIR</dir>");
		text = text.replace("<dir>WINDOWSFONTDIR</dir>", "<dir>" + Paths::fontPath() + "</dir>");
		//qDebug("SMPlayer::createFontFile: %s", text.toUtf8().constData());

		qDebug("SMPlayer::createFontFile: saving %s", output.toUtf8().constData());
		QFile outfile(output);
		if (outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			outfile.write(text.toUtf8());
			outfile.close();
		}
	}
}
#endif

void SMPlayer::showInfo() {
#ifdef Q_OS_WIN
	QString win_ver;
	switch (QSysInfo::WindowsVersion) {
		case QSysInfo::WV_2000: win_ver = "Windows 2000"; break;
		case QSysInfo::WV_XP: win_ver = "Windows XP"; break;
		case QSysInfo::WV_2003: win_ver = "Windows XP Professional x64/Server 2003"; break;
		case QSysInfo::WV_VISTA: win_ver = "Windows Vista/Server 2008"; break;
		#if QT_VERSION >= 0x040501
		case QSysInfo::WV_WINDOWS7: win_ver = "Windows 7/Server 2008 R2"; break;
		#endif
		#if QT_VERSION >= 0x040803
		case QSysInfo::WV_WINDOWS8: win_ver = "Windows 8/Server 2012"; break;
		#endif
		#if ((QT_VERSION >= 0x040806 && QT_VERSION < 0x050000) || (QT_VERSION >= 0x050200))
		case QSysInfo::WV_WINDOWS8_1: win_ver = "Windows 8.1/Server 2012 R2"; break;
		#endif
		#if ((QT_VERSION >= 0x040807 && QT_VERSION < 0x050000) || (QT_VERSION >= 0x050500))
		case QSysInfo::WV_WINDOWS10: win_ver = "Windows 10"; break;
		#endif
		case QSysInfo::WV_NT_based: win_ver = "NT-based Windows"; break;
		default: win_ver = QString("Unknown/Unsupported Windows OS"); break;
	}
#endif
	QString s = QObject::tr("This is SMPlayer v. %1 running on %2")
            .arg(Version::printable())
#ifdef Q_OS_LINUX
           .arg("Linux")
#else
#ifdef Q_OS_WIN
           .arg("Windows ("+win_ver+")")
#else
#ifdef Q_OS_OS2
           .arg("eCS (OS/2)")
#else
		   .arg("Other OS")
#endif
#endif
#endif
           ;

	printf("%s\n", s.toLocal8Bit().data() );
	qDebug("%s", s.toUtf8().data() );
	qDebug("Compiled with Qt v. %s, using %s", QT_VERSION_STR, qVersion());

	qDebug() << " * application path:" << Paths::appPath();
	qDebug() << " * data path:" << Paths::dataPath();
	qDebug() << " * translation path:" << Paths::translationPath();
	qDebug() << " * doc path:" << Paths::docPath();
	qDebug() << " * themes path:" << Paths::themesPath();
	qDebug() << " * shortcuts path:" << Paths::shortcutsPath();
	qDebug() << " * config path:" << Paths::configPath();
	qDebug() << " * ini path:" << Paths::iniPath();
	qDebug() << " * file for subtitles' styles:" << Paths::subtitleStyleFile();
	qDebug() << " * current path:" << QDir::currentPath();
#ifdef FONTS_HACK
	qDebug() << " * font path:" << Paths::fontPath();
#endif
}

#ifdef LOG_SMPLAYER
QFile SMPlayer::output_log;
bool SMPlayer::allow_to_send_log_to_gui = false;

#if QT_VERSION >= 0x050000
void SMPlayer::myMessageOutput( QtMsgType type, const QMessageLogContext &, const QString & msg ) {
#else
void SMPlayer::myMessageOutput( QtMsgType type, const char *msg ) {
#endif
	static QStringList saved_lines;
	static QString orig_line;
	static QString line2;
	static QRegExp rx_log;

	if (pref) {
		if (!pref->log_smplayer) return;
		rx_log.setPattern(pref->log_filter);
	} else {
		rx_log.setPattern(".*");
	}

	line2.clear();

#if QT_VERSION >= 0x050000
	orig_line = msg;
#else
	#ifdef Q_OS_WIN
	orig_line = QString::fromLocal8Bit(msg);
	#else
	orig_line = QString::fromUtf8(msg);
	#endif
#endif

	switch ( type ) {
		case QtDebugMsg:
			if (rx_log.indexIn(orig_line) > -1) {
				#ifndef NO_DEBUG_ON_CONSOLE
				fprintf( stderr, "Debug: %s\n", orig_line.toLocal8Bit().data() );
				#endif
				line2 = orig_line;
			}
			break;
		case QtWarningMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Warning: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			line2 = "WARNING: " + orig_line;
			break;
		case QtFatalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Fatal: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			line2 = "FATAL: " + orig_line;
			abort();                    // deliberately core dump
		case QtCriticalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Critical: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			line2 = "CRITICAL: " + orig_line;
			break;
		#if QT_VERSION >= 0x050500
		case QtInfoMsg:
			break;
		#endif
	}

	if (line2.isEmpty()) return;

	line2 = "["+ QTime::currentTime().toString("hh:mm:ss:zzz") +"] "+ line2;

	if (allow_to_send_log_to_gui && main_window) {
		if (!saved_lines.isEmpty()) {
			// Send saved lines first
			for (int n=0; n < saved_lines.count(); n++) {
				main_window->recordSmplayerLog(saved_lines[n]);
			}
			saved_lines.clear();
		}
		main_window->recordSmplayerLog(line2);
	} else {
		// GUI is not created yet, save lines for later
		saved_lines.append(line2);
		/* printf("SMPlayer::myMessageOutput: no gui\n"); */
	}

	if (pref) {
		if (pref->save_smplayer_log) {
			// Save log to file
			if (!output_log.isOpen()) {
				// FIXME: the config path may not be initialized if USE_LOCKS is not defined
				output_log.setFileName( Paths::configPath() + "/smplayer_log.txt" );
				output_log.open(QIODevice::WriteOnly);
			}
			if (output_log.isOpen()) {
				QString l = line2 + "\r\n";
				output_log.write(l.toUtf8().constData());
				output_log.flush();
			}
		}
	}
}
#endif

/*
void myMessageOutput( QtMsgType type, const char *msg ) {
	static QString orig_line;
	orig_line = QString::fromUtf8(msg);

	switch ( type ) {
		case QtDebugMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Debug: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			break;

		case QtWarningMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Warning: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			break;

		case QtCriticalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Critical: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			break;

		case QtFatalMsg:
			#ifndef NO_DEBUG_ON_CONSOLE
			fprintf( stderr, "Fatal: %s\n", orig_line.toLocal8Bit().data() );
			#endif
			abort();                    // deliberately core dump
	}
}
*/

#include "moc_smplayer.cpp"
