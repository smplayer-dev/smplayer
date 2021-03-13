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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "ui_preferencesdialog.h"

/*
#ifdef Q_OS_WIN
#define USE_ASSOCIATIONS 1
#endif
*/

class InfoWindow;
class QPushButton;

class PrefWidget;
class PrefGeneral;
class PrefDrives;
class PrefPerformance;
class PrefSubtitles;
class PrefInterface;
class PrefInput;
class PrefAdvanced;
class PrefPlaylist;
class PrefTV;
class PrefUpdates;
class PrefNetwork;
class PrefAssociations;

class Preferences;


class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
	Q_OBJECT
	Q_PROPERTY(bool iconMode READ isIconMode WRITE setIconMode)

public:
	enum Section { General=0, Drives=1, Performance=2,
                   Subtitles=3, Gui=4, Mouse=5, Advanced=6, Associations=7 };

	PreferencesDialog( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PreferencesDialog();

	PrefGeneral * mod_general() { return page_general; };
	PrefInterface * mod_interface() { return page_interface; };
	PrefInput * mod_input() { return page_input; };
	PrefAdvanced * mod_advanced() { return page_advanced; };
	PrefPlaylist * mod_playlist() { return page_playlist; };
	PrefUpdates * mod_updates() { return page_updates; };
	PrefNetwork * mod_network() { return page_network; };

	void addSection(PrefWidget *w);

	// Pass data to the standard dialogs
	void setData(Preferences * pref);

	// Apply changes
	void getData(Preferences * pref);

	// Return true if the mplayer process should be restarted.
	bool requiresRestart();

	void setIconMode(bool);
	bool isIconMode() { return icon_mode; };

public slots:
	void showSection(Section s);

	virtual void accept(); // Reimplemented to send a signal
	virtual void reject();

signals:
	void applied();

protected:
	virtual void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;

protected slots:
	void apply();
	void showHelp();

protected:
	PrefGeneral * page_general;
	PrefDrives * page_drives;
	PrefPerformance * page_performance;
	PrefSubtitles * page_subtitles;
	PrefInterface * page_interface;
	PrefInput * page_input;
	PrefPlaylist * page_playlist;
#ifdef TV_SUPPORT
	PrefTV * page_tv;
#endif
	PrefUpdates * page_updates;
	PrefNetwork * page_network;
	PrefAdvanced * page_advanced;

#if USE_ASSOCIATIONS
	PrefAssociations* page_associations;
#endif

	InfoWindow * help_window;

private:
	QPushButton * okButton;
	QPushButton * cancelButton;
	QPushButton * applyButton;
	QPushButton * helpButton;

	bool icon_mode;
};

#endif
