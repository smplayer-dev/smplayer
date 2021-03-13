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

#include "clhelp.h"
#include <QObject>
#include <QApplication>
#include <QFileInfo>

QString CLHelp::formatText(QString s, int col) {
	QString res = "";

	int last = 0;
	int pos;

	pos = s.indexOf(" ");
	while (pos != -1) {

		if (s.count() < col) {
			res = res + s;
			s = "";
			break;
		}

		while ((pos < col) && (pos != -1)) {
			last = pos;
			pos = s.indexOf(" ", pos+1);
		}

		res = res + s.left(last) + "\n";
		s = s.mid(last+1);

		last = 0;
		pos = s.indexOf(" ");

	}

	if (!s.isEmpty()) res = res + s;

	return res;
}

QString CLHelp::formatHelp(QString parameter, QString help, bool html) {
	if (html) {
		return "<tr><td><b>"+parameter+"</b></td><td>"+help+"</td></tr>";
	} else {
		int par_width = 20;
		int help_width = 80 - par_width;

		QString s;
		s = s.fill( ' ', par_width - (parameter.count()+2) );
		s = s + parameter + ": ";

		QString f;
		f = f.fill(' ', par_width);

		QString s2 = formatText(help, help_width);
		int pos = s2.indexOf('\n');
		while (pos != -1) {
			s2 = s2.insert(pos+1, f);
			pos = s2.indexOf('\n', pos+1);
		}

		return s + s2 + "\n";
	}
}


QString CLHelp::help(bool html) {
	QString app_name = QFileInfo(qApp->applicationFilePath()).baseName();

	QString options = QString("%1 [-minigui] [-defaultgui] [-mpcgui] [-config-path %2] "
                        "[-send-action %3] [-actions %4] "
                        "[-close-at-end] [-no-close-at-end] [-fullscreen] [-no-fullscreen] "
                        "[-ontop] [-no-ontop] "
                        "[-sub %5] [-pos x y] [-size %6 %7] "
                        "[-start %9] "
                        "[-add-to-playlist] [-help|--help|-h|-?] "
                        "[%8] [%8]...")
                        .arg(app_name)
                        .arg(QObject::tr("directory"))
                        .arg(QObject::tr("action_name"))
                        .arg(QObject::tr("action_list"))
                        .arg(QObject::tr("subtitle_file"))
                        .arg(QObject::tr("width")).arg(QObject::tr("height"))
                        .arg(QObject::tr("media"))
                        .arg(QObject::tr("time"));

	QString s;

	if (html) {
		s = QObject::tr("Usage:") + " <b>" + options + "</b><br>";
		s += "<table>";
	} else {
		s = formatText(QObject::tr("Usage:") + " " + options, 80);
		s += "\n\n";
	}

#ifdef Q_OS_WIN	
	s += formatHelp( "-uninstall", QObject::tr(
		"Restores the old associations and cleans up the registry."), html );
#endif
	s += formatHelp( "-minigui", QObject::tr(
		"opens the mini gui instead of the default one."), html );

	s += formatHelp( "-mpcgui", QObject::tr(
		"opens the mpc gui."), html );

	s += formatHelp( "-defaultgui", QObject::tr(
		"opens the default gui."), html );

#ifdef SKINS
	s += formatHelp( "-skingui", QObject::tr(
		"opens the gui with support for skins."), html );
#endif

	s += formatHelp( "-config-path", QObject::tr(
		"specifies the directory where smplayer will store its configuration "
        "files (smplayer.ini, smplayer_files.ini...)"), html );

	s += formatHelp( "-send-action", QObject::tr(
		"tries to make a connection to another running instance "
        "and send to it the specified action. Example: -send-action pause "
        "The rest of options (if any) will be ignored and the "
        "application will exit. It will return 0 on success or -1 "
        "on failure."), html );

	s += formatHelp( "-actions", QObject::tr(
		"action_list is a list of actions separated by spaces. "
		"The actions will be executed just after loading the file (if any) "
		"in the same order you entered. For checkable actions you can pass "
		"true or false as parameter. Example: "
		"-actions \"fullscreen compact true\". Quotes are necessary in "
		"case you pass more than one action."), html );

	s += formatHelp( "-close-at-end", QObject::tr(
		"the main window will be closed when the file/playlist finishes."), html );

	s += formatHelp( "-no-close-at-end", QObject::tr(
		"the main window won't be closed when the file/playlist finishes."), html );

	s += formatHelp( "-fullscreen", QObject::tr(
		"the video will be played in fullscreen mode."), html );

	s += formatHelp( "-no-fullscreen", QObject::tr(
		"the video will be played in window mode."), html );

	s += formatHelp( "-ontop", QObject::tr(
		"sets the stay on top option to always."), html );

	s += formatHelp( "-no-ontop", QObject::tr(
		"sets the stay on top option to never."), html );

	s += formatHelp( "-sub", QObject::tr(
		"specifies the subtitle file to be loaded for the first video."), html );

	s += formatHelp( "-media-title", QObject::tr(
		"sets the media title for the first video."), html );

	s += formatHelp( "-pos", QObject::tr(
		"specifies the coordinates where the main window will be displayed."), html );

	s += formatHelp( "-size", QObject::tr(
		"specifies the size of the main window."), html );

	s += formatHelp( "-start", QObject::tr(
		"specifies the start time (in seconds) of the first file to be played. Also valid h:m:s and m:s"), html );

	s += formatHelp( "-help", QObject::tr(
		"will show this message and then will exit."), html );

	s += formatHelp( "-add-to-playlist", QObject::tr(
		"if there's another instance running, the media will be added "
        "to that instance's playlist. If there's no other instance, "
        "this option will be ignored and the "
        "files will be opened in a new instance."), html );

	s += formatHelp( QObject::tr("media"), QObject::tr(
		"'media' is any kind of file that SMPlayer can open. It can "
        "be a local file, a DVD (e.g. dvd://1), an Internet stream "
        "(e.g. mms://....) or a local playlist in format m3u or pls."), html );

	if (html) s += "</table>";

	return s;
}
