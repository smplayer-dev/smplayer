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

#include "baseguiplus.h"
#include "config.h"
#include "myaction.h"
#include "global.h"
#include "images.h"
#include "playlist.h"

#ifdef TV_SUPPORT
#include "tvlist.h"
#else
#include "favorites.h"
#endif

#include "widgetactions.h"
#include "myactiongroup.h"

#include <QMenu>
#include <QCloseEvent>
#include <QApplication>
#include <QDesktopWidget>

#if DOCK_PLAYLIST
#include <QDockWidget>
#include "playlistdock.h"
#include "desktopinfo.h"

#define PLAYLIST_ON_SIDES 1
#endif

#ifdef SCREENS_SUPPORT
#include <QVBoxLayout>
#include <QLabel>
#include "mplayerwindow.h"
#include "infowindow.h"

#if QT_VERSION >= 0x050000
#include <QScreen>
#include <QWindow>
#endif
#endif

#ifdef GLOBALSHORTCUTS
#include "globalshortcuts/globalshortcuts.h"
#include "preferencesdialog.h"
#include "prefinput.h"
#endif

#ifdef CHROMECAST_SUPPORT
#include "chromecast.h"
#endif

using namespace Global;

BaseGuiPlus::BaseGuiPlus( QWidget * parent, Qt::WindowFlags flags)
	: BaseGui( parent, flags )
#ifdef USE_SYSTRAY
	, context_menu(0)
#endif
#ifdef SCREENS_SUPPORT
	, screens_info_window(0)
	, detached_label(0)
#endif
	, mainwindow_visible(true)
#ifdef USE_SYSTRAY
	, trayicon_playlist_was_visible(false)
#endif
	, widgets_size(0)
#if DOCK_PLAYLIST
	, fullscreen_playlist_was_visible(false)
	, fullscreen_playlist_was_floating(false)
	, compact_playlist_was_visible(false)
	, ignore_playlist_events(false)
#endif
{
	// Initialize variables
	//infowindow_visible = false;
	mainwindow_pos = pos();

	quitAct = new MyAction(QKeySequence("Ctrl+Q"), this, "quit");
	connect( quitAct, SIGNAL(triggered()), this, SLOT(quit()) );

#ifdef USE_SYSTRAY
	tray = new QSystemTrayIcon(this);

	tray->setToolTip( "SMPlayer" );
	connect( tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), 
             this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

	showTrayAct = new MyAction(this, "show_tray_icon" );
	showTrayAct->setCheckable(true);
	connect( showTrayAct, SIGNAL(toggled(bool)),
             tray, SLOT(setVisible(bool)) );

	showAllAct = new MyAction(this, "restore/hide");
	connect( showAllAct, SIGNAL(triggered()),
             this, SLOT(toggleShowAll()) );

	// The systray menu will be initialized later in
	// the function populateMainMenu
#endif

#if DOCK_PLAYLIST
	// Playlistdock
	playlistdock = new PlaylistDock(this);
	playlistdock->setObjectName("playlistdock");
	playlistdock->setFloating(false); // To avoid that the playlist is visible for a moment
	playlistdock->setWidget(playlist);
	playlistdock->setWindowTitle(playlist->windowTitle());
	playlistdock->setAllowedAreas(Qt::TopDockWidgetArea | 
                                  Qt::BottomDockWidgetArea
#if PLAYLIST_ON_SIDES
                                  | Qt::LeftDockWidgetArea | 
                                  Qt::RightDockWidgetArea
#endif
                                  );
	addDockWidget(Qt::BottomDockWidgetArea, playlistdock);
	playlistdock->hide();
	playlistdock->setFloating(true); // Floating by default

	connect( playlistdock, SIGNAL(closed()), this, SLOT(playlistClosed()) );
	connect(playlist, SIGNAL(windowTitleChanged(const QString &)), playlistdock, SLOT(setWindowTitle(const QString &)));
#if USE_DOCK_TOPLEVEL_EVENT
	connect( playlistdock, SIGNAL(topLevelChanged(bool)), 
             this, SLOT(dockTopLevelChanged(bool)) );
#else
	connect( playlistdock, SIGNAL(visibilityChanged(bool)), 
             this, SLOT(dockVisibilityChanged(bool)) );
#endif // USE_DOCK_TOPLEVEL_EVENT

	connect(this, SIGNAL(openFileRequested()), this, SLOT(showAll()));

	ignore_playlist_events = false;
#endif // DOCK_PLAYLIST

#ifdef DETACH_VIDEO_OPTION
	detachVideoAct = new MyAction(this, "detach_video");
	detachVideoAct->setCheckable(true);
	connect(detachVideoAct, SIGNAL(toggled(bool)), this, SLOT(detachVideo(bool)));
#endif

#ifdef CHROMECAST_SUPPORT
	playOnChromecastAct = new MyAction(this, "play_on_chromecast");
	connect(playOnChromecastAct, SIGNAL(triggered()), this, SLOT(playOnChromecast()));
#endif

#ifdef SCREENS_SUPPORT
	sendToScreen_menu = new QMenu(this);
	sendToScreen_menu->menuAction()->setObjectName("send_to_screen_menu");

	sendToScreenGroup = new MyActionGroup(this);
	connect(sendToScreenGroup, SIGNAL(activated(int)), this, SLOT(sendVideoToScreen(int)));

	updateSendToScreen();

	showScreensInfoAct = new MyAction(this, "screens_info");
	connect(showScreensInfoAct, SIGNAL(triggered()), this, SLOT(showScreensInfo()));

	#if QT_VERSION >= 0x040600 && QT_VERSION < 0x050000
	connect(qApp->desktop(), SIGNAL(screenCountChanged(int)), this, SLOT(updateSendToScreen()));
	#endif
	#if QT_VERSION >= 0x050000
	connect(qApp, SIGNAL(screenAdded(QScreen *)), this, SLOT(updateSendToScreen()));
	#endif
	#if QT_VERSION >= 0x050400
	connect(qApp, SIGNAL(screenRemoved(QScreen *)), this, SLOT(updateSendToScreen()));
	#endif
	#if QT_VERSION >= 0x050600
	connect(qApp, SIGNAL(primaryScreenChanged(QScreen *)), this, SLOT(updateSendToScreen()));
	#endif

	// Label that replaces the mplayerwindow when detached
	detached_label = new QLabel(panel);
	detached_label->setAlignment(Qt::AlignCenter);
	panel->layout()->addWidget(detached_label);
	detached_label->hide();
#endif

#ifdef SEND_AUDIO_OPTION
	sendAudio_menu = new QMenu(this);
	sendAudio_menu->menuAction()->setObjectName("send_audio_menu");
	updateSendAudioMenu();
#endif

#ifdef GLOBALSHORTCUTS
	global_shortcuts = new GlobalShortcuts(this);
	global_shortcuts->setGrabbedKeys((GlobalShortcuts::MediaKeys) pref->global_shortcuts_grabbed_keys);
	global_shortcuts->setEnabled(pref->use_global_shortcuts);
	connect(this, SIGNAL(preferencesChanged()), this, SLOT(updateGlobalShortcuts()));
#endif

#ifdef CHROMECAST_SUPPORT
	Chromecast::instance()->setSettings(settings);
#endif

#ifdef CHECK_SHORTCUT_EVENTS
	installFilterOnActions();
#endif

#if DOCK_PLAYLIST
	QTimer::singleShot(200, this, SLOT(updateShortcutsContext()));
#endif

	retranslateStrings();
	loadConfig();
}

BaseGuiPlus::~BaseGuiPlus() {
	saveConfig();

#ifdef USE_SYSTRAY
	tray->hide();
#endif

#ifdef CHROMECAST_SUPPORT
	Chromecast::deleteInstance();
#endif
}

void BaseGuiPlus::populateMainMenu() {
	qDebug("BaseGuiPlus::populateMainMenu");

	BaseGui::populateMainMenu();

	if (!pref->tablet_mode) {
		openMenu->addAction(quitAct);
		#ifdef USE_SYSTRAY
		optionsMenu->addAction(showTrayAct);
		#endif
	}

#ifdef DETACH_VIDEO_OPTION
	optionsMenu->addAction(detachVideoAct);
#endif

#ifdef SCREENS_SUPPORT
	videoMenu->insertMenu(videosize_menu->menuAction(), sendToScreen_menu);

	if (!pref->tablet_mode) {
		viewMenu->addSeparator();
		viewMenu->addAction(showScreensInfoAct);
	}

	access_menu->insertMenu(tabletModeAct, sendToScreen_menu);
#endif

#ifdef SEND_AUDIO_OPTION
	audioMenu->insertMenu(audiofilter_menu->menuAction(), sendAudio_menu);
#endif

#ifdef CHROMECAST_SUPPORT
	playMenu->addSeparator();
	playMenu->addAction(playOnChromecastAct);
#endif

#ifdef USE_SYSTRAY
	if (context_menu == 0) initializeSystrayMenu();
#endif
}

bool BaseGuiPlus::startHidden() {
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	return false;
#else
	#ifdef USE_SYSTRAY
	if (!showTrayAct->isChecked() || mainwindow_visible)
	#else
	if (mainwindow_visible)
	#endif
		return false;
	else
		return true;
#endif
}

void BaseGuiPlus::closeEvent( QCloseEvent * e ) {
	qDebug("BaseGuiPlus::closeEvent");
	e->ignore();
	closeWindow();
}

void BaseGuiPlus::updateWidgets() {
	qDebug("BaseGuiPlus::updateWidgets");

	BaseGui::updateWidgets();

#ifdef CHROMECAST_SUPPORT
	playOnChromecastAct->setEnabled(!core->mdat.filename.isEmpty());
#endif
}

void BaseGuiPlus::closeWindow() {
	qDebug("BaseGuiPlus::closeWindow");

#ifndef USE_SYSTRAY
	BaseGui::closeWindow();
#else
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
#endif
}

void BaseGuiPlus::quit() {
	qDebug("BaseGuiPlus::quit");
	BaseGui::closeWindow();
}

void BaseGuiPlus::retranslateStrings() {
	BaseGui::retranslateStrings();

	quitAct->change( Images::icon("exit"), tr("&Quit") );

#ifdef USE_SYSTRAY
	tray->setIcon(Images::icon("logo", 22));
	showTrayAct->change( Images::icon("systray"), tr("S&how icon in system tray") );
	updateShowAllAct();
#endif

#if DOCK_PLAYLIST
	// playlistdock->setWindowTitle( tr("Playlist") );
#endif

#ifdef DETACH_VIDEO_OPTION
	detachVideoAct->change("Detach video");
#endif

#ifdef CHROMECAST_SUPPORT
	playOnChromecastAct->change(Images::icon("chromecast"), tr("Play on &Chromecast"));
#endif

#ifdef SCREENS_SUPPORT
	sendToScreen_menu->menuAction()->setText( tr("Send &video to screen") );
	sendToScreen_menu->menuAction()->setIcon(Images::icon("send_to_screen"));
	showScreensInfoAct->change(tr("Information about connected &screens"));

	detached_label->setText("<img src=\"" + Images::file("send_to_screen") + "\">" +
		"<p style=\"color: white;\">" + tr("Video is sent to an external screen") +"</p");
#endif

#ifdef SEND_AUDIO_OPTION
	sendAudio_menu->menuAction()->setText( tr("Send &audio to") );
	sendAudio_menu->menuAction()->setIcon(Images::icon("send_audio"));
#endif
}

#ifdef USE_SYSTRAY
void BaseGuiPlus::initializeSystrayMenu() {
	qDebug("BaseGuiPlus::initializeSystrayMenu");
	context_menu = new QMenu(this);
	context_menu->addAction(showAllAct);
	context_menu->addSeparator();
	context_menu->addAction(openFileAct);
	context_menu->addMenu(recentfiles_menu);
	context_menu->addAction(openDirectoryAct);
	context_menu->addAction(openDVDAct);
	context_menu->addAction(openURLAct);
	context_menu->addMenu(favorites);
#if defined(TV_SUPPORT) && !defined(Q_OS_WIN)
	context_menu->addMenu(tvlist);
	context_menu->addMenu(radiolist);
#endif
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

	tray->setContextMenu(context_menu);
}

void BaseGuiPlus::updateShowAllAct() {
	if (isVisible()) 
		showAllAct->change( tr("&Hide") );
	else
		showAllAct->change( tr("&Restore") );
}
#endif

void BaseGuiPlus::saveConfig() {
	qDebug("BaseGuiPlus::saveConfig");

	QSettings * set = settings;

	set->beginGroup( "base_gui_plus");

#ifdef USE_SYSTRAY
	set->setValue( "show_tray_icon", showTrayAct->isChecked() );
	set->setValue( "trayicon_playlist_was_visible", trayicon_playlist_was_visible );
#endif
	set->setValue( "mainwindow_visible", isVisible() );

	set->setValue( "widgets_size", widgets_size );
#if DOCK_PLAYLIST
	set->setValue( "fullscreen_playlist_was_visible", fullscreen_playlist_was_visible );
	set->setValue( "fullscreen_playlist_was_floating", fullscreen_playlist_was_floating );
	set->setValue( "compact_playlist_was_visible", compact_playlist_was_visible );
	set->setValue( "ignore_playlist_events", ignore_playlist_events );
#endif

/*
#if DOCK_PLAYLIST
	set->setValue( "playlist_and_toolbars_state", saveState() );
#endif
*/

	set->endGroup();
}

void BaseGuiPlus::loadConfig() {
	qDebug("BaseGuiPlus::loadConfig");

	QSettings * set = settings;

	set->beginGroup( "base_gui_plus");

#ifdef USE_SYSTRAY
	bool show_tray_icon = set->value( "show_tray_icon", false).toBool();
	showTrayAct->setChecked( show_tray_icon );
	//tray->setVisible( show_tray_icon );
	trayicon_playlist_was_visible = set->value( "trayicon_playlist_was_visible", trayicon_playlist_was_visible ).toBool();
#endif

	mainwindow_visible = set->value("mainwindow_visible", true).toBool();

	widgets_size = set->value( "widgets_size", widgets_size ).toInt();
#if DOCK_PLAYLIST
	fullscreen_playlist_was_visible = set->value( "fullscreen_playlist_was_visible", fullscreen_playlist_was_visible ).toBool();
	fullscreen_playlist_was_floating = set->value( "fullscreen_playlist_was_floating", fullscreen_playlist_was_floating ).toBool();
	compact_playlist_was_visible = set->value( "compact_playlist_was_visible", compact_playlist_was_visible ).toBool();
	ignore_playlist_events = set->value( "ignore_playlist_events", ignore_playlist_events ).toBool();
#endif

/*
#if DOCK_PLAYLIST
	restoreState( set->value( "playlist_and_toolbars_state" ).toByteArray() );
#endif
*/

	set->endGroup();

#ifdef USE_SYSTRAY
	updateShowAllAct();
#endif
}

#ifdef USE_SYSTRAY
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
	// Ignore if tray is not visible
	if (tray->isVisible()) {
		showAll( !isVisible() );
	}
}

void BaseGuiPlus::showAll() {
	if (!isVisible()) showAll(true);
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
#endif

void BaseGuiPlus::resizeWindow(int w, int h) {
    qDebug("BaseGuiPlus::resizeWindow: %d, %d", w, h);

#ifdef USE_SYSTRAY
	if ( (tray->isVisible()) && (!isVisible()) ) showAll(true);
#endif

	BaseGui::resizeWindow(w, h );
}

void BaseGuiPlus::updateMediaInfo() {
	qDebug("BaseGuiPlus::updateMediaInfo");
	BaseGui::updateMediaInfo();

#ifdef USE_SYSTRAY
	tray->setToolTip( windowTitle() );
#endif
}

#ifdef FIX_DOCKPLAYLIST_STYLE
void BaseGuiPlus::applyStyles() {
	static bool initialized = false;
	bool playlist_visible = playlistdock->isVisible();
	if (!playlist_visible && !initialized) {
		playlistdock->show();
		qApp->processEvents();
	}

	BaseGui::applyStyles();

	if (!playlist_visible && !initialized) playlistdock->hide();
	initialized = true;
}
#endif

void BaseGuiPlus::setWindowCaption(const QString & title) {
#ifdef USE_SYSTRAY
	tray->setToolTip( title );
#endif

	BaseGui::setWindowCaption( title );
}


// Playlist stuff
void BaseGuiPlus::aboutToEnterFullscreen() {
    qDebug("BaseGuiPlus::aboutToEnterFullscreen");

	BaseGui::aboutToEnterFullscreen();

#if DOCK_PLAYLIST
	playlistdock->setAllowedAreas(Qt::NoDockWidgetArea);

	int playlist_screen = QApplication::desktop()->screenNumber(playlistdock);
	int mainwindow_screen = QApplication::desktop()->screenNumber(this);
	qDebug("BaseGuiPlus::aboutToEnterFullscreen: mainwindow screen: %d, playlist screen: %d", mainwindow_screen, playlist_screen);

	fullscreen_playlist_was_visible = playlistdock->isVisible();
	fullscreen_playlist_was_floating = playlistdock->isFloating();

	ignore_playlist_events = true;

	// Hide the playlist if it's in the same screen as the main window
	if ((playlist_screen == mainwindow_screen) /* || 
        (!fullscreen_playlist_was_floating) */ ) 
	{
		#if QT_VERSION < 0x050000 || !defined(Q_OS_LINUX)
		playlistdock->setFloating(true);
		#endif
		playlistdock->hide();
	}
#endif
}

void BaseGuiPlus::aboutToExitFullscreen() {
	qDebug("BaseGuiPlus::aboutToExitFullscreen");

	BaseGui::aboutToExitFullscreen();

#if DOCK_PLAYLIST
	playlistdock->setAllowedAreas(Qt::TopDockWidgetArea | 
                                  Qt::BottomDockWidgetArea
                                  #if PLAYLIST_ON_SIDES
                                  | Qt::LeftDockWidgetArea | 
                                  Qt::RightDockWidgetArea
                                  #endif
                                  );

	if (fullscreen_playlist_was_visible) {
		playlistdock->show();
	}
	playlistdock->setFloating( fullscreen_playlist_was_floating );
	ignore_playlist_events = false;
#endif
}

void BaseGuiPlus::aboutToEnterCompactMode() {
#if DOCK_PLAYLIST
	compact_playlist_was_visible = (playlistdock->isVisible() && 
                                    !playlistdock->isFloating());
	if (compact_playlist_was_visible)
		playlistdock->hide();
#endif

    widgets_size = height() - panel->height();
    qDebug("BaseGuiPlus::aboutToEnterCompactMode: widgets_size: %d", widgets_size);

	BaseGui::aboutToEnterCompactMode();

	if (pref->resize_method == Preferences::Always) {
		resize( width(), height() - widgets_size );
	}
}

void BaseGuiPlus::aboutToExitCompactMode() {
	BaseGui::aboutToExitCompactMode();

	if (pref->resize_method == Preferences::Always) {
		resize( width(), height() + widgets_size );
	}

#if DOCK_PLAYLIST
	if (compact_playlist_was_visible)
		playlistdock->show();
#endif
}

#if DOCK_PLAYLIST
void BaseGuiPlus::showPlaylist(bool b) {
	qDebug("BaseGuiPlus::showPlaylist: %d", b);
	qDebug("BaseGuiPlus::showPlaylist (before): playlist visible: %d", playlistdock->isVisible());
	qDebug("BaseGuiPlus::showPlaylist (before): playlist position: %d, %d", playlistdock->pos().x(), playlistdock->pos().y());
	qDebug("BaseGuiPlus::showPlaylist (before): playlist size: %d x %d", playlistdock->size().width(), playlistdock->size().height());

	if ( !b ) {
		playlistdock->hide();
	} else {
		exitFullscreenIfNeeded();
		playlistdock->show();

		// Check if playlist is outside of the screen
		if (playlistdock->isFloating()) {
			if (!DesktopInfo::isInsideScreen(playlistdock)) {
				qWarning("BaseGuiPlus::showPlaylist: playlist is outside of the screen");
				playlistdock->move(0,0);
			}
		}
	}
	//updateWidgets();

	qDebug("BaseGuiPlus::showPlaylist (after): playlist visible: %d", playlistdock->isVisible());
	qDebug("BaseGuiPlus::showPlaylist (after): playlist position: %d, %d", playlistdock->pos().x(), playlistdock->pos().y());
	qDebug("BaseGuiPlus::showPlaylist (after): playlist size: %d x %d", playlistdock->size().width(), playlistdock->size().height());

}

void BaseGuiPlus::playlistClosed() {
	showPlaylistAct->setChecked(false);
}

#if !USE_DOCK_TOPLEVEL_EVENT
void BaseGuiPlus::dockVisibilityChanged(bool visible) {
	qDebug("BaseGuiPlus::dockVisibilityChanged: %d", visible);

	if (!playlistdock->isFloating()) {
		if (!visible) shrinkWindow(); else stretchWindow();
	}
}

#else

void BaseGuiPlus::dockTopLevelChanged(bool floating) {
	qDebug("BaseGuiPlus::dockTopLevelChanged: %d", floating);

	if (floating) shrinkWindow(); else stretchWindow();
}
#endif

void BaseGuiPlus::stretchWindow() {
	qDebug("BaseGuiPlus::stretchWindow");
	if ((ignore_playlist_events) || (pref->resize_method!=Preferences::Always)) return;

	qDebug("BaseGuiPlus::stretchWindow: dockWidgetArea: %d", (int) dockWidgetArea(playlistdock) );

	if ( (dockWidgetArea(playlistdock) == Qt::TopDockWidgetArea) ||
         (dockWidgetArea(playlistdock) == Qt::BottomDockWidgetArea) )
	{
		int new_height = height() + playlistdock->height();

		//if (new_height > DesktopInfo::desktop_size(this).height()) 
		//	new_height = DesktopInfo::desktop_size(this).height() - 20;

		qDebug("BaseGuiPlus::stretchWindow: stretching: new height: %d", new_height);
		resize( width(), new_height );

		//resizeWindow(core->mset.win_width, core->mset.win_height);
	}

	else

	if ( (dockWidgetArea(playlistdock) == Qt::LeftDockWidgetArea) ||
         (dockWidgetArea(playlistdock) == Qt::RightDockWidgetArea) )
	{
		int new_width = width() + playlistdock->width();

		qDebug("BaseGuiPlus::stretchWindow: stretching: new width: %d", new_width);
		resize( new_width, height() );
	}
}

void BaseGuiPlus::shrinkWindow() {
	qDebug("BaseGuiPlus::shrinkWindow");
	if ((ignore_playlist_events) || (pref->resize_method!=Preferences::Always)) return;

	qDebug("BaseGuiPlus::shrinkWindow: dockWidgetArea: %d", (int) dockWidgetArea(playlistdock) );

	if ( (dockWidgetArea(playlistdock) == Qt::TopDockWidgetArea) ||
         (dockWidgetArea(playlistdock) == Qt::BottomDockWidgetArea) )
	{
		int new_height = height() - playlistdock->height();
		qDebug("DefaultGui::shrinkWindow: shrinking: new height: %d", new_height);
		resize( width(), new_height );

		//resizeWindow(core->mset.win_width, core->mset.win_height);
	}

	else

	if ( (dockWidgetArea(playlistdock) == Qt::LeftDockWidgetArea) ||
         (dockWidgetArea(playlistdock) == Qt::RightDockWidgetArea) )
	{
		int new_width = width() - playlistdock->width();

		qDebug("BaseGuiPlus::shrinkWindow: shrinking: new width: %d", new_width);
		resize( new_width, height() );
	}
}

void BaseGuiPlus::updateShortcutsContext() {
	qDebug("BaseGuiPlus::updateShortcutsContext");
	QObjectList list = children();
	for (int n = 0; n < list.count(); n++) {
		QString classname = list[n]->metaObject()->className();
		if (classname == "QAction") {
			//qDebug() << "BaseGuiPlus::updateShortcutsContext: object name:" << list[n]->objectName();
			QAction *action = static_cast<QAction *>(list[n]);
			action->setShortcutContext(pref->use_player_shortcuts_in_playlist ? Qt::WindowShortcut : Qt::WidgetWithChildrenShortcut);
		}
	}
}
#endif

#ifdef GLOBALSHORTCUTS
void BaseGuiPlus::showPreferencesDialog() {
	qDebug("BaseGuiPlus::showPreferencesDialog");
	global_shortcuts->setEnabled(false);
	BaseGui::showPreferencesDialog();
}

void BaseGuiPlus::updateGlobalShortcuts() {
	qDebug("BaseGuiPlus::updateGlobalShortcuts");
	global_shortcuts->setEnabled(false);
	global_shortcuts->setGrabbedKeys((GlobalShortcuts::MediaKeys) pref->global_shortcuts_grabbed_keys);
	global_shortcuts->setEnabled(pref->use_global_shortcuts);
}
#endif


// Convenience functions intended for other GUI's
TimeSliderAction * BaseGuiPlus::createTimeSliderAction(QWidget * parent) {
	TimeSliderAction * timeslider_action = new TimeSliderAction( parent );
	timeslider_action->setObjectName("timeslider_action");

#ifdef SEEKBAR_RESOLUTION
	connect( timeslider_action, SIGNAL( posChanged(int) ), 
             core, SLOT(goToPosition(int)) );
	connect( core, SIGNAL(positionChanged(int)), 
             timeslider_action, SLOT(setPos(int)) );
#else
	connect( timeslider_action, SIGNAL( posChanged(int) ), 
             core, SLOT(goToPos(int)) );
	connect( core, SIGNAL(posChanged(int)), 
             timeslider_action, SLOT(setPos(int)) );
#endif
	connect( timeslider_action, SIGNAL( draggingPos(int) ), 
             this, SLOT(displayGotoTime(int)) );
#if ENABLE_DELAYED_DRAGGING
	timeslider_action->setDragDelay( pref->time_slider_drag_delay );

	connect( timeslider_action, SIGNAL( delayedDraggingPos(int) ), 
             this, SLOT(goToPosOnDragging(int)) );
#else
	connect( timeslider_action, SIGNAL( draggingPos(int) ), 
             this, SLOT(goToPosOnDragging(int)) );
#endif

	connect(timeslider_action, SIGNAL(wheelUp()), core, SLOT(wheelUp()));
	connect(timeslider_action, SIGNAL(wheelDown()), core, SLOT(wheelDown()));

	connect(core, SIGNAL(newDuration(double)), timeslider_action, SLOT(setDuration(double)));

	return timeslider_action;
}

VolumeSliderAction * BaseGuiPlus::createVolumeSliderAction(QWidget * parent) {
	VolumeSliderAction * volumeslider_action = new VolumeSliderAction(parent);
	volumeslider_action->setObjectName("volumeslider_action");

	connect( volumeslider_action, SIGNAL( valueChanged(int) ),
             core, SLOT( setVolume(int) ) );
	connect( core, SIGNAL(volumeChanged(int)),
             volumeslider_action, SLOT(setValue(int)) );

	return volumeslider_action;
}

TimeLabelAction * BaseGuiPlus::createTimeLabelAction(TimeLabelAction::TimeLabelType type, QWidget * parent) {
	TimeLabelAction * time_label_action = new TimeLabelAction(type, parent);
	time_label_action->setObjectName("timelabel_action");

	connect(this, SIGNAL(timeChanged(double)), time_label_action, SLOT(setCurrentTime(double)));
	connect(core, SIGNAL(newDuration(double)), time_label_action, SLOT(setTotalTime(double)));

	return time_label_action;
}

#ifdef SCREENS_SUPPORT
void BaseGuiPlus::showScreensInfo() {
	qDebug("BaseGuiPlus::showScreensInfo");

	/*
	updateSendToScreen();
	*/

	if (!screens_info_window) {
		screens_info_window = new InfoWindow(this);
		screens_info_window->setWindowTitle(tr("Information about connected screens"));
	}

	QString t = "<h1>" + tr("Connected screens") + "</h1>";
#if QT_VERSION >= 0x050000
	QList<QScreen *> screen_list = qApp->screens();
	t += "<p>" + tr("Number of screens: %1").arg(screen_list.count());
	QString screen_name = qApp->primaryScreen()->name();
	#ifdef Q_OS_WIN
	screen_name = screen_name.replace("\\\\.\\","");
	#endif
	t += "<p>" + tr("Primary screen: %1").arg(screen_name);

	t += "<ul>";
	foreach(QScreen *screen, screen_list) {
		screen_name = screen->name();
		#ifdef Q_OS_WIN
		screen_name = screen_name.replace("\\\\.\\","");
		#endif
		t += "<li>" + tr("Information for screen %1").arg(screen_name);
		t += "<ul>";
		t += "<li>" + tr("Available geometry: %1 %2 %3 x %4").arg(screen->availableGeometry().x()).arg(screen->availableGeometry().y())
						.arg(screen->availableGeometry().width()).arg(screen->availableGeometry().height()) + "</li>";
		t += "<li>" + tr("Available size: %1 x %2").arg(screen->availableSize().width()).arg(screen->availableSize().height()) + "</li>";
		t += "<li>" + tr("Available virtual geometry: %1 %2 %3 x %4").arg(screen->availableVirtualGeometry().x())
						.arg(screen->availableVirtualGeometry().y())
						.arg(screen->availableVirtualGeometry().width())
						.arg(screen->availableVirtualGeometry().height()) + "</li>";
		t += "<li>" + tr("Available virtual size: %1 x %2").arg(screen->availableVirtualSize().width())
						.arg(screen->availableVirtualSize().height()) + "</li>";
		t += "<li>" + tr("Depth: %1 bits").arg(screen->depth()) + "</li>";
		t += "<li>" + tr("Geometry: %1 %2 %3 x %4").arg(screen->geometry().x()).arg(screen->geometry().y())
						.arg(screen->geometry().width()).arg(screen->geometry().height()) + "</li>";
		t += "<li>" + tr("Logical DPI: %1").arg(screen->logicalDotsPerInch()) + "</li>";
		//t += "<li>" + tr("Orientation: %1").arg(screen->orientation()) + "</li>";
		t += "<li>" + tr("Physical DPI: %1").arg(screen->physicalDotsPerInch()) + "</li>";
		t += "<li>" + tr("Physical size: %1 x %2 mm").arg(screen->physicalSize().width()).arg(screen->physicalSize().height()) + "</li>";
		//t += "<li>" + tr("Primary orientation: %1").arg(screen->primaryOrientation()) + "</li>";
		t += "<li>" + tr("Refresh rate: %1 Hz").arg(screen->refreshRate()) + "</li>";
		t += "<li>" + tr("Size: %1 x %2").arg(screen->size().width()).arg(screen->size().height()) + "</li>";
		t += "<li>" + tr("Virtual geometry: %1 %2 %3 x %4").arg(screen->virtualGeometry().x()).arg(screen->virtualGeometry().y())
						.arg(screen->virtualGeometry().width()).arg(screen->virtualGeometry().height()) + "</li>";
		t += "<li>" + tr("Virtual size: %1 x %2").arg(screen->virtualSize().width()).arg(screen->virtualSize().height()) + "</li>";
		t += "</ul></li>";
	}
	t += "</ul>";
#else
	QDesktopWidget * dw = qApp->desktop();
	t += "<p>" + tr("Number of screens: %1").arg(dw->screenCount());
	t += "<p>" + tr("Primary screen: %1").arg(dw->primaryScreen()+1);

	t += "<ul>";
	for (int n = 0; n < dw->screenCount(); n++) {
		t += "<li>" + tr("Information for screen %1").arg(n+1);
		t += "<ul>";
		t += "<li>" + tr("Available geometry: %1 %2 %3 x %4").arg(dw->availableGeometry(n).x()).arg(dw->availableGeometry(n).y())
						.arg(dw->availableGeometry(n).width()).arg(dw->availableGeometry(n).height()) + "</li>";
		t += "<li>" + tr("Geometry: %1 %2 %3 x %4").arg(dw->screenGeometry(n).x()).arg(dw->screenGeometry(n).y())
						.arg(dw->screenGeometry(n).width()).arg(dw->screenGeometry(n).height()) + "</li>";
		t += "</ul></li>";
	}
	t += "</ul>";
#endif

	screens_info_window->setHtml(t);
	screens_info_window->show();
}

void BaseGuiPlus::updateSendToScreen() {
	qDebug("BaseGuiPlus::updateSendToScreen");

	sendToScreenGroup->clear(true);

#if QT_VERSION >= 0x050000
	QList<QScreen *> screen_list = qApp->screens();
	int n_screens = screen_list.count();
#else
	QDesktopWidget * dw = qApp->desktop();
	int n_screens = dw->screenCount();
#endif

	for (int n = 0; n < n_screens; n++) {
		QString name;
		#if QT_VERSION >= 0x050000
		name = screen_list[n]->name();
		#ifdef Q_OS_WIN
		name = name.replace("\\\\.\\","");
		#endif
		bool is_primary_screen = (screen_list[n] == qApp->primaryScreen());
		#else
		bool is_primary_screen = (n == dw->primaryScreen());
		#endif
		MyAction * screen_item = new MyActionGroupItem(this, sendToScreenGroup, QString("send_to_screen_%1").arg(n+1).toLatin1().constData(), n);
		QString desc = "&" + QString::number(n+1);
		if (!name.isEmpty()) desc += " - " + name;
		if (is_primary_screen) desc += " (" + tr("Primary screen") + ")";
		screen_item->change(desc);
	}

	sendToScreen_menu->clear();
	sendToScreen_menu->addActions(sendToScreenGroup->actions());
	
	if (n_screens == 1 && isVideoDetached()) detachVideo(false);
}

void BaseGuiPlus::sendVideoToScreen(int screen) {
	qDebug() << "BaseGuiPlus::sendVideoToScreen:" << screen;

#if QT_VERSION >= 0x050000
	QList<QScreen *> screen_list = qApp->screens();
	int n_screens = screen_list.count();
#else
	QDesktopWidget * dw = qApp->desktop();
	int n_screens = dw->screenCount();
#endif

	if (screen < n_screens) {
		#if QT_VERSION >= 0x050000
		bool is_primary_screen = (screen_list[screen] == qApp->primaryScreen());
		QRect geometry = screen_list[screen]->geometry();
		#else
		bool is_primary_screen = (screen == dw->primaryScreen());
		QRect geometry = dw->screenGeometry(screen);	
		qDebug() << "BaseGuiPlus::sendVideoToScreen: screen geometry:" << geometry;
		#endif
		qDebug() << "BaseGuiPlus::sendVideoToScreen: is_primary_screen:" << is_primary_screen;
		//is_primary_screen = false;
		if (is_primary_screen) {
			mplayerwindow->showNormal();
			#if QT_VERSION >= 0x050000 && defined(Q_OS_LINUX)
			qApp->processEvents();
			#endif
			detachVideo(false);
		} else {
			detachVideo(true);
			//#if QT_VERSION >= 0x050000
			//mplayerwindow->windowHandle()->setScreen(screen_list[screen]); // Doesn't work
			//#else
			mplayerwindow->move(geometry.x(), geometry.y());
			//#endif
			qApp->processEvents();
			//toggleFullscreen(true);
			mplayerwindow->showFullScreen();
		}
	} else {
		// Error
		qWarning() << "BaseGuiPlus::sendVideoToScreen: screen" << screen << "is not valid";
	}
}

bool BaseGuiPlus::isVideoDetached() {
	return (mplayerwindow->parent() == 0);
}

void BaseGuiPlus::detachVideo(bool detach) {
	qDebug() << "BaseGuiPlus::detachVideo:" << detach;

	if (detach) {
		if (!isVideoDetached()) {
			toggleFullscreen(false);
			fullscreenAct->setEnabled(false);

			panel->layout()->removeWidget(mplayerwindow);
			mplayerwindow->setParent(0);
			mplayerwindow->setWindowTitle(tr("SMPlayer external screen output"));

			detached_label->show();
		}
		mplayerwindow->show();
	} else {
		if (isVideoDetached()) {
			fullscreenAct->setEnabled(true);

			detached_label->hide();

			mplayerwindow->setWindowTitle(QString::null);
			mplayerwindow->setParent(panel);
			#if QT_VERSION >= 0x050000 && defined(Q_OS_LINUX)
			qApp->processEvents();
			#endif
			panel->layout()->addWidget(mplayerwindow);
		}
	}
}

/*
void BaseGuiPlus::toggleFullscreen(bool b) {
	qDebug() << "BaseGuiPlus::toggleFullscreen:" << b;
	if (!isVideoDetached()) {
		BaseGui::toggleFullscreen(b);
	} else {
		if (b == pref->fullscreen) return;
		pref->fullscreen = b;

		if (pref->fullscreen) {
			//aboutToEnterFullscreen();
			mplayerwindow->showFullScreen();
		} else {
			mplayerwindow->showNormal();
			//aboutToExitFullscreen();
		}
	}
}
*/
#endif

#ifdef SEND_AUDIO_OPTION
void BaseGuiPlus::updateSendAudioMenu() {
	qDebug("BaseGuiPlus::updateSendAudioMenu");

	sendAudio_menu->clear();
	QAction * a = new QAction(sendAudio_menu);
	a->setText(tr("&Default audio device"));
	a->setData("");
	connect(a, SIGNAL(triggered()), this, SLOT(sendAudioClicked()));
	sendAudio_menu->addAction(a);

#if USE_PULSEAUDIO_DEVICES
	addListToSendAudioMenu( DeviceInfo::paDevices(), "pulse");
#endif

#if USE_ALSA_DEVICES
	addListToSendAudioMenu( DeviceInfo::alsaDevices(), "alsa");
#endif

#if USE_DSOUND_DEVICES
	if (PlayerID::player(pref->mplayer_bin) == PlayerID::MPLAYER) {
		addListToSendAudioMenu( DeviceInfo::dsoundDevices(), "dsound");
	}
#endif

#if MPV_AUDIO_DEVICES
	if (PlayerID::player(pref->mplayer_bin) == PlayerID::MPV) {
		DeviceInfo::setMpvBin(pref->mplayer_bin);

		#if USE_MPV_ALSA_DEVICES
		addListToSendAudioMenu( DeviceInfo::mpvAlsaDevices(), "alsa");
		#endif

		#if USE_MPV_WASAPI_DEVICES
		addListToSendAudioMenu( DeviceInfo::mpvWasapiDevices(), "wasapi");
		#endif
	}
#endif
}

void BaseGuiPlus::addListToSendAudioMenu(const DeviceList & audio_devices, const QString & device_name) {
	for (int n = 0; n < audio_devices.count(); n++) {
		QAction * a = new QAction(sendAudio_menu);
		a->setText( DeviceInfo::printableName(device_name, audio_devices[n]) );
		a->setData( DeviceInfo::internalName(device_name, audio_devices[n]) );
		connect(a, SIGNAL(triggered()), this, SLOT(sendAudioClicked()));
		sendAudio_menu->addAction(a);
	}
}

void BaseGuiPlus::sendAudioClicked() {
	QAction * a = qobject_cast<QAction *> (sender());
	if (a) {
		QString device = a->data().toString();
		qDebug() << "BaseGuiPlus::sendAudioClicked: device:" << device;
		core->changeAO(device);
	}
}
#endif

#ifdef CHROMECAST_SUPPORT
void BaseGuiPlus::playOnChromecast() {
	qDebug("BaseGuiPlus::playOnChromecast");

	qDebug() << "BaseGuiPlus::playOnChromecast: type:" << core->mdat.type;
	qDebug() << "BaseGuiPlus::playOnChromecast: filename:" << core->mdat.filename;
	qDebug() << "BaseGuiPlus::playOnChromecast: stream_url:" << core->mdat.stream_url;
	qDebug() << "BaseGuiPlus::playOnChromecast: stream_title:" << core->mdat.stream_title;
	qDebug() << "BaseGuiPlus::playOnChromecast: stream_path:" << core->mdat.stream_path;

	QString title = core->mdat.displayName(true);
	if (core->mdat.type == TYPE_STREAM) {
		QString url = core->mdat.filename;
		if (!core->mdat.stream_path.isEmpty()) url = core->mdat.stream_path;
		Chromecast::instance()->openStream(url, title);
	}
	else
	if (core->mdat.type == TYPE_FILE) {
		QString subtitle;
		if (MediaSettings::SubNone != core->mset.current_subtitle_track && MediaSettings::NoneSelected != core->mset.current_subtitle_track) {
			SubData sub = core->mset.subs.itemAt(core->mset.current_subtitle_track);
			qDebug() << "BaseGuiPlus::playOnChromecast: current sub name:" << sub.name() << "filename:" << sub.filename();
			if (!sub.filename().isEmpty()) subtitle = sub.filename(); else subtitle = sub.name();
		}
		Chromecast * cc = Chromecast::instance();
		#ifdef CONVERT_TO_VTT
		cc->setSubtitleEncoding(pref->subcp);
		#endif
		cc->openLocal(core->mdat.filename, title, subtitle);
	}
}
#endif

#ifdef CHECK_SHORTCUT_EVENTS
void BaseGuiPlus::installFilterOnActions() {
	QObjectList list = children();
	for (int n = 0; n < list.count(); n++) {
		QString classname = list[n]->metaObject()->className();
		//qDebug() << "BaseGuiPlus::installFilterOnActions:" << classname;
		if (classname == "QAction") {
			list[n]->installEventFilter(this);
		}
	}
}

bool BaseGuiPlus::eventFilter(QObject * obj, QEvent * e) {
	Q_UNUSED(obj);
	static QTime t(0,0);

	if (e->type() == QEvent::Shortcut) {
		QShortcutEvent * se = static_cast<QShortcutEvent*>(e);
		qDebug() << "BaseGuiPlus::eventFilter: key:" << se->key();
		if (se->key() == QKeySequence(Qt::Key_VolumeMute) ||
			/*
		    se->key() == QKeySequence(Qt::Key_VolumeDown) ||
		    se->key() == QKeySequence(Qt::Key_VolumeUp) ||
			*/
		    se->key() == QKeySequence(Qt::Key_MediaPlay) ||
		    se->key() == QKeySequence(Qt::Key_MediaStop) ||
		    se->key() == QKeySequence(Qt::Key_Stop) ||
		    se->key() == QKeySequence(Qt::Key_MediaPrevious) ||
		    se->key() == QKeySequence(Qt::Key_MediaNext) ||
		    se->key() == QKeySequence(Qt::Key_MediaRecord) ||
		    se->key() == QKeySequence(Qt::Key_MediaPause) ||
		    se->key() == QKeySequence(Qt::Key_MediaTogglePlayPause))
		{
			QTime now = QTime::currentTime();
			int msecs = t.msecsTo(now);
			t = now;
			qDebug() << "BaseGuiPlus::eventFilter: msecs:" << msecs;
			if (msecs < 500) return true;
		}
	}
	return false;
}
#endif

#include "moc_baseguiplus.cpp"
