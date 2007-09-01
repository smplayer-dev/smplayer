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

#include "baseguiplus.h"
#include "myaction.h"
#include "global.h"
#include "images.h"
#include "playlist.h"

#include <QMenu>
#include <QCloseEvent>

#if DOCK_PLAYLIST
#include <QDockWidget>
#include "playlistdock.h"
#include "desktopinfo.h"
#endif


BaseGuiPlus::BaseGuiPlus( QWidget * parent, Qt::WindowFlags flags )
	: BaseGui( parent, flags ),
		mainwindow_visible(true),
		//infowindow_visible(false),
		trayicon_playlist_was_visible(false)
{
	mainwindow_pos = pos();

	tray = new QSystemTrayIcon( Images::icon("logo", 22), this );

	tray->setToolTip( "SMPlayer" );
	connect( tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), 
             this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

	quitAct = new MyAction(this, "quit");
    connect( quitAct, SIGNAL(activated()), this, SLOT(quit()) );
	openMenu->addAction(quitAct);

	showTrayAct = new MyAction(this, "show_tray_icon" );
	showTrayAct->setCheckable(true);
	connect( showTrayAct, SIGNAL(toggled(bool)),
             tray, SLOT(setVisible(bool)) );
	optionsMenu->addAction(showTrayAct);

	showAllAct = new MyAction(this, "restore/hide");
	connect( showAllAct, SIGNAL(activated()),
             this, SLOT(toggleShowAll()) );

	context_menu = new QMenu(this);
	context_menu->addAction(showAllAct);
	context_menu->addSeparator();
	context_menu->addAction(openFileAct);
	context_menu->addMenu(recentfiles_menu);
	context_menu->addAction(openDVDAct);
	context_menu->addAction(openURLAct);
	context_menu->addSeparator();
	context_menu->addAction(playOrPauseAct);
	context_menu->addAction(stopAct);
	context_menu->addSeparator();
	context_menu->addAction(playPrevAct);
	context_menu->addAction(playNextAct);
	context_menu->addSeparator();
	context_menu->addAction(showPlaylistAct);
	context_menu->addAction(showPreferencesAct);
	context_menu->addSeparator();
	context_menu->addAction(quitAct);
	
	tray->setContextMenu( context_menu );

#if DOCK_PLAYLIST
	// Playlistdock
	playlistdock = new PlaylistDock(this);
	playlistdock->setWidget(playlist);
	playlistdock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, playlistdock);
	playlistdock->hide();

	connect( playlistdock, SIGNAL(dockClosed()), this, SLOT(playlistClosed()) );
#endif

	retranslateStrings();

    loadConfig();
}

BaseGuiPlus::~BaseGuiPlus() {
	saveConfig();
}

bool BaseGuiPlus::startHidden() {
	if ( (!showTrayAct->isChecked()) || (mainwindow_visible) ) 
		return false;
	else
		return true;
}

void BaseGuiPlus::closeEvent( QCloseEvent * e ) {
	qDebug("BaseGuiPlus::closeEvent");
	e->ignore();
	closeWindow();
}

void BaseGuiPlus::closeWindow() {
	qDebug("BaseGuiPlus::closeWindow");

	if (tray->isVisible()) {
		//e->ignore();
		exitFullscreen();
		showAll(false); // Hide windows
		if (core->state() == Core::Playing) core->stop();

		if (pref->balloon_count > 0) {
			tray->showMessage( "SMPlayer", 
				tr("SMPlayer is still running here"), 
        	    QSystemTrayIcon::Information, 3000 );
			pref->balloon_count--;
		}

	} else {
		BaseGui::closeWindow();
	}
	//tray->hide();

}

void BaseGuiPlus::quit() {
	qDebug("BaseGuiPlus::quit");
	BaseGui::closeWindow();
}

void BaseGuiPlus::retranslateStrings() {
	BaseGui::retranslateStrings();

	quitAct->change( Images::icon("exit"), tr("&Quit") );
	showTrayAct->change( Images::icon("systray"), tr("S&how icon in system tray") );

	updateShowAllAct();

#if DOCK_PLAYLIST
    playlistdock->setCaption( tr("Playlist") );
#endif
}

void BaseGuiPlus::updateShowAllAct() {
	if (isVisible()) 
		showAllAct->change( tr("&Hide") );
	else
		showAllAct->change( tr("&Restore") );
}

void BaseGuiPlus::saveConfig() {
	qDebug("BaseGuiPlus::saveConfig");

	QSettings * set = settings;

	set->beginGroup( "base_gui_plus");

	set->setValue( "show_tray_icon", showTrayAct->isChecked() );
	set->setValue( "mainwindow_visible", isVisible() );

	set->endGroup();
}

void BaseGuiPlus::loadConfig() {
	qDebug("BaseGuiPlus::loadConfig");

	QSettings * set = settings;

	set->beginGroup( "base_gui_plus");

	bool show_tray_icon = set->value( "show_tray_icon", false).toBool();
	showTrayAct->setChecked( show_tray_icon );
	//tray->setVisible( show_tray_icon );

	mainwindow_visible = set->value("mainwindow_visible", true).toBool();

	set->endGroup();

	updateShowAllAct();
}


void BaseGuiPlus::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
	qDebug("DefaultGui::trayIconActivated: %d", reason);

	updateShowAllAct();

	if (reason == QSystemTrayIcon::Trigger) {
		toggleShowAll();
	}
	else
	if (reason == QSystemTrayIcon::MiddleClick) {
		core->pause();
	}
}

void BaseGuiPlus::toggleShowAll() {
	showAll( !isVisible() );
}

void BaseGuiPlus::showAll(bool b) {
	if (!b) {
		// Hide all
#if DOCK_PLAYLIST
		trayicon_playlist_was_visible = (playlistdock->isVisible() && 
                                         playlistdock->isFloating() );
		if (trayicon_playlist_was_visible)
			playlistdock->hide();

		/*
		trayicon_playlist_was_visible = playlistdock->isVisible();
		playlistdock->hide();
		*/
#else
		trayicon_playlist_was_visible = playlist->isVisible();
		playlist_pos = playlist->pos();
		playlist->hide();
#endif

		mainwindow_pos = pos();
		hide();

		/*
		infowindow_visible = info_window->isVisible();
		infowindow_pos = info_window->pos();
		info_window->hide();
		*/
	} else {
		// Show all
		move(mainwindow_pos);
		show();

#if DOCK_PLAYLIST
		if (trayicon_playlist_was_visible) {
			playlistdock->show();
		}
#else
		if (trayicon_playlist_was_visible) {
			playlist->move(playlist_pos);
			playlist->show();
		}
#endif

		/*
		if (infowindow_visible) {
			info_window->show();
			info_window->move(infowindow_pos);
		}
		*/
	}
	updateShowAllAct();
}

void BaseGuiPlus::resizeWindow(int w, int h) {
    qDebug("BaseGuiPlus::resizeWindow: %d, %d", w, h);

	if ( (tray->isVisible()) && (!isVisible()) ) showAll(true);

	BaseGui::resizeWindow(w, h );
}

void BaseGuiPlus::updateMediaInfo() {
    qDebug("BaseGuiPlus::updateMediaInfo");
	BaseGui::updateMediaInfo();

	tray->setToolTip( windowTitle() );
}

void BaseGuiPlus::setWindowCaption(const QString & title) {
	tray->setToolTip( title );

	BaseGui::setWindowCaption( title );
}


// Playlist stuff
void BaseGuiPlus::aboutToEnterFullscreen() {
    qDebug("BaseGuiPlus::aboutToEnterFullscreen");

	BaseGui::aboutToEnterFullscreen();

#if DOCK_PLAYLIST
	fullscreen_playlist_was_visible = playlistdock->isVisible();
	playlistdock->hide();
#endif
}

void BaseGuiPlus::aboutToExitFullscreen() {
	qDebug("BaseGuiPlus::aboutToExitFullscreen");

	BaseGui::aboutToExitFullscreen();

#if DOCK_PLAYLIST
	if (fullscreen_playlist_was_visible)
		playlistdock->show();
#endif
}

void BaseGuiPlus::aboutToEnterCompactMode() {
	BaseGui::aboutToEnterCompactMode();

#if DOCK_PLAYLIST
	compact_playlist_was_visible = (playlistdock->isVisible() && 
                                    !playlistdock->isFloating());
	if (compact_playlist_was_visible)
		playlistdock->hide();
#endif
}

void BaseGuiPlus::aboutToExitCompactMode() {
	BaseGui::aboutToExitCompactMode();

#if DOCK_PLAYLIST
	if (compact_playlist_was_visible)
		playlistdock->show();
#endif
}

#if DOCK_PLAYLIST
void BaseGuiPlus::showPlaylist(bool b) {
	if ( !b ) {
		playlistdock->hide();
	} else {
		exitFullscreenIfNeeded();
		playlistdock->show();
	}
	//updateWidgets();
}

void BaseGuiPlus::playlistClosed() {
	showPlaylistAct->setChecked(false);
}

#endif

#include "moc_baseguiplus.cpp"
