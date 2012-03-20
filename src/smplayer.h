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

#ifndef _SMPLAYER_H_
#define _SMPLAYER_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include "basegui.h"

class SMPlayer : public QObject
{
	Q_OBJECT

public:
	enum ExitCode { ErrorArgument = -3, NoAction = -2, NoRunningInstance = -1, NoError = 0, NoExit = 1 };

	SMPlayer(const QString & config_path = QString::null, QObject * parent = 0);
	~SMPlayer();

	//! Process arguments. If ExitCode != NoExit the application must be exited.
	ExitCode processArgs(QStringList args);

	BaseGui * gui();

	void start();

#ifdef GUI_CHANGE_ON_RUNTIME
private slots:
	void changeGUI(QString new_gui);
#endif

private:
	BaseGui * createGUI(QString gui_name);
	void deleteGUI();
#ifndef PORTABLE_APP
	void createConfigDirectory();
#endif
	void showInfo();

	static BaseGui * main_window;

	QStringList files_to_play;
	QString subtitle_file;
	QString actions_list; //!< Actions to be run on startup
	QString gui_to_use;

	// Change position and size
	bool move_gui;
	QPoint gui_position;

	bool resize_gui;
	QSize gui_size;

	// Options to pass to gui
	int close_at_end; // -1 = not set, 1 = true, 0 false
	int start_in_fullscreen; // -1 = not set, 1 = true, 0 false

#ifdef LOG_SMPLAYER
	// Output log
	static QFile output_log;
	static void myMessageOutput( QtMsgType type, const char *msg );
	static bool allow_to_send_log_to_gui;
#endif
};

#endif
