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

#include "basegui.h"

#include "filedialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QMenu>
#include <QFileInfo>
#include <QApplication>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QCursor>
#include <QTimer>
#include <QStyle>
#include <QRegExp>
#include <QStatusBar>
#include <QActionGroup>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDesktopServices>
#include <QInputDialog>
#include <QClipboard>
#include <QMimeData>
#include <QDesktopWidget>

#include <QtCore/qmath.h>

#include "mplayerwindow.h"
#include "desktopinfo.h"
#include "helper.h"
#include "paths.h"
#include "colorutils.h"
#include "global.h"
#include "translator.h"
#include "images.h"
#include "preferences.h"
#include "discname.h"
#include "timeslider.h"
#include "logwindow.h"
#include "infowindow.h"
#include "playlist.h"
#include "filepropertiesdialog.h"
#include "eqslider.h"
#include "videoequalizer.h"
#include "audioequalizer.h"
#include "inputdvddirectory.h"
#include "inputmplayerversion.h"
#include "inputurl.h"
#include "recents.h"
#include "urlhistory.h"
#include "about.h"
#include "errordialog.h"
#include "timedialog.h"
#include "stereo3ddialog.h"

#ifdef BOOKMARKS
#include "inputbookmark.h"
#include "bookmarkdialog.h"
#endif

#include "clhelp.h"
#include "mplayerversion.h"

#ifdef FIND_SUBTITLES
#include "findsubtitleswindow.h"
#endif

#ifdef VIDEOPREVIEW
#include "videopreview.h"
#endif

#include "config.h"
#include "actionseditor.h"

#ifdef TV_SUPPORT
#include "tvlist.h"
#else
#include "favorites.h"
#endif

#include "preferencesdialog.h"
#include "prefgeneral.h"
#include "prefinterface.h"
#include "prefinput.h"
#include "prefadvanced.h"
#include "prefplaylist.h"

#include "myaction.h"
#include "myactiongroup.h"
#include "playlist.h"

#include "constants.h"
#include "links.h"

#ifdef MPRIS2
#include "mpris2/mpris2.h"
#endif

#include "extensions.h"
#include "version.h"

#ifdef Q_OS_WIN
#include "deviceinfo.h"
#include <QSysInfo>
#endif

#ifdef UPDATE_CHECKER
#include "updatechecker.h"
#endif

#ifdef YOUTUBE_SUPPORT
  #ifdef YT_USE_YTSIG
  #include "codedownloader.h"
  #endif
#endif

#ifdef SHARE_ACTIONS
#include "sharedialog.h"
#endif

#ifdef SHARE_WIDGET
#include "sharewidget.h"
#endif

#ifdef AUTO_SHUTDOWN_PC
#include "shutdowndialog.h"
#include "shutdown.h"
#endif

using namespace Global;

BaseGui::BaseGui( QWidget* parent, Qt::WindowFlags flags ) 
	: QMainWindow( parent, flags )
#ifdef DETECT_MINIMIZE_WORKAROUND
	, was_minimized(false)
#endif
	, popup(0)
	, clhelp_window(0)
	, pref_dialog(0)
	, file_dialog (0)
#ifdef FIND_SUBTITLES
	, find_subs_dialog(0)
#endif
#ifdef VIDEOPREVIEW
	, video_preview(0)
#endif
#ifdef UPDATE_CHECKER
	, update_checker(0)
#endif
	, arg_close_on_finish(-1)
	, arg_start_in_fullscreen(-1)
#ifdef MG_DELAYED_SEEK
	, delayed_seek_timer(0)
	, delayed_seek_value(0)
#endif
	, was_maximized(false)
#ifdef AVOID_SCREENSAVER
	, just_stopped(false)
#endif
	, ignore_show_hide_events(false)
{
	setWindowTitle( "SMPlayer" );

	// Create objects:
	createPanel();
	setCentralWidget(panel);

	createMplayerWindow();
	createCore();
	createPlaylist();
	createVideoEqualizer();
	createAudioEqualizer();

	// Mouse Wheel
	/*
	connect( this, SIGNAL(wheelUp()),
             core, SLOT(wheelUp()) );
	connect( this, SIGNAL(wheelDown()),
             core, SLOT(wheelDown()) );
	*/
	connect( mplayerwindow, SIGNAL(wheelUp()),
             core, SLOT(wheelUp()) );
	connect( mplayerwindow, SIGNAL(wheelDown()),
             core, SLOT(wheelDown()) );

#if STYLE_SWITCHING
	qApp->setStyleSheet("");
	default_style = qApp->style()->objectName();

	#ifdef Q_OS_LINUX
	// Some controls aren't displayed correctly with the adwaita style
	// so try to prevent to use it as the default style
	if (default_style.toLower() == "adwaita") default_style = "gtk+";
	#endif
	qDebug() << "BaseGui::BaseGui: default_style:" << default_style;
	//qDebug() << "BaseGui::BaseGui: qApp->style:" << qApp->style();
#endif

#ifdef LOG_MPLAYER
	mplayer_log_window = new LogWindow(0);
#endif
#ifdef LOG_SMPLAYER
	smplayer_log_window = new LogWindow(0);
#endif

	createActions();
	createMenus();
#if AUTODISABLE_ACTIONS
	setActionsEnabled(false);
	if (playlist->count() > 0) {
		playAct->setEnabled(true);
		playOrPauseAct->setEnabled(true);
	}
#endif

#if !DOCK_PLAYLIST
	connect(playlist, SIGNAL(visibilityChanged(bool)),
            showPlaylistAct, SLOT(setChecked(bool)) );
#endif

	retranslateStrings();

	setAcceptDrops(true);

	resize(pref->default_size);

	panel->setFocus();

	setupNetworkProxy();

	/* Initialize GUI */
	if (pref->compact_mode) toggleCompactMode(true);
	changeStayOnTop(pref->stay_on_top);
	updateRecents();
	QTimer::singleShot(20, this, SLOT(loadActions()));

#ifdef UPDATE_CHECKER
	update_checker = new UpdateChecker(this, &pref->update_checker_data);
#endif

#ifdef CHECK_UPGRADED
	QTimer::singleShot(2000, this, SLOT(checkIfUpgraded()));
#endif

#ifdef DONATE_REMINDER
	QTimer::singleShot(1000, this, SLOT(checkReminder()));
#endif

#ifdef MPRIS2
	if (pref->use_mpris2) new Mpris2(this, this);
#endif
}

void BaseGui::setupNetworkProxy() {
	qDebug("BaseGui::setupNetworkProxy");

	QNetworkProxy proxy;

	if ( (pref->use_proxy) && (!pref->proxy_host.isEmpty()) ) {
		proxy.setType((QNetworkProxy::ProxyType) pref->proxy_type);
		proxy.setHostName(pref->proxy_host);
		proxy.setPort(pref->proxy_port);
		if ( (!pref->proxy_username.isEmpty()) && (!pref->proxy_password.isEmpty()) ) {
			proxy.setUser(pref->proxy_username);
			proxy.setPassword(pref->proxy_password);
		}
		qDebug("BaseGui::setupNetworkProxy: using proxy: host: %s, port: %d, type: %d", 
               pref->proxy_host.toUtf8().constData(), pref->proxy_port, pref->proxy_type);
	} else {
		// No proxy
		proxy.setType(QNetworkProxy::NoProxy);
		qDebug("BaseGui::setupNetworkProxy: no proxy");
	}

	QNetworkProxy::setApplicationProxy(proxy);
}

#ifdef SINGLE_INSTANCE
void BaseGui::handleMessageFromOtherInstances(const QString& message) {
	qDebug() << "BaseGui::handleMessageFromOtherInstances:" << message;

	int pos = message.indexOf(' ');
	if (pos > -1) {
		QString command = message.left(pos);
		QString arg = message.mid(pos+1);
		qDebug() << "command:" << command;
		qDebug() << "arg:" << arg;

		if (command == "open_file") {
			emit openFileRequested();
			open(arg);
		} 
		else
		if (command == "open_files") {
			QStringList file_list = arg.split(" <<sep>> ");
			emit openFileRequested();
			openFiles(file_list);
		}
		else
		if (command == "add_to_playlist") {
			QStringList file_list = arg.split(" <<sep>> ");
			/* if (core->state() == Core::Stopped) { emit openFileRequested(); } */
			playlist->addFiles(file_list);
		}
		else
		if (command == "media_title") {
			QStringList list = arg.split(" <<sep>> ");
			core->addForcedTitle(list[0], list[1]);
		}
		else
		if (command == "action") {
			processFunction(arg);
		}
		else
		if (command == "load_sub") {
			setInitialSubtitle(arg);
			if (core->state() != Core::Stopped) {
				core->loadSub(arg);
			}
		}
	}
}
#endif

BaseGui::~BaseGui() {
	delete core; // delete before mplayerwindow, otherwise, segfault...
#ifdef LOG_MPLAYER
	delete mplayer_log_window;
#endif
#ifdef LOG_SMPLAYER
	delete smplayer_log_window;
#endif

	delete favorites;

#ifdef TV_SUPPORT
	delete tvlist;
	delete radiolist;
#endif

//#if !DOCK_PLAYLIST
	if (playlist) {
		delete playlist;
		playlist = 0;
	}
//#endif

#ifdef FIND_SUBTITLES
	if (find_subs_dialog) {
		delete find_subs_dialog;
		find_subs_dialog = 0; // Necessary?
	}
#endif

#ifdef VIDEOPREVIEW
	if (video_preview) {
		delete video_preview;
	}
#endif

	if (mplayerwindow) {
		delete mplayerwindow;
	}
}

void BaseGui::createActions() {
	// Menu File
	openFileAct = new MyAction( QKeySequence("Ctrl+F"), this, "open_file" );
	connect( openFileAct, SIGNAL(triggered()),
             this, SLOT(openFile()) );

	openDirectoryAct = new MyAction( this, "open_directory" );
	connect( openDirectoryAct, SIGNAL(triggered()),
             this, SLOT(openDirectory()) );

	openPlaylistAct = new MyAction( this, "open_playlist" );
	connect( openPlaylistAct, SIGNAL(triggered()),
             playlist, SLOT(load()) );

	openVCDAct = new MyAction( this, "open_vcd" );
	connect( openVCDAct, SIGNAL(triggered()),
             this, SLOT(openVCD()) );

	openAudioCDAct = new MyAction( this, "open_audio_cd" );
	connect( openAudioCDAct, SIGNAL(triggered()),
             this, SLOT(openAudioCD()) );

	openDVDAct = new MyAction( this, "open_dvd" );
	connect( openDVDAct, SIGNAL(triggered()),
             this, SLOT(openDVD()) );

	openDVDFolderAct = new MyAction( this, "open_dvd_folder" );
	connect( openDVDFolderAct, SIGNAL(triggered()),
             this, SLOT(openDVDFromFolder()) );

	// Bluray section.
#ifdef BLURAY_SUPPORT
	openBluRayAct = new MyAction( this, "open_bluray" );
	connect( openBluRayAct, SIGNAL(triggered()),
             this, SLOT(openBluRay()));

	openBluRayFolderAct = new MyAction( this, "open_bluray_folder" );
	connect( openBluRayFolderAct, SIGNAL(triggered()),
             this, SLOT(openBluRayFromFolder()));
#endif

	openURLAct = new MyAction( QKeySequence("Ctrl+U"), this, "open_url" );
	connect( openURLAct, SIGNAL(triggered()),
             this, SLOT(openURL()) );

	exitAct = new MyAction( QKeySequence("Ctrl+X"), this, "close" );
	connect( exitAct, SIGNAL(triggered()), this, SLOT(closeWindow()) );

	clearRecentsAct = new MyAction( this, "clear_recents" );
	connect( clearRecentsAct, SIGNAL(triggered()), this, SLOT(clearRecentsList()) );

	// Favorites
	favorites = new Favorites(Paths::configPath() + "/favorites.m3u8", this);
	favorites->menuAction()->setObjectName( "favorites_menu" );
	addAction(favorites->editAct());
	addAction(favorites->jumpAct());
	addAction(favorites->nextAct());
	addAction(favorites->previousAct());
	connect(favorites, SIGNAL(activated(QString)), this, SLOT(openFavorite(QString)));
	connect(core, SIGNAL(mediaPlaying(const QString &, const QString &)),
            favorites, SLOT(getCurrentMedia(const QString &, const QString &)));

	// TV and Radio
#ifdef TV_SUPPORT
	tvlist = new TVList(pref->check_channels_conf_on_startup, 
                        TVList::TV, Paths::configPath() + "/tv.m3u8", this);
	tvlist->menuAction()->setObjectName( "tv_menu" );
	addAction(tvlist->editAct());
	addAction(tvlist->jumpAct());
	addAction(tvlist->nextAct());
	addAction(tvlist->previousAct());
	tvlist->nextAct()->setShortcut( Qt::Key_H );
	tvlist->previousAct()->setShortcut( Qt::Key_L );
	tvlist->nextAct()->setObjectName("next_tv");
	tvlist->previousAct()->setObjectName("previous_tv");
	tvlist->editAct()->setObjectName("edit_tv_list");
	tvlist->jumpAct()->setObjectName("jump_tv_list");
	connect(tvlist, SIGNAL(activated(QString)), this, SLOT(open(QString)));
	connect(core, SIGNAL(mediaPlaying(const QString &, const QString &)),
            tvlist, SLOT(getCurrentMedia(const QString &, const QString &)));

	radiolist = new TVList(pref->check_channels_conf_on_startup, 
                           TVList::Radio, Paths::configPath() + "/radio.m3u8", this);
	radiolist->menuAction()->setObjectName( "radio_menu" );
	addAction(radiolist->editAct());
	addAction(radiolist->jumpAct());
	addAction(radiolist->nextAct());
	addAction(radiolist->previousAct());
	radiolist->nextAct()->setShortcut( Qt::SHIFT | Qt::Key_H );
	radiolist->previousAct()->setShortcut( Qt::SHIFT | Qt::Key_L );
	radiolist->nextAct()->setObjectName("next_radio");
	radiolist->previousAct()->setObjectName("previous_radio");
	radiolist->editAct()->setObjectName("edit_radio_list");
	radiolist->jumpAct()->setObjectName("jump_radio_list");
	connect(radiolist, SIGNAL(activated(QString)), this, SLOT(open(QString)));
	connect(core, SIGNAL(mediaPlaying(const QString &, const QString &)),
            radiolist, SLOT(getCurrentMedia(const QString &, const QString &)));
#endif


	// Menu Play
	playAct = new MyAction( this, "play" );
	connect( playAct, SIGNAL(triggered()),
             core, SLOT(play()) );

	playOrPauseAct = new MyAction( Qt::Key_MediaPlay, this, "play_or_pause" );
	playOrPauseAct->addShortcut(QKeySequence("Toggle Media Play/Pause")); // MCE remote key
	connect( playOrPauseAct, SIGNAL(triggered()),
             core, SLOT(play_or_pause()) );

	pauseAct = new MyAction( Qt::Key_Space, this, "pause" );
	pauseAct->addShortcut(QKeySequence("Media Pause")); // MCE remote key
	connect( pauseAct, SIGNAL(triggered()),
             core, SLOT(pause()) );

	pauseAndStepAct = new MyAction( this, "pause_and_frame_step" );
	connect( pauseAndStepAct, SIGNAL(triggered()),
             core, SLOT(pause_and_frame_step()) );

	stopAct = new MyAction( Qt::Key_MediaStop, this, "stop" );
	connect( stopAct, SIGNAL(triggered()),
             core, SLOT(stop()) );

	frameStepAct = new MyAction( Qt::Key_Period, this, "frame_step" );
	connect( frameStepAct, SIGNAL(triggered()),
             core, SLOT(frameStep()) );

	frameBackStepAct = new MyAction( Qt::Key_Comma, this, "frame_back_step" );
	connect( frameBackStepAct, SIGNAL(triggered()),
             core, SLOT(frameBackStep()) );

	rewind1Act = new MyAction( Qt::Key_Left, this, "rewind1" );
	rewind1Act->addShortcut(QKeySequence("Shift+Ctrl+B")); // MCE remote key
	connect( rewind1Act, SIGNAL(triggered()),
             core, SLOT(srewind()) );

	rewind2Act = new MyAction( Qt::Key_Down, this, "rewind2" );
	connect( rewind2Act, SIGNAL(triggered()),
             core, SLOT(rewind()) );

	rewind3Act = new MyAction( Qt::Key_PageDown, this, "rewind3" );
	connect( rewind3Act, SIGNAL(triggered()),
             core, SLOT(fastrewind()) );

	forward1Act = new MyAction( Qt::Key_Right, this, "forward1" );
	forward1Act->addShortcut(QKeySequence("Shift+Ctrl+F")); // MCE remote key
	connect( forward1Act, SIGNAL(triggered()),
             core, SLOT(sforward()) );

	forward2Act = new MyAction( Qt::Key_Up, this, "forward2" );
	connect( forward2Act, SIGNAL(triggered()),
             core, SLOT(forward()) );

	forward3Act = new MyAction( Qt::Key_PageUp, this, "forward3" );
	connect( forward3Act, SIGNAL(triggered()),
             core, SLOT(fastforward()) );

	setAMarkerAct = new MyAction( this, "set_a_marker" );
	connect( setAMarkerAct, SIGNAL(triggered()),
             core, SLOT(setAMarker()) );

	setBMarkerAct = new MyAction( this, "set_b_marker" );
	connect( setBMarkerAct, SIGNAL(triggered()),
             core, SLOT(setBMarker()) );

	clearABMarkersAct = new MyAction( this, "clear_ab_markers" );
	connect( clearABMarkersAct, SIGNAL(triggered()),
             core, SLOT(clearABMarkers()) );

	repeatAct = new MyAction( this, "repeat" );
	repeatAct->setCheckable( true );
	connect( repeatAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleRepeat(bool)) );

	gotoAct = new MyAction( QKeySequence("Ctrl+J"), this, "jump_to" );
	connect( gotoAct, SIGNAL(triggered()),
             this, SLOT(showGotoDialog()) );

	// Submenu Speed
	normalSpeedAct = new MyAction( Qt::Key_Backspace, this, "normal_speed" );
	connect( normalSpeedAct, SIGNAL(triggered()),
             core, SLOT(normalSpeed()) );

	halveSpeedAct = new MyAction( Qt::Key_BraceLeft, this, "halve_speed" );
	connect( halveSpeedAct, SIGNAL(triggered()),
             core, SLOT(halveSpeed()) );

	doubleSpeedAct = new MyAction( Qt::Key_BraceRight, this, "double_speed" );
	connect( doubleSpeedAct, SIGNAL(triggered()),
             core, SLOT(doubleSpeed()) );

	decSpeed10Act = new MyAction( Qt::Key_BracketLeft, this, "dec_speed" );
	connect( decSpeed10Act, SIGNAL(triggered()),
             core, SLOT(decSpeed10()) );

	incSpeed10Act = new MyAction( Qt::Key_BracketRight, this, "inc_speed" );
	connect( incSpeed10Act, SIGNAL(triggered()),
             core, SLOT(incSpeed10()) );

	decSpeed4Act = new MyAction( this, "dec_speed_4" );
	connect( decSpeed4Act, SIGNAL(triggered()),
             core, SLOT(decSpeed4()) );

	incSpeed4Act = new MyAction( this, "inc_speed_4" );
	connect( incSpeed4Act, SIGNAL(triggered()),
             core, SLOT(incSpeed4()) );

	decSpeed1Act = new MyAction( this, "dec_speed_1" );
	connect( decSpeed1Act, SIGNAL(triggered()),
             core, SLOT(decSpeed1()) );

	incSpeed1Act = new MyAction( this, "inc_speed_1" );
	connect( incSpeed1Act, SIGNAL(triggered()),
             core, SLOT(incSpeed1()) );


	// Menu Video
	fullscreenAct = new MyAction( Qt::Key_F, this, "fullscreen" );
	fullscreenAct->addShortcut(QKeySequence("Ctrl+T")); // MCE remote key
	fullscreenAct->setCheckable( true );
	connect( fullscreenAct, SIGNAL(toggled(bool)),
             this, SLOT(toggleFullscreen(bool)) );

	compactAct = new MyAction( QKeySequence("Ctrl+C"), this, "compact" );
	compactAct->setCheckable( true );
	connect( compactAct, SIGNAL(toggled(bool)),
             this, SLOT(toggleCompactMode(bool)) );

	videoEqualizerAct = new MyAction( QKeySequence("Ctrl+E"), this, "video_equalizer" );
	videoEqualizerAct->setCheckable( true );
	connect( videoEqualizerAct, SIGNAL(toggled(bool)),
             this, SLOT(showVideoEqualizer(bool)) );

	// Single screenshot
	screenshotAct = new MyAction( Qt::Key_S, this, "screenshot" );
	connect( screenshotAct, SIGNAL(triggered()),
             core, SLOT(screenshot()) );

	screenshotWithSubsAct = new MyAction( QKeySequence("Ctrl+Shift+S"), this, "screenshot_with_subtitles" );
	connect( screenshotWithSubsAct, SIGNAL(triggered()),
             core, SLOT(screenshotWithSubtitles()) );

	screenshotWithNoSubsAct = new MyAction( QKeySequence("Ctrl+Alt+S"), this, "screenshot_without_subtitles" );
	connect( screenshotWithNoSubsAct, SIGNAL(triggered()),
             core, SLOT(screenshotWithoutSubtitles()) );

	// Multiple screenshots
	screenshotsAct = new MyAction( QKeySequence("Shift+D"), this, "multiple_screenshots" );
	connect( screenshotsAct, SIGNAL(triggered()),
             core, SLOT(screenshots()) );

#ifdef CAPTURE_STREAM
	capturingAct = new MyAction( /*Qt::Key_C,*/ this, "capture_stream");
	connect( capturingAct, SIGNAL(triggered()),
             core, SLOT(switchCapturing()) );
#endif

#ifdef VIDEOPREVIEW
	videoPreviewAct = new MyAction( this, "video_preview" );
	connect( videoPreviewAct, SIGNAL(triggered()),
             this, SLOT(showVideoPreviewDialog()) );
#endif

	flipAct = new MyAction( this, "flip" );
	flipAct->setCheckable( true );
	connect( flipAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleFlip(bool)) );

	mirrorAct = new MyAction( this, "mirror" );
	mirrorAct->setCheckable( true );
	connect( mirrorAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleMirror(bool)) );

	stereo3dAct = new MyAction( this, "stereo_3d_filter" );
	connect( stereo3dAct, SIGNAL(triggered()),
             this, SLOT(showStereo3dDialog()) );

	// Submenu filter
	postProcessingAct = new MyAction( this, "postprocessing" );
	postProcessingAct->setCheckable( true );
	connect( postProcessingAct, SIGNAL(toggled(bool)),
             core, SLOT(togglePostprocessing(bool)) );

	phaseAct = new MyAction( this, "autodetect_phase" );
	phaseAct->setCheckable( true );
	connect( phaseAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleAutophase(bool)) );

	deblockAct = new MyAction( this, "deblock" );
	deblockAct->setCheckable( true );
	connect( deblockAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleDeblock(bool)) );

	deringAct = new MyAction( this, "dering" );
	deringAct->setCheckable( true );
	connect( deringAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleDering(bool)) );

	gradfunAct = new MyAction( this, "gradfun" );
	gradfunAct->setCheckable( true );
	connect( gradfunAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleGradfun(bool)) );


	addNoiseAct = new MyAction( this, "add_noise" );
	addNoiseAct->setCheckable( true );
	connect( addNoiseAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleNoise(bool)) );

	addLetterboxAct = new MyAction( this, "add_letterbox" );
	addLetterboxAct->setCheckable( true );
	connect( addLetterboxAct, SIGNAL(toggled(bool)),
             core, SLOT(changeLetterbox(bool)) );

	upscaleAct = new MyAction( this, "upscaling" );
	upscaleAct->setCheckable( true );
	connect( upscaleAct, SIGNAL(toggled(bool)),
             core, SLOT(changeUpscale(bool)) );


	// Menu Audio
	audioEqualizerAct = new MyAction( this, "audio_equalizer" );
	audioEqualizerAct->setCheckable( true );
	connect( audioEqualizerAct, SIGNAL(toggled(bool)),
             this, SLOT(showAudioEqualizer(bool)) );

	muteAct = new MyAction( Qt::Key_M, this, "mute" );
	muteAct->addShortcut(Qt::Key_VolumeMute); // MCE remote key
	muteAct->setCheckable( true );
	connect( muteAct, SIGNAL(toggled(bool)),
             core, SLOT(mute(bool)) );

#if USE_MULTIPLE_SHORTCUTS
	decVolumeAct = new MyAction( this, "decrease_volume" );
	decVolumeAct->setShortcuts( ActionsEditor::stringToShortcuts("9,/") );
	decVolumeAct->addShortcut(Qt::Key_VolumeDown); // MCE remote key
#else
	decVolumeAct = new MyAction( Qt::Key_9, this, "dec_volume" );
#endif
	connect( decVolumeAct, SIGNAL(triggered()),
             core, SLOT(decVolume()) );

#if USE_MULTIPLE_SHORTCUTS
	incVolumeAct = new MyAction( this, "increase_volume" );
	incVolumeAct->setShortcuts( ActionsEditor::stringToShortcuts("0,*") );
	incVolumeAct->addShortcut(Qt::Key_VolumeUp); // MCE remote key
#else
	incVolumeAct = new MyAction( Qt::Key_0, this, "inc_volume" );
#endif
	connect( incVolumeAct, SIGNAL(triggered()),
             core, SLOT(incVolume()) );

	decAudioDelayAct = new MyAction( Qt::Key_Minus, this, "dec_audio_delay" );
	connect( decAudioDelayAct, SIGNAL(triggered()),
             core, SLOT(decAudioDelay()) );

	incAudioDelayAct = new MyAction( Qt::Key_Plus, this, "inc_audio_delay" );
	connect( incAudioDelayAct, SIGNAL(triggered()),
             core, SLOT(incAudioDelay()) );

	audioDelayAct = new MyAction( this, "audio_delay" );
	connect( audioDelayAct, SIGNAL(triggered()),
             this, SLOT(showAudioDelayDialog()) );

	loadAudioAct = new MyAction( this, "load_audio_file" );
	connect( loadAudioAct, SIGNAL(triggered()),
             this, SLOT(loadAudioFile()) );

	unloadAudioAct = new MyAction( this, "unload_audio_file" );
	connect( unloadAudioAct, SIGNAL(triggered()),
             core, SLOT(unloadAudioFile()) );


	// Submenu Filters
	extrastereoAct = new MyAction( this, "extrastereo_filter" );
	extrastereoAct->setCheckable( true );
	connect( extrastereoAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleExtrastereo(bool)) );

	karaokeAct = new MyAction( this, "karaoke_filter" );
	karaokeAct->setCheckable( true );
	connect( karaokeAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleKaraoke(bool)) );

	volnormAct = new MyAction( this, "volnorm_filter" );
	volnormAct->setCheckable( true );
	connect( volnormAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleVolnorm(bool)) );

#ifdef MPV_SUPPORT
	earwaxAct = new MyAction( this, "earwax_filter" );
	earwaxAct->setCheckable( true );
	connect( earwaxAct, SIGNAL(toggled(bool)),
             core, SLOT(toggleEarwax(bool)) );
#endif

	// Menu Subtitles
	loadSubsAct = new MyAction( this, "load_subs" );
	connect( loadSubsAct, SIGNAL(triggered()),
             this, SLOT(loadSub()) );

	unloadSubsAct = new MyAction( this, "unload_subs" );
	connect( unloadSubsAct, SIGNAL(triggered()),
             core, SLOT(unloadSub()) );

	decSubDelayAct = new MyAction( Qt::Key_Z, this, "dec_sub_delay" );
	connect( decSubDelayAct, SIGNAL(triggered()),
             core, SLOT(decSubDelay()) );

	incSubDelayAct = new MyAction( Qt::Key_X, this, "inc_sub_delay" );
	connect( incSubDelayAct, SIGNAL(triggered()),
             core, SLOT(incSubDelay()) );

	subDelayAct = new MyAction( this, "sub_delay" );
	connect( subDelayAct, SIGNAL(triggered()),
             this, SLOT(showSubDelayDialog()) );

	decSubPosAct = new MyAction( Qt::Key_R, this, "dec_sub_pos" );
	connect( decSubPosAct, SIGNAL(triggered()),
             core, SLOT(decSubPos()) );
	incSubPosAct = new MyAction( Qt::Key_T, this, "inc_sub_pos" );
	connect( incSubPosAct, SIGNAL(triggered()),
             core, SLOT(incSubPos()) );

	decSubScaleAct = new MyAction( Qt::SHIFT | Qt::Key_R, this, "dec_sub_scale" );
	connect( decSubScaleAct, SIGNAL(triggered()),
             core, SLOT(decSubScale()) );

	incSubScaleAct = new MyAction( Qt::SHIFT | Qt::Key_T, this, "inc_sub_scale" );
	connect( incSubScaleAct, SIGNAL(triggered()),
             core, SLOT(incSubScale()) );

	decSubStepAct = new MyAction( Qt::Key_G, this, "dec_sub_step" );
	connect( decSubStepAct, SIGNAL(triggered()),
             core, SLOT(decSubStep()) );

	incSubStepAct = new MyAction( Qt::Key_Y, this, "inc_sub_step" );
	connect( incSubStepAct, SIGNAL(triggered()),
             core, SLOT(incSubStep()) );

#ifdef MPV_SUPPORT
	seekNextSubAct = new MyAction(Qt::CTRL | Qt::Key_Right, this, "seek_next_sub");
	connect(seekNextSubAct, SIGNAL(triggered()),
            core, SLOT(seekToNextSub()));

	seekPrevSubAct = new MyAction(Qt::CTRL | Qt::Key_Left, this, "seek_prev_sub");
	connect(seekPrevSubAct, SIGNAL(triggered()),
            core, SLOT(seekToPrevSub()));
#endif

	useCustomSubStyleAct = new MyAction(this, "use_custom_sub_style");
	useCustomSubStyleAct->setCheckable(true);
	connect( useCustomSubStyleAct, SIGNAL(toggled(bool)), core, SLOT(changeUseCustomSubStyle(bool)) );

	useForcedSubsOnlyAct = new MyAction(this, "use_forced_subs_only");
	useForcedSubsOnlyAct->setCheckable(true);
	connect( useForcedSubsOnlyAct, SIGNAL(toggled(bool)), core, SLOT(toggleForcedSubsOnly(bool)) );

	subVisibilityAct = new MyAction(Qt::Key_V, this, "subtitle_visibility");
	subVisibilityAct->setCheckable(true);
	connect( subVisibilityAct, SIGNAL(toggled(bool)), core, SLOT(changeSubVisibility(bool)) );

#ifdef FIND_SUBTITLES
	showFindSubtitlesDialogAct = new MyAction( this, "show_find_sub_dialog" );
	connect( showFindSubtitlesDialogAct, SIGNAL(triggered()), 
             this, SLOT(showFindSubtitlesDialog()) );

	openUploadSubtitlesPageAct = new MyAction( this, "upload_subtitles" );		//turbos
	connect( openUploadSubtitlesPageAct, SIGNAL(triggered()),					//turbos
             this, SLOT(openUploadSubtitlesPage()) );							//turbos
#endif

	// Menu Options
	showPlaylistAct = new MyAction( QKeySequence("Ctrl+L"), this, "show_playlist" );
	showPlaylistAct->setCheckable( true );
	connect( showPlaylistAct, SIGNAL(toggled(bool)),
             this, SLOT(showPlaylist(bool)) );

	showPropertiesAct = new MyAction( QKeySequence("Ctrl+I"), this, "show_file_properties" );
	connect( showPropertiesAct, SIGNAL(triggered()),
             this, SLOT(showFilePropertiesDialog()) );

	showPreferencesAct = new MyAction( QKeySequence("Ctrl+P"), this, "show_preferences" );
	connect( showPreferencesAct, SIGNAL(triggered()),
             this, SLOT(showPreferencesDialog()) );

#ifdef YOUTUBE_SUPPORT
	showTubeBrowserAct = new MyAction( Qt::Key_F11, this, "show_tube_browser" );
	connect( showTubeBrowserAct, SIGNAL(triggered()),
             this, SLOT(showTubeBrowser()) );
#endif

	// Submenu Logs
#ifdef LOG_MPLAYER
	showLogMplayerAct = new MyAction( QKeySequence("Ctrl+M"), this, "show_mplayer_log" );
	connect( showLogMplayerAct, SIGNAL(triggered()),
             this, SLOT(showMplayerLog()) );
#endif

#ifdef LOG_SMPLAYER
	showLogSmplayerAct = new MyAction( QKeySequence("Ctrl+S"), this, "show_smplayer_log" );
	connect( showLogSmplayerAct, SIGNAL(triggered()),
             this, SLOT(showLog()) );
#endif

	tabletModeAct = new MyAction(this, "tablet_mode");
	tabletModeAct->setCheckable(true);
	connect(tabletModeAct, SIGNAL(toggled(bool)), this, SLOT(setTabletMode(bool)));


	// Menu Help
	showFirstStepsAct = new MyAction( this, "first_steps" );
	connect( showFirstStepsAct, SIGNAL(triggered()),
             this, SLOT(helpFirstSteps()) );

	showFAQAct = new MyAction( this, "faq" );
	connect( showFAQAct, SIGNAL(triggered()),
             this, SLOT(helpFAQ()) );

	showCLOptionsAct = new MyAction( this, "cl_options" );
	connect( showCLOptionsAct, SIGNAL(triggered()),
             this, SLOT(helpCLOptions()) );

	showCheckUpdatesAct = new MyAction( this, "check_updates" );
	connect( showCheckUpdatesAct, SIGNAL(triggered()),
             this, SLOT(helpCheckUpdates()) );

#if defined(YOUTUBE_SUPPORT) && defined(YT_USE_YTSIG)
	updateYTAct = new MyAction( this, "update_youtube" );
	connect( updateYTAct, SIGNAL(triggered()),
             this, SLOT(YTUpdateScript()) );
#endif

	showConfigAct = new MyAction( this, "show_config" );
	connect( showConfigAct, SIGNAL(triggered()),
             this, SLOT(helpShowConfig()) );

	donateAct = new MyAction( this, "donate" );
	connect( donateAct, SIGNAL(triggered()),
             this, SLOT(helpDonate()) );

	aboutThisAct = new MyAction( this, "about_smplayer" );
	connect( aboutThisAct, SIGNAL(triggered()),
             this, SLOT(helpAbout()) );

#ifdef SHARE_MENU
	facebookAct = new MyAction (this, "facebook");
	twitterAct = new MyAction (this, "twitter");
	gmailAct = new MyAction (this, "gmail");
	hotmailAct = new MyAction (this, "hotmail");
	yahooAct = new MyAction (this, "yahoo");

	connect( facebookAct, SIGNAL(triggered()),
             this, SLOT(shareSMPlayer()) );
	connect( twitterAct, SIGNAL(triggered()),
             this, SLOT(shareSMPlayer()) );
	connect( gmailAct, SIGNAL(triggered()),
             this, SLOT(shareSMPlayer()) );
	connect( hotmailAct, SIGNAL(triggered()),
             this, SLOT(shareSMPlayer()) );
	connect( yahooAct, SIGNAL(triggered()),
             this, SLOT(shareSMPlayer()) );
#endif

	// OSD
	incOSDScaleAct = new MyAction(Qt::SHIFT | Qt::Key_U, this, "inc_osd_scale");
	connect(incOSDScaleAct, SIGNAL(triggered()), core, SLOT(incOSDScale()));

	decOSDScaleAct = new MyAction(Qt::SHIFT | Qt::Key_Y, this, "dec_osd_scale");
	connect(decOSDScaleAct, SIGNAL(triggered()), core, SLOT(decOSDScale()));

#ifdef MPV_SUPPORT
	OSDFractionsAct = new MyAction(this, "osd_fractions");
	OSDFractionsAct->setCheckable(true);
	connect(OSDFractionsAct, SIGNAL(toggled(bool)), core, SLOT(setOSDFractions(bool)));
#endif

	// Playlist
	playNextAct = new MyAction(Qt::Key_Greater, this, "play_next");
	playNextAct->addShortcut(Qt::Key_MediaNext); // MCE remote key
	connect( playNextAct, SIGNAL(triggered()), playlist, SLOT(playNext()) );

	playPrevAct = new MyAction(Qt::Key_Less, this, "play_prev");
	playPrevAct->addShortcut(Qt::Key_MediaPrevious); // MCE remote key
	connect( playPrevAct, SIGNAL(triggered()), playlist, SLOT(playPrev()) );


	// Move video window and zoom
	moveUpAct = new MyAction(Qt::ALT | Qt::Key_Up, this, "move_up");
	connect( moveUpAct, SIGNAL(triggered()), mplayerwindow, SLOT(moveUp()) );

	moveDownAct = new MyAction(Qt::ALT | Qt::Key_Down, this, "move_down");
	connect( moveDownAct, SIGNAL(triggered()), mplayerwindow, SLOT(moveDown()) );

	moveLeftAct = new MyAction(Qt::ALT | Qt::Key_Left, this, "move_left");
	connect( moveLeftAct, SIGNAL(triggered()), mplayerwindow, SLOT(moveLeft()) );

	moveRightAct = new MyAction(Qt::ALT | Qt::Key_Right, this, "move_right");
	connect( moveRightAct, SIGNAL(triggered()), mplayerwindow, SLOT(moveRight()) );

	incZoomAct = new MyAction(Qt::Key_E, this, "inc_zoom");
	connect( incZoomAct, SIGNAL(triggered()), core, SLOT(incZoom()) );

	decZoomAct = new MyAction(Qt::Key_W, this, "dec_zoom");
	connect( decZoomAct, SIGNAL(triggered()), core, SLOT(decZoom()) );

	resetZoomAct = new MyAction(Qt::SHIFT | Qt::Key_E, this, "reset_zoom");
	connect( resetZoomAct, SIGNAL(triggered()), core, SLOT(resetZoom()) );

	autoZoomAct = new MyAction(Qt::SHIFT | Qt::Key_W, this, "auto_zoom");
	connect( autoZoomAct, SIGNAL(triggered()), core, SLOT(autoZoom()) );

	autoZoom169Act = new MyAction(Qt::SHIFT | Qt::Key_A, this, "zoom_169");
	connect( autoZoom169Act, SIGNAL(triggered()), core, SLOT(autoZoomFor169()) );

	autoZoom235Act = new MyAction(Qt::SHIFT | Qt::Key_S, this, "zoom_235");
	connect( autoZoom235Act, SIGNAL(triggered()), core, SLOT(autoZoomFor235()) );


	// Actions not in menus or buttons
	// Volume 2
#if !USE_MULTIPLE_SHORTCUTS
	decVolume2Act = new MyAction( Qt::Key_Slash, this, "dec_volume2" );
	connect( decVolume2Act, SIGNAL(triggered()), core, SLOT(decVolume()) );

	incVolume2Act = new MyAction( Qt::Key_Asterisk, this, "inc_volume2" );
	connect( incVolume2Act, SIGNAL(triggered()), core, SLOT(incVolume()) );
#endif
	// Exit fullscreen
	exitFullscreenAct = new MyAction( Qt::Key_Escape, this, "exit_fullscreen" );
	connect( exitFullscreenAct, SIGNAL(triggered()), this, SLOT(exitFullscreen()) );

	nextOSDAct = new MyAction( Qt::Key_O, this, "next_osd");
	connect( nextOSDAct, SIGNAL(triggered()), core, SLOT(nextOSD()) );

	decContrastAct = new MyAction( Qt::Key_1, this, "dec_contrast");
	connect( decContrastAct, SIGNAL(triggered()), core, SLOT(decContrast()) );

	incContrastAct = new MyAction( Qt::Key_2, this, "inc_contrast");
	connect( incContrastAct, SIGNAL(triggered()), core, SLOT(incContrast()) );

	decBrightnessAct = new MyAction( Qt::Key_3, this, "dec_brightness");
	connect( decBrightnessAct, SIGNAL(triggered()), core, SLOT(decBrightness()) );

	incBrightnessAct = new MyAction( Qt::Key_4, this, "inc_brightness");
	connect( incBrightnessAct, SIGNAL(triggered()), core, SLOT(incBrightness()) );

	decHueAct = new MyAction(Qt::Key_5, this, "dec_hue");
	connect( decHueAct, SIGNAL(triggered()), core, SLOT(decHue()) );

	incHueAct = new MyAction( Qt::Key_6, this, "inc_hue");
	connect( incHueAct, SIGNAL(triggered()), core, SLOT(incHue()) );

	decSaturationAct = new MyAction( Qt::Key_7, this, "dec_saturation");
	connect( decSaturationAct, SIGNAL(triggered()), core, SLOT(decSaturation()) );

	incSaturationAct = new MyAction( Qt::Key_8, this, "inc_saturation");
	connect( incSaturationAct, SIGNAL(triggered()), core, SLOT(incSaturation()) );

	decGammaAct = new MyAction( this, "dec_gamma");
	connect( decGammaAct, SIGNAL(triggered()), core, SLOT(decGamma()) );

	incGammaAct = new MyAction( this, "inc_gamma");
	connect( incGammaAct, SIGNAL(triggered()), core, SLOT(incGamma()) );

	nextVideoAct = new MyAction( this, "next_video");
	connect( nextVideoAct, SIGNAL(triggered()), core, SLOT(nextVideo()) );

	nextAudioAct = new MyAction( Qt::Key_K, this, "next_audio");
	connect( nextAudioAct, SIGNAL(triggered()), core, SLOT(nextAudio()) );

	nextSubtitleAct = new MyAction( Qt::Key_J, this, "next_subtitle");
	connect( nextSubtitleAct, SIGNAL(triggered()), core, SLOT(nextSubtitle()) );

	nextChapterAct = new MyAction( Qt::Key_At, this, "next_chapter");
	connect( nextChapterAct, SIGNAL(triggered()), core, SLOT(nextChapter()) );

	prevChapterAct = new MyAction( Qt::Key_Exclam, this, "prev_chapter");
	connect( prevChapterAct, SIGNAL(triggered()), core, SLOT(prevChapter()) );

	doubleSizeAct = new MyAction( Qt::CTRL | Qt::Key_D, this, "toggle_double_size");
	connect( doubleSizeAct, SIGNAL(triggered()), this, SLOT(toggleDoubleSize()) );

	resetVideoEqualizerAct = new MyAction( this, "reset_video_equalizer");
	connect( resetVideoEqualizerAct, SIGNAL(triggered()), video_equalizer, SLOT(reset()) );

	resetAudioEqualizerAct = new MyAction( this, "reset_audio_equalizer");
	connect( resetAudioEqualizerAct, SIGNAL(triggered()), audio_equalizer, SLOT(reset()) );

	showContextMenuAct = new MyAction( this, "show_context_menu");
	connect( showContextMenuAct, SIGNAL(triggered()), 
             this, SLOT(showPopupMenu()) );

	nextAspectAct = new MyAction( Qt::Key_A, this, "next_aspect");
	connect( nextAspectAct, SIGNAL(triggered()), 
             core, SLOT(nextAspectRatio()) );

	nextWheelFunctionAct = new MyAction(this, "next_wheel_function");
	connect( nextWheelFunctionAct, SIGNAL(triggered()),
			 core, SLOT(nextWheelFunction()) );

	showFilenameAct = new MyAction(Qt::SHIFT | Qt::Key_O, this, "show_filename_osd");
	connect( showFilenameAct, SIGNAL(triggered()), core, SLOT(showFilenameOnOSD()) );

	showMediaInfoAct = new MyAction(Qt::SHIFT | Qt::Key_I, this, "show_info_osd");
	connect( showMediaInfoAct, SIGNAL(triggered()), core, SLOT(showMediaInfoOnOSD()) );

	showTimeAct = new MyAction(Qt::Key_I, this, "show_time");
	connect( showTimeAct, SIGNAL(triggered()), core, SLOT(showTimeOnOSD()) );

	toggleDeinterlaceAct = new MyAction(Qt::Key_D, this, "toggle_deinterlacing");
	connect( toggleDeinterlaceAct, SIGNAL(triggered()), core, SLOT(toggleDeinterlace()) );


	// Group actions

	// OSD
	osdGroup = new MyActionGroup(this);
	osdNoneAct = new MyActionGroupItem(this, osdGroup, "osd_none", Preferences::None);
	osdSeekAct = new MyActionGroupItem(this, osdGroup, "osd_seek", Preferences::Seek);
	osdTimerAct = new MyActionGroupItem(this, osdGroup, "osd_timer", Preferences::SeekTimer);
	osdTotalAct = new MyActionGroupItem(this, osdGroup, "osd_total", Preferences::SeekTimerTotal);
	connect( osdGroup, SIGNAL(activated(int)), core, SLOT(changeOSD(int)) );

	// Denoise
	denoiseGroup = new MyActionGroup(this);
	denoiseNoneAct = new MyActionGroupItem(this, denoiseGroup, "denoise_none", MediaSettings::NoDenoise);
	denoiseNormalAct = new MyActionGroupItem(this, denoiseGroup, "denoise_normal", MediaSettings::DenoiseNormal);
	denoiseSoftAct = new MyActionGroupItem(this, denoiseGroup, "denoise_soft", MediaSettings::DenoiseSoft);
	connect( denoiseGroup, SIGNAL(activated(int)), core, SLOT(changeDenoise(int)) );

	// Unsharp group
	unsharpGroup = new MyActionGroup(this);
	unsharpNoneAct = new MyActionGroupItem(this, unsharpGroup, "unsharp_off", 0);
	blurAct = new MyActionGroupItem(this, unsharpGroup, "blur", 1);
	sharpenAct = new MyActionGroupItem(this, unsharpGroup, "sharpen", 2);
	connect( unsharpGroup, SIGNAL(activated(int)), core, SLOT(changeUnsharp(int)) );

	// Video size
	sizeGroup = new MyActionGroup(this);
	size50 = new MyActionGroupItem(this, sizeGroup, "5&0%", "size_50", 50);
	size75 = new MyActionGroupItem(this, sizeGroup, "7&5%", "size_75", 75);
	size100 = new MyActionGroupItem(this, sizeGroup, "&100%", "size_100", 100);
	size125 = new MyActionGroupItem(this, sizeGroup, "1&25%", "size_125", 125);
	size150 = new MyActionGroupItem(this, sizeGroup, "15&0%", "size_150", 150);
	size175 = new MyActionGroupItem(this, sizeGroup, "1&75%", "size_175", 175);
	size200 = new MyActionGroupItem(this, sizeGroup, "&200%", "size_200", 200);
	size300 = new MyActionGroupItem(this, sizeGroup, "&300%", "size_300", 300);
	size400 = new MyActionGroupItem(this, sizeGroup, "&400%", "size_400", 400);
	size100->setShortcut( Qt::CTRL | Qt::Key_1 );
	size200->setShortcut( Qt::CTRL | Qt::Key_2 );
	connect( sizeGroup, SIGNAL(activated(int)), this, SLOT(changeSizeFactor(int)) );
	// Make all not checkable
	QList <QAction *> size_list = sizeGroup->actions();
	for (int n=0; n < size_list.count(); n++) {
		size_list[n]->setCheckable(false);
	}

	// Deinterlace
	deinterlaceGroup = new MyActionGroup(this);
	deinterlaceNoneAct = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_none", MediaSettings::NoDeinterlace);
	deinterlaceL5Act = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_l5", MediaSettings::L5);
	deinterlaceYadif0Act = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_yadif0", MediaSettings::Yadif);
	deinterlaceYadif1Act = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_yadif1", MediaSettings::Yadif_1);
	deinterlaceLBAct = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_lb", MediaSettings::LB);
	deinterlaceKernAct = new MyActionGroupItem(this, deinterlaceGroup, "deinterlace_kern", MediaSettings::Kerndeint);
	connect( deinterlaceGroup, SIGNAL(activated(int)),
             core, SLOT(changeDeinterlace(int)) );

	// Audio channels
	channelsGroup = new MyActionGroup(this);
	channelsDefaultAct = new MyActionGroupItem(this, channelsGroup, "channels_default", MediaSettings::ChDefault);
	channelsStereoAct = new MyActionGroupItem(this, channelsGroup, "channels_stereo", MediaSettings::ChStereo);
	channelsSurroundAct = new MyActionGroupItem(this, channelsGroup, "channels_surround", MediaSettings::ChSurround);
	channelsFull51Act = new MyActionGroupItem(this, channelsGroup, "channels_ful51", MediaSettings::ChFull51);
	channelsFull61Act = new MyActionGroupItem(this, channelsGroup, "channels_ful61", MediaSettings::ChFull61);
	channelsFull71Act = new MyActionGroupItem(this, channelsGroup, "channels_ful71", MediaSettings::ChFull71);
	connect( channelsGroup, SIGNAL(activated(int)),
             core, SLOT(setAudioChannels(int)) );

	// Stereo mode
	stereoGroup = new MyActionGroup(this);
	stereoAct = new MyActionGroupItem(this, stereoGroup, "stereo", MediaSettings::Stereo);
	leftChannelAct = new MyActionGroupItem(this, stereoGroup, "left_channel", MediaSettings::Left);
	rightChannelAct = new MyActionGroupItem(this, stereoGroup, "right_channel", MediaSettings::Right);
	monoAct = new MyActionGroupItem(this, stereoGroup, "mono", MediaSettings::Mono);
	reverseAct = new MyActionGroupItem(this, stereoGroup, "reverse_channels", MediaSettings::Reverse);
	connect( stereoGroup, SIGNAL(activated(int)),
             core, SLOT(setStereoMode(int)) );

	// Video aspect
	aspectGroup = new MyActionGroup(this);
	aspectDetectAct = new MyActionGroupItem(this, aspectGroup, "aspect_detect", MediaSettings::AspectAuto);
	aspect11Act = new MyActionGroupItem(this, aspectGroup, "aspect_1:1", MediaSettings::Aspect11 );
	aspect54Act = new MyActionGroupItem(this, aspectGroup, "aspect_5:4", MediaSettings::Aspect54 );
	aspect43Act = new MyActionGroupItem(this, aspectGroup, "aspect_4:3", MediaSettings::Aspect43);
	aspect118Act = new MyActionGroupItem(this, aspectGroup, "aspect_11:8", MediaSettings::Aspect118 );
	aspect1410Act = new MyActionGroupItem(this, aspectGroup, "aspect_14:10", MediaSettings::Aspect1410 );
	aspect32Act = new MyActionGroupItem(this, aspectGroup, "aspect_3:2", MediaSettings::Aspect32);
	aspect149Act = new MyActionGroupItem(this, aspectGroup, "aspect_14:9", MediaSettings::Aspect149 );
	aspect1610Act = new MyActionGroupItem(this, aspectGroup, "aspect_16:10", MediaSettings::Aspect1610 );
	aspect169Act = new MyActionGroupItem(this, aspectGroup, "aspect_16:9", MediaSettings::Aspect169 );
	aspect235Act = new MyActionGroupItem(this, aspectGroup, "aspect_2.35:1", MediaSettings::Aspect235 );
	{
		QAction * sep = new QAction(aspectGroup);
		sep->setSeparator(true);
	}
	aspectNoneAct = new MyActionGroupItem(this, aspectGroup, "aspect_none", MediaSettings::AspectNone);

	connect( aspectGroup, SIGNAL(activated(int)),
             core, SLOT(changeAspectRatio(int)) );

	// Rotate
	rotateGroup = new MyActionGroup(this);
	rotateNoneAct = new MyActionGroupItem(this, rotateGroup, "rotate_none", MediaSettings::NoRotate);
	rotateClockwiseFlipAct = new MyActionGroupItem(this, rotateGroup, "rotate_clockwise_flip", MediaSettings::Clockwise_flip);
	rotateClockwiseAct = new MyActionGroupItem(this, rotateGroup, "rotate_clockwise", MediaSettings::Clockwise);
	rotateCounterclockwiseAct = new MyActionGroupItem(this, rotateGroup, "rotate_counterclockwise", MediaSettings::Counterclockwise);
	rotateCounterclockwiseFlipAct = new MyActionGroupItem(this, rotateGroup, "rotate_counterclockwise_flip", MediaSettings::Counterclockwise_flip);
	connect( rotateGroup, SIGNAL(activated(int)),
             core, SLOT(changeRotate(int)) );

	// On Top
	onTopActionGroup = new MyActionGroup(this);
	onTopAlwaysAct = new MyActionGroupItem( this,onTopActionGroup,"on_top_always",Preferences::AlwaysOnTop);
	onTopNeverAct = new MyActionGroupItem( this,onTopActionGroup,"on_top_never",Preferences::NeverOnTop);
	onTopWhilePlayingAct = new MyActionGroupItem( this,onTopActionGroup,"on_top_playing",Preferences::WhilePlayingOnTop);
	connect( onTopActionGroup , SIGNAL(activated(int)),
             this, SLOT(changeStayOnTop(int)) );

	toggleStayOnTopAct = new MyAction( this, "toggle_stay_on_top");
	connect( toggleStayOnTopAct, SIGNAL(triggered()), this, SLOT(toggleStayOnTop()) );


#if USE_ADAPTER
	screenGroup = new MyActionGroup(this);
	screenDefaultAct = new MyActionGroupItem(this, screenGroup, "screen_default", -1);
	#ifdef Q_OS_WIN
	DeviceList display_devices = DeviceInfo::displayDevices();
	if (!display_devices.isEmpty()) {
		for (int n = 0; n < display_devices.count(); n++) {
			int id = display_devices[n].ID().toInt();
			QString desc = display_devices[n].desc();
			MyAction * screen_item = new MyActionGroupItem(this, screenGroup, QString("screen_%1").arg(n).toLatin1().constData(), id);
			screen_item->change( "&"+QString::number(n) + " - " + desc);
		}
	}
	else
	#endif // Q_OS_WIN
	for (int n = 1; n <= 4; n++) {
		MyAction * screen_item = new MyActionGroupItem(this, screenGroup, QString("screen_%1").arg(n).toLatin1().constData(), n);
		screen_item->change( "&"+QString::number(n) );
	}

	connect( screenGroup, SIGNAL(activated(int)),
             core, SLOT(changeAdapter(int)) );
#endif

#if PROGRAM_SWITCH
	// Program track
	programTrackGroup = new MyActionGroup(this);
	connect( programTrackGroup, SIGNAL(activated(int)), 
	         core, SLOT(changeProgram(int)) );
#endif

	// Video track
	videoTrackGroup = new MyActionGroup(this);
	connect( videoTrackGroup, SIGNAL(activated(int)), 
	         core, SLOT(changeVideo(int)) );

	// Audio track
	audioTrackGroup = new MyActionGroup(this);
	connect( audioTrackGroup, SIGNAL(activated(int)), 
	         core, SLOT(changeAudio(int)) );

	// Subtitle track
	subtitleTrackGroup = new MyActionGroup(this);
	connect( subtitleTrackGroup, SIGNAL(activated(int)), 
	         core, SLOT(changeSubtitle(int)) );

#ifdef MPV_SUPPORT
	// Secondary subtitle track
	secondarySubtitleTrackGroup = new MyActionGroup(this);
	connect( secondarySubtitleTrackGroup, SIGNAL(activated(int)), 
	         core, SLOT(changeSecondarySubtitle(int)) );
#endif

	ccGroup = new MyActionGroup(this);
	ccNoneAct = new MyActionGroupItem(this, ccGroup, "cc_none", 0);
	ccChannel1Act = new MyActionGroupItem(this, ccGroup, "cc_ch_1", 1);
	ccChannel2Act = new MyActionGroupItem(this, ccGroup, "cc_ch_2", 2);
	ccChannel3Act = new MyActionGroupItem(this, ccGroup, "cc_ch_3", 3);
	ccChannel4Act = new MyActionGroupItem(this, ccGroup, "cc_ch_4", 4);
	connect( ccGroup, SIGNAL(activated(int)),
             core, SLOT(changeClosedCaptionChannel(int)) );

	subFPSGroup = new MyActionGroup(this);
	subFPSNoneAct = new MyActionGroupItem(this, subFPSGroup, "sub_fps_none", MediaSettings::SFPS_None);
	/* subFPS23Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_23", MediaSettings::SFPS_23); */
	subFPS23976Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_23976", MediaSettings::SFPS_23976);
	subFPS24Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_24", MediaSettings::SFPS_24);
	subFPS25Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_25", MediaSettings::SFPS_25);
	subFPS29970Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_29970", MediaSettings::SFPS_29970);
	subFPS30Act = new MyActionGroupItem(this, subFPSGroup, "sub_fps_30", MediaSettings::SFPS_30);
	connect( subFPSGroup, SIGNAL(activated(int)),
             core, SLOT(changeExternalSubFPS(int)) );

	// Titles
	titleGroup = new MyActionGroup(this);
	connect( titleGroup, SIGNAL(activated(int)),
			 core, SLOT(changeTitle(int)) );

	// Angles
	angleGroup = new MyActionGroup(this);
	connect( angleGroup, SIGNAL(activated(int)),
			 core, SLOT(changeAngle(int)) );

	// Chapters
	chapterGroup = new MyActionGroup(this);
	connect( chapterGroup, SIGNAL(activated(int)),
			 core, SLOT(changeChapter(int)) );

#ifdef BOOKMARKS
	// Bookmarks
	bookmarkGroup = new MyActionGroup(this);
	connect( bookmarkGroup, SIGNAL(activated(int)),
			 core, SLOT(goToSec(int)) );

	addBookmarkAct = new MyAction(Qt::CTRL | Qt::Key_A, this, "add_bookmark");
	connect(addBookmarkAct, SIGNAL(triggered()), this, SLOT(showAddBookmarkDialog()));

	editBookmarksAct = new MyAction(this, "edit_bookmarks");
	connect(editBookmarksAct, SIGNAL(triggered()), this, SLOT(showBookmarkDialog()));

	prevBookmarkAct = new MyAction(Qt::CTRL | Qt::Key_B, this, "prev_bookmark");
	connect(prevBookmarkAct, SIGNAL(triggered()), core, SLOT(prevBookmark()));

	nextBookmarkAct = new MyAction(Qt::CTRL | Qt::Key_N, this, "next_bookmark");
	connect(nextBookmarkAct, SIGNAL(triggered()), core, SLOT(nextBookmark()));
#endif

#if DVDNAV_SUPPORT
	dvdnavUpAct = new MyAction(Qt::SHIFT | Qt::Key_Up, this, "dvdnav_up");
	connect( dvdnavUpAct, SIGNAL(triggered()), core, SLOT(dvdnavUp()) );

	dvdnavDownAct = new MyAction(Qt::SHIFT | Qt::Key_Down, this, "dvdnav_down");
	connect( dvdnavDownAct, SIGNAL(triggered()), core, SLOT(dvdnavDown()) );

	dvdnavLeftAct = new MyAction(Qt::SHIFT | Qt::Key_Left, this, "dvdnav_left");
	connect( dvdnavLeftAct, SIGNAL(triggered()), core, SLOT(dvdnavLeft()) );

	dvdnavRightAct = new MyAction(Qt::SHIFT | Qt::Key_Right, this, "dvdnav_right");
	connect( dvdnavRightAct, SIGNAL(triggered()), core, SLOT(dvdnavRight()) );

	dvdnavMenuAct = new MyAction(Qt::SHIFT | Qt::Key_Return, this, "dvdnav_menu");
	connect( dvdnavMenuAct, SIGNAL(triggered()), core, SLOT(dvdnavMenu()) );

	dvdnavSelectAct = new MyAction(Qt::Key_Return, this, "dvdnav_select");
	connect( dvdnavSelectAct, SIGNAL(triggered()), core, SLOT(dvdnavSelect()) );

	dvdnavPrevAct = new MyAction(Qt::SHIFT | Qt::Key_Escape, this, "dvdnav_prev");
	connect( dvdnavPrevAct, SIGNAL(triggered()), core, SLOT(dvdnavPrev()) );

	dvdnavMouseAct = new MyAction( this, "dvdnav_mouse");
	connect( dvdnavMouseAct, SIGNAL(triggered()), core, SLOT(dvdnavMouse()) );
#endif

}

#if AUTODISABLE_ACTIONS
void BaseGui::setActionsEnabled(bool b) {
	// Menu Play
	playAct->setEnabled(b);
	playOrPauseAct->setEnabled(b);
	pauseAct->setEnabled(b);
	pauseAndStepAct->setEnabled(b);
	stopAct->setEnabled(b);
	frameStepAct->setEnabled(b);
	frameBackStepAct->setEnabled(b);
	rewind1Act->setEnabled(b);
	rewind2Act->setEnabled(b);
	rewind3Act->setEnabled(b);
	forward1Act->setEnabled(b);
	forward2Act->setEnabled(b);
	forward3Act->setEnabled(b);
	//repeatAct->setEnabled(b);
	gotoAct->setEnabled(b);

	// Menu Speed
	normalSpeedAct->setEnabled(b);
	halveSpeedAct->setEnabled(b);
	doubleSpeedAct->setEnabled(b);
	decSpeed10Act->setEnabled(b);
	incSpeed10Act->setEnabled(b);
	decSpeed4Act->setEnabled(b);
	incSpeed4Act->setEnabled(b);
	decSpeed1Act->setEnabled(b);
	incSpeed1Act->setEnabled(b);

	// Menu Video
	videoEqualizerAct->setEnabled(b);
	screenshotAct->setEnabled(b);
	screenshotsAct->setEnabled(b);
	screenshotWithSubsAct->setEnabled(b);
	screenshotWithNoSubsAct->setEnabled(b);
#ifdef CAPTURE_STREAM
	capturingAct->setEnabled(b);
#endif
	flipAct->setEnabled(b);
	mirrorAct->setEnabled(b);
	stereo3dAct->setEnabled(b);
	postProcessingAct->setEnabled(b);
	phaseAct->setEnabled(b);
	deblockAct->setEnabled(b);
	deringAct->setEnabled(b);
	gradfunAct->setEnabled(b);
	addNoiseAct->setEnabled(b);
	addLetterboxAct->setEnabled(b);
	upscaleAct->setEnabled(b);

	// Menu Audio
	audioEqualizerAct->setEnabled(b);
	muteAct->setEnabled(b);
	decVolumeAct->setEnabled(b);
	incVolumeAct->setEnabled(b);
	decAudioDelayAct->setEnabled(b);
	incAudioDelayAct->setEnabled(b);
	audioDelayAct->setEnabled(b);
	extrastereoAct->setEnabled(b);
	karaokeAct->setEnabled(b);
	volnormAct->setEnabled(b);
#ifdef MPV_SUPPORT
	earwaxAct->setEnabled(b);
#endif
	loadAudioAct->setEnabled(b);
	//unloadAudioAct->setEnabled(b);

	// Menu Subtitles
	loadSubsAct->setEnabled(b);
	//unloadSubsAct->setEnabled(b);
	decSubDelayAct->setEnabled(b);
	incSubDelayAct->setEnabled(b);
	subDelayAct->setEnabled(b);
	decSubPosAct->setEnabled(b);
	incSubPosAct->setEnabled(b);
	incSubStepAct->setEnabled(b);
	decSubStepAct->setEnabled(b);
	incSubScaleAct->setEnabled(b);
	decSubScaleAct->setEnabled(b);
#ifdef MPV_SUPPORT
	seekNextSubAct->setEnabled(b);
	seekPrevSubAct->setEnabled(b);
#endif

	// Actions not in menus
#if !USE_MULTIPLE_SHORTCUTS
	decVolume2Act->setEnabled(b);
	incVolume2Act->setEnabled(b);
#endif
	decContrastAct->setEnabled(b);
	incContrastAct->setEnabled(b);
	decBrightnessAct->setEnabled(b);
	incBrightnessAct->setEnabled(b);
	decHueAct->setEnabled(b);
	incHueAct->setEnabled(b);
	decSaturationAct->setEnabled(b);
	incSaturationAct->setEnabled(b);
	decGammaAct->setEnabled(b);
	incGammaAct->setEnabled(b);
	nextVideoAct->setEnabled(b);
	nextAudioAct->setEnabled(b);
	nextSubtitleAct->setEnabled(b);
	nextChapterAct->setEnabled(b);
	prevChapterAct->setEnabled(b);
	doubleSizeAct->setEnabled(b);

	// Moving and zoom
	moveUpAct->setEnabled(b);
	moveDownAct->setEnabled(b);
	moveLeftAct->setEnabled(b);
	moveRightAct->setEnabled(b);
	incZoomAct->setEnabled(b);
	decZoomAct->setEnabled(b);
	resetZoomAct->setEnabled(b);
	autoZoomAct->setEnabled(b);
	autoZoom169Act->setEnabled(b);
	autoZoom235Act->setEnabled(b);

#if DVDNAV_SUPPORT
	dvdnavUpAct->setEnabled(b);
	dvdnavDownAct->setEnabled(b);
	dvdnavLeftAct->setEnabled(b);
	dvdnavRightAct->setEnabled(b);
	dvdnavMenuAct->setEnabled(b);
	dvdnavSelectAct->setEnabled(b);
	dvdnavPrevAct->setEnabled(b);
	dvdnavMouseAct->setEnabled(b);
#endif

	// Groups
	denoiseGroup->setActionsEnabled(b);
	unsharpGroup->setActionsEnabled(b);
	sizeGroup->setActionsEnabled(b);
	deinterlaceGroup->setActionsEnabled(b);
	aspectGroup->setActionsEnabled(b);
	rotateGroup->setActionsEnabled(b);
#if USE_ADAPTER
	screenGroup->setActionsEnabled(b);
#endif
	channelsGroup->setActionsEnabled(b);
	stereoGroup->setActionsEnabled(b);
}

void BaseGui::enableActionsOnPlaying() {
	qDebug("BaseGui::enableActionsOnPlaying");

	setActionsEnabled(true);

	playAct->setEnabled(false);

	// Screenshot option
	/*
	bool screenshots_enabled = ( (pref->use_screenshot) && 
                                 (!pref->screenshot_directory.isEmpty()) &&
                                 (QFileInfo(pref->screenshot_directory).isDir()) );

	screenshotAct->setEnabled( screenshots_enabled );
	screenshotsAct->setEnabled( screenshots_enabled );
	*/

#ifdef CAPTURE_STREAM
	capturingAct->setEnabled(!pref->capture_directory.isEmpty() && QFileInfo(pref->capture_directory).isDir());
#endif

	// Disable the compact action if not using video window
	compactAct->setEnabled( panel->isVisible() );

	// Enable or disable the audio equalizer
	audioEqualizerAct->setEnabled(pref->use_audio_equalizer);

	// Disable audio actions if there's not audio track
	if ((core->mdat.audios.numItems()==0) && (core->mset.external_audio.isEmpty())) {
		audioEqualizerAct->setEnabled(false);
		muteAct->setEnabled(false);
		decVolumeAct->setEnabled(false);
		incVolumeAct->setEnabled(false);
		decAudioDelayAct->setEnabled(false);
		incAudioDelayAct->setEnabled(false);
		audioDelayAct->setEnabled(false);
		extrastereoAct->setEnabled(false);
		karaokeAct->setEnabled(false);
		volnormAct->setEnabled(false);
#ifdef MPV_SUPPORT
		earwaxAct->setEnabled(false);
#endif
		channelsGroup->setActionsEnabled(false);
		stereoGroup->setActionsEnabled(false);
	}

	// Disable video actions if it's an audio file
	if (core->mdat.novideo) {
		videoEqualizerAct->setEnabled(false);
		screenshotAct->setEnabled(false);
		screenshotsAct->setEnabled(false);
		screenshotWithSubsAct->setEnabled(false);
		screenshotWithNoSubsAct->setEnabled(false);
#ifdef CAPTURE_STREAM
		capturingAct->setEnabled(false);
#endif
		flipAct->setEnabled(false);
		mirrorAct->setEnabled(false);
		stereo3dAct->setEnabled(false);
		postProcessingAct->setEnabled(false);
		phaseAct->setEnabled(false);
		deblockAct->setEnabled(false);
		deringAct->setEnabled(false);
		gradfunAct->setEnabled(false);
		addNoiseAct->setEnabled(false);
		addLetterboxAct->setEnabled(false);
		upscaleAct->setEnabled(false);
		doubleSizeAct->setEnabled(false);

		// Moving and zoom
		moveUpAct->setEnabled(false);
		moveDownAct->setEnabled(false);
		moveLeftAct->setEnabled(false);
		moveRightAct->setEnabled(false);
		incZoomAct->setEnabled(false);
		decZoomAct->setEnabled(false);
		resetZoomAct->setEnabled(false);
		autoZoomAct->setEnabled(false);
		autoZoom169Act->setEnabled(false);
		autoZoom235Act->setEnabled(false);

		denoiseGroup->setActionsEnabled(false);
		unsharpGroup->setActionsEnabled(false);
		sizeGroup->setActionsEnabled(false);
		deinterlaceGroup->setActionsEnabled(false);
		aspectGroup->setActionsEnabled(false);
		rotateGroup->setActionsEnabled(false);
#if USE_ADAPTER
		screenGroup->setActionsEnabled(false);
#endif
	}

#if USE_ADAPTER
	screenGroup->setActionsEnabled(pref->vo.startsWith(OVERLAY_VO));
#endif

#ifndef Q_OS_WIN
	// Disable video filters if using vdpau
	if ((pref->vdpau.disable_video_filters) && (pref->vo.startsWith("vdpau"))) {
		/*
		screenshotAct->setEnabled(false);
		screenshotsAct->setEnabled(false);
		*/
		flipAct->setEnabled(false);
		mirrorAct->setEnabled(false);
		stereo3dAct->setEnabled(false);
		postProcessingAct->setEnabled(false);
		phaseAct->setEnabled(false);
		deblockAct->setEnabled(false);
		deringAct->setEnabled(false);
		gradfunAct->setEnabled(false);
		addNoiseAct->setEnabled(false);
		addLetterboxAct->setEnabled(false);
		upscaleAct->setEnabled(false);

		deinterlaceGroup->setActionsEnabled(false);
		rotateGroup->setActionsEnabled(false);
		denoiseGroup->setActionsEnabled(false);
		unsharpGroup->setActionsEnabled(false);

		displayMessage( tr("Video filters are disabled when using vdpau") );
	}
#endif

#if DVDNAV_SUPPORT
	if (!core->mdat.filename.startsWith("dvdnav:")) {
		dvdnavUpAct->setEnabled(false);
		dvdnavDownAct->setEnabled(false);
		dvdnavLeftAct->setEnabled(false);
		dvdnavRightAct->setEnabled(false);
		dvdnavMenuAct->setEnabled(false);
		dvdnavSelectAct->setEnabled(false);
		dvdnavPrevAct->setEnabled(false);
		dvdnavMouseAct->setEnabled(false);
	}
#endif


#ifdef BOOKMARKS
	bool bookmarks_enabled = true;
	if (!pref->remember_media_settings) bookmarks_enabled = false;
	else
	if (core->mdat.type == TYPE_STREAM && !pref->remember_stream_settings) bookmarks_enabled = false;
	else
	if (core->mdat.type != TYPE_FILE && core->mdat.type != TYPE_STREAM) bookmarks_enabled = false;

	addBookmarkAct->setEnabled(bookmarks_enabled);
	editBookmarksAct->setEnabled(bookmarks_enabled);
#endif
}

void BaseGui::disableActionsOnStop() {
	qDebug("BaseGui::disableActionsOnStop");

	setActionsEnabled(false);

	playAct->setEnabled(true);
	playOrPauseAct->setEnabled(true);
	stopAct->setEnabled(true);
}
#endif // AUTODISABLE_ACTIONS

void BaseGui::togglePlayAction(Core::State state) {
	qDebug("BaseGui::togglePlayAction");

#if AUTODISABLE_ACTIONS
	if (state == Core::Playing)
		playAct->setEnabled(false);
	else
		playAct->setEnabled(true);
#endif
}

void BaseGui::retranslateStrings() {
	setWindowIcon( Images::icon("logo", 64) );

	// ACTIONS

	// Menu File
	openFileAct->change( Images::icon("open"), tr("&File...") );
	openDirectoryAct->change( Images::icon("openfolder"), tr("D&irectory...") );
	openPlaylistAct->change( Images::icon("open_playlist"), tr("&Playlist...") );
	openVCDAct->change( Images::icon("vcd"), tr("V&CD") );
	openAudioCDAct->change( Images::icon("cdda"), tr("&Audio CD") );
	openDVDAct->change( Images::icon("dvd"), tr("&DVD from drive") );
	openDVDFolderAct->change( Images::icon("dvd_hd"), tr("D&VD from folder...") );
#ifdef BLURAY_SUPPORT
	openBluRayAct->change( Images::icon("bluray"), tr("&Blu-ray from drive") );
	openBluRayFolderAct->change( Images::icon("bluray_hd"), tr("Blu-&ray from folder...") );
#endif
	openURLAct->change( Images::icon("url"), tr("&URL...") );
	exitAct->change( Images::icon("close"), tr("C&lose") );

	// Favorites
	/*
	favorites->editAct()->setText( tr("&Edit...") );
	favorites->addCurrentAct()->setText( tr("&Add current media") );
	*/

	// TV & Radio submenus
	/*
	tvlist->editAct()->setText( tr("&Edit...") );
	radiolist->editAct()->setText( tr("&Edit...") );
	tvlist->addCurrentAct()->setText( tr("&Add current media") );
	radiolist->addCurrentAct()->setText( tr("&Add current media") );
	tvlist->jumpAct()->setText( tr("&Jump...") );
	radiolist->jumpAct()->setText( tr("&Jump...") );
	tvlist->nextAct()->setText( tr("Next TV channel") );
	tvlist->previousAct()->setText( tr("Previous TV channel") );
	radiolist->nextAct()->setText( tr("Next radio channel") );
	radiolist->previousAct()->setText( tr("Previous radio channel") );
	*/

	// Menu Play
	playAct->change( tr("P&lay") );
	playAct->setIcon( Images::icon("play") );

	pauseAct->change( Images::icon("pause"), tr("&Pause"));
	stopAct->change( Images::icon("stop"), tr("&Stop") );
	frameStepAct->change( Images::icon("frame_step"), tr("&Frame step") );
	frameBackStepAct->change( Images::icon("frame_back_step"), tr("Fra&me back step") );

	playOrPauseAct->change( tr("Play / Pause") );
	playOrPauseAct->setIcon( Images::icon("play_pause") );

	pauseAndStepAct->change( Images::icon("pause"), tr("Pause / Frame step") );

	setJumpTexts(); // Texts for rewind*Act and forward*Act

	// Submenu A-B
	setAMarkerAct->change( Images::icon("a_marker"), tr("Set &A marker") );
	setBMarkerAct->change( Images::icon("b_marker"), tr("Set &B marker") );
	clearABMarkersAct->change( Images::icon("clear_markers"), tr("&Clear A-B markers") );
	repeatAct->change( Images::icon("repeat"), tr("&Repeat") );

	gotoAct->change( Images::icon("jumpto"), tr("&Jump to...") );

	// Submenu speed
	normalSpeedAct->change( tr("&Normal speed") );
	halveSpeedAct->change( tr("&Half speed") );
	doubleSpeedAct->change( tr("&Double speed") );
	decSpeed10Act->change( tr("Speed &-10%") );
	incSpeed10Act->change( tr("Speed &+10%") );
	decSpeed4Act->change( tr("Speed -&4%") );
	incSpeed4Act->change( tr("&Speed +4%") );
	decSpeed1Act->change( tr("Speed -&1%") );
	incSpeed1Act->change( tr("S&peed +1%") );

	// Menu Video
	fullscreenAct->change( Images::icon("fullscreen"), tr("&Fullscreen") );
	compactAct->change( Images::icon("compact"), tr("&Compact mode") );
	videoEqualizerAct->change( Images::icon("equalizer"), tr("&Equalizer") );
	screenshotAct->change( Images::icon("screenshot"), tr("&Screenshot") );
	screenshotWithSubsAct->change( Images::icon("screenshot"), tr("Screenshot with subtitles") );
	screenshotWithNoSubsAct->change( Images::icon("screenshot"), tr("Screenshot without subtitles") );
	screenshotsAct->change( Images::icon("screenshots"), tr("Start/stop takin&g screenshots") );
#ifdef CAPTURE_STREAM
	capturingAct->change(Images::icon("record"), tr("Start/stop capturing stream"));
#endif
#ifdef VIDEOPREVIEW
	videoPreviewAct->change( Images::icon("video_preview"), tr("Thumb&nail Generator...") );
#endif
	flipAct->change( Images::icon("flip"), tr("Fli&p image") );
	mirrorAct->change( Images::icon("mirror"), tr("Mirr&or image") );
	stereo3dAct->change( Images::icon("stereo3d"), tr("Stereo &3D filter") );

	decZoomAct->change( tr("Zoom &-") );
	incZoomAct->change( tr("Zoom &+") );
	resetZoomAct->change( tr("&Reset") );
	autoZoomAct->change( tr("&Auto zoom") );
	autoZoom169Act->change( tr("Zoom for &16:9") );
	autoZoom235Act->change( tr("Zoom for &2.35:1") );
	moveLeftAct->change( tr("Move &left") );
	moveRightAct->change( tr("Move &right") );
	moveUpAct->change( tr("Move &up") );
	moveDownAct->change( tr("Move &down") );

	// Submenu Filters
	postProcessingAct->change( tr("&Postprocessing") );
	phaseAct->change( tr("&Autodetect phase") );
	deblockAct->change( tr("&Deblock") );
	deringAct->change( tr("De&ring") );
	gradfunAct->change( tr("Debanding (&gradfun)") );
	addNoiseAct->change( tr("Add n&oise") );
	addLetterboxAct->change( Images::icon("letterbox"), tr("Add &black borders") );
	upscaleAct->change( Images::icon("upscaling"), tr("Soft&ware scaling") );

	// Menu Audio
	audioEqualizerAct->change( Images::icon("audio_equalizer"), tr("E&qualizer") );
	QIcon icset( Images::icon("volume") );
	icset.addPixmap( Images::icon("mute"), QIcon::Normal, QIcon::On  );
	muteAct->change( icset, tr("&Mute") );
	decVolumeAct->change( Images::icon("audio_down"), tr("Volume &-") );
	incVolumeAct->change( Images::icon("audio_up"), tr("Volume &+") );
	decAudioDelayAct->change( Images::icon("delay_down"), tr("&Delay -") );
	incAudioDelayAct->change( Images::icon("delay_up"), tr("D&elay +") );
	audioDelayAct->change( Images::icon("audio_delay"), tr("Set dela&y...") );
	loadAudioAct->change( Images::icon("open"), tr("&Load external file...") );
	unloadAudioAct->change( Images::icon("unload"), tr("U&nload") );

	// Submenu Filters
	extrastereoAct->change( tr("&Extrastereo") );
	karaokeAct->change( tr("&Karaoke") );
	volnormAct->change( tr("Volume &normalization") );
#ifdef MPV_SUPPORT
	earwaxAct->change( tr("&Headphone optimization") + " (earwax)" );
#endif

	// Menu Subtitles
	loadSubsAct->change( Images::icon("open"), tr("&Load...") );
	unloadSubsAct->change( Images::icon("unload"), tr("U&nload") );
	decSubDelayAct->change( Images::icon("delay_down"), tr("Delay &-") );
	incSubDelayAct->change( Images::icon("delay_up"), tr("Delay &+") );
	subDelayAct->change( Images::icon("sub_delay"), tr("Se&t delay...") );
	decSubPosAct->change( Images::icon("sub_up"), tr("&Up") );
	incSubPosAct->change( Images::icon("sub_down"), tr("&Down") );
	decSubScaleAct->change( Images::icon("dec_sub_scale"), tr("S&ize -") );
	incSubScaleAct->change( Images::icon("inc_sub_scale"), tr("Si&ze +") );
	decSubStepAct->change( Images::icon("dec_sub_step"), 
                           tr("&Previous line in subtitles") );
	incSubStepAct->change( Images::icon("inc_sub_step"), 
                           tr("N&ext line in subtitles") );
#ifdef MPV_SUPPORT
	seekNextSubAct->change(Images::icon("seek_next_sub"), tr("Seek to next subtitle"));
	seekPrevSubAct->change(Images::icon("seek_prev_sub"), tr("Seek to previous subtitle"));
#endif
	useCustomSubStyleAct->change( Images::icon("use_custom_sub_style"), tr("Use custo&m style") );
	useForcedSubsOnlyAct->change( Images::icon("forced_subs"), tr("&Forced subtitles only") );

	subVisibilityAct->change( Images::icon("sub_visibility"), tr("Subtitle &visibility") );

#ifdef FIND_SUBTITLES
	showFindSubtitlesDialogAct->change( Images::icon("download_subs"), tr("Find subtitles at &OpenSubtitles.org...") );
	openUploadSubtitlesPageAct->change( Images::icon("upload_subs"), tr("Upload su&btitles to OpenSubtitles.org...") );
#endif

	ccNoneAct->change( tr("&Off", "closed captions menu") );
	ccChannel1Act->change( "&1" );
	ccChannel2Act->change( "&2" );
	ccChannel3Act->change( "&3" );
	ccChannel4Act->change( "&4" );

	subFPSNoneAct->change( tr("&Default", "subfps menu") );
	/* subFPS23Act->change( "2&3" ); */
	subFPS23976Act->change( "23.9&76" );
	subFPS24Act->change( "2&4" );
	subFPS25Act->change( "2&5" );
	subFPS29970Act->change( "29.&970" );
	subFPS30Act->change( "3&0" );

	// Menu Options
	showPlaylistAct->change( Images::icon("playlist"), tr("&Playlist") );
	showPropertiesAct->change( Images::icon("info"), tr("&Information and properties...") );
	showPreferencesAct->change( Images::icon("prefs"), tr("P&references") );
#ifdef YOUTUBE_SUPPORT
	showTubeBrowserAct->change( Images::icon("tubebrowser"), tr("&YouTube%1 browser").arg(QChar(0x2122)) );
#endif

	// Submenu Logs
#ifdef LOG_MPLAYER
	showLogMplayerAct->change(tr("%1 log").arg(PLAYER_NAME));
#endif
#ifdef LOG_SMPLAYER
	showLogSmplayerAct->change(tr("SMPlayer log"));
#endif
	tabletModeAct->change(Images::icon("tablet_mode"), tr("T&ablet mode"));

	// Menu Help
	showFirstStepsAct->change( Images::icon("guide"), tr("First Steps &Guide") );
	showFAQAct->change( Images::icon("faq"), tr("&FAQ") );
	showCLOptionsAct->change( Images::icon("cl_help"), tr("&Command line options") );
	showCheckUpdatesAct->change( Images::icon("check_updates"), tr("Check for &updates") );

#if defined(YOUTUBE_SUPPORT) && defined(YT_USE_YTSIG)
	updateYTAct->change( Images::icon("update_youtube"), tr("Update the &YouTube code") );
#endif

	showConfigAct->change( Images::icon("show_config"), tr("&Open configuration folder") );
#ifdef SHARE_ACTIONS
	donateAct->change( Images::icon("donate"), tr("&Donate / Share with your friends") );
#else
	donateAct->change( Images::icon("donate"), tr("&Donate") );
#endif
	aboutThisAct->change( Images::icon("logo"), tr("About &SMPlayer") );

#ifdef SHARE_MENU
	facebookAct->change("&Facebook");
	twitterAct->change("&Twitter");
	gmailAct->change("&Gmail");
	hotmailAct->change("&Hotmail");
	yahooAct->change("&Yahoo!");
#endif

	// OSD
	incOSDScaleAct->change(tr("Size &+"));
	decOSDScaleAct->change(tr("Size &-"));

#ifdef MPV_SUPPORT
	OSDFractionsAct->change(tr("Show times with &milliseconds"));
#endif

	// Playlist
	playNextAct->change( tr("&Next") );
	playPrevAct->change( tr("Pre&vious") );

	playNextAct->setIcon( Images::icon("next") );
	playPrevAct->setIcon( Images::icon("previous") );


	// Actions not in menus or buttons
	// Volume 2
#if !USE_MULTIPLE_SHORTCUTS
	decVolume2Act->change( tr("Dec volume (2)") );
	incVolume2Act->change( tr("Inc volume (2)") );
#endif
	// Exit fullscreen
	exitFullscreenAct->change( tr("Exit fullscreen") );

	nextOSDAct->change( tr("OSD - Next level") );
	decContrastAct->change( tr("Dec contrast") );
	incContrastAct->change( tr("Inc contrast") );
	decBrightnessAct->change( tr("Dec brightness") );
	incBrightnessAct->change( tr("Inc brightness") );
	decHueAct->change( tr("Dec hue") );
	incHueAct->change( tr("Inc hue") );
	decSaturationAct->change( tr("Dec saturation") );
	incSaturationAct->change( tr("Inc saturation") );
	decGammaAct->change( tr("Dec gamma") );
	incGammaAct->change( tr("Inc gamma") );
	nextVideoAct->change( tr("Next video") );
	nextAudioAct->change( tr("Next audio") );
	nextSubtitleAct->change( tr("Next subtitle") );
	nextChapterAct->change( tr("Next chapter") );
	prevChapterAct->change( tr("Previous chapter") );
	doubleSizeAct->change( tr("&Toggle double size") );
	resetVideoEqualizerAct->change( tr("Reset video equalizer") );
	resetAudioEqualizerAct->change( tr("Reset audio equalizer") );
	showContextMenuAct->change( tr("Show context menu") );
	nextAspectAct->change( Images::icon("next_aspect"), tr("Next aspect ratio") );
	nextWheelFunctionAct->change( Images::icon("next_wheel_function"), tr("Next wheel function") );

	showFilenameAct->change( tr("Show filename on OSD") );
	showMediaInfoAct->change( tr("Show &info on OSD") );
	showTimeAct->change( tr("Show playback time on OSD") );

	toggleDeinterlaceAct->change( tr("Toggle deinterlacing") );


	// Action groups
	osdNoneAct->change( tr("Subtitles onl&y") );
	osdSeekAct->change( tr("Volume + &Seek") );
	osdTimerAct->change( tr("Volume + Seek + &Timer") );
	osdTotalAct->change( tr("Volume + Seek + Timer + T&otal time") );


	// MENUS
	openMenu->menuAction()->setText( tr("&Open") );
	playMenu->menuAction()->setText( tr("&Play") );
	videoMenu->menuAction()->setText( tr("&Video") );
	audioMenu->menuAction()->setText( tr("&Audio") );
	subtitlesMenu->menuAction()->setText( tr("&Subtitles") );
	browseMenu->menuAction()->setText( tr("&Browse") );
	viewMenu->menuAction()->setText( tr("Vie&w") );
	optionsMenu->menuAction()->setText( tr("Op&tions") );
	helpMenu->menuAction()->setText( tr("&Help") );

	/*
	openMenuAct->setIcon( Images::icon("open_menu") );
	playMenuAct->setIcon( Images::icon("play_menu") );
	videoMenuAct->setIcon( Images::icon("video_menu") );
	audioMenuAct->setIcon( Images::icon("audio_menu") );
	subtitlesMenuAct->setIcon( Images::icon("subtitles_menu") );
	browseMenuAct->setIcon( Images::icon("browse_menu") );
	optionsMenuAct->setIcon( Images::icon("options_menu") );
	helpMenuAct->setIcon( Images::icon("help_menu") );
	*/

	// Menu Open
	recentfiles_menu->menuAction()->setText( tr("&Recent files") );
	recentfiles_menu->menuAction()->setIcon( Images::icon("recents") );
	clearRecentsAct->change( Images::icon("delete"), tr("&Clear") );

	disc_menu->menuAction()->setText( tr("&Disc") );
	disc_menu->menuAction()->setIcon( Images::icon("open_disc") );

	/* favorites->menuAction()->setText( tr("&Favorites") ); */
	favorites->menuAction()->setText( tr("F&avorites") );
	favorites->menuAction()->setIcon( Images::icon("open_favorites") ); 

#ifdef TV_SUPPORT
	tvlist->menuAction()->setText( tr("&TV") );
	tvlist->menuAction()->setIcon( Images::icon("open_tv") );

	radiolist->menuAction()->setText( tr("Radi&o") );
	radiolist->menuAction()->setIcon( Images::icon("open_radio") );
#endif

	// Menu Play
	speed_menu->menuAction()->setText( tr("Sp&eed") );
	speed_menu->menuAction()->setIcon( Images::icon("speed") );

	ab_menu->menuAction()->setText( tr("&A-B section") );
	ab_menu->menuAction()->setIcon( Images::icon("ab_menu") );

	// Menu Video
	videotrack_menu->menuAction()->setText( tr("&Track", "video") );
	videotrack_menu->menuAction()->setIcon( Images::icon("video_track") );

	videosize_menu->menuAction()->setText( tr("Si&ze") );
	videosize_menu->menuAction()->setIcon( Images::icon("video_size") );

	/*
	panscan_menu->menuAction()->setText( tr("&Pan && scan") );
	panscan_menu->menuAction()->setIcon( Images::icon("panscan") );
	*/
	zoom_menu->menuAction()->setText( tr("Zoo&m") );
	zoom_menu->menuAction()->setIcon( Images::icon("zoom") );

	aspect_menu->menuAction()->setText( tr("&Aspect ratio") );
	aspect_menu->menuAction()->setIcon( Images::icon("aspect") );

	deinterlace_menu->menuAction()->setText( tr("&Deinterlace") );
	deinterlace_menu->menuAction()->setIcon( Images::icon("deinterlace") );

	videofilter_menu->menuAction()->setText( tr("F&ilters") );
	videofilter_menu->menuAction()->setIcon( Images::icon("video_filters") );

	rotate_menu->menuAction()->setText( tr("&Rotate") );
	rotate_menu->menuAction()->setIcon( Images::icon("rotate") );

	ontop_menu->menuAction()->setText( tr("S&tay on top") );
	ontop_menu->menuAction()->setIcon( Images::icon("ontop") );

#if USE_ADAPTER
	screen_menu->menuAction()->setText( tr("Scree&n") );
	screen_menu->menuAction()->setIcon( Images::icon("screen") );
#endif

	denoise_menu->menuAction()->setText( tr("De&noise") );
	denoise_menu->menuAction()->setIcon( Images::icon("denoise") );

	unsharp_menu->menuAction()->setText( tr("Blur/S&harp") );
	unsharp_menu->menuAction()->setIcon( Images::icon("unsharp") );

	aspectDetectAct->change( tr("&Auto") );
	aspect11Act->change( "1&:1" );
	aspect32Act->change( "&3:2" );
	aspect43Act->change( "&4:3" );
	aspect118Act->change( "11:&8" );
	aspect54Act->change( "&5:4" );
	aspect149Act->change( "&14:9" );
	aspect1410Act->change( "1&4:10" );
	aspect169Act->change( "16:&9" );
	aspect1610Act->change( "1&6:10" );
	aspect235Act->change( "&2.35:1" );
	aspectNoneAct->change( tr("&Disabled") );

	deinterlaceNoneAct->change( tr("&None") );
	deinterlaceL5Act->change( tr("&Lowpass5") );
	deinterlaceYadif0Act->change( tr("&Yadif (normal)") );
	deinterlaceYadif1Act->change( tr("Y&adif (double framerate)") );
	deinterlaceLBAct->change( tr("Linear &Blend") );
	deinterlaceKernAct->change( tr("&Kerndeint") );

	denoiseNoneAct->change( tr("&Off", "denoise menu") );
	denoiseNormalAct->change( tr("&Normal","denoise menu") );
	denoiseSoftAct->change( tr("&Soft", "denoise menu") );

	unsharpNoneAct->change( tr("&None", "unsharp menu") );
	blurAct->change( tr("&Blur", "unsharp menu") );
	sharpenAct->change( tr("&Sharpen", "unsharp menu") );

	rotateNoneAct->change( tr("&Off") );
	rotateClockwiseFlipAct->change( tr("&Rotate by 90 degrees clockwise and flip") );
	rotateClockwiseAct->change( tr("Rotate by 90 degrees &clockwise") );
	rotateCounterclockwiseAct->change( tr("Rotate by 90 degrees counterclock&wise") );
	rotateCounterclockwiseFlipAct->change( tr("Rotate by 90 degrees counterclockwise and &flip") );

	onTopAlwaysAct->change( tr("&Always") );
	onTopNeverAct->change( tr("&Never") );
	onTopWhilePlayingAct->change( tr("While &playing") );
	toggleStayOnTopAct->change( tr("Toggle stay on top") );

#if USE_ADAPTER
	screenDefaultAct->change( tr("&Default") );
#endif

	// Menu Audio
	audiotrack_menu->menuAction()->setText( tr("&Track", "audio") );
	audiotrack_menu->menuAction()->setIcon( Images::icon("audio_track") );
	audiotrack_menu->menuAction()->setToolTip(tr("Select audio track"));

	audiofilter_menu->menuAction()->setText( tr("&Filters") );
	audiofilter_menu->menuAction()->setIcon( Images::icon("audio_filters") );

	audiochannels_menu->menuAction()->setText( tr("&Channels") );
	audiochannels_menu->menuAction()->setIcon( Images::icon("audio_channels") );

	stereomode_menu->menuAction()->setText( tr("&Stereo mode") );
	stereomode_menu->menuAction()->setIcon( Images::icon("stereo_mode") );

	channelsDefaultAct->change( tr("&Default") );
	channelsStereoAct->change( tr("&Stereo") );
	channelsSurroundAct->change( tr("&4.0 Surround") );
	channelsFull51Act->change( tr("&5.1 Surround") );
	channelsFull61Act->change( tr("&6.1 Surround") );
	channelsFull71Act->change( tr("&7.1 Surround") );

	stereoAct->change( tr("&Stereo") );
	leftChannelAct->change( tr("&Left channel") );
	rightChannelAct->change( tr("&Right channel") );
	monoAct->change( tr("&Mono") );
	reverseAct->change( tr("Re&verse") );

	// Menu Subtitle
#ifdef MPV_SUPPORT
	subtitles_track_menu->menuAction()->setText( tr("Prim&ary track") );
#else
	subtitles_track_menu->menuAction()->setText( tr("&Select") );
#endif
	subtitles_track_menu->menuAction()->setIcon( Images::icon("sub") );
	subtitles_track_menu->menuAction()->setToolTip(tr("Select subtitle track"));

#ifdef MPV_SUPPORT
	secondary_subtitles_track_menu->menuAction()->setText( tr("Secondary trac&k") );
	secondary_subtitles_track_menu->menuAction()->setIcon( Images::icon("secondary_sub") );
	secondary_subtitles_track_menu->menuAction()->setToolTip(tr("Select secondary subtitle track"));
#endif

	closed_captions_menu->menuAction()->setText( tr("&Closed captions") );
	closed_captions_menu->menuAction()->setIcon( Images::icon("closed_caption") );

	subfps_menu->menuAction()->setText( tr("F&rames per second") );
	subfps_menu->menuAction()->setIcon( Images::icon("subfps") );

	// Menu Browse 
	titles_menu->menuAction()->setText( tr("&Title") );
	titles_menu->menuAction()->setIcon( Images::icon("title") );

	chapters_menu->menuAction()->setText( tr("&Chapter") );
	chapters_menu->menuAction()->setIcon( Images::icon("chapter") );

	angles_menu->menuAction()->setText( tr("&Angle") );
	angles_menu->menuAction()->setIcon( Images::icon("angle") );

#ifdef BOOKMARKS
	bookmark_menu->menuAction()->setText( tr("&Bookmarks") );
	bookmark_menu->menuAction()->setIcon( Images::icon("bookmarks") );
#endif

#if PROGRAM_SWITCH
	programtrack_menu->menuAction()->setText( tr("P&rogram", "program") );
	programtrack_menu->menuAction()->setIcon( Images::icon("program_track") );
#endif

#ifdef BOOKMARKS
	addBookmarkAct->change(Images::icon("add_bookmark"), tr("&Add new bookmark"));
	editBookmarksAct->change(Images::icon("edit_bookmarks"), tr("&Edit bookmarks"));
	prevBookmarkAct->change(Images::icon("prev_bookmark"), tr("Previous bookmark"));
	nextBookmarkAct->change(Images::icon("next_bookmark"), tr("Next bookmark"));
#endif

#if DVDNAV_SUPPORT
	dvdnavUpAct->change(Images::icon("dvdnav_up"), tr("DVD menu, move up"));
	dvdnavDownAct->change(Images::icon("dvdnav_down"), tr("DVD menu, move down"));
	dvdnavLeftAct->change(Images::icon("dvdnav_left"), tr("DVD menu, move left"));
	dvdnavRightAct->change(Images::icon("dvdnav_right"), tr("DVD menu, move right"));
	dvdnavMenuAct->change(Images::icon("dvdnav_menu"), tr("DVD &menu"));
	dvdnavSelectAct->change(Images::icon("dvdnav_select"), tr("DVD menu, select option"));
	dvdnavPrevAct->change(Images::icon("dvdnav_prev"), tr("DVD &previous menu"));
	dvdnavMouseAct->change(Images::icon("dvdnav_mouse"), tr("DVD menu, mouse click"));
#endif

	// Menu Options
	osd_menu->menuAction()->setText( tr("&OSD") );
	osd_menu->menuAction()->setIcon( Images::icon("osd") );

#ifdef SHARE_MENU
	share_menu->menuAction()->setText( tr("S&hare SMPlayer with your friends") );
	share_menu->menuAction()->setIcon( Images::icon("share") );
#endif

#if defined(LOG_MPLAYER) || defined(LOG_SMPLAYER)
	//logs_menu->menuAction()->setText( tr("&View logs") );
	//logs_menu->menuAction()->setIcon( Images::icon("logs") );
#endif

	// Access menu
	access_menu->menuAction()->setText( tr("Quick access menu") );
	access_menu->menuAction()->setIcon( Images::icon("quick_access_menu") );

	// To be sure that the "<empty>" string is translated
	initializeMenus();

	// Other things
#ifdef LOG_MPLAYER
	mplayer_log_window->setWindowTitle( tr("%1 log").arg(PLAYER_NAME) );
#endif
#ifdef LOG_SMPLAYER
	smplayer_log_window->setWindowTitle( tr("SMPlayer log") );
#endif

	updateRecents();
	updateWidgets();

	// Update actions view in preferences
	// It has to be done, here. The actions are translated after the
	// preferences dialog.
	if (pref_dialog) pref_dialog->mod_input()->actions_editor->updateView();
}

void BaseGui::setJumpTexts() {
	rewind1Act->change( tr("-%1").arg(Helper::timeForJumps(pref->seeking1)) );
	rewind2Act->change( tr("-%1").arg(Helper::timeForJumps(pref->seeking2)) );
	rewind3Act->change( tr("-%1").arg(Helper::timeForJumps(pref->seeking3)) );

	forward1Act->change( tr("+%1").arg(Helper::timeForJumps(pref->seeking1)) );
	forward2Act->change( tr("+%1").arg(Helper::timeForJumps(pref->seeking2)) );
	forward3Act->change( tr("+%1").arg(Helper::timeForJumps(pref->seeking3)) );

	rewind1Act->setIcon( Images::icon("rewind10s") );
	rewind2Act->setIcon( Images::icon("rewind1m") );
	rewind3Act->setIcon( Images::icon("rewind10m") );

	forward1Act->setIcon( Images::icon("forward10s") );
	forward2Act->setIcon( Images::icon("forward1m") );
	forward3Act->setIcon( Images::icon("forward10m") );
}

void BaseGui::setWindowCaption(const QString & title) {
	setWindowTitle(title);
}

void BaseGui::createCore() {
	core = new Core( mplayerwindow, this );

	connect( core, SIGNAL(menusNeedInitialize()),
             this, SLOT(initializeMenus()) );
	connect( core, SIGNAL(widgetsNeedUpdate()),
             this, SLOT(updateWidgets()) );
	connect( core, SIGNAL(videoEqualizerNeedsUpdate()),
             this, SLOT(updateVideoEqualizer()) );

	connect( core, SIGNAL(audioEqualizerNeedsUpdate()),
             this, SLOT(updateAudioEqualizer()) );

	connect( core, SIGNAL(showFrame(int)),
             this, SIGNAL(frameChanged(int)) );

	connect( core, SIGNAL(ABMarkersChanged(int,int)),
             this, SIGNAL(ABMarkersChanged(int,int)) );

	connect( core, SIGNAL(showTime(double)),
             this, SLOT(gotCurrentTime(double)) );

	connect( core, SIGNAL(needResize(int, int)),
             this, SLOT(resizeWindow(int,int)) );

	connect( core, SIGNAL(needResize(int, int)),
             this, SLOT(centerWindow()) );

	connect( core, SIGNAL(showMessage(QString,int)),
             this, SLOT(displayMessage(QString,int)) );
	connect( core, SIGNAL(showMessage(QString)),
             this, SLOT(displayMessage(QString)) );

	connect( core, SIGNAL(stateChanged(Core::State)),
             this, SLOT(displayState(Core::State)) );
	connect( core, SIGNAL(stateChanged(Core::State)),
             this, SLOT(checkStayOnTop(Core::State)), Qt::QueuedConnection );

	connect( core, SIGNAL(mediaStartPlay()),
             this, SLOT(enterFullscreenOnPlay()), Qt::QueuedConnection );
	connect( core, SIGNAL(mediaStoppedByUser()),
             this, SLOT(exitFullscreenOnStop()) );

	connect( core, SIGNAL(mediaStoppedByUser()),
             mplayerwindow, SLOT(showLogo()) );

	connect( core, SIGNAL(mediaLoaded()),
             this, SLOT(enableActionsOnPlaying()) );

	connect( core, SIGNAL(noFileToPlay()), this, SLOT(gotNoFileToPlay()) );

#if NOTIFY_AUDIO_CHANGES
	connect( core, SIGNAL(audioTracksChanged()),
             this, SLOT(enableActionsOnPlaying()) );
#endif
	connect( core, SIGNAL(mediaFinished()),
             this, SLOT(disableActionsOnStop()) );
	connect( core, SIGNAL(mediaStoppedByUser()),
             this, SLOT(disableActionsOnStop()) );

	connect( core, SIGNAL(stateChanged(Core::State)),
             this, SLOT(togglePlayAction(Core::State)) );

	connect( core, SIGNAL(mediaStartPlay()),
             this, SLOT(newMediaLoaded()), Qt::QueuedConnection );
	connect( core, SIGNAL(mediaInfoChanged()),
             this, SLOT(updateMediaInfo()) );

	connect( core, SIGNAL(mediaStartPlay()),
             this, SLOT(checkPendingActionsToRun()), Qt::QueuedConnection );
#if REPORT_OLD_MPLAYER
	connect( core, SIGNAL(mediaStartPlay()),
             this, SLOT(checkMplayerVersion()), Qt::QueuedConnection );
#endif
	connect( core, SIGNAL(failedToParseMplayerVersion(QString)),
             this, SLOT(askForMplayerVersion(QString)) );

	connect( core, SIGNAL(mplayerFailed(QProcess::ProcessError)),
             this, SLOT(showErrorFromMplayer(QProcess::ProcessError)) );

	connect( core, SIGNAL(mplayerFinishedWithError(int)),
             this, SLOT(showExitCodeFromMplayer(int)) );

	// Hide mplayer window
#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
	if (pref->hide_video_window_on_audio_files) {
		connect( core, SIGNAL(noVideo()), this, SLOT(hidePanel()) );
	} else {
		connect( core, SIGNAL(noVideo()), mplayerwindow, SLOT(showLogo()) );
	}
#else
	connect( core, SIGNAL(noVideo()), this, SLOT(hidePanel()) );
#endif

	// Log mplayer output
#ifdef LOG_MPLAYER
	connect( core, SIGNAL(aboutToStartPlaying()),
             this, SLOT(clearMplayerLog()) );
	connect( core, SIGNAL(logLineAvailable(QString)),
             this, SLOT(recordMplayerLog(QString)) );

	connect( core, SIGNAL(mediaLoaded()), 
             this, SLOT(autosaveMplayerLog()) );
#endif

#ifdef YOUTUBE_SUPPORT
	connect(core, SIGNAL(signatureNotFound(const QString &)),
            this, SLOT(YTNoSignature(const QString &)));
	connect(core, SIGNAL(noSslSupport()),
            this, SLOT(YTNoSslSupport()));
#endif
	connect(core, SIGNAL(receivedForbidden()), this, SLOT(gotForbidden()));
}

void BaseGui::createMplayerWindow() {
	mplayerwindow = new MplayerWindow(panel);
	mplayerwindow->show();
	mplayerwindow->setObjectName("mplayerwindow");
#if USE_COLORKEY
	mplayerwindow->setColorKey( pref->color_key );
#endif
	mplayerwindow->allowVideoMovement( pref->allow_video_movement );
	mplayerwindow->delayLeftClick(pref->delay_left_click);

#if LOGO_ANIMATION
	mplayerwindow->setAnimatedLogo( pref->animated_logo);
#endif

#ifdef SHARE_WIDGET
	sharewidget = new ShareWidget(Global::settings, mplayerwindow);
	mplayerwindow->setCornerWidget(sharewidget);
	//#ifdef SHARE_ACTIONS
	connect(sharewidget, SIGNAL(supportClicked()), this, SLOT(helpDonate()));
	//#endif
#endif

	QVBoxLayout * layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->addWidget(mplayerwindow);
	panel->setLayout(layout);

	// mplayerwindow
	/*
    connect( mplayerwindow, SIGNAL(rightButtonReleased(QPoint)),
	         this, SLOT(showPopupMenu(QPoint)) );
	*/

	// mplayerwindow mouse events
	connect( mplayerwindow, SIGNAL(doubleClicked()),
             this, SLOT(doubleClickFunction()) );
	connect( mplayerwindow, SIGNAL(leftClicked()),
             this, SLOT(leftClickFunction()) );
	connect( mplayerwindow, SIGNAL(rightClicked()),
             this, SLOT(rightClickFunction()) );
	connect( mplayerwindow, SIGNAL(middleClicked()),
             this, SLOT(middleClickFunction()) );
	connect( mplayerwindow, SIGNAL(xbutton1Clicked()),
             this, SLOT(xbutton1ClickFunction()) );
	connect( mplayerwindow, SIGNAL(xbutton2Clicked()),
             this, SLOT(xbutton2ClickFunction()) );

	connect( mplayerwindow, SIGNAL(mouseMovedDiff(QPoint)),
             this, SLOT(processMouseMovedDiff(QPoint)), Qt::QueuedConnection );
#ifdef MOUSE_GESTURES
	mplayerwindow->activateMouseDragTracking(true);
#else
	mplayerwindow->activateMouseDragTracking(pref->drag_function == Preferences::MoveWindow);
#endif
}

void BaseGui::createVideoEqualizer() {
	// Equalizer
	video_equalizer = new VideoEqualizer(this);
	connect( video_equalizer, SIGNAL(contrastChanged(int)), 
             core, SLOT(setContrast(int)) );
	connect( video_equalizer, SIGNAL(brightnessChanged(int)), 
             core, SLOT(setBrightness(int)) );
	connect( video_equalizer, SIGNAL(hueChanged(int)), 
             core, SLOT(setHue(int)) );
	connect( video_equalizer, SIGNAL(saturationChanged(int)), 
             core, SLOT(setSaturation(int)) );
	connect( video_equalizer, SIGNAL(gammaChanged(int)), 
             core, SLOT(setGamma(int)) );

	connect( video_equalizer, SIGNAL(visibilityChanged()),
             this, SLOT(updateWidgets()) );
	connect( video_equalizer, SIGNAL(requestToChangeDefaultValues()),
             this, SLOT(setDefaultValuesFromVideoEqualizer()) );
	connect( video_equalizer, SIGNAL(bySoftwareChanged(bool)),
             this, SLOT(changeVideoEqualizerBySoftware(bool)) );
}

void BaseGui::createAudioEqualizer() {
	// Audio Equalizer
	audio_equalizer = new AudioEqualizer(this);

	connect( audio_equalizer->eq[0], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq0(int)) );
	connect( audio_equalizer->eq[1], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq1(int)) );
	connect( audio_equalizer->eq[2], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq2(int)) );
	connect( audio_equalizer->eq[3], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq3(int)) );
	connect( audio_equalizer->eq[4], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq4(int)) );
	connect( audio_equalizer->eq[5], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq5(int)) );
	connect( audio_equalizer->eq[6], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq6(int)) );
	connect( audio_equalizer->eq[7], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq7(int)) );
	connect( audio_equalizer->eq[8], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq8(int)) );
	connect( audio_equalizer->eq[9], SIGNAL(valueChanged(int)), 
             core, SLOT(setAudioEq9(int)) );

	connect( audio_equalizer, SIGNAL(applyClicked(AudioEqualizerList)), 
             core, SLOT(setAudioAudioEqualizerRestart(AudioEqualizerList)) );

	connect( audio_equalizer, SIGNAL(valuesChanged(AudioEqualizerList)),
             core, SLOT(setAudioEqualizer(AudioEqualizerList)) );

	connect( audio_equalizer, SIGNAL(visibilityChanged()),
             this, SLOT(updateWidgets()) );
}

void BaseGui::createPlaylist() {
#if DOCK_PLAYLIST
	playlist = new Playlist(this, 0);
#else
	playlist = new Playlist(0);
#endif
	playlist->setConfigPath(Paths::configPath());

	connect( playlist, SIGNAL(playlistEnded()),
             this, SLOT(playlistHasFinished()) );

	connect( playlist, SIGNAL(playlistEnded()),
             mplayerwindow, SLOT(showLogo()) );

	connect(playlist, SIGNAL(requestToPlayFile(const QString &, int)),
            core, SLOT(open(const QString &, int)));

	connect(playlist, SIGNAL(requestToPlayStream(const QString &, QStringList)),
            core, SLOT(openStream(const QString &, QStringList)));

	connect(playlist, SIGNAL(requestToAddCurrentFile()), this, SLOT(addToPlaylistCurrentFile()));

	connect( core, SIGNAL(mediaFinished()), playlist, SLOT(playNextAuto()), Qt::QueuedConnection );

	connect( core, SIGNAL(mplayerFailed(QProcess::ProcessError)), playlist, SLOT(playerFailed(QProcess::ProcessError)) );
	connect( core, SIGNAL(mplayerFinishedWithError(int)), playlist, SLOT(playerFinishedWithError(int)) );
	connect(core, SIGNAL(mediaDataReceived(const MediaData &)), playlist, SLOT(getMediaInfo(const MediaData &)));

#ifdef PLAYLIST_DOWNLOAD
	playlist->setMaxItemsUrlHistory( pref->history_urls->maxItems() );
#endif
}

void BaseGui::createPanel() {
	panel = new QWidget( this );
	panel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	panel->setMinimumSize( QSize(1,1) );
	panel->setFocusPolicy( Qt::StrongFocus );
	panel->setObjectName("panel");

	// panel
	/*
	panel->setAutoFillBackground(true);
	ColorUtils::setBackgroundColor( panel, QColor(0,0,0) );
	*/

#ifndef CHANGE_WIDGET_COLOR
	panel->setStyleSheet("#panel { background-color: black; }");
#endif
}

void BaseGui::createPreferencesDialog() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	pref_dialog = new PreferencesDialog(this);
	pref_dialog->setModal(false);
	/* pref_dialog->mod_input()->setActionsList( actions_list ); */
	connect( pref_dialog, SIGNAL(applied()),
             this, SLOT(applyNewPreferences()) );
	QApplication::restoreOverrideCursor();
}

void BaseGui::createFilePropertiesDialog() {
	qDebug("BaseGui::createFilePropertiesDialog");
	QApplication::setOverrideCursor(Qt::WaitCursor);
	file_dialog = new FilePropertiesDialog(this);
	file_dialog->setModal(false);
	connect( file_dialog, SIGNAL(applied()),
             this, SLOT(applyFileProperties()) );
	QApplication::restoreOverrideCursor();
}


void BaseGui::createMenus() {
	// Submenus

	// Recents submenu
	recentfiles_menu = new QMenu(this);
	recentfiles_menu->menuAction()->setObjectName("recents_menu");

	// Disc submenu
	disc_menu = new QMenu(this);
	disc_menu->menuAction()->setObjectName("disc_menu");
	disc_menu->addAction(openDVDAct);
	disc_menu->addAction(openDVDFolderAct);
	#ifdef BLURAY_SUPPORT
	disc_menu->addAction(openBluRayAct);
	disc_menu->addAction(openBluRayFolderAct);
	#endif
	disc_menu->addAction(openVCDAct);
	disc_menu->addAction(openAudioCDAct);

	// Speed submenu
	speed_menu = new QMenu(this);
	speed_menu->menuAction()->setObjectName("speed_menu");
	speed_menu->addAction(normalSpeedAct);
	speed_menu->addSeparator();
	speed_menu->addAction(halveSpeedAct);
	speed_menu->addAction(doubleSpeedAct);
	speed_menu->addSeparator();
	speed_menu->addAction(decSpeed10Act);
	speed_menu->addAction(incSpeed10Act);
	speed_menu->addSeparator();
	speed_menu->addAction(decSpeed4Act);
	speed_menu->addAction(incSpeed4Act);
	speed_menu->addSeparator();
	speed_menu->addAction(decSpeed1Act);
	speed_menu->addAction(incSpeed1Act);

	// A-B submenu
	ab_menu = new QMenu(this);
	ab_menu->menuAction()->setObjectName("ab_menu");
	ab_menu->addAction(setAMarkerAct);
	ab_menu->addAction(setBMarkerAct);
	ab_menu->addAction(clearABMarkersAct);
	ab_menu->addSeparator();
	ab_menu->addAction(repeatAct);

	// Video track submenu
	videotrack_menu = new QMenu(this);
	videotrack_menu->menuAction()->setObjectName("videotrack_menu");

#if USE_ADAPTER
	// Screen submenu
	screen_menu = new QMenu(this);
	screen_menu->menuAction()->setObjectName("screen_menu");
	screen_menu->addActions(screenGroup->actions());
#endif

	// Video size submenu
	videosize_menu = new QMenu(this);
	videosize_menu->menuAction()->setObjectName("videosize_menu");
	videosize_menu->addActions(sizeGroup->actions());
	videosize_menu->addSeparator();
	videosize_menu->addAction(doubleSizeAct);

	// Zoom submenu
	zoom_menu = new QMenu(this);
	zoom_menu->menuAction()->setObjectName("zoom_menu");
	zoom_menu->addAction(resetZoomAct);
	zoom_menu->addSeparator();
	zoom_menu->addAction(autoZoomAct);
	zoom_menu->addAction(autoZoom169Act);
	zoom_menu->addAction(autoZoom235Act);
	zoom_menu->addSeparator();
	zoom_menu->addAction(decZoomAct);
	zoom_menu->addAction(incZoomAct);
	zoom_menu->addSeparator();
	zoom_menu->addAction(moveLeftAct);
	zoom_menu->addAction(moveRightAct);
	zoom_menu->addAction(moveUpAct);
	zoom_menu->addAction(moveDownAct);

	// Aspect submenu
	aspect_menu = new QMenu(this);
	aspect_menu->menuAction()->setObjectName("aspect_menu");
	aspect_menu->addActions(aspectGroup->actions());

	// Deinterlace submenu
	deinterlace_menu = new QMenu(this);
	deinterlace_menu->menuAction()->setObjectName("deinterlace_menu");
	deinterlace_menu->addActions(deinterlaceGroup->actions());

	// Video filter submenu
	videofilter_menu = new QMenu(this);
	videofilter_menu->menuAction()->setObjectName("videofilter_menu");
	videofilter_menu->addAction(postProcessingAct);
	videofilter_menu->addAction(deblockAct);
	videofilter_menu->addAction(deringAct);
	videofilter_menu->addAction(gradfunAct);
	videofilter_menu->addAction(addNoiseAct);
	videofilter_menu->addAction(addLetterboxAct);
	videofilter_menu->addAction(upscaleAct);
	videofilter_menu->addAction(phaseAct);

	// Denoise submenu
	denoise_menu = new QMenu(this);
	denoise_menu->menuAction()->setObjectName("denoise_menu");
	denoise_menu->addActions(denoiseGroup->actions());
	videofilter_menu->addMenu(denoise_menu);

	// Unsharp submenu
	unsharp_menu = new QMenu(this);
	unsharp_menu->menuAction()->setObjectName("unsharp_menu");
	unsharp_menu->addActions(unsharpGroup->actions());
	videofilter_menu->addMenu(unsharp_menu);

	// Rotate submenu
	rotate_menu = new QMenu(this);
	rotate_menu->menuAction()->setObjectName("rotate_menu");
	rotate_menu->addActions(rotateGroup->actions());

	// Ontop submenu
	ontop_menu = new QMenu(this);
	ontop_menu->menuAction()->setObjectName("ontop_menu");
	ontop_menu->addActions(onTopActionGroup->actions());


	// Audio track submenu
	audiotrack_menu = new QMenu(this);
	audiotrack_menu->menuAction()->setObjectName("audiotrack_menu");

	// Audio filter submenu
	audiofilter_menu = new QMenu(this);
	audiofilter_menu->menuAction()->setObjectName("audiofilter_menu");
	audiofilter_menu->addAction(extrastereoAct);
	audiofilter_menu->addAction(karaokeAct);
	#ifdef MPV_SUPPORT
	audiofilter_menu->addAction(earwaxAct);
	#endif
	audiofilter_menu->addAction(volnormAct);

	// Audio channels submenu
	audiochannels_menu = new QMenu(this);
	audiochannels_menu->menuAction()->setObjectName("audiochannels_menu");
	audiochannels_menu->addActions(channelsGroup->actions());

	// Stereo mode submenu
	stereomode_menu = new QMenu(this);
	stereomode_menu->menuAction()->setObjectName("stereomode_menu");
	stereomode_menu->addActions(stereoGroup->actions());


	// Subtitles track submenu
	subtitles_track_menu = new QMenu(this);
	subtitles_track_menu->menuAction()->setObjectName("subtitlestrack_menu");

	// Subtitles secondary track submenu
	#ifdef MPV_SUPPORT
	secondary_subtitles_track_menu = new QMenu(this);
	secondary_subtitles_track_menu->menuAction()->setObjectName("secondary_subtitles_track_menu");
	#endif

	// Subtitles fps submenu
	subfps_menu = new QMenu(this);
	subfps_menu->menuAction()->setObjectName("subfps_menu");
	subfps_menu->addAction( subFPSNoneAct );
	/* subfps_menu->addAction( subFPS23Act ); */
	subfps_menu->addAction( subFPS23976Act );
	subfps_menu->addAction( subFPS24Act );
	subfps_menu->addAction( subFPS25Act );
	subfps_menu->addAction( subFPS29970Act );
	subfps_menu->addAction( subFPS30Act );

	// Closed captions submenu
	closed_captions_menu = new QMenu(this);
	closed_captions_menu->menuAction()->setObjectName("closed_captions_menu");
	closed_captions_menu->addAction( ccNoneAct);
	closed_captions_menu->addAction( ccChannel1Act);
	closed_captions_menu->addAction( ccChannel2Act);
	closed_captions_menu->addAction( ccChannel3Act);
	closed_captions_menu->addAction( ccChannel4Act);


	// Titles submenu
	titles_menu = new QMenu(this);
	titles_menu->menuAction()->setObjectName("titles_menu");

	// Chapters submenu
	chapters_menu = new QMenu(this);
	chapters_menu->menuAction()->setObjectName("chapters_menu");

	// Angles submenu
	angles_menu = new QMenu(this);
	angles_menu->menuAction()->setObjectName("angles_menu");

	// Bookmarks submenu
	#ifdef BOOKMARKS
	bookmark_menu = new QMenu(this);
	bookmark_menu->menuAction()->setObjectName("bookmarks_menu");
	#endif

	// Program submenu
	#if PROGRAM_SWITCH
	programtrack_menu = new QMenu(this);
	programtrack_menu->menuAction()->setObjectName("programtrack_menu");
	#endif


	// OSD submenu
	osd_menu = new QMenu(this);
	osd_menu->menuAction()->setObjectName("osd_menu");
	osd_menu->addActions(osdGroup->actions());
	osd_menu->addSeparator();
	osd_menu->addAction(showMediaInfoAct);
	osd_menu->addSeparator();
	osd_menu->addAction(decOSDScaleAct);
	osd_menu->addAction(incOSDScaleAct);
#ifdef MPV_SUPPORT
	osd_menu->addSeparator();
	osd_menu->addAction(OSDFractionsAct);
#endif

	// Share submenu
	#ifdef SHARE_MENU
	share_menu = new QMenu(this);
	share_menu->addAction(facebookAct);
	share_menu->addAction(twitterAct);
	share_menu->addAction(gmailAct);
	share_menu->addAction(hotmailAct);
	share_menu->addAction(yahooAct);
	#endif

	// MENUS
	openMenu = menuBar()->addMenu("Open");
	playMenu = menuBar()->addMenu("Play");
	videoMenu = menuBar()->addMenu("Video");
	audioMenu = menuBar()->addMenu("Audio");
	subtitlesMenu = menuBar()->addMenu("Subtitles");
	/* menuBar()->addMenu(favorites); */
	browseMenu = menuBar()->addMenu("Browse");
	viewMenu = menuBar()->addMenu("View");
	optionsMenu = menuBar()->addMenu("Options");
	helpMenu = menuBar()->addMenu("Help");

	// POPUP MENU
	if (!popup)
		popup = new QMenu(this);
	else
		popup->clear();

	popup->addMenu( openMenu );
	popup->addMenu( playMenu );
	popup->addMenu( videoMenu );
	popup->addMenu( audioMenu );
	popup->addMenu( subtitlesMenu );
	popup->addMenu( favorites );
	popup->addMenu( browseMenu );
	popup->addMenu( viewMenu );
	popup->addMenu( optionsMenu );

	// Access menu
	access_menu = new QMenu(this);
	access_menu->menuAction()->setObjectName("quick_access_menu");

	//populateMainMenu();

	// let's show something, even a <empty> entry
	initializeMenus();
}

void BaseGui::populateMainMenu() {
	qDebug("BaseGui::populateMainMenu");

	openMenu->clear();
	playMenu->clear();
	videoMenu->clear();
	audioMenu->clear();
	subtitlesMenu->clear();
	browseMenu->clear();
	viewMenu->clear();
	optionsMenu->clear();
	helpMenu->clear();

	// OPEN MENU
	openMenu->addAction(openFileAct);
	openMenu->addMenu(recentfiles_menu);
	openMenu->addMenu(favorites);
	openMenu->addAction(openDirectoryAct);
	openMenu->addAction(openPlaylistAct);
	if (!pref->tablet_mode) {
		openMenu->addMenu(disc_menu);
	}
	openMenu->addAction(openURLAct);
#ifdef TV_SUPPORT
	if (!pref->tablet_mode) {
		openMenu->addMenu(tvlist);
		openMenu->addMenu(radiolist);
	}
#endif
	openMenu->addSeparator();
	openMenu->addAction(exitAct);

	// PLAY MENU
	if (!pref->tablet_mode) {
		playMenu->addAction(playAct);
		playMenu->addAction(pauseAct);
		/* playMenu->addAction(playOrPauseAct); */
		playMenu->addAction(stopAct);
	}
	playMenu->addAction(frameStepAct);
	playMenu->addAction(frameBackStepAct);
	playMenu->addSeparator();
	if (!pref->tablet_mode) {
		playMenu->addAction(rewind1Act);
		playMenu->addAction(forward1Act);
	}
	playMenu->addAction(rewind2Act);
	playMenu->addAction(forward2Act);
	playMenu->addAction(rewind3Act);
	playMenu->addAction(forward3Act);
	playMenu->addSeparator();
	playMenu->addMenu(speed_menu);
	playMenu->addSeparator();
	if (!pref->tablet_mode) {
		playMenu->addMenu(ab_menu);
		playMenu->addSeparator();
		playMenu->addAction(gotoAct);
		playMenu->addSeparator();
	}
	playMenu->addAction(playPrevAct);
	playMenu->addAction(playNextAct);

	// VIDEO MENU
	videoMenu->addMenu(videotrack_menu);
	videoMenu->addAction(fullscreenAct);
	if (!pref->tablet_mode) {
		videoMenu->addAction(compactAct);
		#if USE_ADAPTER
		videoMenu->addMenu(screen_menu);
		#endif
	}
	videoMenu->addMenu(videosize_menu);
	videoMenu->addMenu(zoom_menu);
	videoMenu->addMenu(aspect_menu);
	videoMenu->addMenu(deinterlace_menu);
	videoMenu->addMenu(videofilter_menu);
	videoMenu->addMenu(rotate_menu);
	videoMenu->addAction(flipAct);
	videoMenu->addAction(mirrorAct);
	if (!pref->tablet_mode) {
		videoMenu->addAction(stereo3dAct);
		videoMenu->addSeparator();
	}
	videoMenu->addAction(videoEqualizerAct);
	videoMenu->addAction(screenshotAct);
	if (!pref->tablet_mode) {
		videoMenu->addAction(screenshotsAct);
		videoMenu->addMenu(ontop_menu);
	}
	#ifdef VIDEOPREVIEW
	videoMenu->addSeparator();
	videoMenu->addAction(videoPreviewAct);
	#endif

	// AUDIO MENU
	audioMenu->addMenu(audiotrack_menu);
	audioMenu->addAction(loadAudioAct);
	audioMenu->addAction(unloadAudioAct);
	audioMenu->addMenu(audiofilter_menu);
	if (!pref->tablet_mode) {
		audioMenu->addMenu(audiochannels_menu);
		audioMenu->addMenu(stereomode_menu);
	}
	audioMenu->addAction(audioEqualizerAct);
	audioMenu->addSeparator();
	if (!pref->tablet_mode) {
		audioMenu->addAction(muteAct);
		audioMenu->addSeparator();
		audioMenu->addAction(decVolumeAct);
		audioMenu->addAction(incVolumeAct);
		audioMenu->addSeparator();
	}
	audioMenu->addAction(decAudioDelayAct);
	audioMenu->addAction(incAudioDelayAct);
	audioMenu->addAction(audioDelayAct);


	// SUBTITLES MENU
	subtitlesMenu->addMenu(subtitles_track_menu);
	#ifdef MPV_SUPPORT
	subtitlesMenu->addMenu(secondary_subtitles_track_menu);
	#endif
	subtitlesMenu->addSeparator();
	subtitlesMenu->addAction(loadSubsAct);
	subtitlesMenu->addAction(unloadSubsAct);
	if (!pref->tablet_mode) {
		subtitlesMenu->addMenu(subfps_menu);
		subtitlesMenu->addSeparator();
		subtitlesMenu->addMenu(closed_captions_menu);
	}
	subtitlesMenu->addSeparator();
	subtitlesMenu->addAction(decSubDelayAct);
	subtitlesMenu->addAction(incSubDelayAct);
	subtitlesMenu->addAction(subDelayAct);
	subtitlesMenu->addSeparator();
	if (!pref->tablet_mode) {
		subtitlesMenu->addAction(decSubPosAct);
		subtitlesMenu->addAction(incSubPosAct);
		subtitlesMenu->addSeparator();
		subtitlesMenu->addAction(decSubScaleAct);
		subtitlesMenu->addAction(incSubScaleAct);
		subtitlesMenu->addSeparator();
		subtitlesMenu->addAction(decSubStepAct);
		subtitlesMenu->addAction(incSubStepAct);
		#ifdef MPV_SUPPORT
		subtitlesMenu->addSeparator();
		subtitlesMenu->addAction(seekPrevSubAct);
		subtitlesMenu->addAction(seekNextSubAct);
		#endif
		subtitlesMenu->addSeparator();
		subtitlesMenu->addAction(useForcedSubsOnlyAct);
		subtitlesMenu->addSeparator();
	}
	subtitlesMenu->addAction(subVisibilityAct);
	subtitlesMenu->addSeparator();
	subtitlesMenu->addAction(useCustomSubStyleAct);
	#ifdef FIND_SUBTITLES
	subtitlesMenu->addSeparator();
	subtitlesMenu->addAction(showFindSubtitlesDialogAct);
	if (!pref->tablet_mode) {
		subtitlesMenu->addAction(openUploadSubtitlesPageAct);
	}
	#endif

	// BROWSE MENU
	if (!pref->tablet_mode) {
		browseMenu->addMenu(titles_menu);
	}
	browseMenu->addMenu(chapters_menu);
	if (!pref->tablet_mode) {
		browseMenu->addMenu(angles_menu);
	}
	#ifdef BOOKMARKS
	browseMenu->addMenu(bookmark_menu);
	#endif
	#if DVDNAV_SUPPORT
	if (!pref->tablet_mode) {
		browseMenu->addSeparator();
		browseMenu->addAction(dvdnavMenuAct);
		browseMenu->addAction(dvdnavPrevAct);
	}
	#endif
	#if PROGRAM_SWITCH
	if (!pref->tablet_mode) {
		browseMenu->addSeparator();
		browseMenu->addMenu(programtrack_menu);
	}
	#endif

	// VIEW MENU
	viewMenu->addAction(showPropertiesAct);
	viewMenu->addAction(showPlaylistAct);
	#ifdef YOUTUBE_SUPPORT
	if (!pref->tablet_mode) {
		viewMenu->addAction(showTubeBrowserAct);
	}
	#endif
	viewMenu->addMenu(osd_menu);
	#if defined(LOG_MPLAYER) || defined(LOG_SMPLAYER)
	if (!pref->tablet_mode) {
		viewMenu->addSeparator()->setText(tr("Logs"));
		#ifdef LOG_MPLAYER
		viewMenu->addAction(showLogMplayerAct);
		#endif
		#ifdef LOG_SMPLAYER
		viewMenu->addAction(showLogSmplayerAct);
		#endif
	}
	#endif

	// OPTIONS MENU
	optionsMenu->addAction(showPreferencesAct);
	optionsMenu->addAction(tabletModeAct);

	// HELP MENU
	#ifdef SHARE_MENU
	if (!pref->tablet_mode) {
		helpMenu->addMenu(share_menu);
		helpMenu->addSeparator();
	}
	#endif
	if (!pref->tablet_mode) {
		helpMenu->addAction(showFirstStepsAct);
		helpMenu->addAction(showFAQAct);
		helpMenu->addAction(showCLOptionsAct);
		helpMenu->addSeparator();
	}
	helpMenu->addAction(showCheckUpdatesAct);
	#if defined(YOUTUBE_SUPPORT) && defined(YT_USE_YTSIG)
	helpMenu->addAction(updateYTAct);
	#endif
	helpMenu->addSeparator();
	if (!pref->tablet_mode) {
		helpMenu->addAction(showConfigAct);
		helpMenu->addSeparator();
	}
	helpMenu->addAction(donateAct);
	helpMenu->addSeparator();
	helpMenu->addAction(aboutThisAct);

	// Access menu
	access_menu->clear();
	access_menu->addAction(openFileAct);
	access_menu->addAction(openURLAct);
	access_menu->addMenu(recentfiles_menu);
	access_menu->addMenu(favorites);
	access_menu->addSeparator();
	access_menu->addAction(playPrevAct);
	access_menu->addAction(playNextAct);
	access_menu->addSeparator();
	access_menu->addMenu(audiotrack_menu);
	access_menu->addMenu(subtitles_track_menu);
	access_menu->addSeparator();
	access_menu->addMenu(aspect_menu);
	access_menu->addSeparator();
	access_menu->addAction(showPlaylistAct);
	access_menu->addAction(showPreferencesAct);
	access_menu->addAction(tabletModeAct);
}

/*
void BaseGui::closeEvent( QCloseEvent * e )  {
	qDebug("BaseGui::closeEvent");

	qDebug("mplayer_log_window: %d x %d", mplayer_log_window->width(), mplayer_log_window->height() );
	qDebug("smplayer_log_window: %d x %d", smplayer_log_window->width(), smplayer_log_window->height() );

	mplayer_log_window->close();
	smplayer_log_window->close();
	playlist->close();
	equalizer->close();

	core->stop();
	e->accept();
}
*/


void BaseGui::closeWindow() {
	qDebug("BaseGui::closeWindow");

	if (core->state() != Core::Stopped) {
		core->stop();
	}

	//qApp->quit();
	emit quitSolicited();
}

void BaseGui::showPlaylist() {
	showPlaylist( !playlist->isVisible() );
}

void BaseGui::showPlaylist(bool b) {
	if ( !b ) {
		playlist->hide();
	} else {
		exitFullscreenIfNeeded();
		playlist->show();
	}
	//updateWidgets();
}

void BaseGui::showVideoEqualizer() {
	showVideoEqualizer( !video_equalizer->isVisible() );
}

void BaseGui::showVideoEqualizer(bool b) {
	if (!b) {
		video_equalizer->hide();
	} else {
		// Exit fullscreen, otherwise dialog is not visible
		exitFullscreenIfNeeded();
		video_equalizer->show();
	}
	updateWidgets();
}

void BaseGui::showAudioEqualizer() {
	showAudioEqualizer( !audio_equalizer->isVisible() );
}

void BaseGui::showAudioEqualizer(bool b) {
	if (!b) {
		audio_equalizer->hide();
	} else {
		// Exit fullscreen, otherwise dialog is not visible
		exitFullscreenIfNeeded();
		audio_equalizer->show();
	}
	updateWidgets();
}

void BaseGui::showPreferencesDialog() {
	qDebug("BaseGui::showPreferencesDialog");

	exitFullscreenIfNeeded();
	
	if (!pref_dialog) {
		createPreferencesDialog();
	}

	pref_dialog->setData(pref);

	pref_dialog->mod_input()->actions_editor->clear();
	pref_dialog->mod_input()->actions_editor->addActions(this);
#if !DOCK_PLAYLIST
	pref_dialog->mod_input()->actions_editor->addActions(playlist);
#endif

	// Set playlist preferences
	PrefPlaylist * pl = pref_dialog->mod_playlist();
	pl->setDirectoryRecursion(playlist->directoryRecursion());
	pl->setAutoGetInfo(playlist->autoGetInfo());
	pl->setSavePlaylistOnExit(playlist->savePlaylistOnExit());
	pl->setPlayFilesFromStart(playlist->playFilesFromStart());
	pl->setPlayOnLoad(playlist->startPlayOnLoad());
	pl->setPlayNextAutomatically(playlist->automaticallyPlayNext());
	pl->setIgnorePlayerErrors(playlist->ignorePlayerErrors());
	pl->setAutoSort(playlist->autoSort());
	pl->setFilterCaseSensitive(playlist->filterCaseSensitive());

#ifdef PLAYLIST_DELETE_FROM_DISK
	pl->allowDeleteFromDisk(playlist->isDeleteFromDiskAllowed());
#endif

	pref_dialog->show();
}

// The user has pressed OK in preferences dialog
void BaseGui::applyNewPreferences() {
	qDebug("BaseGui::applyNewPreferences");

	bool need_update_language = false;
	bool need_apply_styles = false;

	PlayerID::Player old_player_type = PlayerID::player(pref->mplayer_bin);

	pref_dialog->getData(pref);

	// Setup proxy
	setupNetworkProxy();

	PrefGeneral *_general = pref_dialog->mod_general();
	if (_general->fileSettingsMethodChanged()) {
		core->changeFileSettingsMethod(pref->file_settings_method);
	}

	PrefInterface *_interface = pref_dialog->mod_interface();
	if (_interface->recentsChanged()) {
		updateRecents();
	}
	if (_interface->languageChanged()) need_update_language = true;

	if (_interface->iconsetChanged()) { 
		need_update_language = true;
		// Stylesheet
		#if ALLOW_CHANGE_STYLESHEET
		if (!_interface->guiChanged()) need_apply_styles = true;
		#endif
	}

	if (_interface->fontChanged()) need_apply_styles = true;

#ifndef MOUSE_GESTURES
	mplayerwindow->activateMouseDragTracking(pref->drag_function == Preferences::MoveWindow);
#endif
	mplayerwindow->delayLeftClick(pref->delay_left_click);

#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
	if (pref->hide_video_window_on_audio_files) {
		connect( core, SIGNAL(noVideo()), this, SLOT(hidePanel()) );
		disconnect( core, SIGNAL(noVideo()), mplayerwindow, SLOT(hideLogo()) );
	} else {
		disconnect( core, SIGNAL(noVideo()), this, SLOT(hidePanel()) );
		connect( core, SIGNAL(noVideo()), mplayerwindow, SLOT(showLogo()) );
		if (!panel->isVisible()) {
			resize( width(), height() + 200);
			panel->show();
		}
	}
#endif

	PrefAdvanced *advanced = pref_dialog->mod_advanced();
#if REPAINT_BACKGROUND_OPTION
	if (advanced->repaintVideoBackgroundChanged()) {
		mplayerwindow->videoLayer()->setRepaintBackground(pref->repaint_video_background);
	}
#endif
#if USE_COLORKEY
	if (advanced->colorkeyChanged()) {
		mplayerwindow->setColorKey( pref->color_key );
	}
#endif
	if (advanced->monitorAspectChanged()) {
		mplayerwindow->setMonitorAspect( pref->monitor_aspect_double() );
	}
#if ALLOW_DEMUXER_CODEC_CHANGE
	if (advanced->lavfDemuxerChanged()) {
		core->mset.forced_demuxer = pref->use_lavf_demuxer ? "lavf" : "";
	}
#endif

	// Update playlist preferences
	PrefPlaylist * pl = pref_dialog->mod_playlist();
	playlist->setDirectoryRecursion(pl->directoryRecursion());
	playlist->setAutoGetInfo(pl->autoGetInfo());
	playlist->setSavePlaylistOnExit(pl->savePlaylistOnExit());
	playlist->setPlayFilesFromStart(pl->playFilesFromStart());
	playlist->setStartPlayOnLoad(pl->playOnLoad());
	playlist->setAutomaticallyPlayNext(pl->playNextAutomatically());
	playlist->setIgnorePlayerErrors(pl->ignorePlayerErrors());
	playlist->setAutoSort(pl->autoSort());
	playlist->setFilterCaseSensitive(pl->filterCaseSensitive());

#ifdef PLAYLIST_DELETE_FROM_DISK
	playlist->allowDeleteFromDisk(pl->isDeleteFromDiskAllowed());
#endif

#ifdef PLAYLIST_DOWNLOAD
	playlist->setMaxItemsUrlHistory( pref->history_urls->maxItems() );
#endif

	if (need_update_language) {
		translator->load(pref->language);
	}

	setJumpTexts(); // Update texts in menus
	updateWidgets(); // Update the screenshot action

#if STYLE_SWITCHING
	if (_interface->styleChanged()) {
		need_apply_styles = true;
	}
#endif

	if (need_apply_styles) applyStyles();

	// Restart the video if needed
	if (pref_dialog->requiresRestart())
		core->restart();

	// Update actions
	pref_dialog->mod_input()->actions_editor->applyChanges();
	saveActions();

	pref->save();

	emit preferencesChanged();

	if (_interface->guiChanged()) {
#ifdef GUI_CHANGE_ON_RUNTIME
		core->stop();
		emit guiChanged(pref->gui);
#else
		QMessageBox::information(this, tr("Information"),
			tr("You need to restart SMPlayer to use the new GUI.") );
#endif
	}

	if (old_player_type != PlayerID::player(pref->mplayer_bin)) {
		qDebug("BaseGui::applyNewPreferences: player changed!");
		// Hack, simulate a change of GUI to restart the interface
		// FIXME: try to create a new Core::proc in the future
		#ifdef GUI_CHANGE_ON_RUNTIME
		core->stop();
		emit guiChanged(pref->gui);
		#endif
	}
}


void BaseGui::showFilePropertiesDialog() {
	qDebug("BaseGui::showFilePropertiesDialog");

	exitFullscreenIfNeeded();

	if (!file_dialog) {
		createFilePropertiesDialog();
	}

	setDataToFileProperties();

	file_dialog->show();
}

void BaseGui::setDataToFileProperties() {
#if ALLOW_DEMUXER_CODEC_CHANGE
	InfoReader *i = InfoReader::obj();
	i->getInfo();
	file_dialog->setCodecs( i->vcList(), i->acList(), i->demuxerList() );

	// Save a copy of the original values
	if (core->mset.original_demuxer.isEmpty()) 
		core->mset.original_demuxer = core->mdat.demuxer;

	if (core->mset.original_video_codec.isEmpty()) 
		core->mset.original_video_codec = core->mdat.video_codec;

	if (core->mset.original_audio_codec.isEmpty()) 
		core->mset.original_audio_codec = core->mdat.audio_codec;

	QString demuxer = core->mset.forced_demuxer;
	if (demuxer.isEmpty()) demuxer = core->mdat.demuxer;

	QString ac = core->mset.forced_audio_codec;
	if (ac.isEmpty()) ac = core->mdat.audio_codec;

	QString vc = core->mset.forced_video_codec;
	if (vc.isEmpty()) vc = core->mdat.video_codec;

	file_dialog->setDemuxer(demuxer, core->mset.original_demuxer);
	file_dialog->setAudioCodec(ac, core->mset.original_audio_codec);
	file_dialog->setVideoCodec(vc, core->mset.original_video_codec);
#endif

	file_dialog->setMplayerAdditionalArguments( core->mset.mplayer_additional_options );
	file_dialog->setMplayerAdditionalVideoFilters( core->mset.mplayer_additional_video_filters );
	file_dialog->setMplayerAdditionalAudioFilters( core->mset.mplayer_additional_audio_filters );

	file_dialog->setMediaData( core->mdat );
}

void BaseGui::applyFileProperties() {
	qDebug("BaseGui::applyFileProperties");

	bool need_restart = false;

#undef TEST_AND_SET
#define TEST_AND_SET( Pref, Dialog ) \
	if ( Pref != Dialog ) { Pref = Dialog; need_restart = true; }

#if ALLOW_DEMUXER_CODEC_CHANGE
	bool demuxer_changed = false;

	QString prev_demuxer = core->mset.forced_demuxer;

	QString demuxer = file_dialog->demuxer();
	if (demuxer == core->mset.original_demuxer) demuxer="";
	TEST_AND_SET(core->mset.forced_demuxer, demuxer);

	if (prev_demuxer != core->mset.forced_demuxer) {
		// Demuxer changed
		demuxer_changed = true;
		core->mset.current_audio_id = MediaSettings::NoneSelected;
		core->mset.current_sub_id = MediaSettings::NoneSelected;
	}

	QString ac = file_dialog->audioCodec();
	if (ac == core->mset.original_audio_codec) ac="";
	TEST_AND_SET(core->mset.forced_audio_codec, ac);

	QString vc = file_dialog->videoCodec();
	if (vc == core->mset.original_video_codec) vc="";
	TEST_AND_SET(core->mset.forced_video_codec, vc);
#endif

	TEST_AND_SET(core->mset.mplayer_additional_options, file_dialog->mplayerAdditionalArguments());
	TEST_AND_SET(core->mset.mplayer_additional_video_filters, file_dialog->mplayerAdditionalVideoFilters());
	TEST_AND_SET(core->mset.mplayer_additional_audio_filters, file_dialog->mplayerAdditionalAudioFilters());

#if ALLOW_DEMUXER_CODEC_CHANGE
	// Restart the video to apply
	if (need_restart) {
		if (demuxer_changed) {
			core->reload();
		} else {
			core->restart();
		}
	}
#endif
}


void BaseGui::updateMediaInfo() {
	qDebug("BaseGui::updateMediaInfo");

	if (file_dialog) {
		if (file_dialog->isVisible()) setDataToFileProperties();
	}

	setWindowCaption( core->mdat.displayName(pref->show_tag_in_window_title) + " - SMPlayer" );

	emit videoInfoChanged(core->mdat.video_width, core->mdat.video_height, core->mdat.video_fps.toDouble());
}

void BaseGui::newMediaLoaded() {
	qDebug("BaseGui::newMediaLoaded");

	QString stream_title = core->mdat.stream_title;
	qDebug("BaseGui::newMediaLoaded: mdat.stream_title: %s", stream_title.toUtf8().constData());

	if (!stream_title.isEmpty()) {
		pref->history_recents->addItem( core->mdat.filename, stream_title );
		//pref->history_recents->list();
	} else {
		pref->history_recents->addItem( core->mdat.filename );
	}
	updateRecents();

	// If a VCD, Audio CD or DVD, add items to playlist
	bool is_disc = ( (core->mdat.type == TYPE_VCD) || (core->mdat.type == TYPE_DVD) || (core->mdat.type == TYPE_AUDIO_CD) );
#if DVDNAV_SUPPORT
	// Don't add the list of titles if using dvdnav
	if ((core->mdat.type == TYPE_DVD) && (core->mdat.filename.startsWith("dvdnav:"))) is_disc = false;
#endif
	if (pref->auto_add_to_playlist && is_disc)
	{
		int first_title = 1;
		if (core->mdat.type == TYPE_VCD) first_title = pref->vcd_initial_title;

		QString type = "dvd"; // FIXME: support dvdnav
		if (core->mdat.type == TYPE_VCD) type="vcd";
		else
		if (core->mdat.type == TYPE_AUDIO_CD) type="cdda";

		if (core->mset.current_title_id == first_title) {
			playlist->clear();
			QStringList l;
			QString s;
			QString folder;
			if (core->mdat.type == TYPE_DVD) {
				DiscData disc_data = DiscName::split(core->mdat.filename);
				folder = disc_data.device;
			}
			for (int n=0; n < core->mdat.titles.numItems(); n++) {
				s = type + "://" + QString::number(core->mdat.titles.itemAt(n).ID());
				if ( !folder.isEmpty() ) {
					s += "/" + folder; // FIXME: dvd names are not created as they should
				}
				l.append(s);
			}
			playlist->addFiles(l);
			//playlist->setModified(false); // Not a real playlist
		}
	} /*else {
		playlist->clear();
		playlist->addCurrentFile();
	}*/

	// Automatically add files to playlist
	if ((core->mdat.type == TYPE_FILE) && (pref->auto_add_to_playlist)) {
		//qDebug("BaseGui::newMediaLoaded: playlist count: %d", playlist->count());
		QStringList files_to_add;
		if (playlist->count() == 1) {
			files_to_add = Helper::filesForPlaylist(core->mdat.filename, pref->media_to_add_to_playlist);
		}
		if (!files_to_add.empty()) playlist->addFiles(files_to_add);
	}
}

void BaseGui::gotNoFileToPlay() {
	//qDebug("BaseGui::gotNoFileToPlay");
	playlist->resumePlay();
}

#ifdef LOG_MPLAYER
void BaseGui::clearMplayerLog() {
	mplayer_log.clear();
	if (mplayer_log_window->isVisible()) mplayer_log_window->clear();
}

void BaseGui::recordMplayerLog(QString line) {
	if (pref->log_mplayer) {
		if ( (line.indexOf("A:")==-1) && (line.indexOf("V:")==-1) ) {
			line.append("\n");
			mplayer_log.append(line);
			if (mplayer_log_window->isVisible()) mplayer_log_window->appendText(line);
		}
	}
}

/*! 
	Save the mplayer log to a file, so it can be used by external
	applications.
*/
void BaseGui::autosaveMplayerLog() {
	qDebug("BaseGui::autosaveMplayerLog");

	if (pref->autosave_mplayer_log) {
		if (!pref->mplayer_log_saveto.isEmpty()) {
			QFile file( pref->mplayer_log_saveto );
			if ( file.open( QIODevice::WriteOnly ) ) {
				QTextStream strm( &file );
				strm << mplayer_log;
				file.close();
			}
		}
	}
}

void BaseGui::showMplayerLog() {
	qDebug("BaseGui::showMplayerLog");

	exitFullscreenIfNeeded();

	mplayer_log_window->setText( mplayer_log );
	mplayer_log_window->show();
}
#endif

#ifdef LOG_SMPLAYER
void BaseGui::recordSmplayerLog(QString line) {
	if (pref->log_smplayer) {
		line.append("\n");
		smplayer_log.append(line);
		if (smplayer_log_window->isVisible()) smplayer_log_window->appendText(line);
	}
}

void BaseGui::showLog() {
	qDebug("BaseGui::showLog");

	exitFullscreenIfNeeded();

	smplayer_log_window->setText( smplayer_log );
	smplayer_log_window->show();
}
#endif


void BaseGui::initializeMenus() {
	qDebug("BaseGui::initializeMenus");

#define EMPTY 1

	int n;

	// Subtitles
	subtitleTrackGroup->clear(true);
	QAction * subNoneAct = subtitleTrackGroup->addAction( tr("&None") );
	subNoneAct->setData(MediaSettings::SubNone);
	subNoneAct->setCheckable(true);
	for (n=0; n < core->mdat.subs.numItems(); n++) {
		QAction *a = new QAction(subtitleTrackGroup);
		a->setCheckable(true);
		a->setText(core->mdat.subs.itemAt(n).displayName());
		a->setData(n);
	}
	subtitles_track_menu->addActions( subtitleTrackGroup->actions() );

#ifdef MPV_SUPPORT
	// Secondary Subtitles
	secondarySubtitleTrackGroup->clear(true);
	QAction * subSecNoneAct = secondarySubtitleTrackGroup->addAction( tr("&None") );
	subSecNoneAct->setData(MediaSettings::SubNone);
	subSecNoneAct->setCheckable(true);
	for (n=0; n < core->mdat.subs.numItems(); n++) {
		QAction *a = new QAction(secondarySubtitleTrackGroup);
		a->setCheckable(true);
		a->setText(core->mdat.subs.itemAt(n).displayName());
		a->setData(n);
	}
	secondary_subtitles_track_menu->addActions( secondarySubtitleTrackGroup->actions() );
#endif

	// Audio
	audioTrackGroup->clear(true);
	// If using an external audio file, show the file in the menu, but disabled.
	if (!core->mset.external_audio.isEmpty()) {
		QAction * a = audioTrackGroup->addAction( QFileInfo(core->mset.external_audio).fileName() );
		a->setEnabled(false);
		a->setCheckable(true);
		a->setChecked(true);
	}
	else
	if (core->mdat.audios.numItems()==0) {
		QAction * a = audioTrackGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	} else {
		for (n=0; n < core->mdat.audios.numItems(); n++) {
			QAction *a = new QAction(audioTrackGroup);
			a->setCheckable(true);
			a->setText(core->mdat.audios.itemAt(n).displayName());
			a->setData(core->mdat.audios.itemAt(n).ID());
		}
	}
	audiotrack_menu->addActions( audioTrackGroup->actions() );

#if PROGRAM_SWITCH
	// Program
	programTrackGroup->clear(true);
	if (core->mdat.programs.numItems()==0) {
		QAction * a = programTrackGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	} else {
		for (n=0; n < core->mdat.programs.numItems(); n++) {
			QAction *a = new QAction(programTrackGroup);
			a->setCheckable(true);
			a->setText(core->mdat.programs.itemAt(n).displayName());
			a->setData(core->mdat.programs.itemAt(n).ID());
		}
	}
	programtrack_menu->addActions( programTrackGroup->actions() );
#endif

	// Video
	videoTrackGroup->clear(true);
	if (core->mdat.videos.numItems()==0) {
		QAction * a = videoTrackGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	} else {
		for (n=0; n < core->mdat.videos.numItems(); n++) {
			QAction *a = new QAction(videoTrackGroup);
			a->setCheckable(true);
			a->setText(core->mdat.videos.itemAt(n).displayName());
			a->setData(core->mdat.videos.itemAt(n).ID());
		}
	}
	videotrack_menu->addActions( videoTrackGroup->actions() );

	// Titles
	titleGroup->clear(true);
	if (core->mdat.titles.numItems()==0) {
		QAction * a = titleGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	} else {
		for (n=0; n < core->mdat.titles.numItems(); n++) {
			QAction *a = new QAction(titleGroup);
			a->setCheckable(true);
			a->setText(core->mdat.titles.itemAt(n).displayName());
			a->setData(core->mdat.titles.itemAt(n).ID());
		}
	}
	titles_menu->addActions( titleGroup->actions() );

	// Chapters
	chapterGroup->clear(true);
	//qDebug("BaseGui::initializeMenus: mdat.chapters.numItems: %d", core->mdat.chapters.numItems());
	if (core->mdat.chapters.numItems() > 0) {
		for (n=0; n < core->mdat.chapters.numItems(); n++) {
			QAction *a = new QAction(chapterGroup);
			//a->setCheckable(true);
			//qDebug("BaseGui::initializeMenus: chapter %d name: %s", n, core->mdat.chapters.itemAt(n).name().toUtf8().constData());
			a->setText(core->mdat.chapters.itemAt(n).name());
			a->setData(core->mdat.chapters.itemAt(n).ID());
		}
	} 
	else
	if (core->mdat.n_chapters > 0) {
		for (n=0; n < core->mdat.n_chapters; n++) {
			QAction *a = new QAction(chapterGroup);
			//a->setCheckable(true);
			a->setText( QString::number(n+1) );
			a->setData( n + Core::firstChapter() );
		}
	} 
	else {
		QAction * a = chapterGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	}
	chapters_menu->addActions( chapterGroup->actions() );

	// Angles
	angleGroup->clear(true);
	int n_angles = 0;
	if (core->mset.current_angle_id > 0) {
		int i = core->mdat.titles.find(core->mset.current_angle_id);
		if (i > -1) n_angles = core->mdat.titles.itemAt(i).angles();
	}
	if (n_angles > 0) {
		for (n=1; n <= n_angles; n++) {
			QAction *a = new QAction(angleGroup);
			a->setCheckable(true);
			a->setText( QString::number(n) );
			a->setData( n );
		}
	} else {
		QAction * a = angleGroup->addAction( tr("<empty>") );
		a->setEnabled(false);
	}
	angles_menu->addActions( angleGroup->actions() );

#ifdef BOOKMARKS
	updateBookmarks();
#endif
}

#ifdef BOOKMARKS
void BaseGui::updateBookmarks() {
	qDebug("BaseGui::updateBookmarks");

	// Bookmarks
	bookmarkGroup->clear(true);
	int n_bookmarks = core->mset.bookmarks.size();
	if (n_bookmarks > 0) {
		QMap<int, QString>::const_iterator i = core->mset.bookmarks.constBegin();
		while (i != core->mset.bookmarks.constEnd()) {
			QString name = i.value();
			int time = i.key();
			QAction *a = new QAction(bookmarkGroup);
			QString text;
			if (name.isEmpty()) {
				text = Helper::formatTime(time);
			} else {
				text = QString("%1 (%2)").arg(name).arg(Helper::formatTime(time));
			}
			a->setCheckable(false);
			a->setText(text);
			a->setData(time);
			i++;
		}
	}
	bookmark_menu->clear();
	bookmark_menu->addAction(addBookmarkAct);
	bookmark_menu->addAction(editBookmarksAct);
	bookmark_menu->addSeparator();
	bookmark_menu->addActions(bookmarkGroup->actions());
}
#endif

void BaseGui::updateRecents() {
	qDebug("BaseGui::updateRecents");

	recentfiles_menu->clear();

	int current_items = 0;

	if (pref->history_recents->count() > 0) {
		for (int n=0; n < pref->history_recents->count(); n++) {
			QString i = QString::number( n+1 );
			QString fullname = pref->history_recents->item(n);
			QString filename = fullname;
			QFileInfo fi(fullname);
			//if (fi.exists()) filename = fi.fileName(); // Can be slow

			// Let's see if it looks like a file (no dvd://1 or something)
			if (fullname.indexOf(QRegExp("^.*://.*")) == -1) filename = fi.fileName();

			if (filename.size() > 85) {
				filename = filename.left(80) + "...";
			}

			QString show_name = filename;
			QString title = pref->history_recents->title(n);
			if (!title.isEmpty()) show_name = title;

			QAction * a = recentfiles_menu->addAction( QString("%1. " + show_name ).arg( i.insert( i.size()-1, '&' ), 3, ' ' ));
			a->setStatusTip(fullname);
			a->setData(n);
			connect(a, SIGNAL(triggered()), this, SLOT(openRecent()));
			current_items++;
		}
	} else {
		QAction * a = recentfiles_menu->addAction( tr("<empty>") );
		a->setEnabled(false);
	}

	recentfiles_menu->menuAction()->setVisible( current_items > 0 );
	if (current_items  > 0) {
		recentfiles_menu->addSeparator();
		recentfiles_menu->addAction( clearRecentsAct );
	}
}

void BaseGui::clearRecentsList() {
	int ret = QMessageBox::question(this, tr("Confirm deletion - SMPlayer"),
				tr("Delete the list of recent files?"),
				QMessageBox::Cancel, QMessageBox::Ok);

	if (ret == QMessageBox::Ok) {
		// Delete items in menu
		pref->history_recents->clear();
		updateRecents();
	}
}

void BaseGui::updateWidgets() {
	qDebug("BaseGui::updateWidgets");

	// Subtitles menu
	subtitleTrackGroup->setChecked( core->mset.current_sub_id );

#ifdef MPV_SUPPORT
	// Secondary subtitles menu
	secondarySubtitleTrackGroup->setChecked( core->mset.current_secondary_sub_id );
#endif

	// Disable the unload subs action if there's no external subtitles
	unloadSubsAct->setEnabled( !core->mset.external_subtitles.isEmpty() );

	subFPSGroup->setEnabled( !core->mset.external_subtitles.isEmpty() );

	// Closed caption menu
	ccGroup->setChecked( core->mset.closed_caption_channel );

	// Subfps menu
	subFPSGroup->setChecked( core->mset.external_subtitles_fps );

	// Audio menu
	audioTrackGroup->setChecked( core->mset.current_audio_id );
	channelsGroup->setChecked( core->mset.audio_use_channels );
	stereoGroup->setChecked( core->mset.stereo_mode );
	// Disable the unload audio file action if there's no external audio file
	unloadAudioAct->setEnabled( !core->mset.external_audio.isEmpty() );

#if PROGRAM_SWITCH
	// Program menu
	programTrackGroup->setChecked( core->mset.current_program_id );
#endif

	// Video menu
	videoTrackGroup->setChecked( core->mset.current_video_id );

	// Aspect ratio
	aspectGroup->setChecked( core->mset.aspect_ratio_id );

	// Rotate
	rotateGroup->setChecked( core->mset.rotate );

#if USE_ADAPTER
	screenGroup->setChecked( pref->adapter );
#endif

	// OSD
	osdGroup->setChecked( pref->osd );

#ifdef MPV_SUPPORT
	OSDFractionsAct->setChecked(pref->osd_fractions);
#endif

	// Titles
	titleGroup->setChecked( core->mset.current_title_id );

	// Angles
	angleGroup->setChecked( core->mset.current_angle_id );

	// Deinterlace menu
	deinterlaceGroup->setChecked( core->mset.current_deinterlacer );

	// Video size menu
	sizeGroup->setChecked( pref->size_factor );

	// Auto phase
	phaseAct->setChecked( core->mset.phase_filter );

	// Deblock
	deblockAct->setChecked( core->mset.deblock_filter );

	// Dering
	deringAct->setChecked( core->mset.dering_filter );

	// Gradfun
	gradfunAct->setChecked( core->mset.gradfun_filter );

	// Add noise
	addNoiseAct->setChecked( core->mset.noise_filter );

	// Letterbox
	addLetterboxAct->setChecked( core->mset.add_letterbox );

	// Upscaling
	upscaleAct->setChecked( core->mset.upscaling_filter );


	// Postprocessing
	postProcessingAct->setChecked( core->mset.postprocessing_filter );

	// Denoise submenu
	denoiseGroup->setChecked( core->mset.current_denoiser );

	// Unsharp submenu
	unsharpGroup->setChecked( core->mset.current_unsharp );

	/*
	// Fullscreen button
	fullscreenbutton->setOn(pref->fullscreen); 

	// Mute button
	mutebutton->setOn(core->mset.mute);
	if (core->mset.mute) 
		mutebutton->setPixmap( Images::icon("mute_small") );
	else
		mutebutton->setPixmap( Images::icon("volume_small") );

	// Volume slider
	volumeslider->setValue( core->mset.volume );
	*/

	// Mute menu option
	muteAct->setChecked( (pref->global_volume ? pref->mute : core->mset.mute) );

	// Karaoke menu option
	karaokeAct->setChecked( core->mset.karaoke_filter );

	// Extrastereo menu option
	extrastereoAct->setChecked( core->mset.extrastereo_filter );

	// Volnorm menu option
	volnormAct->setChecked( core->mset.volnorm_filter );

#ifdef MPV_SUPPORT
	// Earwax menu option
	earwaxAct->setChecked( core->mset.earwax_filter );
#endif

	// Repeat menu option
	repeatAct->setChecked( core->mset.loop );

	// Fullscreen action
	fullscreenAct->setChecked( pref->fullscreen );

	// Time slider
	if (core->state()==Core::Stopped) {
		//FIXME
		//timeslider->setValue( (int) core->mset.current_sec );
	}

	// Video equalizer
	videoEqualizerAct->setChecked( video_equalizer->isVisible() );
	video_equalizer->setBySoftware( pref->use_soft_video_eq );

	// Audio equalizer
	audioEqualizerAct->setChecked( audio_equalizer->isVisible() );

	// Playlist
#if !DOCK_PLAYLIST
	//showPlaylistAct->setChecked( playlist->isVisible() );
#endif

#if DOCK_PLAYLIST
	showPlaylistAct->setChecked( playlist->isVisible() );
#endif

	// Compact mode
	compactAct->setChecked( pref->compact_mode );

	// Stay on top
	onTopActionGroup->setChecked( (int) pref->stay_on_top );

	// Flip
	flipAct->setChecked( core->mset.flip );

	// Mirror
	mirrorAct->setChecked( core->mset.mirror );

	// Use custom style
	useCustomSubStyleAct->setChecked( pref->enable_ass_styles );

	// Forced subs
	useForcedSubsOnlyAct->setChecked( pref->use_forced_subs_only );

	// Subtitle visibility
	subVisibilityAct->setChecked(pref->sub_visibility);

	// Enable or disable subtitle options
	bool e = ((core->mset.current_sub_id != MediaSettings::SubNone) &&
              (core->mset.current_sub_id != MediaSettings::NoneSelected));

	if (core->mset.closed_caption_channel !=0 ) e = true; // Enable if using closed captions

	decSubDelayAct->setEnabled(e);
	incSubDelayAct->setEnabled(e);
	subDelayAct->setEnabled(e);
	decSubPosAct->setEnabled(e);
	incSubPosAct->setEnabled(e);
	decSubScaleAct->setEnabled(e);
	incSubScaleAct->setEnabled(e);
	decSubStepAct->setEnabled(e);
	incSubStepAct->setEnabled(e);
#ifdef MPV_SUPPORT
	seekNextSubAct->setEnabled(e);
	seekPrevSubAct->setEnabled(e);
#endif

	tabletModeAct->setChecked(pref->tablet_mode);

#if defined(MPV_SUPPORT) && defined(MPLAYER_SUPPORT)
	if (PlayerID::player(pref->mplayer_bin) == PlayerID::MPLAYER) {
		secondary_subtitles_track_menu->setEnabled(false);
		frameBackStepAct->setEnabled(false);
		OSDFractionsAct->setEnabled(false);
		earwaxAct->setEnabled(false);
	} else {
		//karaokeAct->setEnabled(false);
	}
#endif
}

void BaseGui::updateVideoEqualizer() {
	// Equalizer
	video_equalizer->setContrast( core->mset.contrast );
	video_equalizer->setBrightness( core->mset.brightness );
	video_equalizer->setHue( core->mset.hue );
	video_equalizer->setSaturation( core->mset.saturation );
	video_equalizer->setGamma( core->mset.gamma );
}

void BaseGui::updateAudioEqualizer() {
	// Audio Equalizer
	AudioEqualizerList l = pref->global_audio_equalizer ? pref->audio_equalizer : core->mset.audio_equalizer;
	audio_equalizer->blockSignals(true);
	audio_equalizer->setEqualizer(l);
	audio_equalizer->blockSignals(false);
}

void BaseGui::setDefaultValuesFromVideoEqualizer() {
	qDebug("BaseGui::setDefaultValuesFromVideoEqualizer");

	pref->initial_contrast = video_equalizer->contrast();
	pref->initial_brightness = video_equalizer->brightness();
	pref->initial_hue = video_equalizer->hue();
	pref->initial_saturation = video_equalizer->saturation();
	pref->initial_gamma = video_equalizer->gamma();

	QMessageBox::information(this, tr("Information"), 
                             tr("The current values have been stored to be "
                                "used as default.") );
}

void BaseGui::changeVideoEqualizerBySoftware(bool b) {
	qDebug("BaseGui::changeVideoEqualizerBySoftware: %d", b);

	if (b != pref->use_soft_video_eq) {
		pref->use_soft_video_eq = b;
		core->restart();
	}
}

/*
void BaseGui::playlistVisibilityChanged() {
#if !DOCK_PLAYLIST
	bool visible = playlist->isVisible();

	showPlaylistAct->setChecked( visible );
#endif
}
*/

/*
void BaseGui::openRecent(int item) {
	qDebug("BaseGui::openRecent: %d", item);
	if ((item > -1) && (item < RECENTS_CLEAR)) { // 1000 = Clear item
		open( recents->item(item) );
	}
}
*/

void BaseGui::openRecent() {
	QAction *a = qobject_cast<QAction *> (sender());
	if (a) {
		int item = a->data().toInt();
		qDebug("BaseGui::openRecent: %d", item);
		QString file = pref->history_recents->item(item);

		if (pref->auto_add_to_playlist) {
			if (playlist->maybeSave()) {
				playlist->clear();
				playlist->addFile(file, Playlist::NoGetInfo);

				open( file );
			}
		} else {
			open( file );
		}

	}
}

void BaseGui::open(QString file) {
	qDebug("BaseGui::open: '%s'", file.toUtf8().data());

	// If file is a playlist, open that playlist
	QString extension = QFileInfo(file).suffix().toLower();
	if ( ((extension=="m3u") || (extension=="m3u8")) && (QFile::exists(file)) ) {
		playlist->load_m3u(file);
	} 
	else
	if (extension=="pls") {
		playlist->load_pls(file);
	}
	else
	if (extension=="xspf") {
		playlist->loadXSPF(file);
	}
	else
	if (QFileInfo(file).isDir()) {
		openDirectory(file);
	} 
	else {
		// Let the core to open it, autodetecting the file type
		//if (playlist->maybeSave()) {
		//	playlist->clear();
		//	playlist->addFile(file);

			core->open(file);
		//}
	}

	if (QFile::exists(file)) pref->latest_dir = QFileInfo(file).absolutePath();
}

void BaseGui::openFiles(QStringList files) {
	qDebug("BaseGui::openFiles");
	if (files.empty()) return;

	#ifdef Q_OS_WIN
	files = Helper::resolveSymlinks(files); // Check for Windows shortcuts
	#endif

	if (files.count()==1) {
		if (pref->auto_add_to_playlist) {
			if (playlist->maybeSave()) {
				playlist->clear();
				playlist->addFile(files[0], Playlist::NoGetInfo);

				open(files[0]);
			}
		} else {
			open(files[0]);
		}
	} else {
		if (playlist->maybeSave()) {
			playlist->clear();
			playlist->addFiles(files);
			open(files[0]);
		}
	}
}

void BaseGui::openFavorite(QString file) {
	qDebug("BaseGui::openFavorite");

	QUrl url(file);
	if (url.isValid() && url.scheme().toLower() == "file") {
		file = url.toLocalFile();
	}

	openFiles(QStringList() << file);
}

void BaseGui::openURL() {
	qDebug("BaseGui::openURL");

	exitFullscreenIfNeeded();

	/*
    bool ok;
    QString s = QInputDialog::getText(this, 
            tr("SMPlayer - Enter URL"), tr("URL:"), QLineEdit::Normal,
            pref->last_url, &ok );

    if ( ok && !s.isEmpty() ) {

		//playlist->clear();
		//playlistdock->hide();

		openURL(s);
    } else {
        // user entered nothing or pressed Cancel
    }
	*/

	InputURL d(this);

	// Get url from clipboard
	QString clipboard_text = QApplication::clipboard()->text();
	if ((!clipboard_text.isEmpty()) && (clipboard_text.contains("://")) /*&& (QUrl(clipboard_text).isValid())*/) {
		d.setURL(clipboard_text);
	}

	for (int n=0; n < pref->history_urls->count(); n++) {
		d.setURL( pref->history_urls->url(n) );
	}

	if (d.exec() == QDialog::Accepted ) {
		QString url = d.url();
		if (!url.isEmpty()) {
			pref->history_urls->addUrl(url);
			openURL(url);
		}
	}
}

void BaseGui::openURL(QString url) {
	if (!url.isEmpty()) {
		//pref->history_urls->addUrl(url);

		if (pref->auto_add_to_playlist) {
			if (playlist->maybeSave()) {
				core->openStream(url);

				playlist->clear();
				playlist->addFile(url, Playlist::NoGetInfo);
			}
		} else {
			core->openStream(url);
		}
	}
}


void BaseGui::openFile() {
	qDebug("BaseGui::openFile");

	exitFullscreenIfNeeded();

	Extensions e;
    QString s = MyFileDialog::getOpenFileName(
                       this, tr("Choose a file"), pref->latest_dir, 
                       tr("Multimedia") + e.allPlayable().forFilter()+";;" +
                       tr("Video") + e.video().forFilter()+";;" +
                       tr("Audio") + e.audio().forFilter()+";;" +
                       tr("Playlists") + e.playlist().forFilter()+";;" +
                       tr("All files") +" (*.*)" );

    if ( !s.isEmpty() ) {
		openFile(s);
	}
}

void BaseGui::openFile(QString file) {
	qDebug() << "BaseGui::openFile:" << file;

   if ( !file.isEmpty() ) {

		#ifdef Q_OS_WIN
		// Check for Windows shortcuts
		QFileInfo fi(file);
		if (fi.isSymLink()) {
			file = fi.symLinkTarget();
		}
		#endif
	
		//playlist->clear();
		//playlistdock->hide();

		// If file is a playlist, open that playlist
		QString extension = QFileInfo(file).suffix().toLower();
		if ( (extension=="m3u") || (extension=="m3u8") ) {
			playlist->load_m3u(file);
		} 
		else
		if (extension=="pls") {
			playlist->load_pls(file);
		}
		else
		if (extension=="xspf") {
			playlist->loadXSPF(file);
		}
		else
		if (extension=="iso") {
			if (playlist->maybeSave()) {
				core->open(file);
			}
		}
		else {
			if (pref->auto_add_to_playlist) {
				if (playlist->maybeSave()) {
					core->openFile(file);

					playlist->clear();
					playlist->addFile(file, Playlist::NoGetInfo);
				}
			} else {
				core->openFile(file);
			}
		}
		if (QFile::exists(file)) pref->latest_dir = QFileInfo(file).absolutePath();
	}
}

void BaseGui::configureDiscDevices() {
	QMessageBox::information( this, tr("SMPlayer - Information"),
			tr("The CDROM / DVD drives are not configured yet.\n"
			   "The configuration dialog will be shown now, "
               "so you can do it."), QMessageBox::Ok);
	
	showPreferencesDialog();
	pref_dialog->showSection( PreferencesDialog::Drives );
}

void BaseGui::openVCD() {
	qDebug("BaseGui::openVCD");

	if ( (pref->dvd_device.isEmpty()) || 
         (pref->cdrom_device.isEmpty()) )
	{
		configureDiscDevices();
	} else {
		if (playlist->maybeSave()) {
			core->openVCD( pref->vcd_initial_title );
		}
	}
}

void BaseGui::openAudioCD() {
	qDebug("BaseGui::openAudioCD");

	if ( (pref->dvd_device.isEmpty()) || 
         (pref->cdrom_device.isEmpty()) )
	{
		configureDiscDevices();
	} else {
		if (playlist->maybeSave()) {
			core->openAudioCD();
		}
	}
}

void BaseGui::openDVD() {
	qDebug("BaseGui::openDVD");

	if ( (pref->dvd_device.isEmpty()) || 
         (pref->cdrom_device.isEmpty()) )
	{
		configureDiscDevices();
	} else {
		if (playlist->maybeSave()) {
#if DVDNAV_SUPPORT
			int first_title = 0;
			if (!pref->use_dvdnav) first_title = core->firstDVDTitle();
			core->openDVD( DiscName::joinDVD(first_title, pref->dvd_device, pref->use_dvdnav) );
#else
			core->openDVD( DiscName::joinDVD(core->firstDVDTitle(), pref->dvd_device, false) );
#endif
		}
	}
}

void BaseGui::openDVDFromFolder() {
	qDebug("BaseGui::openDVDFromFolder");

	if (playlist->maybeSave()) {
		InputDVDDirectory *d = new InputDVDDirectory(this);
		d->setFolder( pref->last_dvd_directory );

		if (d->exec() == QDialog::Accepted) {
			qDebug("BaseGui::openDVDFromFolder: accepted");
			openDVDFromFolder( d->folder() );
		}

		delete d;
	}
}

void BaseGui::openDVDFromFolder(QString directory) {
	pref->last_dvd_directory = directory;
#if DVDNAV_SUPPORT
	int first_title = 0;
	if (!pref->use_dvdnav) first_title = core->firstDVDTitle();
	core->openDVD( DiscName::joinDVD(first_title, directory, pref->use_dvdnav) );
#else
	core->openDVD( DiscName::joinDVD(core->firstDVDTitle(), directory, false) );
#endif
}

#ifdef BLURAY_SUPPORT
/**
 * Minimal BaseGui abstraction for calling openBluRay. It's called from
 * OpenBluRayFromFolder()
 */
void BaseGui::openBluRayFromFolder(QString directory) {
	pref->last_dvd_directory = directory;
	core->openBluRay( DiscName::join(DiscName::BLURAY, core->firstBlurayTitle(), directory) );
}

/**
 * Attempts to open a bluray from pref->bluray_device. If not set, calls configureDiscDevices.
 * If successful, calls Core::OpenBluRay(QString)
 */
void BaseGui::openBluRay() {
	qDebug("BaseGui::openBluRay");

	if ( (pref->dvd_device.isEmpty()) || 
         (pref->cdrom_device.isEmpty()) || pref->bluray_device.isEmpty())
	{
		configureDiscDevices();
	} else {
		core->openBluRay( DiscName::join(DiscName::BLURAY, core->firstBlurayTitle(), pref->bluray_device) );
	}
}

void BaseGui::openBluRayFromFolder() {
	qDebug("BaseGui::openBluRayFromFolder");

	if (playlist->maybeSave()) {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Select the Blu-ray folder"),
                          pref->last_dvd_directory, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty()) {
			openBluRayFromFolder(dir);
		}
	}
}
#endif

void BaseGui::openDirectory() {
	qDebug("BaseGui::openDirectory");

	QString s = MyFileDialog::getExistingDirectory(
                    this, tr("Choose a directory"),
                    pref->latest_dir );

	if (!s.isEmpty()) {
		openDirectory(s);
	}
}

void BaseGui::openDirectory(QString directory) {
	qDebug("BaseGui::openDirectory: '%s'", directory.toUtf8().data());

	if (Helper::directoryContainsDVD(directory)) {
		core->open(directory);
	} 
	else {
		QFileInfo fi(directory);
		if ( (fi.exists()) && (fi.isDir()) ) {
			playlist->clear();
			//playlist->addDirectory(directory);
			playlist->addDirectory( fi.absoluteFilePath() );
			playlist->startPlay();
		} else {
			qDebug("BaseGui::openDirectory: directory is not valid");
		}
	}
}

void BaseGui::loadSub() {
	qDebug("BaseGui::loadSub");

	exitFullscreenIfNeeded();

	Extensions e;
    QString s = MyFileDialog::getOpenFileName(
        this, tr("Choose a file"), 
	    pref->latest_dir, 
        tr("Subtitles") + e.subtitles().forFilter()+ ";;" +
        tr("All files") +" (*.*)" );

	if (!s.isEmpty()) core->loadSub(s);
}

void BaseGui::setInitialSubtitle(const QString & subtitle_file) {
	qDebug("BaseGui::setInitialSubtitle: '%s'", subtitle_file.toUtf8().constData());

	core->setInitialSubtitle(subtitle_file);
}

void BaseGui::loadAudioFile() {
	qDebug("BaseGui::loadAudioFile");

	exitFullscreenIfNeeded();

	Extensions e;
	QString s = MyFileDialog::getOpenFileName(
        this, tr("Choose a file"), 
	    pref->latest_dir, 
        tr("Audio") + e.audio().forFilter()+";;" +
        tr("All files") +" (*.*)" );

	if (!s.isEmpty()) core->loadAudioFile(s);
}

void BaseGui::helpFirstSteps() {
	QDesktopServices::openUrl(QString(URL_FIRST_STEPS "?version=%1").arg(Version::printable()));
}

void BaseGui::helpFAQ() {
	QString url = URL_FAQ;
	/* if (!pref->language.isEmpty()) url += QString("?tr_lang=%1").arg(pref->language); */
	QDesktopServices::openUrl( QUrl(url) );
}

void BaseGui::helpCLOptions() {
	if (clhelp_window == 0) {
		clhelp_window = new InfoWindow(this);
	}
	clhelp_window->setWindowTitle( tr("SMPlayer command line options") );
	clhelp_window->setWindowIcon( Images::icon("logo") );
	clhelp_window->setHtml(CLHelp::help(true));
	clhelp_window->show();
}

void BaseGui::helpCheckUpdates() {
#ifdef UPDATE_CHECKER
	update_checker->check();
#else
	QString url = QString(URL_CHANGES "?version=%1").arg(Version::with_revision());
	QDesktopServices::openUrl( QUrl(url) );
#endif
}

void BaseGui::helpShowConfig() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(Paths::configPath()));
}

#ifdef SHARE_ACTIONS
void BaseGui::helpDonate() {
	ShareDialog d(this);
	d.showRemindCheck(false);

	#ifdef SHARE_WIDGET
	d.setActions(sharewidget->actions());
	#endif

	d.exec();
	int action = d.actions();
	qDebug("BaseGui::helpDonate: action: %d", action);

	if (action > 0) {
		#ifdef SHARE_WIDGET
		sharewidget->setActions(action);
		#else
		QSettings * set = Global::settings;
		set->beginGroup("reminder");
		set->setValue("action", action);
		set->endGroup();
		#endif
	}
}
#else
void BaseGui::helpDonate() {
	qDebug("BaseGui::helpDonate");

	int action = 0;
	bool accepted;
	showHelpDonateDialog(&accepted);
	if (accepted) action = 1;

	if (action > 0) {
		QSettings * set = Global::settings;
		set->beginGroup("reminder");
		set->setValue("action", action);
		set->endGroup();
	}
}

void BaseGui::showHelpDonateDialog(bool * accepted) {
	bool result = false;

	QMessageBox d(this);
	d.setIconPixmap(Images::icon("donate"));
	d.setWindowTitle(tr("Support SMPlayer"));

	QPushButton * ok_button = d.addButton(tr("Donate"), QMessageBox::YesRole);
	d.addButton(tr("No"), QMessageBox::NoRole);
	d.setDefaultButton(ok_button);

	d.setText("<h1>" + tr("SMPlayer needs you") + "</h1><p>" +
		tr("SMPlayer is free software. However the development requires a lot of time and a lot of work.") + "<p>" +
		tr("In order to keep developing SMPlayer with new features we need your help.") + "<p>" +
		tr("Please consider to support the SMPlayer project by sending a donation.") + " " +
		tr("Even the smallest amount will help a lot.")
	);
	d.exec();
	if (d.clickedButton() == ok_button) {
		QDesktopServices::openUrl(QUrl(URL_DONATE));
		result = true;
	}
	if (accepted != 0) *accepted = result;
}
#endif

void BaseGui::helpAbout() {
	About d(this);
	d.exec();
}

#ifdef SHARE_MENU
void BaseGui::shareSMPlayer() {
	QString text = QString("SMPlayer - Free Media Player with built-in codecs that can play and download Youtube videos").replace(" ","+");
	QString url = URL_HOMEPAGE;

	if (sender() == twitterAct) {
		QDesktopServices::openUrl(QUrl("http://twitter.com/intent/tweet?text=" + text + "&url=" + url + "/&via=smplayer_dev"));
	}
	else
	if (sender() == gmailAct) {
		QDesktopServices::openUrl(QUrl("https://mail.google.com/mail/?view=cm&fs=1&to&su=" + text + "&body=" + url + "&ui=2&tf=1&shva=1"));
	}
	else
	if (sender() == yahooAct) {
		QDesktopServices::openUrl(QUrl("http://compose.mail.yahoo.com/?To=&Subject=" + text + "&body=" + url));
	}
	else
	if (sender() == hotmailAct) {
		QDesktopServices::openUrl(QUrl("http://www.hotmail.msn.com/secure/start?action=compose&to=&subject=" + text + "&body=" + url));
	}
	else
	if (sender() == facebookAct) {
		QDesktopServices::openUrl(QUrl("http://www.facebook.com/sharer.php?u=" + url + "&t=" + text));

		#ifdef SHARE_ACTIONS
		QSettings * set = Global::settings;
		set->beginGroup("reminder");
		set->setValue("action", 2);
		set->endGroup();
		#endif
	}
}
#endif

void BaseGui::showGotoDialog() {
	TimeDialog d(this);
	d.setLabel(tr("&Jump to:"));
	d.setWindowTitle(tr("SMPlayer - Seek"));
	d.setMaximumTime( (int) core->mdat.duration);
	d.setTime( (int) core->mset.current_sec);
	if (d.exec() == QDialog::Accepted) {
		core->goToSec( d.time() );
	}
}

void BaseGui::showAudioDelayDialog() {
	bool ok;
	#if QT_VERSION >= 0x050000
	int delay = QInputDialog::getInt(this, tr("SMPlayer - Audio delay"),
                                     tr("Audio delay (in milliseconds):"), core->mset.audio_delay, 
                                     -3600000, 3600000, 1, &ok);
	#else
	int delay = QInputDialog::getInteger(this, tr("SMPlayer - Audio delay"),
                                         tr("Audio delay (in milliseconds):"), core->mset.audio_delay, 
                                         -3600000, 3600000, 1, &ok);
	#endif
	if (ok) {
		core->setAudioDelay(delay);
	}
}

void BaseGui::showSubDelayDialog() {
	bool ok;
	#if QT_VERSION >= 0x050000
	int delay = QInputDialog::getInt(this, tr("SMPlayer - Subtitle delay"),
                                     tr("Subtitle delay (in milliseconds):"), core->mset.sub_delay, 
                                     -3600000, 3600000, 1, &ok);
	#else
	int delay = QInputDialog::getInteger(this, tr("SMPlayer - Subtitle delay"),
                                         tr("Subtitle delay (in milliseconds):"), core->mset.sub_delay, 
                                         -3600000, 3600000, 1, &ok);
	#endif
	if (ok) {
		core->setSubDelay(delay);
	}
}

void BaseGui::showStereo3dDialog() {
	Stereo3dDialog d(this);
	d.setInputFormat(core->mset.stereo3d_in);
	d.setOutputFormat(core->mset.stereo3d_out);

	if (d.exec() == QDialog::Accepted) {
		core->changeStereo3d(d.inputFormat(), d.outputFormat());
	}
}

#ifdef BOOKMARKS
void BaseGui::showAddBookmarkDialog() {
	InputBookmark d(this);
	d.setTime( (int) core->mset.current_sec);
	if (d.exec() == QDialog::Accepted) {
		core->mset.bookmarks.insert(d.time(), d.name());
		updateBookmarks();
		core->saveBookmarks();
	}
}

void BaseGui::showBookmarkDialog() {
	qDebug("BaseGui::showBookmarkDialog");
	BookmarkDialog d(this);
	d.setBookmarks(core->mset.bookmarks);
	if (d.exec() == QDialog::Accepted) {
		core->mset.bookmarks = d.bookmarks();
		updateBookmarks();
		core->saveBookmarks();
	}
}
#endif

void BaseGui::exitFullscreen() {
	if (pref->fullscreen) {
		toggleFullscreen(false);
	}
}

void BaseGui::toggleFullscreen() {
	qDebug("BaseGui::toggleFullscreen");

	toggleFullscreen(!pref->fullscreen);
}

void BaseGui::toggleFullscreen(bool b) {
	qDebug("BaseGui::toggleFullscreen: %d", b);

	if (b==pref->fullscreen) {
		// Nothing to do
		qDebug("BaseGui::toggleFullscreen: nothing to do, returning");
		return;
	}

	pref->fullscreen = b;

	// If using mplayer window
	if (pref->use_mplayer_window) {
		core->changeFullscreenMode(b);
		updateWidgets();
		return;
	}

	if (!panel->isVisible()) return; // mplayer window is not used.

	if (pref->fullscreen) {
		compactAct->setEnabled(false);

		if (pref->restore_pos_after_fullscreen) {
			win_pos = pos();
			win_size = size();
		}

		was_maximized = isMaximized();
		qDebug("BaseGui::toggleFullscreen: was_maximized: %d", was_maximized);

		aboutToEnterFullscreen();

		#ifdef Q_OS_WIN
		// Hack to avoid the windows taskbar to be visible on Windows XP
		if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {
			if (!pref->pause_when_hidden) hide();
		}
		#endif

		showFullScreen();

	} else {
		showNormal();

		if (was_maximized) showMaximized(); // It has to be called after showNormal()

		aboutToExitFullscreen();

		if (pref->restore_pos_after_fullscreen) {
			move( win_pos );
			resize( win_size );
		}

		compactAct->setEnabled(true);
	}

	updateWidgets();

#ifdef ADD_BLACKBORDERS_FS
	if ((pref->add_blackborders_on_fullscreen) && 
        (!core->mset.add_letterbox)) 
	{
		core->changeLetterboxOnFullscreen(b);
		/* core->restart(); */
	}
#endif

	setFocus(); // Fixes bug #2493415
}


void BaseGui::aboutToEnterFullscreen() {
	if (pref->stay_on_top == Preferences::WhilePlayingOnTop && core->state() == Core::Playing) {
		setStayOnTop(false);
	}

	if (!pref->compact_mode) {
		menuBar()->hide();
		statusBar()->hide();
	}
}

void BaseGui::aboutToExitFullscreen() {
	if (!pref->compact_mode) {
		menuBar()->show();
		statusBar()->show();
	}

	if (pref->stay_on_top == Preferences::WhilePlayingOnTop) {
		#if QT_VERSION < 0x050000
		qApp->processEvents();
		#endif
		setStayOnTop(core->state() == Core::Playing);
	}
}


void BaseGui::leftClickFunction() {
	qDebug("BaseGui::leftClickFunction");

	if (!pref->mouse_left_click_function.isEmpty()) {
		processFunction(pref->mouse_left_click_function);
	}
}

void BaseGui::rightClickFunction() {
	qDebug("BaseGui::rightClickFunction");

	if (!pref->mouse_right_click_function.isEmpty()) {
		processFunction(pref->mouse_right_click_function);
	}
}

void BaseGui::doubleClickFunction() {
	qDebug("BaseGui::doubleClickFunction");

	if (!pref->mouse_double_click_function.isEmpty()) {
		processFunction(pref->mouse_double_click_function);
	}
}

void BaseGui::middleClickFunction() {
	qDebug("BaseGui::middleClickFunction");

	if (!pref->mouse_middle_click_function.isEmpty()) {
		processFunction(pref->mouse_middle_click_function);
	}
}

void BaseGui::xbutton1ClickFunction() {
	qDebug("BaseGui::xbutton1ClickFunction");

	if (!pref->mouse_xbutton1_click_function.isEmpty()) {
		processFunction(pref->mouse_xbutton1_click_function);
	}
}

void BaseGui::xbutton2ClickFunction() {
	qDebug("BaseGui::xbutton2ClickFunction");

	if (!pref->mouse_xbutton2_click_function.isEmpty()) {
		processFunction(pref->mouse_xbutton2_click_function);
	}
}

void BaseGui::processFunction(QString function) {
	qDebug("BaseGui::processFunction: '%s'", function.toUtf8().data());

	//parse args for checkable actions
	QRegExp func_rx("(.*) (true|false)");
	bool value = false;
	bool checkableFunction = false;

	if(func_rx.indexIn(function) > -1){
		function = func_rx.cap(1);
		value = (func_rx.cap(2) == "true");
		checkableFunction = true;
	} //end if

	QAction * action = ActionsEditor::findAction(this, function);
	if (!action) action = ActionsEditor::findAction(playlist, function);

	if (action) {
		qDebug("BaseGui::processFunction: action found");

		if (!action->isEnabled()) {
			qDebug("BaseGui::processFunction: action is disabled, doing nothing");
			return;
		}

		if (action->isCheckable()){
			if(checkableFunction)
				action->setChecked(value);
			else
				//action->toggle();
				action->trigger();
		}else{
			action->trigger();
		}
	}
}

void BaseGui::runActions(QString actions) {
	qDebug("BaseGui::runActions");

	actions = actions.simplified(); // Remove white space

	QAction * action;
	QStringList actionsList = actions.split(" ");

	for (int n = 0; n < actionsList.count(); n++) {
		QString actionStr = actionsList[n];
		QString par = ""; //the parameter which the action takes

		//set par if the next word is a boolean value
		if ( (n+1) < actionsList.count() ) {
			if ( (actionsList[n+1].toLower() == "true") || (actionsList[n+1].toLower() == "false") ) {
				par = actionsList[n+1].toLower();
				n++;
			} //end if
		} //end if

		action = ActionsEditor::findAction(this, actionStr);
		if (!action) action = ActionsEditor::findAction(playlist, actionStr);

		if (action) {
			qDebug("BaseGui::runActions: running action: '%s' (par: '%s')",
				   actionStr.toUtf8().data(), par.toUtf8().data() );

			if (action->isCheckable()) {
				if (par.isEmpty()) {
					//action->toggle();
					action->trigger();
				} else {
					action->setChecked( (par == "true") );
				} //end if
			} else {
				action->trigger();
			} //end if
		} else {
			qWarning("BaseGui::runActions: action: '%s' not found",actionStr.toUtf8().data());
		} //end if
	} //end for
}

void BaseGui::checkPendingActionsToRun() {
	qDebug("BaseGui::checkPendingActionsToRun");

	QString actions;
	if (!pending_actions_to_run.isEmpty()) {
		actions = pending_actions_to_run;
		pending_actions_to_run.clear();
		if (!pref->actions_to_run.isEmpty()) {
			actions = pref->actions_to_run +" "+ actions;
		}
	} else {
		actions = pref->actions_to_run;
	}

	if (!actions.isEmpty()) {
		qDebug("BaseGui::checkPendingActionsToRun: actions: '%s'", actions.toUtf8().constData());
		runActions(actions);
	}
}

#if REPORT_OLD_MPLAYER
void BaseGui::checkMplayerVersion() {
	qDebug("BaseGui::checkMplayerVersion");

	// Qt 4.3.5 is crazy, I can't popup a messagebox here, it calls 
	// this function once and again when the messagebox is shown

	if ( (pref->mplayer_detected_version > 0) && (!MplayerVersion::isMplayerAtLeast(25158)) ) {
		QTimer::singleShot(1000, this, SLOT(displayWarningAboutOldMplayer()));
	}
}

void BaseGui::displayWarningAboutOldMplayer() {
	qDebug("BaseGui::displayWarningAboutOldMplayer");

	if (!pref->reported_mplayer_is_old) {
		QMessageBox::warning(this, tr("Warning - Using old MPlayer"),
			tr("The version of MPlayer (%1) installed on your system "
               "is obsolete. SMPlayer can't work well with it: some "
               "options won't work, subtitle selection may fail...")
               .arg(MplayerVersion::toString(pref->mplayer_detected_version)) +
            "<br><br>" + 
            tr("Please, update your MPlayer.") +
            "<br><br>" + 
            tr("(This warning won't be displayed anymore)") );

		pref->reported_mplayer_is_old = true;
	}
	//else
	//statusBar()->showMessage( tr("Using an old MPlayer, please update it"), 10000 );
}
#endif

#ifdef CHECK_UPGRADED
void BaseGui::checkIfUpgraded() {
	qDebug("BaseGui::checkIfUpgraded");

	if ( (pref->check_if_upgraded) && (pref->smplayer_stable_version != Version::stable()) ) {
		// Running a new version
		qDebug("BaseGui::checkIfUpgraded: running a new version: %s", Version::stable().toUtf8().constData());
		QString os = "other";
		#ifdef Q_OS_WIN
		os = "win";
		#endif
		#ifdef Q_OS_LINUX
		os = "linux";
		#endif
		QDesktopServices::openUrl(QString(URL_THANK_YOU "?version=%1&so=%2").arg(Version::printable()).arg(os));
	}
	pref->smplayer_stable_version = Version::stable();
}
#endif

#ifdef DONATE_REMINDER
void BaseGui::checkReminder() {
	qDebug("BaseGui::checkReminder");

	if (core->state() == Core::Playing) return;

	QSettings * set = Global::settings;
	set->beginGroup("reminder");
	int count = set->value("count", 0).toInt();
	count++;
	set->setValue("count", count);
	int action = set->value("action", 0).toInt();
	bool dont_show = set->value("dont_show_anymore", false).toBool();
	set->endGroup();

#if 1
	if (dont_show) return;

	if (action != 0) return;
	if ((count != 25) && (count != 45)) return;
#endif

#ifdef SHARE_ACTIONS
	ShareDialog d(this);
	//d.showRemindCheck(false);
	d.exec();
	action = d.actions();
	qDebug("BaseGui::checkReminder: action: %d", action);

	if (!d.isRemindChecked()) {
		set->beginGroup("reminder");
		set->setValue("dont_show_anymore", true);
		set->endGroup();
	}
#else
	action = 0;
	bool accepted;
	showHelpDonateDialog(&accepted);
	if (accepted) action = 1;
#endif

	if (action > 0) {
		set->beginGroup("reminder");
		set->setValue("action", action);
		set->endGroup();
	}

	//qDebug() << "size:" << d.size();
}
#endif

#ifdef YOUTUBE_SUPPORT
void BaseGui::YTNoSslSupport() {
	qDebug("BaseGui::YTNoSslSupport");
	QMessageBox::warning(this, tr("Connection failed"),
		tr("The video you requested needs to open a HTTPS connection.") +"<br>"+
		tr("Unfortunately the OpenSSL component, required for it, is not available in your system.") +"<br>"+
		tr("Please, visit %1 to know how to fix this problem.")
			.arg("<a href=\"" URL_OPENSSL_INFO "\">" + tr("this link") + "</a>") );
}

void BaseGui::YTNoSignature(const QString & title) {
	qDebug("BaseGui::YTNoSignature: %s", title.toUtf8().constData());

	QString t = title;

	QString info_text;
	if (title.isEmpty()) {
		info_text = tr("Unfortunately due to changes in the Youtube page, this video can't be played.");
	} else {
		t.replace(" - YouTube", "");
		info_text = tr("Unfortunately due to changes in the Youtube page, the video '%1' can't be played.").arg(t);
	}

	#ifdef YT_USE_YTSIG
	int ret = QMessageBox::question(this, tr("Problems with Youtube"),
				info_text + "<br><br>" +
				tr("Do you want to update the Youtube code? This may fix the problem."),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (ret == QMessageBox::Yes) {
		YTUpdateScript();
	}
	#else
	QMessageBox::warning(this, tr("Problems with Youtube"),
		info_text + "<br><br>" +
		tr("Maybe updating SMPlayer could fix the problem."));
	#endif
}

#ifdef YT_USE_YTSIG
void BaseGui::YTUpdateScript() {
	static CodeDownloader * downloader = 0;
	if (!downloader) downloader = new CodeDownloader(this);
	downloader->saveAs(Paths::configPath() + "/yt.js");
	downloader->show();
	downloader->download(QUrl(URL_YT_CODE));
}
#endif // YT_USE_YTSIG
#endif //YOUTUBE_SUPPORT

void BaseGui::gotForbidden() {
	qDebug("BaseGui::gotForbidden");

	if (!pref->report_mplayer_crashes) {
		qDebug("BaseGui::gotForbidden: not displaying error dialog");
		return;
	}

	static bool busy = false;

	if (busy) return;

	busy = true;
#ifdef YOUTUBE_SUPPORT
	if (core->mdat.filename.contains("youtube.com")) {
		YTNoSignature("");
	} else
#endif
	{
		QMessageBox::warning(this, tr("Error detected"), 
			tr("Unfortunately this video can't be played.") +"<br>"+
			tr("The server returned '%1'").arg("403: Forbidden"));
	}
	busy = false;
}

void BaseGui::dragEnterEvent( QDragEnterEvent *e ) {
	qDebug("BaseGui::dragEnterEvent");

	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}



void BaseGui::dropEvent( QDropEvent *e ) {
	qDebug("BaseGui::dropEvent");

	QStringList files;

	if (e->mimeData()->hasUrls()) {
		QList <QUrl> l = e->mimeData()->urls();
		QString s;
		for (int n=0; n < l.count(); n++) {
			if (l[n].isValid()) {
				qDebug() << "BaseGui::dropEvent: scheme:" << l[n].scheme();
				if (l[n].scheme() == "file") 
					s = l[n].toLocalFile();
				else
					s = l[n].toString();
				/*
				qDebug(" * '%s'", l[n].toString().toUtf8().data());
				qDebug(" * '%s'", l[n].toLocalFile().toUtf8().data());
				*/
				qDebug() << "BaseGui::dropEvent: file:" << s;
				files.append(s);
			}
		}
	}

	QStringList file_list;
	QStringList dir_list;
	QString sub_file;

#ifdef Q_OS_WIN
	if (files.count() > 0) {
		files = Helper::resolveSymlinks(files); // Check for Windows shortcuts
	}
#endif
	files.sort();

	Extensions ext;
	QRegExp ext_sub(ext.subtitles().forRegExp());
	ext_sub.setCaseSensitivity(Qt::CaseInsensitive);

	foreach (QString file, files) {
		QFileInfo fi(file);
		if (fi.isDir()) {
			// Folder
			dir_list << file;
		}
		else
		if (ext_sub.indexIn(fi.suffix()) > -1) {
			// Subtitle file
			if (sub_file.isEmpty()) sub_file = file;
		}
		else {
			// File (or something else)
			file_list << file;
		}
	}

	qDebug() << "BaseGui::dropEvent: list of dirs:" << dir_list;
	qDebug() << "BaseGui::dropEvent: list of files:" << file_list;
	qDebug() << "BaseGui::dropEvent: subtitle file:" << sub_file;

	/* If only one file is dropped and it's not a folder nor a subtitle,
       open it with openFile so that it remembers the position */
	if (dir_list.isEmpty() && sub_file.isEmpty() && file_list.count() == 1 && QFile::exists(file_list[0])) {
		openFile(file_list[0]);
		return;
	}

	if (!sub_file.isEmpty()) {
		core->loadSub(sub_file);
		return;
	}

	if (file_list.isEmpty() && dir_list.isEmpty()) {
		return;
	}

	if (dir_list.count() == 1 && file_list.isEmpty()) {
		openDirectory(dir_list[0]);
		return;
	}

	if (pref->auto_add_to_playlist) {
		if (!playlist->maybeSave()) return;
		playlist->clear();

		if (!dir_list.isEmpty()) {
			// Add directories to the playlist
			foreach(QString dir, dir_list) playlist->addDirectory(dir);
		}

		if (!file_list.isEmpty()) {
			// Add files to the playlist
			playlist->addFiles(file_list, Playlist::NoGetInfo);
		}

		// All files are in the playlist, let's start to play
		playlist->startPlay();
	} else {
		// It wasn't possible to add files to the list
		// Let's open the first directory or file
		if (!dir_list.isEmpty()) openDirectory(dir_list[0]); // Bug? This actually modifies the playlist...
		else
		if (!file_list.isEmpty()) open(file_list[0]);
	}
}

void BaseGui::showPopupMenu() {
	showPopupMenu(QCursor::pos());
}

void BaseGui::showPopupMenu( QPoint p ) {
	//qDebug("BaseGui::showPopupMenu: %d, %d", p.x(), p.y());
	popup->move( p );
	popup->show();
}

/*
void BaseGui::mouseReleaseEvent( QMouseEvent * e ) {
	qDebug("BaseGui::mouseReleaseEvent");

	if (e->button() == Qt::LeftButton) {
		e->accept();
		emit leftClicked();
	}
	else
	if (e->button() == Qt::MidButton) {
		e->accept();
		emit middleClicked();
	}
	//
	//else
	//if (e->button() == Qt::RightButton) {
	//	showPopupMenu( e->globalPos() );
    //}
	//
	else 
		e->ignore();
}

void BaseGui::mouseDoubleClickEvent( QMouseEvent * e ) {
	e->accept();
	emit doubleClicked();
}
*/

/*
void BaseGui::wheelEvent( QWheelEvent * e ) {
	qDebug("BaseGui::wheelEvent: delta: %d", e->delta());
	e->accept();

	if (e->orientation() == Qt::Vertical) {
	    if (e->delta() >= 0)
	        emit wheelUp();
	    else
	        emit wheelDown();
	} else {
		qDebug("BaseGui::wheelEvent: horizontal event received, doing nothing");
	}
}
*/

// Called when a video has started to play
void BaseGui::enterFullscreenOnPlay() {
	qDebug("BaseGui::enterFullscreenOnPlay: arg_start_in_fullscreen: %d, pref->start_in_fullscreen: %d", arg_start_in_fullscreen, pref->start_in_fullscreen);

	if (arg_start_in_fullscreen != 0) {
		if ( (arg_start_in_fullscreen == 1) || (pref->start_in_fullscreen) ) {
			if (!pref->fullscreen) toggleFullscreen(true);
		}
	}
}

// Called when the playlist has stopped
void BaseGui::exitFullscreenOnStop() {
    if (pref->fullscreen) {
		toggleFullscreen(false);
	}
}

void BaseGui::playlistHasFinished() {
	qDebug("BaseGui::playlistHasFinished");
	core->stop();

	exitFullscreenOnStop();

	qDebug("BaseGui::playlistHasFinished: arg_close_on_finish: %d, pref->close_on_finish: %d", arg_close_on_finish, pref->close_on_finish);

	if (arg_close_on_finish != 0) {
		if ((arg_close_on_finish == 1) || (pref->close_on_finish)) {
			#ifdef AUTO_SHUTDOWN_PC
			if (pref->auto_shutdown_pc) {
				ShutdownDialog d(this);
				if (d.exec() == QDialog::Accepted) {
					qDebug("BaseGui::playlistHasFinished: the PC will shut down");
					Shutdown::shutdown();
				} else {
					qDebug("BaseGui::playlistHasFinished: shutdown aborted");
				}
			}
			#endif
			exitAct->trigger();
		}
	}
}

void BaseGui::addToPlaylistCurrentFile() {
	qDebug("BaseGui::addToPlaylistCurrentFile");
	if (!core->mdat.filename.isEmpty()) {
		playlist->addItem(core->mdat.filename, "", 0);
		playlist->setModified(true);
		playlist->getMediaInfo(core->mdat);
	}
}

void BaseGui::displayState(Core::State state) {
	qDebug() << "BaseGui::displayState:" << core->stateToString();

	switch (state) {
		//case Core::Playing:	statusBar()->showMessage( tr("Playing %1").arg(core->mdat.filename), 2000); break;
		case Core::Playing:	statusBar()->showMessage(""); break;
		case Core::Paused:	statusBar()->showMessage( tr("Pause") ); break;
		case Core::Stopped:	statusBar()->showMessage( tr("Stop") , 2000); break;
		case Core::Buffering: /* statusBar()->showMessage(tr("Buffering...")); */ break;
	}

	if (state == Core::Stopped) setWindowCaption( "SMPlayer" );

#ifdef AVOID_SCREENSAVER
	/* Disable screensaver by event */
	just_stopped = false;
	
	if (state == Core::Stopped) {
		just_stopped = true;
		int time = 1000 * 60; // 1 minute
		QTimer::singleShot( time, this, SLOT(clear_just_stopped()) );
	}
#endif
}

void BaseGui::displayMessage(QString message, int time) {
	statusBar()->showMessage(message, time);
}

void BaseGui::displayMessage(QString message) {
	displayMessage(message, 2000);
}

void BaseGui::gotCurrentTime(double sec) {
	//qDebug() << "BaseGui::gotCurrentTime:" << sec;

	static int last_second = 0;

	if (qFloor(sec) == last_second) return; // Update only once per second
	last_second = qFloor(sec);

	QString time = Helper::formatTime( (int) sec ) + " / " +
                           Helper::formatTime( (int) core->mdat.duration );

	emit timeChanged(sec);
	emit timeChanged(time);
}

void BaseGui::changeSizeFactor(int factor) {
	// If fullscreen, don't resize!
	if (pref->fullscreen) return;

	if (isMaximized()) showNormal();

	if (!pref->use_mplayer_window) {
		pref->size_factor = factor;
		resizeMainWindow(core->mset.win_width, core->mset.win_height);
	}
}

void BaseGui::toggleDoubleSize() {
	if (pref->size_factor != 100) changeSizeFactor(100); else changeSizeFactor(200);
}

void BaseGui::resizeWindow(int w, int h) {
	qDebug("BaseGui::resizeWindow: %d, %d", w, h);

	// If fullscreen or maximized, don't resize!
	if (pref->fullscreen || isMaximized()) return;

	if ( (pref->resize_method==Preferences::Never) && (panel->isVisible()) ) {
		return;
	}

	if (!panel->isVisible()) {
		panel->show();

		// Enable compact mode
		//compactAct->setEnabled(true);
	}

	resizeMainWindow(w, h);
}

void BaseGui::resizeMainWindow(int w, int h) {
	if (pref->size_factor != 100) {
		w = w * pref->size_factor / 100;
		h = h * pref->size_factor / 100;
	}

	qDebug("BaseGui::resizeWindow: size to scale: %d, %d", w, h);

	QSize video_size(w,h);

	if (video_size == panel->size()) {
		qDebug("BaseGui::resizeWindow: the panel size is already the required size. Doing nothing.");
		return;
	}

	int diff_width = this->width() - panel->width();
	int diff_height = this->height() - panel->height();

	int new_width = w + diff_width;
	int new_height = h + diff_height;

#if USE_MINIMUMSIZE
	int minimum_width = minimumSizeHint().width();
	if (pref->gui_minimum_width != 0) minimum_width = pref->gui_minimum_width;
	if (new_width < minimum_width) {
		qDebug("BaseGui::resizeWindow: width is too small, setting width to %d", minimum_width);
		new_width = minimum_width;
	}
#endif

	qDebug("BaseGui::resizeWindow: new_width: %d new_height: %d", new_width, new_height);

#ifdef Q_OS_WIN
	QRect desktop_rect = QApplication::desktop()->availableGeometry(this);
	QSize desktop_size = QSize(desktop_rect.width(), desktop_rect.height());
	//desktop_size.setWidth(1000); desktop_size.setHeight(1000); // test
	if (new_width > desktop_size.width()) {
		//double aspect = (double) new_width / new_height;
		qDebug("BaseGui::resizeWindow: width (%d) is larger than desktop width (%d)", new_width, desktop_size.width());
		new_width = desktop_size.width();
		/*
		new_height = new_width / aspect;
		qDebug() << "BaseGui::resizeWindow: aspect:" << aspect;
		qDebug("BaseGui::resizeWindow: height: %d", new_height);
		*/
	}
	if (new_height > desktop_size.height()) {
		qDebug("BaseGui::resizeWindow: height (%d) is larger than desktop height (%d)", new_height, desktop_size.height());
		new_height = desktop_size.height();
	}
#endif

	resize(new_width, new_height);

	qDebug("BaseGui::resizeWindow: done: window size: %d, %d", this->width(), this->height());
	qDebug("BaseGui::resizeWindow: done: panel->size: %d, %d", 
           panel->size().width(),  
           panel->size().height() );
	qDebug("BaseGui::resizeWindow: done: mplayerwindow->size: %d, %d", 
           mplayerwindow->size().width(),
           mplayerwindow->size().height() );

	// Check if a part of the window is outside of the desktop
	// and center the window in that case
	if ((pref->center_window_if_outside) && (!core->mdat.novideo)) {
		QRect screen_rect = QApplication::desktop()->availableGeometry(this);
		QPoint right_bottom = QPoint(this->pos().x() + this->width(), this->pos().y() + this->height());
		qDebug("BaseGui::resizeWindow: right bottom point: %d, %d", right_bottom.x(), right_bottom.y());;
		if (!screen_rect.contains(right_bottom) || !screen_rect.contains(this->pos())) {
			qDebug("BaseGui::resizeWindow: the window is outside of the desktop, it will be moved");
			//move(screen_rect.x(), screen_rect.y());
			int x = screen_rect.x() + ((screen_rect.width() - width()) / 2);
			int y = screen_rect.y() + ((screen_rect.height() - height()) / 2);
			//qDebug() << "BaseGui::resizeWindow: screen_rect.x:" << screen_rect.x() << "screen_rect.y:" << screen_rect.y();
			//qDebug() << "BaseGui::resizeWindow: width:" << ((screen_rect.width() - width()) / 2);
			//qDebug() << "BaseGui::resizeWindow: height:" << ((screen_rect.height() - height()) / 2);
			if (x < screen_rect.x()) x = screen_rect.x();
			if (y < screen_rect.y()) y = screen_rect.y();
			//qDebug() << "BaseGui::resizeWindow: x:" << x << "y:" << y;
			move(x, y);
		}
	}

}

void BaseGui::hidePanel() {
	qDebug("BaseGui::hidePanel");

	if (panel->isVisible()) {
		if (isMaximized()) showNormal();

		// Exit from fullscreen mode 
		if (pref->fullscreen) { toggleFullscreen(false); update(); }

		// Exit from compact mode first
		if (pref->compact_mode) toggleCompactMode(false);

		//resizeWindow( size().width(), 0 );
		int width = size().width();
		if (width > pref->default_size.width()) width = pref->default_size.width();
		resize( width, size().height() - panel->size().height() );
		panel->hide();

		// Disable compact mode
		//compactAct->setEnabled(false);
	}
}

void BaseGui::centerWindow() {
	qDebug("BaseGui::centerWindow");
	if (pref->center_window && !pref->fullscreen && isVisible()) {
		QRect r = QApplication::desktop()->screenGeometry(this);
		// r.setX(500); r.setY(150); // Test
		qDebug() << "BaseGui::centerWindow: desktop rect:" << r;
		int x = r.x() + ((r.width() - width()) / 2);
		int y = r.y() + ((r.height() - height()) / 2);
		move(x, y);
	}
}

void BaseGui::displayGotoTime(int t) {
#ifdef SEEKBAR_RESOLUTION
	int jump_time = (int)core->mdat.duration * t / SEEKBAR_RESOLUTION;
#else
	int jump_time = (int)core->mdat.duration * t / 100;
#endif
	QString s = tr("Jump to %1").arg( Helper::formatTime(jump_time) );
	statusBar()->showMessage( s, 1000 );

	if (pref->fullscreen) {
		core->displayTextOnOSD( s );
	}
}

void BaseGui::goToPosOnDragging(int t) {
	if (pref->update_while_seeking) {
#if ENABLE_DELAYED_DRAGGING
		#ifdef SEEKBAR_RESOLUTION
		core->goToPosition(t);
		#else
		core->goToPos(t);
		#endif
#else
		if ( ( t % 4 ) == 0 ) {
			qDebug("BaseGui::goToPosOnDragging: %d", t);
			#ifdef SEEKBAR_RESOLUTION
			core->goToPosition(t);
			#else
			core->goToPos(t);
			#endif
		}
#endif
	}
}

void BaseGui::toggleCompactMode() {
	toggleCompactMode( !pref->compact_mode );
}

void BaseGui::toggleCompactMode(bool b) {
	qDebug("BaseGui::toggleCompactMode: %d", b);

	if (b) 
		aboutToEnterCompactMode();
	else
		aboutToExitCompactMode();

	pref->compact_mode = b;
	updateWidgets();
}

void BaseGui::aboutToEnterCompactMode() {
	menuBar()->hide();
	statusBar()->hide();
}

void BaseGui::aboutToExitCompactMode() {
	menuBar()->show();
	statusBar()->show();
}

void BaseGui::setStayOnTop(bool b) {
	qDebug("BaseGui::setStayOnTop: %d", b);

	if ( (b && (windowFlags() & Qt::WindowStaysOnTopHint)) ||
         (!b && (!(windowFlags() & Qt::WindowStaysOnTopHint))) )
	{
		// identical do nothing
		qDebug("BaseGui::setStayOnTop: nothing to do");
		return;
	}

	ignore_show_hide_events = true;

	bool visible = isVisible();

	QPoint old_pos = pos();

	if (b) {
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
	}
	else {
		setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
	}

	move(old_pos);

	if (visible) {
		show();
	}

	ignore_show_hide_events = false;
}

void BaseGui::changeStayOnTop(int stay_on_top) {
	switch (stay_on_top) {
		case Preferences::AlwaysOnTop : setStayOnTop(true); break;
		case Preferences::NeverOnTop  : setStayOnTop(false); break;
		case Preferences::WhilePlayingOnTop : setStayOnTop((core->state() == Core::Playing)); break;
	}

	pref->stay_on_top = (Preferences::OnTop) stay_on_top;
	updateWidgets();
}

void BaseGui::checkStayOnTop(Core::State state) {
	qDebug("BaseGui::checkStayOnTop");
	if ((!pref->fullscreen) && (pref->stay_on_top == Preferences::WhilePlayingOnTop)) {
		if (state != Core::Buffering) {
			setStayOnTop((state == Core::Playing));
		}
	}
}

void BaseGui::toggleStayOnTop() {
	if (pref->stay_on_top == Preferences::AlwaysOnTop) 
		changeStayOnTop(Preferences::NeverOnTop);
	else
	if (pref->stay_on_top == Preferences::NeverOnTop) 
		changeStayOnTop(Preferences::AlwaysOnTop);
}

// Called when a new window (equalizer, preferences..) is opened.
void BaseGui::exitFullscreenIfNeeded() {
	/*
	if (pref->fullscreen) {
		toggleFullscreen(false);
	}
	*/
}

#if ALLOW_CHANGE_STYLESHEET
QString BaseGui::loadQss(QString filename) {
	qDebug("BaseGui::loadQss: %s", filename.toUtf8().constData());

	QFile file( filename );
	file.open(QFile::ReadOnly);
	QString stylesheet = QLatin1String(file.readAll());

	QString path;
#ifdef USE_RESOURCES
	Images::setTheme(pref->iconset);
	if (Images::has_rcc || Images::is_internal) {
		path = ":/" + pref->iconset;
	} else {
		QDir current = QDir::current();
		QString td = Images::themesDirectory();
		path = current.relativeFilePath(td);
	}
#else
	if (Images::is_internal) {
		path = ":/" + pref->iconset;
	} else {
		QDir current = QDir::current();
		QString td = Images::themesDirectory();
		path = current.relativeFilePath(td);
	}
#endif
	stylesheet.replace(QRegExp("url\\s*\\(\\s*([^\\);]+)\\s*\\)", Qt::CaseSensitive, QRegExp::RegExp2),
						QString("url(%1\\1)").arg(path + "/"));
	//qDebug("BaseGui::loadQss: styleSheet: %s", stylesheet.toUtf8().constData());
	return stylesheet;
}

void BaseGui::changeStyleSheet(QString style) {
	qDebug() << "BaseGui::changeStyleSheet:" << style;

	// Load default stylesheet
	QString stylesheet = loadQss(":/default-theme/style.qss");

	if (!style.isEmpty()) {
		// Check main.css
		QString qss_file = Paths::configPath() + "/themes/" + pref->iconset + "/main.css";
		if (!QFile::exists(qss_file)) {
			qss_file = Paths::themesPath() +"/"+ pref->iconset + "/main.css";
		}

		// Check style.qss
		if (!QFile::exists(qss_file)) {
			QString ifile = ":/" + pref->iconset + "/style.qss";
			if (QFile::exists(ifile)) {
				qss_file = ifile;
			}
		}

		if (!QFile::exists(qss_file)) {
			qss_file = Paths::configPath() + "/themes/" + pref->iconset + "/style.qss";
			if (!QFile::exists(qss_file)) {
				qss_file = Paths::themesPath() +"/"+ pref->iconset + "/style.qss";
			}
		}

		// Load style file
		if (QFile::exists(qss_file)) {
			qDebug() << "BaseGui::changeStyleSheet:" <<  qss_file;
			stylesheet += loadQss(qss_file);
		}
	}

	if (pref->tablet_mode) {
		QString tf = Images::file("tabletmode.css");
		qDebug() << "BaseGui::changeStyleSheet: tablet stylesheet file:" << tf;

		QFile file(tf);
		if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			stylesheet += file.readAll();
		}
	}

	// Use the user specified font
	if (!pref->default_font.isEmpty()) {
		QFont f;
		f.fromString( pref->default_font );
		QString fstyle = QString("QWidget { font: %1 %2 %3pt %4; }")
			.arg(f.bold() ? "bold" :"").arg(f.italic() ? "italic" : "")
			.arg(f.pointSize()).arg(f.family());
		stylesheet = fstyle + stylesheet;
	}
	//qDebug() << "BaseGui::changeStyleSheet: styleSheet:" << stylesheet;
	qApp->setStyleSheet(stylesheet);
}
#endif

void BaseGui::applyStyles() {
	qDebug("BaseGui::applyStyles");

#if !ALLOW_CHANGE_STYLESHEET
	if (!pref->default_font.isEmpty()) {
		QFont f;
		f.fromString( pref->default_font );
		if (QApplication::font() != f) {
			qDebug() << "BaseGui::applyStyles: setting new font:" << pref->default_font;
			QApplication::setFont(f);
		}
	}
#endif

#if ALLOW_CHANGE_STYLESHEET
	qDebug() << "BaseGui::applyStyles: stylesheet:" << pref->iconset;
	changeStyleSheet(pref->iconset);
#endif

#if STYLE_SWITCHING
	QString style = pref->style;
	if (style.isEmpty()) style = default_style;
	qDebug() << "BaseGui::applyStyles: style:" << style;
	if (!style.isEmpty()) {
		qApp->setStyle(style);
		#ifdef Q_OS_WIN
		qApp->setPalette(qApp->style()->standardPalette());
		#endif
	}
#endif

}

void BaseGui::setTabletMode(bool b) {
	qDebug() << "BaseGui::setTabletMode:" << b;
	pref->tablet_mode = b;
	populateMainMenu();
	applyStyles();

	emit tabletModeChanged(b);
}


void BaseGui::loadActions() {
	qDebug("BaseGui::loadActions");
	ActionsEditor::loadFromConfig(this, settings);
#if !DOCK_PLAYLIST
	ActionsEditor::loadFromConfig(playlist, settings);
#endif

	actions_list = ActionsEditor::actionsNames(this);
#if !DOCK_PLAYLIST
	actions_list += ActionsEditor::actionsNames(playlist);
#endif
}

void BaseGui::saveActions() {
	qDebug("BaseGui::saveActions");

	ActionsEditor::saveToConfig(this, settings);
#if !DOCK_PLAYLIST
	ActionsEditor::saveToConfig(playlist, settings);
#endif
}

void BaseGui::processMouseMovedDiff(QPoint diff) {
	//qDebug() << "BaseGui::processMouseMovedDiff" << diff;

#ifdef MOUSE_GESTURES
	#ifdef MG_DELAYED_SEEK
	if (delayed_seek_timer == 0) {
		delayed_seek_timer = new QTimer(this);
		delayed_seek_timer->setSingleShot(true);
		delayed_seek_timer->setInterval(250);
		connect(delayed_seek_timer, SIGNAL(timeout()), this, SLOT(delayedSeek()));
	}
	#endif

	if (pref->drag_function == Preferences::Gestures || pref->tablet_mode) {
		if (core->state() == Core::Stopped) return;

		int t = 1;

		int h_desp = qAbs(diff.x());
		int v_desp = qAbs(diff.y());
		
		int d = qAbs(h_desp - v_desp);
		
		//qDebug() << "BaseGui::processMouseMovedDiff: h_desp:" << h_desp << "v_desp:" << v_desp << "d:" << d;
		if (d < 2) return;

		if (h_desp > v_desp) {
			// Horizontal
			if (diff.x() > t) {
				#ifdef MG_DELAYED_SEEK
				delayed_seek_value += h_desp;
				delayed_seek_timer->start();
				#else
				core->sforward();
				#endif
			}
			else
			if (diff.x() < -t) {
				#ifdef MG_DELAYED_SEEK
				delayed_seek_value -= h_desp;
				delayed_seek_timer->start();
				#else
				core->srewind();
				#endif
			}
			#ifdef MG_DELAYED_SEEK
			/*
			int time = qAbs(delayed_seek_value);
			int minutes = time / 60;
			int seconds = time - (minutes * 60);
			QString s;
			if (delayed_seek_value >= 0) s = "+"; else s = "-";
			s += QString("%1").arg(minutes, 2, 10, QChar('0')) + ":";
			s += QString("%1").arg(seconds, 2, 10, QChar('0'));
			*/
			int time = core->mset.current_sec + delayed_seek_value;
			if (time < 0) time = 0;
			QString s;
			s = tr("Jump to %1").arg(Helper::formatTime(time));
			if (pref->fullscreen) {
				core->displayTextOnOSD(s, 1000);
			} else {
				displayMessage(s, 1000);
			}
			#endif
		} 
		else
		if (h_desp < v_desp) {
			// Vertical
			if (diff.y() > t) core->decVolume(1);
			else
			if (diff.y() < -t) core->incVolume(1);
		}
	}
#endif

	if (pref->drag_function == Preferences::MoveWindow && !pref->tablet_mode) {
		moveWindowDiff(diff);
	}
}

void BaseGui::moveWindowDiff(QPoint diff) {
	//qDebug() << "BaseGui::moveWindowDiff:" << diff;

	QWidget * w = this;
	if (mplayerwindow->parent() == 0) w = mplayerwindow;

	if (pref->fullscreen || isMaximized()) {
		return;
	}

#if QT_VERSION >= 0x050000
	// Move the window with some delay.
	// Seems to work better with Qt 5

	static QPoint d;
	static int count = 0;

	d += diff;
	count++;

	if (count > 3) {
		//qDebug() << "BaseGui::moveWindowDiff:" << d;
		QPoint new_pos = w->pos() + d;
		/*
		if (new_pos.y() < 0) new_pos.setY(0);
		if (new_pos.x() < 0) new_pos.setX(0);
		*/
		//qDebug() << "BaseGui::moveWindowDiff: new_pos:" << new_pos;
		w->move(new_pos);
		count = 0;
		d = QPoint(0,0);
	}
#else
	//qDebug() << "BaseGui::moveWindowDiff:" << diff;
	w->move(w->pos() + diff);
#endif
}

#ifdef MG_DELAYED_SEEK
void BaseGui::delayedSeek() {
	qDebug() << "BaseGui::delayedSeek:" << delayed_seek_value;
	if (delayed_seek_value != 0) {
		core->seek(delayed_seek_value);
		delayed_seek_value = 0;
	}
}
#endif


#ifndef DETECT_MINIMIZE_WORKAROUND
void BaseGui::showEvent( QShowEvent * ) {
	qDebug("BaseGui::showEvent");

	if (ignore_show_hide_events) return;

	//qDebug("BaseGui::showEvent: pref->pause_when_hidden: %d", pref->pause_when_hidden);
	if ((pref->pause_when_hidden) && (core->state() == Core::Paused)) {
		qDebug("BaseGui::showEvent: unpausing");
		core->pause(); // Unpauses
	}
}

void BaseGui::hideEvent( QHideEvent * ) {
	qDebug("BaseGui::hideEvent");

	if (ignore_show_hide_events) return;

	//qDebug("BaseGui::hideEvent: pref->pause_when_hidden: %d", pref->pause_when_hidden);
	if ((pref->pause_when_hidden) && (core->state() == Core::Playing)) {
		qDebug("BaseGui::hideEvent: pausing");
		core->pause();
	}
}
#else
// Qt 5 doesn't call showEvent / hideEvent when the window is minimized or unminimized
bool BaseGui::event(QEvent * e) {
	//qDebug("BaseGui::event: %d", e->type());

	bool result = QMainWindow::event(e);
	if ((ignore_show_hide_events) || (!pref->pause_when_hidden)) return result;

	if (e->type() == QEvent::WindowStateChange) {
		qDebug("BaseGui::event: WindowStateChange");

		if (isMinimized()) {
			was_minimized = true;
			if (core->state() == Core::Playing) {
				qDebug("BaseGui::event: pausing");
				core->pause();
			}
		}
	}

	if ((e->type() == QEvent::ActivationChange) && (isActiveWindow())) {
		qDebug("BaseGui::event: ActivationChange: %d", was_minimized);

		if ((!isMinimized()) && (was_minimized)) {
			was_minimized = false;
			if (core->state() == Core::Paused) {
				qDebug("BaseGui::showEvent: unpausing");
				core->pause(); // Unpauses
			}
		}
	}

	return result;
}
#endif

void BaseGui::askForMplayerVersion(QString line) {
	qDebug("BaseGui::askForMplayerVersion: %s", line.toUtf8().data());

	if (pref->mplayer_user_supplied_version <= 0) {
		InputMplayerVersion d(this);
		d.setVersion( pref->mplayer_user_supplied_version );
		d.setVersionFromOutput(line);
		if (d.exec() == QDialog::Accepted) {
			pref->mplayer_user_supplied_version = d.version();
			qDebug("BaseGui::askForMplayerVersion: user supplied version: %d", pref->mplayer_user_supplied_version);
		}
	} else {
		qDebug("BaseGui::askForMplayerVersion: already have a version supplied by user, so no asking");
	}
}

void BaseGui::showExitCodeFromMplayer(int exit_code) {
	qDebug("BaseGui::showExitCodeFromMplayer: %d", exit_code);

	if (!pref->report_mplayer_crashes) {
		qDebug("BaseGui::showExitCodeFromMplayer: not displaying error dialog");
		return;
	}

	if (exit_code != 255 ) {
		ErrorDialog d(this);
		d.setWindowTitle(tr("%1 Error").arg(PLAYER_NAME));
		QString text = tr("%1 has finished unexpectedly.").arg(PLAYER_NAME) + " " + 
					   tr("Exit code: %1").arg(exit_code);
		
		#if defined(Q_OS_WIN) && defined(LOG_MPLAYER)
		bool ytdl_fails = false;
		
		QString ytdl_bin = QFileInfo(pref->mplayer_bin).absolutePath() +"/youtube-dl.exe";
		qDebug() << "BaseGui::showExitCodeFromMplayer: youtube-dl path:" << ytdl_bin;
		
		#if 1
		// Newer versions of mpv display this message
		if (mplayer_log.contains("youtube-dl failed")) {
			int code = QProcess::execute(ytdl_bin, QStringList() << "--version");
			qDebug() << "BaseGui::showExitCodeFromMplayer: youtube-dl exit code:" << code;
			if (code == -1) ytdl_fails = true;
		}
		else
		#endif
		if (mplayer_log.contains("youtube-dl not found, not executable, or broken")) {
			bool exists_ytdl = QFile::exists(ytdl_bin);
			qDebug() << "BaseGui::showExitCodeFromMplayer: check if" << ytdl_bin << "exists:" << exists_ytdl;
			if (exists_ytdl) ytdl_fails = true;
		}
		
		if (ytdl_fails) {
			text += "<br><br>" + tr("The component youtube-dl failed to run.") +" "+
					tr("Installing the Microsoft Visual C++ 2010 Redistributable Package (x86) may fix the problem.") +
					"<br><a href=\"https://www.microsoft.com/en-US/download/details.aspx?id=5555\">" +
					tr("Click here to get it") + "</a>.";
		}
		#endif
		
		d.setText(text);
		
		#ifdef LOG_MPLAYER
		d.setLog( mplayer_log );
		#endif
		d.exec();
	} 
}

void BaseGui::showErrorFromMplayer(QProcess::ProcessError e) {
	qDebug("BaseGui::showErrorFromMplayer");

	if (!pref->report_mplayer_crashes) {
		qDebug("BaseGui::showErrorFromMplayer: not displaying error dialog");
		return;
	}

	if ((e == QProcess::FailedToStart) || (e == QProcess::Crashed)) {
		ErrorDialog d(this);
		d.setWindowTitle(tr("%1 Error").arg(PLAYER_NAME));
		if (e == QProcess::FailedToStart) {
			d.setText(tr("%1 failed to start.").arg(PLAYER_NAME) + " " + 
                         tr("Please check the %1 path in preferences.").arg(PLAYER_NAME));
		} else {
			d.setText(tr("%1 has crashed.").arg(PLAYER_NAME) + " " + 
                      tr("See the log for more info."));
		}
#ifdef LOG_MPLAYER
		d.setLog( mplayer_log );
#endif
		d.exec();
	}
}


#ifdef FIND_SUBTITLES
void BaseGui::showFindSubtitlesDialog() {
	qDebug("BaseGui::showFindSubtitlesDialog");

	if (!find_subs_dialog) {
		find_subs_dialog = new FindSubtitlesWindow(0);
		find_subs_dialog->setSettings(Global::settings);
		find_subs_dialog->setWindowIcon(windowIcon());
#if DOWNLOAD_SUBS
		connect(find_subs_dialog, SIGNAL(subtitleDownloaded(const QString &)),
                core, SLOT(loadSub(const QString &)));
#endif
	}

	find_subs_dialog->show();
	find_subs_dialog->setMovie(core->mdat.filename);
}

void BaseGui::openUploadSubtitlesPage() {	
	QDesktopServices::openUrl( QUrl("http://www.opensubtitles.org/upload") );
}
#endif

#ifdef VIDEOPREVIEW
void BaseGui::showVideoPreviewDialog() {
	qDebug("BaseGui::showVideoPreviewDialog");

	if (video_preview == 0) {
		video_preview = new VideoPreview( pref->mplayer_bin, this );
		video_preview->setSettings(Global::settings);
	}

	if (!core->mdat.filename.isEmpty()) {
		video_preview->setVideoFile(core->mdat.filename);

		// DVD
		if (core->mdat.type==TYPE_DVD) {
			QString file = core->mdat.filename;
			DiscData disc_data = DiscName::split(file);
			QString dvd_folder = disc_data.device;
			if (dvd_folder.isEmpty()) dvd_folder = pref->dvd_device;
			int dvd_title = disc_data.title;
			file = disc_data.protocol + "://" + QString::number(dvd_title);

			video_preview->setVideoFile(file);
			video_preview->setDVDDevice(dvd_folder);
		} else {
			video_preview->setDVDDevice("");
		}
	}

	video_preview->setMplayerPath(pref->mplayer_bin);

	if ( (video_preview->showConfigDialog(this)) && (video_preview->createThumbnails()) ) {
		video_preview->show();
		video_preview->adjustWindowSize();
	}
}
#endif

#ifdef YOUTUBE_SUPPORT
void BaseGui::showTubeBrowser() {
	qDebug("BaseGui::showTubeBrowser");
	#ifdef Q_OS_WIN
	QString exec = Paths::appPath() + "/smtube.exe";
	#else
	QString exec = Helper::findExecutable("smtube");
	#endif

	if (exec.isEmpty() || !QFile::exists(exec)) {
		QMessageBox::warning(this, "SMPlayer",
			tr("The YouTube Browser is not installed.") +"<br>"+ 
			tr("Visit %1 to get it.").arg("<a href=http://www.smtube.org>http://www.smtube.org</a>"));
		return;
	}

	QStringList args;
	if (!pref->language.isEmpty()) args << "-lang" << pref->language;
	qDebug() << "BaseGui::showTubeBrowser: exec:" << exec << "args:" << args;

	if (!QProcess::startDetached(exec, args)) {
		QMessageBox::warning(this, "SMPlayer",
			tr("The YouTube Browser failed to run.") +"<br>"+
			tr("Be sure it's installed correctly.") +"<br>"+
			tr("Visit %1 to get it.").arg("<a href=http://www.smtube.org>http://www.smtube.org</a>"));
	}
}
#endif

// Language change stuff
void BaseGui::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QMainWindow::changeEvent(e);
	}
}

#ifdef NUMPAD_WORKAROUND
// Due to a bug in Qt 5 on linux, accelerators in numeric keypad don't work
// This catches the key presses in the numeric keypad and calls the associated action
void BaseGui::keyPressEvent(QKeyEvent *event) {
	if (event->modifiers().testFlag(Qt::KeypadModifier)) {
		qDebug() << "BaseGui::keyPressEvent: key:" << event->key() << "modifiers:" << event->modifiers();

		QKeySequence ks(event->key());
		QList<QAction *> actions = this->actions();
		foreach(QAction * action, actions) {
			QList<QKeySequence> shortcuts = action->shortcuts();
			foreach(QKeySequence s, shortcuts) {
				bool match = (s == ks);
				if (match) {
					qDebug() << "BaseGui::keyPressEvent: action found:" << action->objectName() << "enabled:" << action->isEnabled();
				}
				if (match && action->isEnabled()) {
					if (action->isCheckable() && action->objectName() != "play_or_pause") {
						action->toggle();
					} else {
						action->trigger();
					}
					return;
				}
			}
		}
	}

	QMainWindow::keyPressEvent(event);
}
#endif

#ifdef Q_OS_WIN

#ifndef SM_CONVERTIBLESLATEMODE
#define SM_CONVERTIBLESLATEMODE 0x2003
#endif

#ifndef SM_SYSTEMDOCKED
#define SM_SYSTEMDOCKED 0x2004
#endif

bool BaseGui::winEvent ( MSG * m, long * result ) {
	//qDebug() << "BaseGui::winEvent:" << m;
	if (m && m->message == WM_SETTINGCHANGE && m->lParam) {
		QString text = QString::fromWCharArray((TCHAR*)m->lParam);
		qDebug() << "BaseGui::winEvent: WM_SETTINGCHANGE:" << text;

		#if ((QT_VERSION >= 0x040807 && QT_VERSION < 0x050000) || (QT_VERSION >= 0x050500))
		if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10) {
			// Windows 10 check
			if (text == "UserInteractionMode") {
				QTimer::singleShot(1000, this, SLOT(checkSystemTabletMode()));
			}
		}
		else 
		if (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS8_1) {
			if (text == "ConvertibleSlateMode") checkSystemTabletMode();
		}
		#endif
		
		*result = 0;
		return true;
	}
#ifdef AVOID_SCREENSAVER
	else
	if (m->message==WM_SYSCOMMAND) {
		if ((m->wParam & 0xFFF0)==SC_SCREENSAVE || (m->wParam & 0xFFF0)==SC_MONITORPOWER) {
			qDebug("BaseGui::winEvent: received SC_SCREENSAVE or SC_MONITORPOWER");
			qDebug("BaseGui::winEvent: avoid_screensaver: %d", pref->avoid_screensaver);
			qDebug("BaseGui::winEvent: playing: %d", core->state()==Core::Playing);
			qDebug("BaseGui::winEvent: video: %d", !core->mdat.novideo);
			
			if ((pref->avoid_screensaver) && (core->state()==Core::Playing) && (!core->mdat.novideo)) {
				qDebug("BaseGui::winEvent: not allowing screensaver");
				(*result) = 0;
				return true;
			} else {
				if ((pref->avoid_screensaver) && (just_stopped)) {
					qDebug("BaseGui::winEvent: file just stopped, so not allowing screensaver for a while");
					(*result) = 0;
					return true;
				} else {
					qDebug("BaseGui::winEvent: allowing screensaver");
					return false;
				}
			}
		}
	}
#endif
	return false;
}

#if QT_VERSION >= 0x050000
bool BaseGui::nativeEvent(const QByteArray &eventType, void * message, long * result) {
	//qDebug() << "BaseGui::nativeEvent:" << eventType;
	
	if (eventType == "windows_generic_MSG") {
		MSG * m = static_cast<MSG *>(message);
		return winEvent(m, result);
	}
	
	return false;
}
#endif

void BaseGui::checkSystemTabletMode() {
	#if ((QT_VERSION >= 0x040807 && QT_VERSION < 0x050000) || (QT_VERSION >= 0x050500))
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10) {
		// Windows 10 code (don't know if this works on Windows 8)
		QSettings set("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", QSettings::NativeFormat);
		QVariant v = set.value("TabletMode");
		if (v.isValid()) {
			bool system_tablet_mode = (v.toInt() == 1);
			qDebug() << "BaseGui::checkSystemTabletMode: system_tablet_mode:" << system_tablet_mode;
			systemTabletModeChanged(system_tablet_mode);
		}
	}
	else
	if (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS8_1 || 
        QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS8)
	{
		bool slate_mode = (GetSystemMetrics(SM_CONVERTIBLESLATEMODE) == 0);
		qDebug() << "BaseGui::checkSystemTabletMode: slate_mode:" << slate_mode;
		/*
		bool docked = (GetSystemMetrics(SM_SYSTEMDOCKED) != 0);
		qDebug() << "BaseGui::checkSystemTabletMode: docked:" << docked;
		*/
		bool system_tablet_mode = slate_mode;
		systemTabletModeChanged(system_tablet_mode);
	}
	#endif
}

void BaseGui::systemTabletModeChanged(bool system_tablet_mode) {
	qDebug() << "BaseGui::systemTabletModeChanged:" << system_tablet_mode;
	
	if (pref->tablet_mode != system_tablet_mode) {
		qDebug("BaseGui::systemTabletModeChanged: tablet mode should be changed");
		
		if (pref->tablet_mode_change_answer == "yes") {
			setTabletMode(system_tablet_mode);
		}
		else
		if (pref->tablet_mode_change_answer == "no") {
			return;
		}
		else {
			// Ask the user
			QString text;
			if (system_tablet_mode)
				text = tr("The system has switched to tablet mode. Should SMPlayer change to tablet mode as well?");
			else
				text = tr("The system has exited tablet mode. Should SMPlayer turn off the tablet mode as well?");
		
			QMessageBox mb(QMessageBox::Question, "SMPlayer", text, QMessageBox::Yes | QMessageBox::No);
			#if QT_VERSION >= 0x050200
			QCheckBox cb(tr("Remember my decision and don't ask again"));
			mb.setCheckBox(&cb);
			#endif
			if (mb.exec() == QMessageBox::Yes) {
				setTabletMode(system_tablet_mode);
			}
			#if QT_VERSION >= 0x050200
			if (cb.isChecked()) {
				pref->tablet_mode_change_answer = (mb.result() == QMessageBox::Yes ? "yes" : "no");
			}
			#endif
		}
		// Update action button
		tabletModeAct->setChecked(pref->tablet_mode);
	}
}

#ifdef AVOID_SCREENSAVER
void BaseGui::clear_just_stopped() {
	qDebug("BaseGui::clear_just_stopped");
	just_stopped = false;
}
#endif

#endif // Q_OS_WIN

#include "moc_basegui.cpp"
