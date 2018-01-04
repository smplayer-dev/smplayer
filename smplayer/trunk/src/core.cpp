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

#include "core.h"
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>
#include <QUrl>
#include <QNetworkProxy>

#ifdef Q_OS_OS2
#include <QEventLoop>
#endif

#include <cmath>

#include "mplayerwindow.h"
#include "desktopinfo.h"
#include "helper.h"
#include "paths.h"
#include "preferences.h"
#include "global.h"
#include "config.h"
#include "mplayerversion.h"
#include "constants.h"
#include "colorutils.h"
#include "discname.h"
#include "filters.h"
#include "extensions.h"

#ifdef Q_OS_WIN
#include <windows.h> // To change app priority
#include <QSysInfo> // To get Windows version
#endif
#ifdef SCREENSAVER_OFF
#include "screensaver.h"
#endif

#include "filesettings.h"
#include "filesettingshash.h"

#ifdef TV_SUPPORT
#include "tvlist.h"
#include "tvsettings.h"
#endif

#ifdef YOUTUBE_SUPPORT
#include "retrieveyoutubeurl.h"
  #ifdef YT_USE_YTSIG
  #include "ytsig.h"
  #endif

  #define PREF_YT_ENABLED pref->streaming_type == Preferences::StreamingYT || pref->streaming_type == Preferences::StreamingAuto
#endif

//#define SIMPLE_TRACK_SELECTION

using namespace Global;

Core::Core( MplayerWindow *mpw, QWidget* parent ) 
	: QObject( parent ) 
{
	qRegisterMetaType<Core::State>("Core::State");

	mplayerwindow = mpw;

	_state = Stopped;

	we_are_restarting = false;
	just_loaded_external_subs = false;
	just_unloaded_external_subs = false;
	change_volume_after_unpause = false;

#if DVDNAV_SUPPORT
	dvdnav_title_is_menu = true; // Enabled by default for compatibility with previous versions of mplayer
#endif

	// Create file_settings
	file_settings = 0;
	changeFileSettingsMethod(pref->file_settings_method);

	// TV settings
#ifdef TV_SUPPORT
	tv_settings = new TVSettings(Paths::iniPath());
#endif

	proc = PlayerProcess::createPlayerProcess(pref->mplayer_bin);

	// Do this the first
	connect( proc, SIGNAL(processExited()),
             mplayerwindow->videoLayer(), SLOT(playingStopped()) );

	connect( proc, SIGNAL(error(QProcess::ProcessError)),
             mplayerwindow->videoLayer(), SLOT(playingStopped()) );

	// Necessary to hide/unhide mouse cursor on black borders
	connect( proc, SIGNAL(processExited()),
             mplayerwindow, SLOT(playingStopped()) );

	connect( proc, SIGNAL(error(QProcess::ProcessError)),
             mplayerwindow, SLOT(playingStopped()) );


	connect( proc, SIGNAL(receivedCurrentSec(double)),
             this, SLOT(changeCurrentSec(double)) );

	connect( proc, SIGNAL(receivedCurrentFrame(int)),
             this, SIGNAL(showFrame(int)) );

	connect( proc, SIGNAL(receivedPause()),
			 this, SLOT(changePause()) );

    connect( proc, SIGNAL(processExited()),
	         this, SLOT(processFinished()), Qt::QueuedConnection );

	connect( proc, SIGNAL(mplayerFullyLoaded()),
			 this, SLOT(finishRestart()), Qt::QueuedConnection );

	connect( proc, SIGNAL(lineAvailable(QString)),
             this, SIGNAL(logLineAvailable(QString)) );

	connect( proc, SIGNAL(receivedCacheMessage(QString)),
			 this, SLOT(displayMessage(QString)) );

	/*
	connect( proc, SIGNAL(receivedCacheMessage(QString)),
			 this, SIGNAL(buffering()));
	*/

	connect( proc, SIGNAL(receivedBuffering()),
			 this, SIGNAL(buffering()));

	connect( proc, SIGNAL(receivedPlaying()),
			 this, SLOT(displayPlaying()));

	connect( proc, SIGNAL(receivedCacheEmptyMessage(QString)),
			 this, SIGNAL(buffering()));

	connect( proc, SIGNAL(receivedCreatingIndex(QString)),
			 this, SLOT(displayMessage(QString)) );

	connect( proc, SIGNAL(receivedCreatingIndex(QString)),
			 this, SIGNAL(buffering()));

	connect( proc, SIGNAL(receivedConnectingToMessage(QString)),
			 this, SLOT(displayMessage(QString)) );

	connect( proc, SIGNAL(receivedConnectingToMessage(QString)),
			 this, SIGNAL(buffering()));

	connect( proc, SIGNAL(receivedResolvingMessage(QString)),
			 this, SLOT(displayMessage(QString)) );

	connect( proc, SIGNAL(receivedResolvingMessage(QString)),
			 this, SIGNAL(buffering()));

	connect( proc, SIGNAL(receivedScreenshot(QString)),
             this, SLOT(displayScreenshotName(QString)) );

	connect( proc, SIGNAL(receivedUpdatingFontCache()),
             this, SLOT(displayUpdatingFontCache()) );

	connect( proc, SIGNAL(receivedScanningFont(QString)),
			 this, SLOT(displayMessage(QString)) );

	connect( proc, SIGNAL(receivedWindowResolution(int,int)),
             this, SLOT(gotWindowResolution(int,int)) );

	connect( proc, SIGNAL(receivedNoVideo()),
             this, SLOT(gotNoVideo()) );

	connect( proc, SIGNAL(receivedVO(QString)),
             this, SLOT(gotVO(QString)) );

	connect( proc, SIGNAL(receivedAO(QString)),
             this, SLOT(gotAO(QString)) );

	connect( proc, SIGNAL(receivedEndOfFile()),
             this, SLOT(fileReachedEnd()), Qt::QueuedConnection );

	connect( proc, SIGNAL(receivedStartingTime(double)),
             this, SLOT(gotStartingTime(double)) );

	connect( proc, SIGNAL(receivedVideoBitrate(int)), this, SLOT(gotVideoBitrate(int)) );
	connect( proc, SIGNAL(receivedAudioBitrate(int)), this, SLOT(gotAudioBitrate(int)) );

	connect( proc, SIGNAL(receivedStreamTitle(QString)),
             this, SLOT(streamTitleChanged(QString)) );

	connect( proc, SIGNAL(receivedStreamTitleAndUrl(QString,QString)),
             this, SLOT(streamTitleAndUrlChanged(QString,QString)) );

	connect( proc, SIGNAL(failedToParseMplayerVersion(QString)),
             this, SIGNAL(failedToParseMplayerVersion(QString)) );

	connect( this, SIGNAL(mediaLoaded()), this, SLOT(checkIfVideoIsHD()), Qt::QueuedConnection );
#if DELAYED_AUDIO_SETUP_ON_STARTUP
	connect( this, SIGNAL(mediaLoaded()), this, SLOT(initAudioTrack()), Qt::QueuedConnection );
#endif
#if NOTIFY_SUB_CHANGES
	connect( proc, SIGNAL(subtitleInfoChanged(const SubTracks &)), 
             this, SLOT(initSubtitleTrack(const SubTracks &)), Qt::QueuedConnection );
	connect( proc, SIGNAL(subtitleInfoReceivedAgain(const SubTracks &)), 
             this, SLOT(setSubtitleTrackAgain(const SubTracks &)), Qt::QueuedConnection );
#endif
#if NOTIFY_AUDIO_CHANGES
	connect( proc, SIGNAL(audioInfoChanged(const Tracks &)), 
             this, SLOT(initAudioTrack(const Tracks &)), Qt::QueuedConnection );
#endif
#if NOTIFY_VIDEO_CHANGES
	connect( proc, SIGNAL(videoInfoChanged(const Tracks &)),
             this, SLOT(initVideoTrack(const Tracks &)), Qt::QueuedConnection );
#endif
#if NOTIFY_CHAPTER_CHANGES
	connect( proc, SIGNAL(chaptersChanged(const Chapters &)), 
             this, SLOT(updateChapterInfo(const Chapters &)), Qt::QueuedConnection );
#endif

#if DVDNAV_SUPPORT
	connect( proc, SIGNAL(receivedDVDTitle(int)), 
             this, SLOT(dvdTitleChanged(int)), Qt::QueuedConnection );
	connect( proc, SIGNAL(receivedDuration(double)), 
             this, SLOT(durationChanged(double)), Qt::QueuedConnection );

	QTimer * ask_timer = new QTimer(this);
	connect( ask_timer, SIGNAL(timeout()), this, SLOT(askForInfo()) );
	ask_timer->start(5000);

	connect( proc, SIGNAL(receivedTitleIsMenu()),
             this, SLOT(dvdTitleIsMenu()) );
	connect( proc, SIGNAL(receivedTitleIsMovie()),
             this, SLOT(dvdTitleIsMovie()) );
#endif

	connect( proc, SIGNAL(receivedForbiddenText()), this, SIGNAL(receivedForbidden()) );

	connect( this, SIGNAL(stateChanged(Core::State)), 
	         this, SLOT(watchState(Core::State)) );

	connect( this, SIGNAL(mediaInfoChanged()), this, SLOT(sendMediaInfo()) );

	connect( proc, SIGNAL(error(QProcess::ProcessError)), 
             this, SIGNAL(mplayerFailed(QProcess::ProcessError)) );

	//pref->load();
	mset.reset();

	// Mplayerwindow
	connect( this, SIGNAL(aboutToStartPlaying()),
             mplayerwindow->videoLayer(), SLOT(playingStarted()) );

	// Necessary to hide/unhide mouse cursor on black borders
	connect( this, SIGNAL(aboutToStartPlaying()),
             mplayerwindow, SLOT(playingStarted()) );

#if DVDNAV_SUPPORT
	connect( mplayerwindow->videoLayer(), SIGNAL(mouseMoved(QPoint)),
             this, SLOT(dvdnavUpdateMousePos(QPoint)) );
#endif

#if REPAINT_BACKGROUND_OPTION
	mplayerwindow->videoLayer()->setRepaintBackground(pref->repaint_video_background);
#endif
	mplayerwindow->setMonitorAspect( pref->monitor_aspect_double() );

#ifdef SCREENSAVER_OFF
	// Windows or OS2 screensaver
	win_screensaver = new WinScreenSaver();
	connect( this, SIGNAL(aboutToStartPlaying()), this, SLOT(disableScreensaver()) );
	connect( proc, SIGNAL(processExited()), this, SLOT(enableScreensaver()) );
	connect( proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(enableScreensaver()) );
#endif

#ifdef YOUTUBE_SUPPORT
	yt = new RetrieveYoutubeUrl(this);
	yt->setUseHttpsMain(pref->yt_use_https_main);
	yt->setUseHttpsVi(pref->yt_use_https_vi);

	#ifdef YT_USE_SIG
	QSettings * sigset = new QSettings(Paths::configPath() + "/sig.ini", QSettings::IniFormat, this);
	yt->setSettings(sigset);
	#endif

	connect(yt, SIGNAL(gotPreferredUrl(const QString &, int)), this, SLOT(openYT(const QString &)));
	connect(yt, SIGNAL(connecting(QString)), this, SLOT(connectingToYT(QString)));
	connect(yt, SIGNAL(errorOcurred(int,QString)), this, SLOT(YTFailed(int,QString)));
	connect(yt, SIGNAL(noSslSupport()), this, SIGNAL(noSslSupport()));
	connect(yt, SIGNAL(signatureNotFound(const QString&)), this, SIGNAL(signatureNotFound(const QString&)));
	connect(yt, SIGNAL(gotEmptyList()), this, SLOT(YTNoVideoUrl()));
#endif

	connect(this, SIGNAL(buffering()), this, SLOT(displayBuffering()));
}


Core::~Core() {
	saveMediaInfo();

	if (proc->isRunning()) stopMplayer();
	proc->terminate();
	delete proc;

	delete file_settings;
#ifdef TV_SUPPORT
	delete tv_settings;
#endif

#ifdef SCREENSAVER_OFF
	delete win_screensaver;
#endif

#ifdef YOUTUBE_SUPPORT
	delete yt;
#endif
}

void Core::changeFileSettingsMethod(QString method) {
	qDebug("Core::changeFileSettingsMethod: %s", method.toUtf8().constData());
	if (file_settings) delete file_settings;

	if (method.toLower() == "hash")
		file_settings = new FileSettingsHash(Paths::iniPath());
	else
		file_settings = new FileSettings(Paths::iniPath());
}

void Core::setState(State s) {
	//qDebug() << "Core::setState: old state:" << _state << "new state:" << s;
	if (s != _state) {
		_state = s;
		emit stateChanged(_state);
	}
}

QString Core::stateToString() {
	if (state()==Playing) return "Playing";
	else
	if (state()==Stopped) return "Stopped";
	else
	if (state()==Paused) return "Paused";
	else
	return "Unknown";
}

// Public restart
void Core::restart() {
	qDebug("Core::restart");
	if (proc->isRunning()) {
		restartPlay();
	} else {
		qDebug("Core::restart: mplayer is not running");
	}
}

void Core::reload() {
	qDebug("Core::reload");

	stopMplayer();
	we_are_restarting = false;

	initPlaying();
}

void Core::saveMediaInfo() {
	qDebug("Core::saveMediaInfo");

	if (!pref->remember_media_settings) {
		qDebug("Core::saveMediaInfo: saving settings for files is disabled");
		return;
	}

	if (mdat.type == TYPE_STREAM && !pref->remember_stream_settings) {
		qDebug("Core::saveMediaInfo: saving settings for streams is disabled");
		return;
	}

	if ( (mdat.type == TYPE_FILE || mdat.type == TYPE_STREAM) && (!mdat.filename.isEmpty()) ) {
		file_settings->saveSettingsFor(mdat.filename, mdat.type, mset, proc->player());
	}
#ifdef TV_SUPPORT
	else
	if ( (mdat.type == TYPE_TV) && (!mdat.filename.isEmpty()) ) {
		tv_settings->saveSettingsFor(mdat.filename, mdat.type, mset, proc->player());
	}
#endif
}

void Core::restoreSettingsForMedia(const QString & name, int type) {
	qDebug() << "Core::restoreSettingsForMedia:" << name << "type:" << type;

	if (!pref->remember_media_settings) {
		qDebug("Core::restoreSettingsForMedia: remember settings for files is disabled");
		return;
	}

	if (type == TYPE_STREAM && !pref->remember_stream_settings) {
		qDebug("Core::restoreSettingsForMedia: remember settings for streams is disabled");
		return;
	}

	file_settings->loadSettingsFor(name, type, mset, proc->player());
	qDebug("Core::restoreSettingsForMedia: media settings read");

	// Resize the window and set the aspect as soon as possible
	int saved_width = mset.win_width;
	int saved_height = mset.win_height;
	// 400x300 is the default size for win_width and win_height
	// so we set them to 0 to avoid to resize the window on
	// audio files
	if ((saved_width == 400) && (saved_height == 300)) {
		saved_width = 0;
		saved_height = 0;
	}
	if ((saved_width > 0) && (saved_height > 0)) {
		emit needResize(mset.win_width, mset.win_height);
		changeAspectRatio(mset.aspect_ratio_id);
	}

	if (!pref->remember_time_pos) {
		mset.current_sec = 0;
		qDebug("Core::restoreSettingsForMedia: time pos reset to 0");
	}
}

void Core::initializeMenus() {
	qDebug("Core::initializeMenus");

	emit menusNeedInitialize();
}


void Core::updateWidgets() {
	qDebug("Core::updateWidgets");

	emit widgetsNeedUpdate();
}


void Core::changeFullscreenMode(bool b) {
	proc->setFullscreen(b);
}

void Core::displayTextOnOSD(QString text, int duration, int level, QString prefix) {
	qDebug("Core::displayTextOnOSD: '%s'", text.toUtf8().constData());

	if (proc->isRunning()) {
		proc->setPausingPrefix(prefix);
		proc->showOSDText(text, duration, level);
	}
}

// Generic open, autodetect type
void Core::open(QString file, int seek) {
	qDebug("Core::open: '%s'", file.toUtf8().data());

	if (file.startsWith("file:")) {
		file = QUrl(file).toLocalFile();
		qDebug("Core::open: converting url to local file: %s", file.toUtf8().constData());
	}

	QFileInfo fi(file);

	if ( (fi.exists()) && (fi.suffix().toLower()=="iso") ) {
		qDebug("Core::open: * identified as a dvd iso");
#if DVDNAV_SUPPORT
		openDVD( DiscName::joinDVD(0, file, pref->use_dvdnav) );
#else
		openDVD( DiscName::joinDVD(firstDVDTitle(), file, false) );
#endif
	}
	else
	if ( (fi.exists()) && (!fi.isDir()) ) {
		qDebug("Core::open: * identified as local file");
		// Local file
		file = QFileInfo(file).absoluteFilePath();
		openFile(file, seek);
	} 
	else
	if ( (fi.exists()) && (fi.isDir()) ) {
		// Directory
		qDebug("Core::open: * identified as a directory");
		qDebug("Core::open:   checking if contains a dvd");
		file = QFileInfo(file).absoluteFilePath();
		if (Helper::directoryContainsDVD(file)) {
			qDebug("Core::open: * directory contains a dvd");
#if DVDNAV_SUPPORT
			openDVD( DiscName::joinDVD(firstDVDTitle(), file, pref->use_dvdnav) );
#else
			openDVD( DiscName::joinDVD(firstDVDTitle(), file, false) );
#endif
		} else {
			qDebug("Core::open: * directory doesn't contain a dvd");
			qDebug("Core::open:   opening nothing");
		}
	}
	else 
	if ((file.toLower().startsWith("dvd:")) || (file.toLower().startsWith("dvdnav:"))) {
		qDebug("Core::open: * identified as dvd");
		openDVD(file);
		/*
		QString f = file.lower();
		QRegExp s("^dvd://(\\d+)");
		if (s.indexIn(f) != -1) {
			int title = s.cap(1).toInt();
			openDVD(title);
		} else {
			qWarning("Core::open: couldn't parse dvd title, playing first one");
			openDVD();
		}
		*/
	}
	else
#ifdef BLURAY_SUPPORT
	if (file.toLower().startsWith("br:")) {
		qDebug("Core::open: * identified as blu-ray");
		openBluRay(file);
	}
	else
#endif
	if (file.toLower().startsWith("vcd:")) {
		qDebug("Core::open: * identified as vcd");

		QString f = file.toLower();
		QRegExp s("^vcd://(\\d+)");
		if (s.indexIn(f) != -1) {
			int title = s.cap(1).toInt();
			openVCD(title);
		} else {
			qWarning("Core::open: couldn't parse vcd title, playing first one");
			openVCD();
		}
	}
	else
	if (file.toLower().startsWith("cdda:")) {
		qDebug("Core::open: * identified as cdda");

		QString f = file.toLower();
		QRegExp s("^cdda://(\\d+)");
		if (s.indexIn(f) != -1) {
			int title = s.cap(1).toInt();
			openAudioCD(title);
		} else {
			qWarning("Core::open: couldn't parse cdda title, playing first one");
			openAudioCD();
		}
	}
#ifdef TV_SUPPORT
	else
	if ((file.toLower().startsWith("dvb:")) || (file.toLower().startsWith("tv:"))) {
		qDebug("Core::open: * identified as TV");
		openTV(file);
	}
#endif
	else {
		qDebug("Core::open: * not identified, playing as stream");
		openStream(file);
	}
}

void Core::openFile(QString filename, int seek) {
	qDebug("Core::openFile: '%s'", filename.toUtf8().data());

	QFileInfo fi(filename);
	if (fi.exists()) {
		playNewFile(fi.absoluteFilePath(), seek);
	} else {
		//File doesn't exists
		//TODO: error message
	}
}

#ifdef YOUTUBE_SUPPORT
void Core::openYT(const QString & url) {
	qDebug("Core::openYT: %s", url.toUtf8().constData());
	openStream(url);
	yt->close();
}

void Core::connectingToYT(QString host) {
	emit showMessage( tr("Connecting to %1").arg(host) );
}

void Core::YTFailed(int /*error_number*/, QString /*error_str*/) {
	emit showMessage( tr("Unable to retrieve the Youtube page") );
}

void Core::YTNoVideoUrl() {
	emit showMessage( tr("Unable to locate the URL of the video") );
}
#endif

#ifdef SCREENSAVER_OFF
void Core::enableScreensaver() {
	qDebug("Core::enableScreensaver");
	if (pref->turn_screensaver_off) {
		win_screensaver->enable();
	}
}

void Core::disableScreensaver() {
	qDebug("Core::disableScreensaver");
	if (pref->turn_screensaver_off) {
		win_screensaver->disable();
	}
}
#endif

void Core::loadSub(const QString & sub ) {
    if ( (!sub.isEmpty()) && (QFile::exists(sub)) ) {
#if NOTIFY_SUB_CHANGES
		mset.external_subtitles = sub;
		just_loaded_external_subs = true;

		QFileInfo fi(sub);
		bool is_idx = (fi.suffix().toLower() == "idx");
		if (proc->isMPV()) is_idx = false; // Hack to ignore the idx extension with mpv

		if ((pref->fast_load_sub) && (!is_idx) && (mset.external_subtitles_fps == MediaSettings::SFPS_None)) {
			QString sub_file = sub;
			#ifdef Q_OS_WIN
			if (pref->use_short_pathnames) {
				sub_file = Helper::shortPathName(sub);
				// For some reason it seems it's necessary to change the path separator to unix style
				// otherwise mplayer fails to load it
				sub_file = sub_file.replace("\\","/");
			}
			#endif
			proc->setExternalSubtitleFile(sub_file);
		} else {
			restartPlay();
		}
#else
		mset.external_subtitles = sub;
		just_loaded_external_subs = true;
		restartPlay();
#endif
	} else {
		qWarning("Core::loadSub: file '%s' is not valid", sub.toUtf8().constData());
	}
}

void Core::unloadSub() {
	if ( !mset.external_subtitles.isEmpty() ) {
		mset.external_subtitles = "";
		just_unloaded_external_subs = true;
		restartPlay();
	}
}

void Core::loadAudioFile(const QString & audiofile) {
	if (!audiofile.isEmpty()) {
		mset.external_audio = audiofile;
		restartPlay();
	}
}

void Core::unloadAudioFile() {
	if (!mset.external_audio.isEmpty()) {
		mset.external_audio = "";
		restartPlay();
	}
}

/*
void Core::openDVD( bool from_folder, QString directory) {
	qDebug("Core::openDVD");

	if (from_folder) {
		if (!directory.isEmpty()) {
			QFileInfo fi(directory);
			if ( (fi.exists()) && (fi.isDir()) ) {
				pref->dvd_directory = directory;
				pref->play_dvd_from_hd = true;
				openDVD();
			} else {
				qDebug("Core::openDVD: directory '%s' is not valid", directory.toUtf8().data());
			}
		} else {
			qDebug("Core::openDVD: directory is empty");
		}
	} else {
		pref->play_dvd_from_hd = false;
		openDVD();
	}
}

void Core::openDVD() {
	openDVD(1);
}

void Core::openDVD(int title) {
	qDebug("Core::openDVD: %d", title);

	if (proc->isRunning()) {
		stopMplayer();
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = "dvd://" + QString::number(title);
	mdat.type = TYPE_DVD;

	mset.reset();

	mset.current_title_id = title;
	mset.current_angle_id = 1;

	initializeMenus();

	initPlaying();
}
*/

void Core::openVCD(int title) {
	qDebug("Core::openVCD: %d", title);

	if (title == -1) title = pref->vcd_initial_title;

	if (proc->isRunning()) {
		stopMplayer();
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = "vcd://" + QString::number(title);
	mdat.type = TYPE_VCD;

	mset.reset();

	mset.current_title_id = title;
	mset.current_angle_id = -1;

	/* initializeMenus(); */

	initPlaying();
}

void Core::openAudioCD(int title) {
	qDebug("Core::openAudioCD: %d", title);

	if (title == -1) title = 1;

	if (proc->isRunning()) {
		stopMplayer();
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = "cdda://" + QString::number(title);
	mdat.type = TYPE_AUDIO_CD;

	mset.reset();

	mset.current_title_id = title;
	mset.current_angle_id = -1;

	/* initializeMenus(); */

	initPlaying();
}

void Core::openDVD(QString dvd_url) {
	qDebug("Core::openDVD: '%s'", dvd_url.toUtf8().data());

	//Checks
	DiscData disc_data = DiscName::split(dvd_url);
	QString folder = disc_data.device;
	int title = disc_data.title;

	if (title == -1) {
		qWarning("Core::openDVD: title invalid, not playing dvd");
		return;
	}

	if (folder.isEmpty()) {
		qDebug("Core::openDVD: not folder");
	} else {
		QFileInfo fi(folder);
		if ( (!fi.exists()) /*|| (!fi.isDir())*/ ) {
			qWarning("Core::openDVD: folder invalid, not playing dvd");
			return;
		}
	}

	if (proc->isRunning()) {
		stopMplayer();
		we_are_restarting = false;
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = dvd_url;
	mdat.type = TYPE_DVD;

	mset.reset();

	mset.current_title_id = title;
	mset.current_angle_id = 1;

	/* initializeMenus(); */

	initPlaying();
}


#ifdef BLURAY_SUPPORT
/**
 * Opens a BluRay, taking advantage of mplayer's capabilities to do so.
 */
void Core::openBluRay(QString bluray_url) {
	qDebug("Core::openBluRay: '%s'", bluray_url.toUtf8().data());

	//Checks
	DiscData disc_data = DiscName::split(bluray_url);
	QString folder = disc_data.device;
	int title = disc_data.title;

	if (title == -1) {
		qWarning("Core::openBluRay: title invalid, not playing bluray");
		return;
	}

	QFileInfo fi(folder);
	if ( (!fi.exists()) || (!fi.isDir()) ) {
		qWarning("Core::openBluRay: folder invalid, not playing bluray");
		return;
	}

	if (proc->isRunning()) {
		stopMplayer();
		we_are_restarting = false;
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = bluray_url;
	mdat.type = TYPE_BLURAY;

	mset.reset();

	mset.current_title_id = title;
	mset.current_angle_id = 1;

	/* initializeMenus(); */

	initPlaying();
}
#endif

#ifdef TV_SUPPORT
void Core::openTV(QString channel_id) {
	qDebug("Core::openTV: '%s'", channel_id.toUtf8().constData());

	if (proc->isRunning()) {
		stopMplayer();
		we_are_restarting = false;
	}

	// Save data of previous file:
	saveMediaInfo();

	// Use last channel if the name is just "dvb://" or "tv://"
	if ((channel_id == "dvb://") && (!pref->last_dvb_channel.isEmpty())) {
		channel_id = pref->last_dvb_channel;
	}
	else
	if ((channel_id == "tv://") && (!pref->last_tv_channel.isEmpty())) {
		channel_id = pref->last_tv_channel;
	}

	// Save last channel
	if (channel_id.startsWith("dvb://")) pref->last_dvb_channel = channel_id;
	else
	if (channel_id.startsWith("tv://")) pref->last_tv_channel = channel_id;

	mdat.reset();
	mdat.filename = channel_id;
	mdat.type = TYPE_TV;

	mset.reset();

	// Set the default deinterlacer for TV
	mset.current_deinterlacer = pref->initial_tv_deinterlace;

	if (pref->remember_media_settings) {
		// Check if we already have info about this file
		if (tv_settings->existSettingsFor(channel_id, mdat.type)) {
			qDebug("Core::openTV: we have settings for this file!!!");

			// In this case we read info from config
			tv_settings->loadSettingsFor(channel_id, mdat.type, mset, proc->player());
			qDebug("Core::openTV: media settings read");
		}
	}

	/* initializeMenus(); */

	initPlaying();
}
#endif

void Core::openStream(QString name, QStringList params) {
	qDebug() << "Core::openStream:" << name << "params:" << params;

#ifdef YOUTUBE_SUPPORT
	if (PREF_YT_ENABLED) {
		// Check if the stream is a youtube url
		QString yt_full_url = yt->fullUrl(name);
		if (!yt_full_url.isEmpty()) {
			qDebug("Core::openStream: youtube url detected: %s", yt_full_url.toLatin1().constData());
			name = yt_full_url;
			yt->setPreferredQuality( (RetrieveYoutubeUrl::Quality) pref->yt_quality );
			qDebug("Core::openStream: user_agent: '%s'", pref->yt_user_agent.toUtf8().constData());
			/*if (!pref->yt_user_agent.isEmpty()) yt->setUserAgent(pref->yt_user_agent); */
			yt->setUserAgent(pref->yt_user_agent);
			#ifdef YT_USE_YTSIG
			YTSig::setScriptFile( Paths::configPath() + "/yt.js" );
			#endif
			yt->fetchPage(name);
			return;
		}
	}
#endif

	if (proc->isRunning()) {
		stopMplayer();
		we_are_restarting = false;
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = name;
	mdat.type = TYPE_STREAM;
	mdat.extra_params = params;

	mset.reset();

	#ifdef YOUTUBE_SUPPORT
	if (PREF_YT_ENABLED) {
		if (mdat.filename == yt->latestPreferredUrl()) name = yt->origUrl();
	}
	#endif
	// Check if we already have info about this file
	if (file_settings->existSettingsFor(name, mdat.type)) {
		qDebug("Core::openStream: we have settings for this stream");
		restoreSettingsForMedia(name, mdat.type);
	}

	/* initializeMenus(); */

	initPlaying();
}


void Core::playNewFile(QString file, int seek) {
	qDebug("Core::playNewFile: '%s'", file.toUtf8().data());

	if (proc->isRunning()) {
		stopMplayer();
		we_are_restarting = false;
	}

	// Save data of previous file:
	saveMediaInfo();

	mdat.reset();
	mdat.filename = file;
	mdat.type = TYPE_FILE;

	int old_volume = mset.volume;
	mset.reset();

	// Check if we already have info about this file
	if (file_settings->existSettingsFor(file, mdat.type)) {
		qDebug("Core::playNewFile: we have settings for this file");
		restoreSettingsForMedia(file, mdat.type);
	} else {
		// Recover volume
		mset.volume = old_volume;
	}

	/* initializeMenus(); */

	qDebug("Core::playNewFile: volume: %d, old_volume: %d", mset.volume, old_volume);
	initPlaying(seek);
}


void Core::restartPlay() {
	we_are_restarting = true;
	initPlaying();
}

void Core::initPlaying(int seek) {
	qDebug("Core::initPlaying");

	/*
	mdat.list();
	mset.list();
	*/

	/* updateWidgets(); */

	mplayerwindow->hideLogo();

	if (proc->isRunning()) {
		stopMplayer();
	}

	int start_sec = (int) mset.current_sec;
	if (seek > -1) start_sec = seek;

#ifdef YOUTUBE_SUPPORT
	if (PREF_YT_ENABLED) {
		// Avoid to pass to mplayer the youtube page url
		if (mdat.type == TYPE_STREAM) {
			if (mdat.filename == yt->origUrl()) {
				mdat.filename = yt->latestPreferredUrl();
			}
		}
	}
#endif

	startMplayer( mdat.filename, start_sec );
}

// This is reached when a new video has just started playing
// and maybe we need to give some defaults
void Core::newMediaPlaying() {
	qDebug("Core::newMediaPlaying: --- start ---");

	QString file = mdat.filename;
	int type = mdat.type;
	mdat = proc->mediaData();
	mdat.filename = file;
	mdat.type = type;

	initializeMenus(); // Old

	// Copy the demuxer
	mset.current_demuxer = mdat.demuxer;

	// Video
	#if 0
	if ( (mset.current_video_id == MediaSettings::NoneSelected) && 
         (mdat.videos.numItems() > 0) ) 
	{
		changeVideo( mdat.videos.itemAt(0).ID(), false ); // Don't allow to restart
	}
	#endif

#if !DELAYED_AUDIO_SETUP_ON_STARTUP && !NOTIFY_AUDIO_CHANGES
	// First audio if none selected
	if ( (mset.current_audio_id == MediaSettings::NoneSelected) && 
         (mdat.audios.numItems() > 0) ) 
	{
		// Don't set mset.current_audio_id here! changeAudio will do. 
		// Otherwise changeAudio will do nothing.

		int audio = mdat.audios.itemAt(0).ID(); // First one
		if (mdat.audios.existsItemAt(pref->initial_audio_track-1)) {
			audio = mdat.audios.itemAt(pref->initial_audio_track-1).ID();
		}

		// Check if one of the audio tracks is the user preferred.
		if (!pref->audio_lang.isEmpty()) {
			int res = mdat.audios.findLang( pref->audio_lang );
			if (res != -1) audio = res;
		}

		// Change the audio without restarting mplayer, it's not
		// safe to do it here.
		changeAudio( audio, false );

	}
#endif

#if !NOTIFY_SUB_CHANGES
	// Subtitles
	if (mset.external_subtitles.isEmpty()) {
		if (pref->autoload_sub) {
			//Select first subtitle if none selected
			if (mset.current_sub_id == MediaSettings::NoneSelected) {
				int sub = mdat.subs.selectOne( pref->subtitle_lang, pref->initial_subtitle_track-1 );
				changeSubtitle( sub );
			}
		} else {
			changeSubtitle( MediaSettings::SubNone );
		}
	}
#endif

	mdat.initialized = true;

	// MPlayer doesn't display the length in ID_LENGTH for audio CDs...
	if ((mdat.duration == 0) && (mdat.type == TYPE_AUDIO_CD)) {
		/*
		qDebug(" *** get duration here from title info *** ");
		qDebug(" *** current title: %d", mset.current_title_id );
		*/
		if (mset.current_title_id > 0) {
			mdat.duration = mdat.titles.item(mset.current_title_id).duration();
		}
	}

	/* updateWidgets(); */

	mdat.list();
	mset.list();

	qDebug("Core::newMediaPlaying: --- end ---");
}

void Core::finishRestart() {
	qDebug("Core::finishRestart: --- start ---");

	proc->enableOSDInCommands(false);

	if (!we_are_restarting) {
		newMediaPlaying();
		//QTimer::singleShot(1000, this, SIGNAL(mediaStartPlay())); 
		emit mediaStartPlay();
	} 

	if (we_are_restarting) {
		// Update info about codecs and demuxer
		mdat.video_codec = proc->mediaData().video_codec;
		mdat.audio_codec = proc->mediaData().audio_codec;
		mdat.demuxer = proc->mediaData().demuxer;
	}

	if (forced_titles.contains(mdat.filename)) {
		mdat.clip_name = forced_titles[mdat.filename];
	}

#ifdef YOUTUBE_SUPPORT
	if (PREF_YT_ENABLED) {
		// Change the real url with the youtube page url and set the title
		if (mdat.type == TYPE_STREAM) {
			if (mdat.filename == yt->latestPreferredUrl()) {
				mdat.filename = yt->origUrl();
				mdat.stream_title = yt->urlTitle();
			}
		}
	}
#endif

#if !NOTIFY_SUB_CHANGES
	// Subtitles
	//if (we_are_restarting) {
	if ( (just_loaded_external_subs) || (just_unloaded_external_subs) ) {
		qDebug("Core::finishRestart: processing new subtitles");

		// Just to simplify things
		if (mset.current_sub_id == MediaSettings::NoneSelected) {
			mset.current_sub_id = MediaSettings::SubNone;
		}

		// Save current sub
		SubData::Type type;
		int ID;
		int old_item = -1;
		if ( mset.current_sub_id != MediaSettings::SubNone ) {
			old_item = mset.current_sub_id;
			type = mdat.subs.itemAt(old_item).type();
			ID = mdat.subs.itemAt(old_item).ID();
		}

		// Use the subtitle info from mplayerprocess
		qDebug( "Core::finishRestart: copying sub data from proc to mdat");
	    mdat.subs = proc->mediaData().subs;
		initializeMenus();
		int item = MediaSettings::SubNone;

		// Try to recover old subtitle
		if (just_unloaded_external_subs) {
			if (old_item > -1) {
				int new_item = mdat.subs.find(type, ID);
				if (new_item > -1) item = new_item;
			}
		}

		// If we've just loaded a subtitle file
		// select one if the user wants to autoload
		// one subtitle
		if (just_loaded_external_subs) {
			if ( (pref->autoload_sub) && (item == MediaSettings::SubNone) ) {
				qDebug("Core::finishRestart: cannot find previous subtitle");
				qDebug("Core::finishRestart: selecting a new one");
				item = mdat.subs.selectOne( pref->subtitle_lang );
			}
		}
		changeSubtitle( item );
		just_loaded_external_subs = false;
		just_unloaded_external_subs = false;
	} else {
		// Normal restart, subtitles haven't changed
		// Recover current subtitle
		changeSubtitle( mset.current_sub_id );
		changeSecondarySubtitle( mset.current_secondary_sub_id );
	}
#endif

	we_are_restarting = false;

	changeAspectRatio(mset.aspect_ratio_id);

	if (pref->mplayer_additional_options.contains("-volume")) {
		qDebug("Core::finishRestart: don't set volume since -volume is used");
	} else {
		// Code to set the volume, used when mplayer didn't have the -volume option
		/*
		if (pref->global_volume) {
			bool was_muted = pref->mute;
			setVolume( pref->volume, true);
			if (was_muted) mute(true);
		} else {
			bool was_muted = mset.mute;
			setVolume( mset.volume, true );
			if (was_muted) mute(true);
		}
		*/
		int vol = (pref->global_volume ? pref->volume : mset.volume);
		volumeChanged(vol);

		if (proc->isMPlayer() && pref->mute) {
			// Set mute here because mplayer doesn't have an option to set mute from the command line
			mute(true);
		}
	}

#if 0
// Old. Gamma already set with option -gamma
	if (pref->change_video_equalizer_on_startup && (mset.gamma != 0)) {
		int gamma = mset.gamma;
		mset.gamma = -1000; // if mset.gamma == new value, mset.gamma is not changed!
		setGamma( gamma );
	}
#endif
	// Hack to be sure that the equalizers are up to date
	emit videoEqualizerNeedsUpdate();
	emit audioEqualizerNeedsUpdate();

	changeZoom(mset.zoom_factor);

	// Toggle subtitle visibility
	changeSubVisibility(pref->sub_visibility);

	// A-B marker
	emit ABMarkersChanged(mset.A_marker, mset.B_marker);

#ifdef MPLAYER_SUPPORT
	// Initialize the OSD level
	if (proc->isMPlayer()) {
		QTimer::singleShot(pref->osd_delay, this, SLOT(initializeOSD()));
	}
#endif

	emit mediaLoaded();
	emit mediaInfoChanged();
	emit newDuration(mdat.duration);

	emit mediaDataReceived(mdat);

	updateWidgets(); // New

	proc->enableOSDInCommands(true);

	qDebug("Core::finishRestart: --- end ---");
}

#ifdef MPLAYER_SUPPORT
void Core::initializeOSD() {
	qDebug("Core::initializeOSD");
	changeOSD(pref->osd);
	#ifdef MPV_SUPPORT
	setOSDFractions(pref->osd_fractions);
	#endif
}
#endif

void Core::stop()
{
	qDebug("Core::stop");
	qDebug("Core::stop: state: %s", stateToString().toUtf8().data());
	
	if (state()==Stopped) {
		// if pressed stop twice, reset video to the beginning
		qDebug("Core::stop: mset.current_sec: %f", mset.current_sec);
		mset.current_sec = 0;
		qDebug("Core::stop: mset.current_sec set to 0");
		emit showTime( mset.current_sec );
		#ifdef SEEKBAR_RESOLUTION
		emit positionChanged( 0 );
		#else
		emit posChanged( 0 );
		#endif
		//updateWidgets();
	}

	stopMplayer();
	emit mediaStoppedByUser();

	if (pref->reset_stop) {
		mset.current_sec = 0;
		emit showTime( mset.current_sec );
		#ifdef SEEKBAR_RESOLUTION
		emit positionChanged( 0 );
		#else
		emit posChanged( 0 );
		#endif
	}
}


void Core::play() {
	qDebug("Core::play");

	if ((proc->isRunning()) && (state()==Paused)) {
		proc->setPause(false);
	}
	else
	if ((proc->isRunning()) && (state()==Playing)) {
		// nothing to do, continue playing
	}
	else {
		// if we're stopped, play it again
		if ( !mdat.filename.isEmpty() ) {
			/*
			qDebug( "current_sec: %f, duration: %f", mset.current_sec, mdat.duration);
			if ( (floor(mset.current_sec)) >= (floor(mdat.duration)) ) {
				mset.current_sec = 0;
			}
			*/
			restartPlay();
		} else {
			emit noFileToPlay();
		}
	}
}

void Core::pause_and_frame_step() {
	qDebug("Core::pause_and_frame_step");
	
	if (proc->isRunning()) {
		if (state() == Paused) {
			proc->frameStep();
		} else {
			proc->setPause(true);
		}
	}
}

void Core::pause() {
	qDebug() << "Core::pause: current state:" << stateToString();

	if (proc->isRunning()) {
		// Pauses and unpauses
		if (state() == Paused) proc->setPause(false); else proc->setPause(true);
	}
}

void Core::play_or_pause() {
	qDebug("Core::play_or_pause");

	if (proc->isRunning()) {
		pause();
	} else {
		play();
	}
}

void Core::frameStep() {
	qDebug("Core::frameStep");

	if (proc->isRunning()) {
		proc->frameStep();
	}
}

void Core::frameBackStep() {
	qDebug("Core::frameBackStep");

	if (proc->isRunning()) {
		proc->frameBackStep();
	}
}

void Core::screenshot(bool include_subtitles) {
	qDebug() << "Core::screenshot: include_subtitles:" << include_subtitles;

	if (!pref->screenshot_directory.isEmpty()
        /* && QFileInfo(pref->screenshot_directory).isDir() */)
	{
		proc->setPausingPrefix(pausing_prefix());
		proc->takeScreenshot(PlayerProcess::Single, include_subtitles);
		qDebug("Core::screenshot: screenshot taken");
	} else {
		qDebug("Core::screenshot: error: directory for screenshots not valid");
		emit showMessage( tr("Screenshot NOT taken, folder not configured") );
	}
}

void Core::screenshots() {
	qDebug("Core::screenshots");

	if (!pref->screenshot_directory.isEmpty()
        /* && QFileInfo(pref->screenshot_directory).isDir() */)
	{
		proc->takeScreenshot(PlayerProcess::Multiple, pref->subtitles_on_screenshots);
	} else {
		qDebug("Core::screenshots: error: directory for screenshots not valid");
		emit showMessage( tr("Screenshots NOT taken, folder not configured") );
	}
}

void Core::screenshot() {
	qDebug("Core::screenshot");
	screenshot(pref->subtitles_on_screenshots);
}

void Core::screenshotWithSubtitles() {
	qDebug("Core::screenshotWithSubtitles");
	screenshot(true);
}

void Core::screenshotWithoutSubtitles() {
	qDebug("Core::screenshotWithoutSubtitles");
	screenshot(false);
}

#ifdef CAPTURE_STREAM
void Core::switchCapturing() {
	qDebug("Core::switchCapturing");
	proc->switchCapturing();
}
#endif

void Core::processFinished()
{
    qDebug("Core::processFinished");
	qDebug("Core::processFinished: we_are_restarting: %d", we_are_restarting);

	//mset.current_sec = 0;

	if (!we_are_restarting) {
		qDebug("Core::processFinished: play has finished!");
		setState(Stopped);
		//emit stateChanged(state());
	}

	int exit_code = proc->exitCode();
	qDebug("Core::processFinished: exit_code: %d", exit_code);
	if (exit_code != 0) {
		setState(Stopped);
		emit stateChanged(Stopped);
		emit mplayerFinishedWithError(exit_code);
	}
}

void Core::fileReachedEnd() {
	/*
	if (mdat.type == TYPE_VCD) {
		// If the first vcd title has nothing, it doesn't start to play
        // and menus are not initialized.
		initializeMenus();
	}
	*/

	// If we're at the end of the movie, reset to 0
	mset.current_sec = 0;
	updateWidgets();

	emit mediaFinished();
}

#if SEEKBAR_RESOLUTION
void Core::goToPosition(int value) {
	qDebug("Core::goToPosition: value: %d", value);

	if (pref->relative_seeking) {
		goToPos( (double) value / (SEEKBAR_RESOLUTION / 100) );
	}
	else {
		if (mdat.duration > 0) {
			int jump_time = (int) mdat.duration * value / SEEKBAR_RESOLUTION;
			goToSec(jump_time);
		}
	}
}

void Core::goToPos(double perc) {
	qDebug("Core::goToPos: per: %f", perc);
	seek_cmd(perc, 1);
}
#else
void Core::goToPos(int perc) {
	qDebug("Core::goToPos: per: %d", perc);
	seek_cmd(perc, 1);
}
#endif


void Core::startMplayer( QString file, double seek ) {
	qDebug("Core::startMplayer");

	if (file.isEmpty()) {
		qWarning("Core:startMplayer: file is empty!");
		return;
	}

	if (proc->isRunning()) {
		qWarning("Core::startMplayer: MPlayer still running!");
		return;
    }

#ifdef YOUTUBE_SUPPORT
	// Stop any pending request
	#if 0
	qDebug("Core::startMplayer: yt state: %d", yt->state());
	if (yt->state() != QHttp::Unconnected) {
		//yt->abort(); /* Make the app to crash, don't know why */
	}
	#endif
	yt->close();
#endif

	// DVD
	QString dvd_folder;
	int dvd_title = -1;
	if (mdat.type==TYPE_DVD) {
		DiscData disc_data = DiscName::split(file);
		dvd_folder = disc_data.device;
		if (dvd_folder.isEmpty()) dvd_folder = pref->dvd_device;
		dvd_title = disc_data.title;
		file = disc_data.protocol + "://";
		if (dvd_title > -1) file += QString::number(dvd_title);
	}

	// Check URL playlist
	bool url_is_playlist = false;
	if (file.endsWith("|playlist")) {
		url_is_playlist = true;
		file = file.remove("|playlist");
	} else {
		QUrl url(file);
		qDebug("Core::startMplayer: checking if stream is a playlist");
		qDebug("Core::startMplayer: url path: '%s'", url.path().toUtf8().constData());

		if (url.scheme().toLower() != "ffmpeg") {
			QRegExp rx("\\.ram$|\\.asx$|\\.m3u$|\\.m3u8$|\\.pls$", Qt::CaseInsensitive);
			url_is_playlist = (rx.indexIn(url.path()) != -1);
		}
	}
	qDebug("Core::startMplayer: url_is_playlist: %d", url_is_playlist);

	// Hack: don't use -ss with m3u(8) streams
	if (mdat.type == TYPE_STREAM) {
		QString extension = Extensions::extensionFromUrl(file);
		qDebug() << "Core::startMplayer: URL extension:" << extension;
		if (extension.contains("m3u")) {
			seek = 0;
		}
	}

	// Check if a m4a file exists with the same name of file, in that cause if will be used as audio
	if (pref->autoload_m4a && mset.external_audio.isEmpty()) {
		QFileInfo fi(file);
		if (fi.exists() && !fi.isDir()) {
			if (fi.suffix().toLower() == "mp4") {
				QString file2 = fi.path() + "/" + fi.completeBaseName() + ".m4a";
				//qDebug("Core::startMplayer: file2: %s", file2.toUtf8().constData());
				if (!QFile::exists(file2)) {
					// Check for upper case
					file2 = fi.path() + "/" + fi.completeBaseName() + ".M4A";
				}
				if (QFile::exists(file2)) {
					qDebug("Core::startMplayer: found %s, so it will be used as audio file", file2.toUtf8().constData());
					mset.external_audio = file2;
				}
			}
		}
	}


	bool screenshot_enabled = (pref->use_screenshot && !pref->screenshot_directory.isEmpty()
                               /* && QFileInfo(pref->screenshot_directory).isDir() */);

	proc->clearArguments();

	// Set the screenshot directory
	/* deleted (done later) */

	// Use absolute path, otherwise after changing to the screenshot directory
	// the mplayer path might not be found if it's a relative path
	// (seems to be necessary only for linux)
	QString mplayer_bin = pref->mplayer_bin;
	QFileInfo fi(mplayer_bin);
	if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
		mplayer_bin = fi.absoluteFilePath();
	}

#ifdef MPLAYER2_SUPPORT
	if (fi.baseName().toLower() == "mplayer2") {
		if (!pref->mplayer_is_mplayer2) {
			qDebug("Core::startMplayer: this seems mplayer2");
			pref->mplayer_is_mplayer2 = true;
		}
	}
#endif

	proc->setExecutable(mplayer_bin);
	proc->setFixedOptions();

#ifdef LOG_MPLAYER
	if (pref->verbose_log) {
		proc->setOption("verbose");
	}
#endif

	if (pref->fullscreen && pref->use_mplayer_window) {
		proc->setOption("fs", true);
	} else {
		// No mplayer fullscreen mode
		proc->setOption("fs", false);
	}

#if !ALLOW_DEMUXER_CODEC_CHANGE
	if (pref->use_lavf_demuxer) {
		proc->setOption("demuxer", "lavf");
	}
#else
	// Demuxer and audio and video codecs:
	if (!mset.forced_demuxer.isEmpty()) {
		proc->setOption("demuxer", mset.forced_demuxer);
	}
	if (!mset.forced_audio_codec.isEmpty()) {
		proc->setOption("ac", mset.forced_audio_codec);
	}
	if (!mset.forced_video_codec.isEmpty()) {
		proc->setOption("vc", mset.forced_video_codec);
	}
	else
#endif
	{
		#ifndef Q_OS_WIN
		/* if (pref->vo.startsWith("x11")) { */ // My card doesn't support vdpau, I use x11 to test
		if (pref->vo.startsWith("vdpau")) {
			QString c;
			if (pref->vdpau.ffh264vdpau) c += "ffh264vdpau,";
			if (pref->vdpau.ffmpeg12vdpau) c += "ffmpeg12vdpau,";
			if (pref->vdpau.ffwmv3vdpau) c += "ffwmv3vdpau,";
			if (pref->vdpau.ffvc1vdpau) c += "ffvc1vdpau,";
			if (pref->vdpau.ffodivxvdpau) c += "ffodivxvdpau,";
			if (pref->vdpau.ffhevcvdpau) c += "ffhevcvdpau,";
			if (!c.isEmpty()) {
				proc->setOption("vc", c);
			}
		}
		else {
		#endif
			if (pref->coreavc) {
				proc->setOption("vc", "coreserve,");
			}
		#ifndef Q_OS_WIN
		}
		#endif
	}

	if (pref->use_hwac3) {
		proc->setOption("afm", "hwac3");
	}


	if (proc->isMPlayer()) {
		// MPlayer
		QString lavdopts;

		if ( (pref->h264_skip_loop_filter == Preferences::LoopDisabled) || 
	         ((pref->h264_skip_loop_filter == Preferences::LoopDisabledOnHD) && 
	          (mset.is264andHD)) )
		{
			if (!lavdopts.isEmpty()) lavdopts += ":";
			lavdopts += "skiploopfilter=all";
		}

		if (pref->threads > 1) {
			if (!lavdopts.isEmpty()) lavdopts += ":";
			lavdopts += "threads=" + QString::number(pref->threads);
		}

		if (!lavdopts.isEmpty()) {
			proc->setOption("lavdopts", lavdopts);
		}
	}
	else {
		// MPV
		if ( (pref->h264_skip_loop_filter == Preferences::LoopDisabled) || 
	         ((pref->h264_skip_loop_filter == Preferences::LoopDisabledOnHD) && 
	          (mset.is264andHD)) )
		{
			proc->setOption("skiploopfilter");
		}

		if (pref->threads > 1) {
			proc->setOption("threads", QString::number(pref->threads));
		}
	}

	if (!pref->hwdec.isEmpty()) proc->setOption("hwdec", pref->hwdec);

	proc->setOption("sub-fuzziness", pref->subfuzziness);

	if (!pref->vo.isEmpty()) {
		QString vo = pref->vo;
		if (!vo.endsWith(",")) vo += ",";
		proc->setOption("vo", vo);
	}
	#ifdef Q_OS_WIN
	else {
		if (proc->isMPlayer() && QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA) {
			proc->setOption("vo", "direct3d,");
		}
	}
	#endif

#if USE_ADAPTER
	if (pref->adapter > -1) {
		proc->setOption("adapter", QString::number(pref->adapter));
	}
#endif

	if (!pref->ao.isEmpty()) {
		QString ao = pref->ao;
		//if (!ao.endsWith(",")) ao += ",";
		proc->setOption("ao", ao);
	}

#if !defined(Q_OS_WIN) && !defined(Q_OS_OS2)
	if (pref->vo.startsWith("x11")) {
		proc->setOption("zoom");
	}
#endif

	// Performance options
	#ifdef Q_OS_WIN
	QString p;
	int app_p = NORMAL_PRIORITY_CLASS;
	switch (pref->priority) {
		case Preferences::Realtime: 	p = "realtime"; 
										app_p = REALTIME_PRIORITY_CLASS;
										break;
		case Preferences::High:			p = "high"; 
										app_p = REALTIME_PRIORITY_CLASS;
										break;
		case Preferences::AboveNormal:	p = "abovenormal"; 
										app_p = HIGH_PRIORITY_CLASS;
										break;
		case Preferences::Normal: 		p = "normal"; 
										app_p = ABOVE_NORMAL_PRIORITY_CLASS; 
										break;
		case Preferences::BelowNormal: 	p = "belownormal"; break;
		case Preferences::Idle: 		p = "idle"; break;
		default: 						p = "normal";
	}
	proc->setOption("priority", p);
	/*
	SetPriorityClass(GetCurrentProcess(), app_p);
	qDebug("Core::startMplayer: priority of smplayer process set to %d", app_p);
	*/
	#endif

	if (pref->frame_drop && pref->hard_frame_drop) {
		proc->setOption("framedrop", "decoder+vo");
	}
	else
	if (pref->frame_drop) {
		proc->setOption("framedrop", "vo");
	}
	else
	if (pref->hard_frame_drop) {
		proc->setOption("framedrop", "decoder");
	}

	if (pref->autosync) {
		proc->setOption("autosync", QString::number(pref->autosync_factor));
	}

	if (pref->use_mc) {
		proc->setOption("mc", QString::number(pref->mc_value));
	}

	proc->setOption("dr", pref->use_direct_rendering);
	proc->setOption("double", pref->use_double_buffer);

#ifdef Q_WS_X11
	proc->setOption("stop-xscreensaver", pref->disable_screensaver);
#endif

	if (!pref->use_mplayer_window) {
		proc->disableInput();
		proc->setOption("keepaspect", false);

#if defined(Q_OS_OS2)
		#define WINIDFROMHWND(hwnd) ( ( hwnd ) - 0x80000000UL )
		proc->setOption("wid", QString::number( WINIDFROMHWND( (int) mplayerwindow->videoLayer()->winId() ) ));
#else
		proc->setOption("wid", QString::number( (qint64) mplayerwindow->videoLayer()->winId() ) );
#endif

#if USE_COLORKEY
		#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
		if ((pref->vo.startsWith("directx")) || (pref->vo.startsWith("kva")) || (pref->vo.isEmpty())) {
			proc->setOption("colorkey", ColorUtils::colorToRGB(pref->color_key));
		} else {
		#endif
			/*
			qDebug("Core::startMplayer: * not using -colorkey for %s", pref->vo.toUtf8().data());
			qDebug("Core::startMplayer: * report if you can't see the video"); 
			*/
		#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
		}
		#endif
#endif

		// Square pixels
		proc->setOption("monitorpixelaspect", "1");
	} else {
		// no -wid
		proc->setOption("keepaspect", true);
		if (!pref->monitor_aspect.isEmpty()) {
			proc->setOption("monitoraspect", pref->monitor_aspect);
		}
	}

	// OSD
	#ifdef MPLAYER_SUPPORT
	// Enable the OSD later, to avoid a lot of messages to be
	// printed on startup
	if (proc->isMPlayer()) {
		proc->setOption("osd-level", "0");
	}
	#endif
	#ifdef MPV_SUPPORT
	if (proc->isMPV()) {
		proc->setOption("osd-level", pref->osd);
	}
	proc->setOption("osd-fractions", pref->osd_fractions);
	#endif
	proc->setOption("osd-scale", proc->isMPlayer() ? pref->subfont_osd_scale : pref->osd_scale);
	proc->setOption("osd-bar-pos", pref->osd_bar_pos);

	// Subtitles fonts
	if ((pref->use_ass_subtitles) && (pref->freetype_support)) {
		// ASS:
		proc->setOption("ass");
		proc->setOption("embeddedfonts");

		proc->setOption("ass-line-spacing", QString::number(pref->ass_line_spacing));

		proc->setOption("ass-font-scale", QString::number(mset.sub_scale_ass));

		#ifdef MPLAYER2_SUPPORT
		if (!pref->mplayer_is_mplayer2)
		#endif
		{
			proc->setOption("flip-hebrew",false); // It seems to be necessary to display arabic subtitles correctly when using -ass
		}

		if (pref->enable_ass_styles) {
			QString ass_force_style;
			if (!pref->user_forced_ass_style.isEmpty()) {
				ass_force_style = pref->user_forced_ass_style;
			} else {
				ass_force_style = pref->ass_styles.toString();
			}

			if (proc->isMPV()) {
				// MPV
				proc->setSubStyles(pref->ass_styles);
				if (pref->force_ass_styles) {
					proc->setOption("ass-force-style", ass_force_style);
				}
			} else {
				// MPlayer
				if (!pref->force_ass_styles) {
					proc->setSubStyles(pref->ass_styles, Paths::subtitleStyleFile());
				} else {
					proc->setOption("ass-force-style", ass_force_style);
				}
			}
		}

		// Use the same font for OSD
		// deleted

		// Set the size of OSD
		// deleted
	} else {
		// NO ASS:
		if (pref->freetype_support) proc->setOption("noass");
		proc->setOption("subfont-text-scale", QString::number(mset.sub_scale));
	}

	// Subtitle encoding
	proc->setSubEncoding(pref->subcp, pref->use_enca ? pref->enca_lang : "");

	if (mset.closed_caption_channel > 0) {
		proc->setOption("subcc", QString::number(mset.closed_caption_channel));
	}

	if (pref->use_forced_subs_only) {
		proc->setOption("forcedsubsonly");
	}

#if PROGRAM_SWITCH
	if ( (mset.current_program_id != MediaSettings::NoneSelected) /*&& 
         (mset.current_video_id == MediaSettings::NoneSelected) && 
         (mset.current_audio_id == MediaSettings::NoneSelected)*/ )
	{
		proc->setOption("tsprog", QString::number(mset.current_program_id));
	}
	// Don't set video and audio track if using -tsprog
	else {
#endif

#if 1
	if (mset.current_video_id != MediaSettings::NoneSelected) {
		proc->setOption("vid", QString::number(mset.current_video_id));
	}

	if (mset.external_audio.isEmpty()) {
		if (mset.current_audio_id != MediaSettings::NoneSelected) {
			// Workaround for MPlayer bug #1321 (http://bugzilla.mplayerhq.hu/show_bug.cgi?id=1321)
			if (mdat.audios.numItems() != 1) {
				proc->setOption("aid", QString::number(mset.current_audio_id));
			}
		}
	}

	#ifdef SIMPLE_TRACK_SELECTION
	if (proc->isMPV() && mset.current_sub_id != MediaSettings::NoneSelected) {
		int real_id = getSubRealID(mset.current_sub_id);
		proc->setOption("sid", QString::number(real_id));
	}
	#endif
#endif

#if PROGRAM_SWITCH
	}
#endif

	if (!initial_subtitle.isEmpty()) {
		mset.external_subtitles = initial_subtitle;
		initial_subtitle = "";
		just_loaded_external_subs = true; // Big ugly hack :(
	}
	if (!mset.external_subtitles.isEmpty()) {
		bool is_idx = (QFileInfo(mset.external_subtitles).suffix().toLower()=="idx");
		if (proc->isMPV()) is_idx = false; // Hack to ignore the idx extension with mpv

		if (is_idx) {
			// sub/idx subtitles
			QFileInfo fi;

			#ifdef Q_OS_WIN
			if (pref->use_short_pathnames)
				fi.setFile(Helper::shortPathName(mset.external_subtitles));
			else
			#endif
			fi.setFile(mset.external_subtitles);

			QString s = fi.path() +"/"+ fi.completeBaseName();
			qDebug("Core::startMplayer: subtitle file without extension: '%s'", s.toUtf8().data());
			proc->setOption("vobsub", s);
		} else {
			#ifdef Q_OS_WIN
			if (pref->use_short_pathnames)
				proc->setOption("sub", Helper::shortPathName(mset.external_subtitles));
			else
			#endif
			{
				proc->setOption("sub", mset.external_subtitles);
			}
		}
		if (mset.external_subtitles_fps != MediaSettings::SFPS_None) {
			QString fps;
			switch (mset.external_subtitles_fps) {
				case MediaSettings::SFPS_23: fps = "23"; break;
				case MediaSettings::SFPS_24: fps = "24"; break;
				case MediaSettings::SFPS_25: fps = "25"; break;
				case MediaSettings::SFPS_30: fps = "30"; break;
				case MediaSettings::SFPS_23976: fps = "24000/1001"; break;
				case MediaSettings::SFPS_29970: fps = "30000/1001"; break;
				default: fps = "25";
			}
			proc->setOption("subfps", fps);
		}
	}

	if (!mset.external_audio.isEmpty()) {
		#ifdef Q_OS_WIN
		if (pref->use_short_pathnames)
			proc->setOption("audiofile", Helper::shortPathName(mset.external_audio));
		else
		#endif
		{
			proc->setOption("audiofile", mset.external_audio);
		}
	}

	proc->setOption("subpos", QString::number(mset.sub_pos));

	if (mset.audio_delay != 0) {
		proc->setOption("delay", QString::number((double) mset.audio_delay/1000));
	}

	if (mset.sub_delay != 0) {
		proc->setOption("subdelay", QString::number((double) mset.sub_delay/1000));
	}

	// Contrast, brightness...
	if (pref->change_video_equalizer_on_startup) {
		/*
		if (mset.contrast != 0) {
			proc->setOption("contrast", QString::number(mset.contrast));
		}
	
		if (mset.brightness != 0) {
			proc->setOption("brightness", QString::number(mset.brightness));
		}

		if (mset.hue != 0) {
			proc->setOption("hue", QString::number(mset.hue));
		}

		if (mset.saturation != 0) {
			proc->setOption("saturation", QString::number(mset.saturation));
		}

		if (mset.gamma != 0) {
			proc->setOption("gamma", QString::number(mset.gamma));
		}
		*/
		proc->setVideoEqualizerOptions(mset.contrast, mset.brightness, mset.hue, mset.saturation, mset.gamma, pref->use_soft_video_eq);
	}


	if (pref->mplayer_additional_options.contains("-volume")) {
		qDebug("Core::startMplayer: don't set volume since -volume is used");
	} else {
		int vol = (pref->global_volume ? pref->volume : mset.volume);
		if (proc->isMPV()) {
			vol = adjustVolume(vol, pref->use_soft_vol ? pref->softvol_max : 100);
		}
		proc->setOption("volume", QString::number(vol));
	}

	if (pref->mute) {
		proc->setOption("mute");
	}


	if (mdat.type==TYPE_DVD) {
		if (!dvd_folder.isEmpty()) {
			#ifdef Q_OS_WIN
			if (pref->use_short_pathnames) {
				proc->setOption("dvd-device", Helper::shortPathName(dvd_folder));
			}
			else
			#endif
			proc->setOption("dvd-device", dvd_folder);
		} else {
			qWarning("Core::startMplayer: dvd device is empty!");
		}
	}

	if ((mdat.type==TYPE_VCD) || (mdat.type==TYPE_AUDIO_CD)) {
		if (!pref->cdrom_device.isEmpty()) {
			proc->setOption("cdrom-device", pref->cdrom_device);
		}
	}

	if (mset.current_angle_id > 0) {
		proc->setOption("dvdangle", QString::number( mset.current_angle_id));
	}


	int cache = 0;
	switch (mdat.type) {
		case TYPE_FILE	 	: cache = pref->cache_for_files; break;
		case TYPE_DVD 		: cache = pref->cache_for_dvds; 
							  #if DVDNAV_SUPPORT
							  if (file.startsWith("dvdnav:")) cache = 0;
							  #endif
		                      break;
		case TYPE_STREAM 	: cache = pref->cache_for_streams; break;
		case TYPE_VCD 		: cache = pref->cache_for_vcds; break;
		case TYPE_AUDIO_CD	: cache = pref->cache_for_audiocds; break;
#ifdef TV_SUPPORT
		case TYPE_TV		: cache = pref->cache_for_tv; break;
#endif
#ifdef BLURAY_SUPPORT
		case TYPE_BLURAY	: cache = pref->cache_for_dvds; break; // FIXME: use cache for bluray?
#endif
		default: cache = 0;
	}

	if (pref->cache_auto) {
		proc->setOption("cache_auto");
	} else {
		proc->setOption("cache", QString::number(cache));
	}

	if (mset.speed != 1.0) {
		proc->setOption("speed", QString::number(mset.speed));
	}

	if (mdat.type != TYPE_TV) {
		// Play A - B
		if ((mset.A_marker > -1) && (mset.B_marker > mset.A_marker)) {
			if (proc->isMPV()) {
				if (mset.loop) {
					proc->setOption("ab-loop-a", QString::number(mset.A_marker));
					proc->setOption("ab-loop-b", QString::number(mset.B_marker));
				}
				proc->setOption("ss", QString::number(seek));
			} else {
				proc->setOption("ss", QString::number(mset.A_marker));
				proc->setOption("endpos", QString::number(mset.B_marker - mset.A_marker));
			}
		}
		else
		// If seek < 5 it's better to allow the video to start from the beginning
		if ((seek >= 5) && (!mset.loop)) {
			proc->setOption("ss", QString::number(seek));
		}
	}

	if (pref->use_idx) {
		proc->setOption("idx");
	}

	if (mdat.type == TYPE_STREAM) {
		if (pref->prefer_ipv4) {
			proc->setOption("prefer-ipv4");
		} else {
			proc->setOption("prefer-ipv6");
		}
	}

	if (pref->use_correct_pts != Preferences::Detect) {
		proc->setOption("correct-pts", (pref->use_correct_pts == Preferences::Enabled));
	}

	bool force_noslices = false;

#ifndef Q_OS_WIN
	if (proc->isMPlayer()) {
		if ((pref->vdpau.disable_video_filters) && (pref->vo.startsWith("vdpau"))) {
			qDebug("Core::startMplayer: using vdpau, video filters are ignored");
			goto end_video_filters;
		}
	} else {
		// MPV
		if (!pref->hwdec.isEmpty() && pref->hwdec != "no") {
			qDebug("Core::startMplayer: hardware decoding is enabled. The video filters will be ignored");
			goto end_video_filters;
		}
	}
#endif

	// Video filters:
	// Phase
	if (mset.phase_filter) {
		proc->addVF("phase", "A");
	}

	// Deinterlace
	if (mset.current_deinterlacer != MediaSettings::NoDeinterlace) {
		switch (mset.current_deinterlacer) {
			case MediaSettings::L5: 		proc->addVF("l5"); break;
			case MediaSettings::Yadif: 		proc->addVF("yadif"); break;
			case MediaSettings::LB:			proc->addVF("lb"); break;
			case MediaSettings::Yadif_1:	proc->addVF("yadif", "1"); break;
			case MediaSettings::Kerndeint:	proc->addVF("kerndeint", "5"); break;
		}
	}

	// 3D stereo
	if (mset.stereo3d_in != "none" && !mset.stereo3d_out.isEmpty()) {
		proc->addStereo3DFilter(mset.stereo3d_in, mset.stereo3d_out);
	}

	// Denoise
	if (mset.current_denoiser != MediaSettings::NoDenoise) {
		if (mset.current_denoiser==MediaSettings::DenoiseSoft) {
			proc->addVF("hqdn3d", pref->filters->item("denoise_soft").options());
		} else {
			proc->addVF("hqdn3d", pref->filters->item("denoise_normal").options());
		}
	}

	// Unsharp
	if (mset.current_unsharp != 0) {
		if (mset.current_unsharp == 1) {
			proc->addVF("blur", pref->filters->item("blur").options());
		} else {
			proc->addVF("sharpen", pref->filters->item("sharpen").options());
		}
	}

	// Deblock
	if (mset.deblock_filter) {
		proc->addVF("deblock", pref->filters->item("deblock").options());
	}

	// Dering
	if (mset.dering_filter) {
		proc->addVF("dering");
	}

	// Gradfun
	if (mset.gradfun_filter) {
		proc->addVF("gradfun", pref->filters->item("gradfun").options());
	}

	// Upscale
	if (mset.upscaling_filter) {
		int width = DesktopInfo::desktop_size(mplayerwindow).width();
		proc->setOption("sws", "9");
		proc->addVF("scale", QString::number(width) + ":-2");
	}

	// Addnoise
	if (mset.noise_filter) {
		proc->addVF("noise", pref->filters->item("noise").options());
	}

	// Postprocessing
	if (mset.postprocessing_filter) {
		proc->addVF("postprocessing");
		proc->setOption("autoq", QString::number(pref->autoq));
	}



	// Letterbox (expand)
	if ((mset.add_letterbox)
         #ifdef ADD_BLACKBORDERS_FS
         || (pref->fullscreen && pref->add_blackborders_on_fullscreen)
         #endif
    ) {
		proc->addVF("letterbox", DesktopInfo::desktop_size(mplayerwindow));
	}

	// Software equalizer
	/*
	if ( (pref->use_soft_video_eq) ) {
		proc->addVF("eq2");
		proc->addVF("hue");
		if ( (pref->vo == "gl") || (pref->vo == "gl2") || (pref->vo == "gl_tiled")
		#ifdef Q_OS_WIN
             || (pref->vo == "directx:noaccel")
		#endif
		    )
		{
			proc->addVF("scale");
		}
	}
	*/

	// Additional video filters, supplied by user
	// File
	if ( !mset.mplayer_additional_video_filters.isEmpty() ) {
		proc->setOption("vf-add", mset.mplayer_additional_video_filters);
	}
	// Global
	if ( !pref->mplayer_additional_video_filters.isEmpty() ) {
		proc->setOption("vf-add", pref->mplayer_additional_video_filters);
	}

	// Filters for subtitles on screenshots
	if ((screenshot_enabled) && (pref->subtitles_on_screenshots)) 
	{
		if (pref->use_ass_subtitles) {
			proc->addVF("subs_on_screenshots", "ass");
		} else {
			proc->addVF("subs_on_screenshots");
			force_noslices = true;
		}
	}

	// Rotate
	if (mset.rotate != MediaSettings::NoRotate) {
		proc->addVF("rotate", QString::number(mset.rotate));
	}

	// Flip
	if (mset.flip) {
		proc->addVF("flip");
	}

	// Mirror
	if (mset.mirror) {
		proc->addVF("mirror");
	}

	// Screenshots
	#ifdef MPLAYER_SUPPORT
	if (screenshot_enabled && proc->isMPlayer()) {
		QString dir = pref->screenshot_directory;
		#ifdef Q_OS_WIN
		if (pref->use_short_pathnames) dir = Helper::shortPathName(pref->screenshot_directory);
		#endif
		proc->enableScreenshots(dir);
	}
	#endif

#ifndef Q_OS_WIN
	end_video_filters:
#endif

	#ifdef MPV_SUPPORT
	if (screenshot_enabled && proc->isMPV()) {
		QString dir = pref->screenshot_directory;
		#ifdef Q_OS_WIN
		if (pref->use_short_pathnames) dir = Helper::shortPathName(pref->screenshot_directory);
		#endif
		proc->enableScreenshots(dir, pref->screenshot_template, pref->screenshot_format);
	}
	#endif

	// slices
	if ((pref->use_slices) && (!force_noslices)) {
		proc->setOption("slices", true);
	} else {
		proc->setOption("slices", false);
	}


	// Audio channels
	if (mset.audio_use_channels != 0) {
		proc->setOption("channels", QString::number(mset.audio_use_channels));
	}

	if (!pref->use_hwac3) {

		// Audio filters
		if (mset.karaoke_filter) {
			proc->addAF("karaoke");
		}

		// Stereo mode
		if (mset.stereo_mode != 0) {
			switch (mset.stereo_mode) {
				case MediaSettings::Left: proc->addAF("stereo-mode", "left"); break;
				case MediaSettings::Right: proc->addAF("stereo-mode", "right"); break;
				case MediaSettings::Mono: proc->addAF("stereo-mode", "mono"); break;
				case MediaSettings::Reverse: proc->addAF("stereo-mode", "reverse"); break;
			}
		}

		if (mset.extrastereo_filter) {
			proc->addAF("extrastereo");
		}

		if (mset.volnorm_filter) {
			QString options = proc->isMPlayer() ? pref->filters->item("volnorm").options() : pref->filters->item("acompressor").options();
			proc->addAF("volnorm", options);
		}

		#ifdef MPV_SUPPORT
		if (mset.earwax_filter) {
			proc->addAF("earwax");
		}
		#endif

		if (proc->isMPlayer()) {
			bool use_scaletempo = (pref->use_scaletempo == Preferences::Enabled);
			if (pref->use_scaletempo == Preferences::Detect) {
				use_scaletempo = (MplayerVersion::isMplayerAtLeast(24924));
			}
			if (use_scaletempo) {
				proc->addAF("scaletempo");
			}
		} else {
			// MPV
			proc->setOption("scaletempo", pref->use_scaletempo != Preferences::Disabled);
		}

		// Audio equalizer
		if (pref->use_audio_equalizer) {
			AudioEqualizerList l = pref->global_audio_equalizer ? pref->audio_equalizer : mset.audio_equalizer;
			proc->addAF("equalizer", l);
		}

		// Additional audio filters, supplied by user
		// File
		if ( !pref->mplayer_additional_audio_filters.isEmpty() ) {
			proc->setOption("af-add", pref->mplayer_additional_audio_filters);
		}
		// Global
		if ( !mset.mplayer_additional_audio_filters.isEmpty() ) {
			proc->setOption("af-add", mset.mplayer_additional_audio_filters);
		}
	} else {
		// Don't use audio filters if using the S/PDIF output
			qDebug("Core::startMplayer: audio filters are disabled when using the S/PDIF output!");
	}

	if (pref->use_soft_vol) {
		proc->setOption("softvol", QString::number(pref->softvol_max));
	} else {
		proc->setOption("softvol", "off");
	}

#ifdef MPV_SUPPORT
	if (mdat.type == TYPE_STREAM) {
		if (pref->streaming_type == Preferences::StreamingAuto) {
			bool is_youtube = false;
			#ifdef YOUTUBE_SUPPORT
			if (PREF_YT_ENABLED) is_youtube = (file == yt->latestPreferredUrl());
			#endif
			qDebug() << "Core::startMplayer: is_youtube:" << is_youtube;
			bool enable_sites = !is_youtube;

			if (!is_youtube) {
				// Check if the URL contains a media extension
				QString extension = Extensions::extensionFromUrl(file);
				qDebug() << "Core::startMplayer: URL extension:" << extension;
				Extensions e;
				if (e.allPlayable().contains(extension)) {
					qDebug() << "Core::startMplayer: extension found in URL";
					enable_sites = false;
				}
			}
			qDebug() << "Core::startMplayer: enable_sites:" << enable_sites;
			proc->setOption("enable_streaming_sites_support", enable_sites);
			if (enable_sites) proc->setOption("ytdl_quality", pref->ytdl_quality);
		} else {
			bool enable_sites = pref->streaming_type == Preferences::StreamingYTDL;
			proc->setOption("enable_streaming_sites_support", enable_sites);
			if (enable_sites) proc->setOption("ytdl_quality", pref->ytdl_quality);
		}
	}
#endif

#if defined(TV_SUPPORT) && !defined(Q_OS_WIN)
	if (proc->isMPV() && file.startsWith("dvb:")) {
		QString channels_file = TVList::findChannelsFile();
		qDebug("Core::startMplayer: channels_file: %s", channels_file.toUtf8().constData());
		if (!channels_file.isEmpty()) proc->setChannelsFile(channels_file);
	}
#endif

#ifdef CAPTURE_STREAM
	// Set the capture directory
	proc->setCaptureDirectory(pref->capture_directory);
#endif

	// Load edl file
	if (pref->use_edl_files) {
		QString edl_f;
		QFileInfo f(file);
		QString basename = f.path() + "/" + f.completeBaseName();

		//qDebug("Core::startMplayer: file basename: '%s'", basename.toUtf8().data());

		if (QFile::exists(basename+".edl")) 
			edl_f = basename+".edl";
		else
		if (QFile::exists(basename+".EDL")) 
			edl_f = basename+".EDL";

		qDebug("Core::startMplayer: edl file: '%s'", edl_f.toUtf8().data());
		if (!edl_f.isEmpty()) {
			proc->setOption("edl", edl_f);
		}
	}

	// Process extra params
	qDebug() << "Core::startMplayer: extra_params:" << mdat.extra_params;
	foreach(QString par, mdat.extra_params) {
		QRegExp rx_ref("^http-referrer=(.*)", Qt::CaseInsensitive);
		QRegExp rx_agent("^http-user-agent=(.*)", Qt::CaseInsensitive);

		if (rx_ref.indexIn(par) > -1) {
			QString referrer = rx_ref.cap(1);
			qDebug() << "Core::startMplayer: referrer:" << referrer;
			proc->setOption("referrer", referrer);
		}
		else
		if (rx_agent.indexIn(par) > -1) {
			QString user_agent = rx_agent.cap(1);
			qDebug() << "Core::startMplayer: user_agent:" << user_agent;
			proc->setOption("user-agent", user_agent);
		}
	}

	// Additional options supplied by the user
	// File
	if (!mset.mplayer_additional_options.isEmpty()) {
		QStringList args = MyProcess::splitArguments(mset.mplayer_additional_options);
		for (int n = 0; n < args.count(); n++) {
			QString arg = args[n].simplified();
			if (!arg.isEmpty()) {
				proc->addUserOption(arg);
			}
		}
	}

	// Global
	if (!pref->mplayer_additional_options.isEmpty()) {
		QString additional_options = pref->mplayer_additional_options;
		#ifdef MPLAYER2_SUPPORT
		// mplayer2 doesn't support -fontconfig and -nofontconfig
		if (pref->mplayer_is_mplayer2) {
			additional_options.replace("-fontconfig", "");
			additional_options.replace("-nofontconfig", "");
		}
		#endif
		QStringList args = MyProcess::splitArguments(additional_options);
		for (int n = 0; n < args.count(); n++) {
			QString arg = args[n].simplified();
			if (!arg.isEmpty()) {
				qDebug("arg %d: %s", n, arg.toUtf8().constData());
				proc->addUserOption(arg);
			}
		}

	}

	// Last checks for the file

	// Open https URLs with ffmpeg
	#if 0
	// It doesn't seem necessary anymore
	if (proc->isMPlayer() && file.startsWith("https")) {
		file = "ffmpeg://" + file;
	}
	#endif

#if DVDNAV_SUPPORT
	if (proc->isMPV() && file.startsWith("dvdnav:")) {
		// Hack to open the DVD menu with MPV
		file = "dvd://menu";
	}
#endif

#ifdef Q_OS_WIN
	if (pref->use_short_pathnames) {
		file = Helper::shortPathName(file);
	}
#endif

#ifdef MPLAYER_SUPPORT
	if (proc->isMPlayer()) {
		proc->setMedia(file, pref->use_playlist_option ? url_is_playlist : false);
	} else
#endif
	{
		proc->setMedia(file, false); // Don't use playlist with mpv
	}

	// It seems the loop option must be after the filename
	if (mset.loop) {
		proc->setOption("loop", "0");
	}

	emit aboutToStartPlaying();

	QString commandline = proc->arguments().join(" ");
	qDebug("Core::startMplayer: command: '%s'", commandline.toUtf8().data());

	//Log command
	QString line_for_log = commandline + "\n";
	emit logLineAvailable(line_for_log);

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	if ((pref->use_proxy) && (pref->proxy_type == QNetworkProxy::HttpProxy) && (!pref->proxy_host.isEmpty())) {
		QString proxy = QString("http://%1:%2@%3:%4").arg(pref->proxy_username).arg(pref->proxy_password).arg(pref->proxy_host).arg(pref->proxy_port);
		env.insert("http_proxy", proxy);
	}
	//qDebug("Core::startMplayer: env: %s", env.toStringList().join(",").toUtf8().constData());
	#ifdef FONTS_HACK
	if (!pref->use_windowsfontdir) {
		env.insert("FONTCONFIG_FILE", Paths::configPath() + "/fonts.conf");
	}
	#endif
	proc->setProcessEnvironment(env);

	if ( !proc->start() ) {
	    // error handling
		qWarning("Core::startMplayer: mplayer process didn't start");
	}

}

void Core::stopMplayer() {
	qDebug("Core::stopMplayer");

	if (!proc->isRunning()) {
		qWarning("Core::stopMplayer: mplayer is not running!");
		return;
	}

#ifdef Q_OS_OS2
	QEventLoop eventLoop;

	connect(proc, SIGNAL(processExited()), &eventLoop, SLOT(quit()));

	proc->quit();

	QTimer::singleShot(5000, &eventLoop, SLOT(quit()));
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

	if (proc->isRunning()) {
		qWarning("Core::stopMplayer: process didn't finish. Killing it...");
		proc->kill();
	}
#else
	proc->quit();

	qDebug("Core::stopMplayer: Waiting mplayer to finish...");
	if (!proc->waitForFinished(pref->time_to_kill_mplayer)) {
		qWarning("Core::stopMplayer: process didn't finish. Killing it...");
		proc->kill();
	}
#endif

	qDebug("Core::stopMplayer: Finished. (I hope)");
}


void Core::goToSec( double sec ) {
	qDebug("Core::goToSec: %f", sec);

	if (sec < 0) sec = 0;
	if (sec > mdat.duration ) sec = mdat.duration - 20;
	seek_cmd(sec, 2);
}


void Core::seek(int secs) {
	qDebug("Core::seek: %d", secs);
	if ( (proc->isRunning()) && (secs!=0) ) {
		seek_cmd(secs, 0);
	}
}

void Core::seek_cmd(double secs, int mode) {
	proc->seek(secs, mode, pref->precise_seeking);
}

void Core::sforward() {
	qDebug("Core::sforward");
	seek( pref->seeking1 ); // +10s
}

void Core::srewind() {
	qDebug("Core::srewind");
	seek( -pref->seeking1 ); // -10s
}


void Core::forward() {
	qDebug("Core::forward");
	seek( pref->seeking2 ); // +1m
}


void Core::rewind() {
	qDebug("Core::rewind");
	seek( -pref->seeking2 ); // -1m
}


void Core::fastforward() {
	qDebug("Core::fastforward");
	seek( pref->seeking3 ); // +10m
}


void Core::fastrewind() {
	qDebug("Core::fastrewind");
	seek( -pref->seeking3 ); // -10m
}

void Core::forward(int secs) {
	qDebug("Core::forward: %d", secs);
	seek(secs);
}

void Core::rewind(int secs) {
	qDebug("Core::rewind: %d", secs);
	seek(-secs);
}

#ifdef MPV_SUPPORT
void Core::seekToNextSub() {
	qDebug("Core::seekToNextSub");
	proc->seekSub(1);
}

void Core::seekToPrevSub() {
	qDebug("Core::seekToPrevSub");
	proc->seekSub(-1);
}
#endif

void Core::wheelUp() {
	qDebug("Core::wheelUp");
	switch (pref->wheel_function) {
		case Preferences::Volume : incVolume(); break;
		case Preferences::Zoom : incZoom(); break;
		case Preferences::Seeking : pref->wheel_function_seeking_reverse ? rewind( pref->seeking4 ) : forward( pref->seeking4 ); break;
		case Preferences::ChangeSpeed : incSpeed10(); break;
		default : {} // do nothing
	}
}

void Core::wheelDown() {
	qDebug("Core::wheelDown");
	switch (pref->wheel_function) {
		case Preferences::Volume : decVolume(); break;
		case Preferences::Zoom : decZoom(); break;
		case Preferences::Seeking : pref->wheel_function_seeking_reverse ? forward( pref->seeking4 ) : rewind( pref->seeking4 ); break;
		case Preferences::ChangeSpeed : decSpeed10(); break;
		default : {} // do nothing
	}
}

void Core::setAMarker() {
	setAMarker((int)mset.current_sec);
}

void Core::setAMarker(int sec) {
	qDebug("Core::setAMarker: %d", sec);

	mset.A_marker = sec;
	displayMessage( tr("\"A\" marker set to %1").arg(Helper::formatTime(sec)) );

	if (proc->isMPV()) {
		if (mset.loop) proc->setAMarker(sec);
	} else {
		// MPlayer
		if (mset.B_marker > mset.A_marker) {
			if (proc->isRunning()) restartPlay();
		}
	}

	emit ABMarkersChanged(mset.A_marker, mset.B_marker);
}

void Core::setBMarker() {
	setBMarker((int)mset.current_sec);
}

void Core::setBMarker(int sec) {
	qDebug("Core::setBMarker: %d", sec);

	mset.B_marker = sec;
	displayMessage( tr("\"B\" marker set to %1").arg(Helper::formatTime(sec)) );

	if (proc->isMPV()) {
		if (mset.loop) proc->setBMarker(sec);
	} else {
		// MPlayer
		if ((mset.A_marker > -1) && (mset.A_marker < mset.B_marker)) {
			if (proc->isRunning()) restartPlay();
		}
	}

	emit ABMarkersChanged(mset.A_marker, mset.B_marker);
}

void Core::clearABMarkers() {
	qDebug("Core::clearABMarkers");

	if ((mset.A_marker != -1) || (mset.B_marker != -1)) {
		mset.A_marker = -1;
		mset.B_marker = -1;
		displayMessage( tr("A-B markers cleared") );
		if (proc->isMPV()) {
			proc->clearABMarkers();
		} else {
			// MPlayer
			if (proc->isRunning()) restartPlay();
		}
	}

	emit ABMarkersChanged(mset.A_marker, mset.B_marker);
}

void Core::toggleRepeat() {
	qDebug("Core::toggleRepeat");
	toggleRepeat( !mset.loop );
}

void Core::toggleRepeat(bool b) {
	qDebug("Core::toggleRepeat: %d", b);
	if ( mset.loop != b ) {
		mset.loop = b;
		if (MplayerVersion::isMplayerAtLeast(23747)) {
			// Use slave command
			int v = -1; // no loop
			if (mset.loop) v = 0; // infinite loop
			if (proc->isMPV()) {
				// Enable A-B markers
				proc->clearABMarkers();
				if (b) {
					if (mset.A_marker > -1) proc->setAMarker(mset.A_marker);
					if (mset.B_marker > -1) proc->setBMarker(mset.B_marker);
				}
			}
			proc->setLoop(v);
		} else {
			// Restart mplayer
			if (proc->isRunning()) restartPlay();
		}
	}
}


// Audio filters
void Core::toggleKaraoke() {
	toggleKaraoke( !mset.karaoke_filter );
}

void Core::toggleKaraoke(bool b) {
	qDebug("Core::toggleKaraoke: %d", b);
	if (b != mset.karaoke_filter) {
		mset.karaoke_filter = b;
		if (MplayerVersion::isMplayerAtLeast(31030)) {
			// Change filter without restarting
			proc->enableKaraoke(b);
		} else {
			restartPlay();
		}
	}
}

void Core::toggleExtrastereo() {
	toggleExtrastereo( !mset.extrastereo_filter );
}

void Core::toggleExtrastereo(bool b) {
	qDebug("Core::toggleExtrastereo: %d", b);
	if (b != mset.extrastereo_filter) {
		mset.extrastereo_filter = b;
		if (MplayerVersion::isMplayerAtLeast(31030)) {
			// Change filter without restarting
			proc->enableExtrastereo(b);
		} else {
			restartPlay();
		}
	}
}

void Core::toggleVolnorm() {
	toggleVolnorm( !mset.volnorm_filter );
}

void Core::toggleVolnorm(bool b) {
	qDebug("Core::toggleVolnorm: %d", b);
	if (b != mset.volnorm_filter) {
		mset.volnorm_filter = b;
		if (MplayerVersion::isMplayerAtLeast(31030)) {
			// Change filter without restarting
			QString options = proc->isMPlayer() ? pref->filters->item("volnorm").options() : pref->filters->item("acompressor").options();
			proc->enableVolnorm(b, options);
		} else {
			restartPlay();
		}
	}
}

#ifdef MPV_SUPPORT
void Core::toggleEarwax() {
	toggleEarwax( !mset.earwax_filter );
}

void Core::toggleEarwax(bool b) {
	qDebug("Core::toggleEarwax: %d", b);
	if (b != mset.earwax_filter) {
		mset.earwax_filter = b;
		proc->enableEarwax(b);
	}
}
#endif

void Core::setAudioChannels(int channels) {
	qDebug("Core::setAudioChannels: %d", channels);
	if (channels != mset.audio_use_channels ) {
		mset.audio_use_channels = channels;
		restartPlay();
	}
}

void Core::setStereoMode(int mode) {
	qDebug("Core::setStereoMode: %d", mode);
	if (mode != mset.stereo_mode ) {
		if (proc->isMPlayer()) {
			mset.stereo_mode = mode;
			restartPlay();
		} else {
			// MPV
			// Remove previous filter
			switch (mset.stereo_mode) {
				case MediaSettings::Left: proc->changeAF("stereo-mode", false, "left"); break;
				case MediaSettings::Right: proc->changeAF("stereo-mode", false, "right"); break;
				case MediaSettings::Mono: proc->changeAF("stereo-mode", false, "mono"); break;
				case MediaSettings::Reverse: proc->changeAF("stereo-mode", false, "reverse"); break;
			}
			// New filter
			mset.stereo_mode = mode;
			switch (mset.stereo_mode) {
				case MediaSettings::Left: proc->changeAF("stereo-mode", true, "left"); break;
				case MediaSettings::Right: proc->changeAF("stereo-mode", true, "right"); break;
				case MediaSettings::Mono: proc->changeAF("stereo-mode", true, "mono"); break;
				case MediaSettings::Reverse: proc->changeAF("stereo-mode", true, "reverse"); break;
			}
		}
	}
}


// Video filters

#define CHANGE_VF(Filter, Enable, Option) \
	if (proc->isMPV()) { \
		proc->changeVF(Filter, Enable, Option); \
	} else { \
		restartPlay(); \
	}

void Core::toggleFlip() {
	qDebug("Core::toggleFlip");
	toggleFlip( !mset.flip );
}

void Core::toggleFlip(bool b) {
	qDebug("Core::toggleFlip: %d", b);
	if (mset.flip != b) {
		mset.flip = b;
		CHANGE_VF("flip", b, QVariant());
	}
}

void Core::toggleMirror() {
	qDebug("Core::toggleMirror");
	toggleMirror( !mset.mirror );
}

void Core::toggleMirror(bool b) {
	qDebug("Core::toggleMirror: %d", b);
	if (mset.mirror != b) {
		mset.mirror = b;
		CHANGE_VF("mirror", b, QVariant());
	}
}

void Core::toggleAutophase() {
	toggleAutophase( !mset.phase_filter );
}

void Core::toggleAutophase( bool b ) {
	qDebug("Core::toggleAutophase: %d", b);
	if ( b != mset.phase_filter) {
		mset.phase_filter = b;
		CHANGE_VF("phase", b, "A");
	}
}

void Core::toggleDeblock() {
	toggleDeblock( !mset.deblock_filter );
}

void Core::toggleDeblock(bool b) {
	qDebug("Core::toggleDeblock: %d", b);
	if ( b != mset.deblock_filter ) {
		mset.deblock_filter = b;
		CHANGE_VF("deblock", b, pref->filters->item("deblock").options());
	}
}

void Core::toggleDering() {
	toggleDering( !mset.dering_filter );
}

void Core::toggleDering(bool b) {
	qDebug("Core::toggleDering: %d", b);
	if ( b != mset.dering_filter) {
		mset.dering_filter = b;
		CHANGE_VF("dering", b, QVariant());
	}
}

void Core::toggleGradfun() {
	toggleGradfun( !mset.gradfun_filter );
}

void Core::toggleGradfun(bool b) {
	qDebug("Core::toggleGradfun: %d", b);
	if ( b != mset.gradfun_filter) {
		mset.gradfun_filter = b;
		CHANGE_VF("gradfun", b, pref->filters->item("gradfun").options());
	}
}

void Core::toggleNoise() {
	toggleNoise( !mset.noise_filter );
}

void Core::toggleNoise(bool b) {
	qDebug("Core::toggleNoise: %d", b);
	if ( b != mset.noise_filter ) {
		mset.noise_filter = b;
		CHANGE_VF("noise", b, QVariant());
	}
}

void Core::togglePostprocessing() {
	togglePostprocessing( !mset.postprocessing_filter );
}

void Core::togglePostprocessing(bool b) {
	qDebug("Core::togglePostprocessing: %d", b);
	if ( b != mset.postprocessing_filter ) {
		mset.postprocessing_filter = b;
		CHANGE_VF("postprocessing", b, QVariant());
	}
}

void Core::changeDenoise(int id) {
	qDebug( "Core::changeDenoise: %d", id );
	if (id != mset.current_denoiser) {
		if (proc->isMPlayer()) {
			mset.current_denoiser = id;
			restartPlay();
		} else {
			// MPV
			QString dsoft = pref->filters->item("denoise_soft").options();
			QString dnormal = pref->filters->item("denoise_normal").options();
			// Remove previous filter
			switch (mset.current_denoiser) {
				case MediaSettings::DenoiseSoft: proc->changeVF("hqdn3d", false, dsoft); break;
				case MediaSettings::DenoiseNormal: proc->changeVF("hqdn3d", false, dnormal); break;
			}
			// New filter
			mset.current_denoiser = id;
			switch (mset.current_denoiser) {
				case MediaSettings::DenoiseSoft: proc->changeVF("hqdn3d", true, dsoft); break;
				case MediaSettings::DenoiseNormal: proc->changeVF("hqdn3d", true, dnormal); break;
			}
		}
	}
}

void Core::changeUnsharp(int id) {
	qDebug( "Core::changeUnsharp: %d", id );
	if (id != mset.current_unsharp) {
		if (proc->isMPlayer()) {
			mset.current_unsharp = id;
			restartPlay();
		} else {
			// MPV
			// Remove previous filter
			switch (mset.current_unsharp) {
				// Current is blur
				case 1: proc->changeVF("blur", false); break;
				// Current if sharpen
				case 2: proc->changeVF("sharpen", false); break;
			}
			// New filter
			mset.current_unsharp = id;
			switch (mset.current_unsharp) {
				case 1: proc->changeVF("blur", true); break;
				case 2: proc->changeVF("sharpen", true); break;
			}
		}
	}
}

void Core::changeUpscale(bool b) {
	qDebug( "Core::changeUpscale: %d", b );
	if (mset.upscaling_filter != b) {
		mset.upscaling_filter = b;
		int width = DesktopInfo::desktop_size(mplayerwindow).width();
		CHANGE_VF("scale", b, QString::number(width) + ":-2");
	}
}

void Core::changeStereo3d(const QString & in, const QString & out) {
	qDebug("Core::changeStereo3d: in: %s out: %s", in.toUtf8().constData(), out.toUtf8().constData());

	if ((mset.stereo3d_in != in) || (mset.stereo3d_out != out)) {
		if (proc->isMPlayer()) {
			mset.stereo3d_in = in;
			mset.stereo3d_out = out;
			restartPlay();
		} else {
			// Remove previous filter
			if (mset.stereo3d_in != "none" && !mset.stereo3d_out.isEmpty()) {
				proc->changeStereo3DFilter(false, mset.stereo3d_in, mset.stereo3d_out);
			}

			// New filter
			mset.stereo3d_in = in;
			mset.stereo3d_out = out;
			if (mset.stereo3d_in != "none" && !mset.stereo3d_out.isEmpty()) {
				proc->changeStereo3DFilter(true, mset.stereo3d_in, mset.stereo3d_out);
			}
		}
	}
}

void Core::setBrightness(int value) {
	qDebug("Core::setBrightness: %d", value);

	if (value > 100) value = 100;
	if (value < -100) value = -100;

	if (value != mset.brightness) {
		proc->setPausingPrefix(pausing_prefix());
		proc->setBrightness(value);
		mset.brightness = value;
		displayMessage( tr("Brightness: %1").arg(value) );
		emit videoEqualizerNeedsUpdate();
	}
}


void Core::setContrast(int value) {
	qDebug("Core::setContrast: %d", value);

	if (value > 100) value = 100;
	if (value < -100) value = -100;

	if (value != mset.contrast) {
		proc->setPausingPrefix(pausing_prefix());
		proc->setContrast(value);
		mset.contrast = value;
		displayMessage( tr("Contrast: %1").arg(value) );
		emit videoEqualizerNeedsUpdate();
	}
}

void Core::setGamma(int value) {
	qDebug("Core::setGamma: %d", value);

	if (value > 100) value = 100;
	if (value < -100) value = -100;

	if (value != mset.gamma) {
		proc->setPausingPrefix(pausing_prefix());
		proc->setGamma(value);
		mset.gamma= value;
		displayMessage( tr("Gamma: %1").arg(value) );
		emit videoEqualizerNeedsUpdate();
	}
}

void Core::setHue(int value) {
	qDebug("Core::setHue: %d", value);

	if (value > 100) value = 100;
	if (value < -100) value = -100;

	if (value != mset.hue) {
		proc->setPausingPrefix(pausing_prefix());
		proc->setHue(value);
		mset.hue = value;
		displayMessage( tr("Hue: %1").arg(value) );
		emit videoEqualizerNeedsUpdate();
	}
}

void Core::setSaturation(int value) {
	qDebug("Core::setSaturation: %d", value);

	if (value > 100) value = 100;
	if (value < -100) value = -100;

	if (value != mset.saturation) {
		proc->setPausingPrefix(pausing_prefix());
		proc->setSaturation(value);
		mset.saturation = value;
		displayMessage( tr("Saturation: %1").arg(value) );
		emit videoEqualizerNeedsUpdate();
	}
}

void Core::incBrightness() {
	setBrightness(mset.brightness + pref->min_step);
}

void Core::decBrightness() {
	setBrightness(mset.brightness - pref->min_step);
}

void Core::incContrast() {
	setContrast(mset.contrast + pref->min_step);
}

void Core::decContrast() {
	setContrast(mset.contrast - pref->min_step);
}

void Core::incGamma() {
	setGamma(mset.gamma + pref->min_step);
}

void Core::decGamma() {
	setGamma(mset.gamma - pref->min_step);
}

void Core::incHue() {
	setHue(mset.hue + pref->min_step);
}

void Core::decHue() {
	setHue(mset.hue - pref->min_step);
}

void Core::incSaturation() {
	setSaturation(mset.saturation + pref->min_step);
}

void Core::decSaturation() {
	setSaturation(mset.saturation - pref->min_step);
}

void Core::setSpeed( double value ) {
	qDebug("Core::setSpeed: %f", value);

	if (value < 0.10) value = 0.10;
	if (value > 100) value = 100;

	mset.speed = value;
	proc->setSpeed(value);

	displayMessage( tr("Speed: %1").arg(value) );
}

void Core::incSpeed10() {
	qDebug("Core::incSpeed10");
	setSpeed( (double) mset.speed + 0.1 );
}

void Core::decSpeed10() {
	qDebug("Core::decSpeed10");
	setSpeed( (double) mset.speed - 0.1 );
}

void Core::incSpeed4() {
	qDebug("Core::incSpeed4");
	setSpeed( (double) mset.speed + 0.04 );
}

void Core::decSpeed4() {
	qDebug("Core::decSpeed4");
	setSpeed( (double) mset.speed - 0.04 );
}

void Core::incSpeed1() {
	qDebug("Core::incSpeed1");
	setSpeed( (double) mset.speed + 0.01 );
}

void Core::decSpeed1() {
	qDebug("Core::decSpeed1");
	setSpeed( (double) mset.speed - 0.01 );
}

void Core::doubleSpeed() {
	qDebug("Core::doubleSpeed");
	setSpeed( (double) mset.speed * 2 );
}

void Core::halveSpeed() {
	qDebug("Core::halveSpeed");
	setSpeed( (double) mset.speed / 2 );
}

void Core::normalSpeed() {
	setSpeed(1);
}

int Core::adjustVolume(int v, int max_vol) {
	//qDebug() << "Core::adjustVolume: v:" << v << "max_vol:" << max_vol;
	if (max_vol < 100) max_vol = 100;
	int vol = v * max_vol / 100;
	return vol;
}

void Core::setVolume(int volume, bool force) {
	qDebug("Core::setVolume: %d", volume);

	int current_volume = (pref->global_volume ? pref->volume : mset.volume);

	if ((volume == current_volume) && (!force)) return;

	current_volume = volume;
	if (current_volume > 100) current_volume = 100;
	if (current_volume < 0) current_volume = 0;

	if (proc->isMPV()) {
		// MPV
		int vol = adjustVolume(current_volume, pref->use_soft_vol ? pref->softvol_max : 100);
		proc->setVolume(vol);
	} else {
		// MPlayer
		if (state() == Paused) {
			// Change volume later, after quiting pause
			change_volume_after_unpause = true;
		} else {
			proc->setVolume(current_volume);
		}
	}

	if (pref->global_volume) {
		pref->volume = current_volume;
		pref->mute = false;
	} else {
		mset.volume = current_volume;
		mset.mute = false;
	}

	updateWidgets();

	displayMessage( tr("Volume: %1").arg(current_volume) );
	emit volumeChanged( current_volume );
}

void Core::switchMute() {
	qDebug("Core::switchMute");

	mset.mute = !mset.mute;
	mute(mset.mute);
}

void Core::mute(bool b) {
	qDebug("Core::mute");

	proc->setPausingPrefix(pausing_prefix());
	proc->mute(b);

	if (pref->global_volume) {
		pref->mute = b;
	} else {
		mset.mute = b;
	}

	updateWidgets();
}

void Core::incVolume(int step) {
	//qDebug("Core::incVolume");
	int vol = pref->global_volume ? pref->volume : mset.volume;
	setVolume(vol + step);
}

void Core::decVolume(int step) {
	//qDebug("Core::incVolume");
	int vol = pref->global_volume ? pref->volume : mset.volume;
	setVolume(vol - step);
}

void Core::incVolume() {
	qDebug("Core::incVolume");
	incVolume(pref->min_step);
}

void Core::decVolume() {
	qDebug("Core::incVolume");
	decVolume(pref->min_step);
}

void Core::setSubDelay(int delay) {
	qDebug("Core::setSubDelay: %d", delay);
	mset.sub_delay = delay;
	proc->setPausingPrefix(pausing_prefix());
	proc->setSubDelay((double) mset.sub_delay/1000);
	displayMessage( tr("Subtitle delay: %1 ms").arg(delay) );
}

void Core::incSubDelay() {
	qDebug("Core::incSubDelay");
	setSubDelay(mset.sub_delay + 100);
}

void Core::decSubDelay() {
	qDebug("Core::decSubDelay");
	setSubDelay(mset.sub_delay - 100);
}

void Core::setAudioDelay(int delay) {
	qDebug("Core::setAudioDelay: %d", delay);
	mset.audio_delay = delay;
	proc->setPausingPrefix(pausing_prefix());
	proc->setAudioDelay((double) mset.audio_delay/1000);
	displayMessage( tr("Audio delay: %1 ms").arg(delay) );
}

void Core::incAudioDelay() {
	qDebug("Core::incAudioDelay");
	setAudioDelay(mset.audio_delay + 100);
}

void Core::decAudioDelay() {
	qDebug("Core::decAudioDelay");
	setAudioDelay(mset.audio_delay - 100);
}

void Core::incSubPos() {
	qDebug("Core::incSubPos");

	mset.sub_pos++;
	if (mset.sub_pos > 100) mset.sub_pos = 100;
	proc->setSubPos(mset.sub_pos);
}

void Core::decSubPos() {
	qDebug("Core::decSubPos");

	mset.sub_pos--;
	if (mset.sub_pos < 0) mset.sub_pos = 0;
	proc->setSubPos(mset.sub_pos);
}

bool Core::subscale_need_restart() {
	bool need_restart = false;

	need_restart = (pref->change_sub_scale_should_restart == Preferences::Enabled);
	if (pref->change_sub_scale_should_restart == Preferences::Detect) {
		if (pref->use_ass_subtitles) 
			need_restart = (!MplayerVersion::isMplayerAtLeast(25843));
		else
			need_restart = (!MplayerVersion::isMplayerAtLeast(23745));
	}
	return need_restart;
}

void Core::changeSubScale(double value) {
	qDebug("Core::changeSubScale: %f", value);

	bool need_restart = subscale_need_restart();

	if (value < 0) value = 0;

	if (pref->use_ass_subtitles) {
		if (value != mset.sub_scale_ass) {
			mset.sub_scale_ass = value;
			if (need_restart) {
				restartPlay();
			} else {
				proc->setSubScale(mset.sub_scale_ass);
			}
			displayMessage( tr("Font scale: %1").arg(mset.sub_scale_ass) );
		}
	} else {
		// No ass
		if (value != mset.sub_scale) {
			mset.sub_scale = value;
			if (need_restart) {
				restartPlay();
			} else {
				proc->setSubScale(mset.sub_scale);
			}
			displayMessage( tr("Font scale: %1").arg(mset.sub_scale) );
		}
	}
}

void Core::incSubScale() {
	double step = 0.20;

	if (pref->use_ass_subtitles) {
		changeSubScale( mset.sub_scale_ass + step );
	} else {
		if (subscale_need_restart()) step = 1;
		changeSubScale( mset.sub_scale + step );
	}
}

void Core::decSubScale() {
	double step = 0.20;

	if (pref->use_ass_subtitles) {
		changeSubScale( mset.sub_scale_ass - step );
	} else {
		if (subscale_need_restart()) step = 1;
		changeSubScale( mset.sub_scale - step );
	}
}

void Core::changeOSDScale(double value) {
	qDebug("Core::changeOSDScale: %f", value);

	if (value < 0) value = 0;

	if (proc->isMPlayer()) {
		if (value != pref->subfont_osd_scale) {
			pref->subfont_osd_scale = value;
			restartPlay();
		}
	} else {
		if (value != pref->osd_scale) {
			pref->osd_scale = value;
			proc->setOSDScale(pref->osd_scale);
		}
	}
}

void Core::incOSDScale() {
	if (proc->isMPlayer()) {
		changeOSDScale(pref->subfont_osd_scale + 1);
	} else {
		changeOSDScale(pref->osd_scale + 0.20);
	}
}

void Core::decOSDScale() {
	if (proc->isMPlayer()) {
		changeOSDScale(pref->subfont_osd_scale - 1);
	} else {
		changeOSDScale(pref->osd_scale - 0.20);
	}
}

#ifdef MPV_SUPPORT
void Core::setOSDFractions(bool active) {
	qDebug("Core::setOSDFractions: %d", active);
	pref->osd_fractions = active;
	proc->setOSDFractions(active);
}
#endif

void Core::incSubStep() {
	qDebug("Core::incSubStep");
	proc->setSubStep(+1);
}

void Core::decSubStep() {
	qDebug("Core::decSubStep");
	proc->setSubStep(-1);
}

void Core::changeSubVisibility(bool visible) {
	qDebug("Core::changeSubVisilibity: %d", visible);
	pref->sub_visibility = visible;
	proc->setSubtitlesVisibility(pref->sub_visibility);

	if (pref->sub_visibility) 
		displayMessage( tr("Subtitles on") );
	else
		displayMessage( tr("Subtitles off") );
}

void Core::changeExternalSubFPS(int fps_id) {
	qDebug("Core::setExternalSubFPS: %d", fps_id);
	mset.external_subtitles_fps = fps_id;
	if (!mset.external_subtitles.isEmpty()) {
		restartPlay();
	}
}

// Audio equalizer functions
void Core::setAudioEqualizer(AudioEqualizerList values, bool restart) {
	qDebug("Core::setAudioEqualizer");

	if (pref->global_audio_equalizer) {
		pref->audio_equalizer = values;
	} else {
		mset.audio_equalizer = values;
	}

	if (!pref->use_audio_equalizer) {
		qDebug("Core::setAudioEqualizer: the audio equalizer is disabled. Ignoring.");
		return;
	}

	if (!restart) {
		proc->setAudioEqualizer(values);
	} else {
		restartPlay();
	}

	// Infinite recursion
	//emit audioEqualizerNeedsUpdate();
}

void Core::updateAudioEqualizer() {
	setAudioEqualizer(pref->global_audio_equalizer ? pref->audio_equalizer : mset.audio_equalizer);
}

void Core::setAudioEq(int eq, int value) {
	if (pref->global_audio_equalizer) {
		pref->audio_equalizer[eq] = value;
	} else {
		mset.audio_equalizer[eq] = value;
	}
	updateAudioEqualizer();
}

void Core::setAudioEq0(int value) {
	setAudioEq(0, value);
}

void Core::setAudioEq1(int value) {
	setAudioEq(1, value);
}

void Core::setAudioEq2(int value) {
	setAudioEq(2, value);
}

void Core::setAudioEq3(int value) {
	setAudioEq(3, value);
}

void Core::setAudioEq4(int value) {
	setAudioEq(4, value);
}

void Core::setAudioEq5(int value) {
	setAudioEq(5, value);
}

void Core::setAudioEq6(int value) {
	setAudioEq(6, value);
}

void Core::setAudioEq7(int value) {
	setAudioEq(7, value);
}

void Core::setAudioEq8(int value) {
	setAudioEq(8, value);
}

void Core::setAudioEq9(int value) {
	setAudioEq(9, value);
}



void Core::changeCurrentSec(double sec) {
	//qDebug() << "Core::changeCurrentSec:" << sec << "starting_time:" << mset.starting_time;

	mset.current_sec = sec;

#ifdef MSET_USE_STARTING_TIME
	if (mset.starting_time != -1) {
		mset.current_sec -= mset.starting_time;

		// handle PTS rollover at MPEG-TS
		if (mset.current_sec < 0 && mset.current_demuxer == "mpegts") {
			mset.current_sec += 8589934592.0 / 90000.0;	// 2^33 / 90 kHz
		}
	}
#endif

	if (state() != Playing) {
		setState(Playing);
		qDebug("Core::changeCurrentSec: mplayer reports that now it's playing");
		//emit mediaStartPlay();
		//emit stateChanged(state());
	}

	emit showTime(mset.current_sec);

	// Emit posChanged:
	static int last_second = 0;

	if (floor(sec)==last_second) return; // Update only once per second
	last_second = (int) floor(sec);

#ifdef SEEKBAR_RESOLUTION
	int value = 0;
	if ( (mdat.duration > 1) && (mset.current_sec > 1) &&
         (mdat.duration > mset.current_sec) )
	{
		value = ( (int) mset.current_sec * SEEKBAR_RESOLUTION) / (int) mdat.duration;
	}
	emit positionChanged(value);
#else
	int perc = 0;
	if ( (mdat.duration > 1) && (mset.current_sec > 1) &&
         (mdat.duration > mset.current_sec) )
	{
		perc = ( (int) mset.current_sec * 100) / (int) mdat.duration;
	}
	emit posChanged( perc );
#endif
}

void Core::gotStartingTime(double time) {
	qDebug("Core::gotStartingTime: %f", time);
	qDebug("Core::gotStartingTime: current_sec: %f", mset.current_sec);

#ifdef MSET_USE_STARTING_TIME
	if ((mset.starting_time == -1.0) && (mset.current_sec == 0)) {
		mset.starting_time = time;
		qDebug("Core::gotStartingTime: starting time set to %f", time);
	}
#endif
}

void Core::gotVideoBitrate(int b) {
	//qDebug("Core::gotVideoBitrate: %d", b);
	mdat.video_bitrate = b;
	emit bitrateChanged(mdat.video_bitrate, mdat.audio_bitrate);
}

void Core::gotAudioBitrate(int b) {
	//qDebug("Core::gotAudioBitrate: %d", b);
	mdat.audio_bitrate = b;
	emit bitrateChanged(mdat.video_bitrate, mdat.audio_bitrate);
}

void Core::changePause() {
	qDebug("Core::changePause");
	qDebug("Core::changePause: mplayer reports that it's paused");
	setState(Paused);
	//emit stateChanged(state());
}

void Core::changeDeinterlace(int ID) {
	qDebug("Core::changeDeinterlace: %d", ID);

	if (ID != mset.current_deinterlacer) {
		if (proc->isMPlayer()) {
			mset.current_deinterlacer = ID;
			restartPlay();
		} else {
			// MPV
			// Remove previous filter
			switch (mset.current_deinterlacer) {
				case MediaSettings::L5:			proc->changeVF("l5", false); break;
				case MediaSettings::Yadif:		proc->changeVF("yadif", false); break;
				case MediaSettings::LB:			proc->changeVF("lb", false); break;
				case MediaSettings::Yadif_1:	proc->changeVF("yadif", false, "1"); break;
				case MediaSettings::Kerndeint:	proc->changeVF("kerndeint", false, "5"); break;
			}
			mset.current_deinterlacer = ID;
			// New filter
			switch (mset.current_deinterlacer) {
				case MediaSettings::L5:			proc->changeVF("l5", true); break;
				case MediaSettings::Yadif:		proc->changeVF("yadif", true); break;
				case MediaSettings::LB:			proc->changeVF("lb", true); break;
				case MediaSettings::Yadif_1:	proc->changeVF("yadif", true, "1"); break;
				case MediaSettings::Kerndeint:	proc->changeVF("kerndeint", true, "5"); break;
			}
		}
	}
}


int Core::getSubRealID(int ID) {
	if (ID == MediaSettings::SubNone || ID == MediaSettings::NoneSelected) {
		ID = -1;
	}
	if (ID == -1) return ID;

	int real_id = -1;

	bool valid_item = ( (ID >= 0) && (ID < mdat.subs.numItems()) );

	if ( (mdat.subs.numItems() > 0) && (valid_item) ) {
		real_id = mdat.subs.itemAt(ID).ID();
	}

	return real_id;
}

void Core::changeSubtitle(int ID) {
	qDebug("Core::changeSubtitle: ID: %d", ID);

	mset.current_sub_id = ID;
	if (ID == MediaSettings::SubNone) {
		ID = -1;
	}

	if (ID == MediaSettings::NoneSelected) {
		ID = -1;
		qDebug("Core::changeSubtitle: subtitle is NoneSelected, this shouldn't happen. ID set to -1.");
	}

	qDebug("Core::changeSubtitle: ID: %d", ID);

	int real_id = -1;
	if (ID == -1) {
		proc->disableSubtitles();
	} else {
		bool valid_item = ( (ID >= 0) && (ID < mdat.subs.numItems()) );
		if (!valid_item) qWarning("Core::changeSubtitle: ID: %d is not valid!", ID);
		if ( (mdat.subs.numItems() > 0) && (valid_item) ) {
			real_id = mdat.subs.itemAt(ID).ID();
			proc->setSubtitle(mdat.subs.itemAt(ID).type(), real_id);
		} else {
			qWarning("Core::changeSubtitle: subtitle list is empty!");
		}
	}

	updateWidgets();
}

void Core::nextSubtitle() {
	qDebug("Core::nextSubtitle");

	if ( (mset.current_sub_id == MediaSettings::SubNone) && 
         (mdat.subs.numItems() > 0) ) 
	{
		changeSubtitle(0);
	} 
	else {
		int item = mset.current_sub_id + 1;
		if (item >= mdat.subs.numItems()) {
			item = MediaSettings::SubNone;
		}
		changeSubtitle( item );
	}
}

#ifdef MPV_SUPPORT
void Core::changeSecondarySubtitle(int ID) {
	// MPV only
	qDebug("Core::changeSecondarySubtitle: ID: %d", ID);

	mset.current_secondary_sub_id = ID;
	if (ID == MediaSettings::SubNone) {
		ID = -1;
	}
	if (ID == MediaSettings::NoneSelected) {
		ID = -1;
	}

	if (ID == -1) {
		proc->disableSecondarySubtitles();
	} else {
		int real_id = -1;
		bool valid_item = ( (ID >= 0) && (ID < mdat.subs.numItems()) );
		if (!valid_item) qWarning("Core::changeSecondarySubtitle: ID: %d is not valid!", ID);
		if ( (mdat.subs.numItems() > 0) && (valid_item) ) {
			real_id = mdat.subs.itemAt(ID).ID();
			proc->setSecondarySubtitle(real_id);
		}
	}
}
#endif

void Core::changeAudio(int ID, bool allow_restart) {
	qDebug("Core::changeAudio: ID: %d, allow_restart: %d", ID, allow_restart);

	if (ID!=mset.current_audio_id) {
		mset.current_audio_id = ID;
		qDebug("changeAudio: ID: %d", ID);

		bool need_restart = false;
		#ifdef OBSOLETE_FAST_AUDIO_CHANGE
		if (allow_restart) {
			need_restart = (pref->fast_audio_change == Preferences::Disabled);
			if (pref->fast_audio_change == Preferences::Detect) {
				need_restart = (!MplayerVersion::isMplayerAtLeast(21441));
			}
		}
		#endif

		if (need_restart) {
			restartPlay();
		} else {
			proc->setAudio(ID);
			// Workaround for a mplayer problem in windows,
			// volume is too loud after changing audio.

			// Workaround too for a mplayer problem in linux,
			// the volume is reduced if using -softvol-max.

			if (proc->isMPlayer()) {
				if (pref->mplayer_additional_options.contains("-volume")) {
					qDebug("Core::changeAudio: don't set volume since -volume is used");
				} else {
					if (pref->global_volume) {
						setVolume( pref->volume, true);
						if (pref->mute) mute(true);
					} else {
						setVolume( mset.volume, true );
						if (mset.mute) mute(true); // if muted, mute again
					}
				}
			}
			updateWidgets();
		}
	}
}

void Core::nextAudio() {
	qDebug("Core::nextAudio");

	int item = mdat.audios.find( mset.current_audio_id );
	if (item == -1) {
		qWarning("Core::nextAudio: audio ID %d not found!", mset.current_audio_id);
	} else {
		qDebug( "Core::nextAudio: numItems: %d, item: %d", mdat.audios.numItems(), item);
		item++;
		if (item >= mdat.audios.numItems()) item=0;
		int ID = mdat.audios.itemAt(item).ID();
		qDebug( "Core::nextAudio: item: %d, ID: %d", item, ID);
		changeAudio( ID );
	}
}

void Core::changeVideo(int ID, bool allow_restart) {
	qDebug("Core::changeVideo: ID: %d, allow_restart: %d", ID, allow_restart);

	if (ID != mset.current_video_id) {
		mset.current_video_id = ID;
		qDebug("Core::changeVideo: ID set to: %d", ID);

		bool need_restart = false;
		if (allow_restart) {
			// afaik lavf doesn't require to restart, any other?
			need_restart = ((mdat.demuxer != "lavf") && (mdat.demuxer != "mpegts"));
		}

		if (need_restart) {
			restartPlay(); 
		} else {
			if (mdat.demuxer == "nsv") {
				// Workaround a problem with the nsv demuxer
				qWarning("Core::changeVideo: not changing the video with nsv to prevent mplayer go crazy");
			} else {
				proc->setVideo(ID);
			}
		}
	}
}

void Core::nextVideo() {
	qDebug("Core::nextVideo");

	int item = mdat.videos.find( mset.current_video_id );
	if (item == -1) {
		qWarning("Core::nextVideo: video ID %d not found!", mset.current_video_id);
	} else {
		qDebug( "Core::nextVideo: numItems: %d, item: %d", mdat.videos.numItems(), item);
		item++;
		if (item >= mdat.videos.numItems()) item=0;
		int ID = mdat.videos.itemAt(item).ID();
		qDebug( "Core::nextVideo: item: %d, ID: %d", item, ID);
		changeVideo( ID );
	}
}

#if PROGRAM_SWITCH
void Core::changeProgram(int ID) {
	qDebug("Core::changeProgram: %d", ID);

	if (ID != mset.current_program_id) {
		mset.current_program_id = ID;
		proc->setTSProgram(ID);

		/*
		mset.current_video_id = MediaSettings::NoneSelected;
		mset.current_audio_id = MediaSettings::NoneSelected;

		updateWidgets();
		*/
	}
}

void Core::nextProgram() {
	qDebug("Core::nextProgram");
	// Not implemented yet
}

#endif

void Core::changeTitle(int ID) {
	if (mdat.type == TYPE_VCD) {
		// VCD
		openVCD( ID );
	}
	else 
	if (mdat.type == TYPE_AUDIO_CD) {
		// AUDIO CD
		openAudioCD( ID );
	}
	else
	if (mdat.type == TYPE_DVD) {
		#if DVDNAV_SUPPORT
		if (mdat.filename.startsWith("dvdnav:")) {
			proc->setTitle(ID);
		} else {
		#endif
			DiscData disc_data = DiscName::split(mdat.filename);
			disc_data.title = ID;
			QString dvd_url = DiscName::join(disc_data);

			openDVD( DiscName::join(disc_data) );
		#if DVDNAV_SUPPORT
		}
		#endif
	}
#ifdef BLURAY_SUPPORT
	else
	if (mdat.type == TYPE_BLURAY) {
		//DiscName::test();

		DiscData disc_data = DiscName::split(mdat.filename);
		disc_data.title = ID;
		QString bluray_url = DiscName::join(disc_data);
		qDebug("Core::changeTitle: bluray_url: %s", bluray_url.toUtf8().constData());
		openBluRay(bluray_url);
	}
#endif
}

void Core::changeChapter(int ID) {
	qDebug("Core::changeChapter: ID: %d", ID);
	proc->setChapter(ID);
}

int Core::firstChapter() {
	if ( (MplayerVersion::isMplayerAtLeast(25391)) && 
         (!MplayerVersion::isMplayerAtLeast(29407)) ) 
		return 1;
	else
		return 0;
}

int Core::firstDVDTitle() {
	if (proc->isMPV()) {
		return 0;
	} else {
		return 1;
	}
}

int Core::firstBlurayTitle() {
	if (proc->isMPV()) {
		return 0;
	} else {
		return 1;
	}
}

void Core::prevChapter() {
	qDebug("Core::prevChapter");
	proc->previousChapter();
}

void Core::nextChapter() {
	qDebug("Core::nextChapter");
	proc->nextChapter();
}

void Core::changeAngle(int ID) {
	qDebug("Core::changeAngle: ID: %d", ID);

	if (ID != mset.current_angle_id) {
		mset.current_angle_id = ID;
		restartPlay();
	}
}

void Core::changeAspectRatio( int ID ) {
	qDebug("Core::changeAspectRatio: %d", ID);

	mset.aspect_ratio_id = ID;

	double asp = mset.aspectToNum( (MediaSettings::Aspect) ID);

	if (!pref->use_mplayer_window) {
		mplayerwindow->setAspect(asp);
	} else {
		// Using mplayer own window
		if (!mdat.novideo) {
			if (ID == MediaSettings::AspectAuto) {
				asp = mdat.video_aspect;
			}
			proc->setAspect(asp);
		}
	}

	QString asp_name = MediaSettings::aspectToString( (MediaSettings::Aspect) mset.aspect_ratio_id);
	displayMessage( tr("Aspect ratio: %1").arg(asp_name) );
}

void Core::nextAspectRatio() {
	// Ordered list
	QList<int> s;
	s << MediaSettings::AspectNone 
      << MediaSettings::AspectAuto
      << MediaSettings::Aspect11	// 1
      << MediaSettings::Aspect54	// 1.25
      << MediaSettings::Aspect43	// 1.33
      << MediaSettings::Aspect118	// 1.37
      << MediaSettings::Aspect1410	// 1.4
      << MediaSettings::Aspect32	// 1.5
      << MediaSettings::Aspect149	// 1.55
      << MediaSettings::Aspect1610	// 1.6
      << MediaSettings::Aspect169	// 1.77
      << MediaSettings::Aspect235;	// 2.35

	int i = s.indexOf( mset.aspect_ratio_id ) + 1;
	if (i >= s.count()) i = 0;

	int new_aspect_id = s[i];

	changeAspectRatio( new_aspect_id );
	updateWidgets();
}

void Core::nextWheelFunction() {
	int a = pref->wheel_function;

	bool done = false;
	if(((int ) pref->wheel_function_cycle)==0)
		return;
	while(!done){
		// get next a

		a = a*2;
		if(a==32)
			a = 2;
		// See if we are done
		if (pref->wheel_function_cycle.testFlag((Preferences::WheelFunction)a))
			done = true;
	}
	pref->wheel_function = a;
	QString m = "";
	switch(a){
	case Preferences::Seeking:
		m = tr("Mouse wheel seeks now");
		break;
	case Preferences::Volume:
		m = tr("Mouse wheel changes volume now");
		break;
	case Preferences::Zoom:
		m = tr("Mouse wheel changes zoom level now");
		break;
	case Preferences::ChangeSpeed:
		m = tr("Mouse wheel changes speed now");
		break;
	}
	displayMessage(m);
}

void Core::changeLetterbox(bool b) {
	qDebug("Core::changeLetterbox: %d", b);

	if (mset.add_letterbox != b) {
		mset.add_letterbox = b;
		CHANGE_VF("letterbox", b, DesktopInfo::desktop_size(mplayerwindow));
	}
}

#ifdef ADD_BLACKBORDERS_FS
void Core::changeLetterboxOnFullscreen(bool b) {
	qDebug("Core::changeLetterboxOnFullscreen: %d", b);
	CHANGE_VF("letterbox", b, DesktopInfo::desktop_size(mplayerwindow));
}
#endif

void Core::changeOSD(int v) {
	qDebug("Core::changeOSD: %d", v);

	pref->osd = v;

	proc->setPausingPrefix(pausing_prefix());
	proc->setOSD(pref->osd);

	updateWidgets();
}

void Core::nextOSD() {
	int osd = pref->osd + 1;
	if (osd > Preferences::SeekTimerTotal) {
		osd = Preferences::None;	
	}
	changeOSD( osd );
}

void Core::changeRotate(int r) {
	qDebug("Core::changeRotate: %d", r);

	if (mset.rotate != r) {
		if (proc->isMPlayer()) {
			mset.rotate = r;
			restartPlay();
		} else {
			// MPV
			// Remove previous filter
			switch (mset.rotate) {
				case MediaSettings::Clockwise_flip: proc->changeVF("rotate", false, MediaSettings::Clockwise_flip); break;
				case MediaSettings::Clockwise: proc->changeVF("rotate", false, MediaSettings::Clockwise); break;
				case MediaSettings::Counterclockwise: proc->changeVF("rotate", false, MediaSettings::Counterclockwise); break;
				case MediaSettings::Counterclockwise_flip: proc->changeVF("rotate", false, MediaSettings::Counterclockwise_flip); break;
			}
			mset.rotate = r;
			// New filter
			switch (mset.rotate) {
				case MediaSettings::Clockwise_flip: proc->changeVF("rotate", true, MediaSettings::Clockwise_flip); break;
				case MediaSettings::Clockwise: proc->changeVF("rotate", true, MediaSettings::Clockwise); break;
				case MediaSettings::Counterclockwise: proc->changeVF("rotate", true, MediaSettings::Counterclockwise); break;
				case MediaSettings::Counterclockwise_flip: proc->changeVF("rotate", true, MediaSettings::Counterclockwise_flip); break;
			}
		}
	}
}

#if USE_ADAPTER
void Core::changeAdapter(int n) {
	qDebug("Core::changeScreen: %d", n);

	if (pref->adapter != n) {
		pref->adapter = n;
		restartPlay();
	}
}
#endif

void Core::changeAO(const QString & new_ao) {
	qDebug() << "Core::changeAO:" << new_ao;
	if (pref->ao != new_ao) {
		pref->ao = new_ao;
		if (proc->isRunning()) restartPlay();
	}
}

#if 0
void Core::changeSize(int n) {
	if ( /*(n != pref->size_factor) &&*/ (!pref->use_mplayer_window) ) {
		pref->size_factor = n;

		emit needResize(mset.win_width, mset.win_height);
		updateWidgets();
	}
}

void Core::toggleDoubleSize() {
	if (pref->size_factor != 100) 
		changeSize(100);
	else
		changeSize(200);
}
#endif

void Core::changeZoom(double p) {
	qDebug("Core::changeZoom: %f", p);
	if (p < ZOOM_MIN) p = ZOOM_MIN;

	mset.zoom_factor = p;
	mplayerwindow->setZoom(p);
	displayMessage( tr("Zoom: %1").arg(mset.zoom_factor) );
}

void Core::resetZoom() {
	changeZoom(1.0);
}

void Core::autoZoom() {
	double video_aspect = mset.aspectToNum( (MediaSettings::Aspect) mset.aspect_ratio_id);

	if (video_aspect <= 0) {
		QSize w = mplayerwindow->videoLayer()->size();
		video_aspect = (double) w.width() / w.height();
	}

	double screen_aspect = DesktopInfo::desktop_aspectRatio(mplayerwindow);
	double zoom_factor;

	if (video_aspect > screen_aspect)
		zoom_factor = video_aspect / screen_aspect;
	else
		zoom_factor = screen_aspect / video_aspect;

	qDebug("Core::autoZoom: video_aspect: %f", video_aspect);
	qDebug("Core::autoZoom: screen_aspect: %f", screen_aspect);
	qDebug("Core::autoZoom: zoom_factor: %f", zoom_factor);

	changeZoom(zoom_factor);
}

void Core::autoZoomFromLetterbox(double aspect) {
	qDebug("Core::autoZoomFromLetterbox: %f", aspect);

	// Probably there's a much easy way to do this, but I'm not good with maths...

	QSize desktop =  DesktopInfo::desktop_size(mplayerwindow);

	double video_aspect = mset.aspectToNum( (MediaSettings::Aspect) mset.aspect_ratio_id);

	if (video_aspect <= 0) {
		QSize w = mplayerwindow->videoLayer()->size();
		video_aspect = (double) w.width() / w.height();
	}

	// Calculate size of the video in fullscreen
	QSize video;
	video.setHeight( desktop.height() );;
	video.setWidth( (int) (video.height() * video_aspect) );
	if (video.width() > desktop.width()) {
		video.setWidth( desktop.width() );;
		video.setHeight( (int) (video.width() / video_aspect) );
	}

	qDebug("Core::autoZoomFromLetterbox: max. size of video: %d %d", video.width(), video.height());

	// Calculate the size of the actual video inside the letterbox
	QSize actual_video;
	actual_video.setWidth( video.width() );
	actual_video.setHeight( (int) (actual_video.width() / aspect) );

	qDebug("Core::autoZoomFromLetterbox: calculated size of actual video for aspect %f: %d %d", aspect, actual_video.width(), actual_video.height());

	double zoom_factor = (double) desktop.height() / actual_video.height();

	qDebug("Core::autoZoomFromLetterbox: calculated zoom factor: %f", zoom_factor);
	changeZoom(zoom_factor);
}

void Core::autoZoomFor169() {
	autoZoomFromLetterbox((double) 16 / 9);
}

void Core::autoZoomFor235() {
	autoZoomFromLetterbox(2.35);
}

void Core::incZoom() {
	qDebug("Core::incZoom");
	changeZoom( mset.zoom_factor + ZOOM_STEP );
}

void Core::decZoom() {
	qDebug("Core::decZoom");
	changeZoom( mset.zoom_factor - ZOOM_STEP );
}

void Core::showFilenameOnOSD() {
	qDebug("Core::showFilenameOnOSD");
	proc->showFilenameOnOSD();
}

void Core::showMediaInfoOnOSD() {
	qDebug("Core::showMediaInfoOnOSD");
#ifdef MPV_SUPPORT
	if (proc->isMPV()) proc->setOSDMediaInfo(pref->mpv_osd_media_info);
#endif
#ifdef MPLAYER_SUPPORT
	if (proc->isMPlayer()) proc->setOSDMediaInfo(pref->mplayer_osd_media_info);
#endif
	proc->showMediaInfoOnOSD();
}

void Core::showTimeOnOSD() {
	proc->showTimeOnOSD();
}

void Core::toggleDeinterlace() {
	qDebug("Core::toggleDeinterlace");
	proc->toggleDeinterlace();
}

void Core::changeUseCustomSubStyle(bool b) {
	qDebug("Core::changeUseCustomSubStyle: %d", b);

	if (pref->enable_ass_styles != b) {
		pref->enable_ass_styles = b;
		if (proc->isRunning()) restartPlay();
	}
}

void Core::toggleForcedSubsOnly(bool b) {
	qDebug("Core::toggleForcedSubsOnly: %d", b);

	if (pref->use_forced_subs_only != b) {
		pref->use_forced_subs_only = b;
		//if (proc->isRunning()) restartPlay();
		proc->setSubForcedOnly(b);
	}
}

void Core::changeClosedCaptionChannel(int c) {
	qDebug("Core::changeClosedCaptionChannel: %d", c);
	if (c != mset.closed_caption_channel) {
		mset.closed_caption_channel = c;
		if (proc->isRunning()) restartPlay();
	}
}

/*
void Core::nextClosedCaptionChannel() {
	int c = mset.closed_caption_channel;
	c++;
	if (c > 4) c = 0;
	changeClosedCaptionChannel(c);
}

void Core::prevClosedCaptionChannel() {
	int c = mset.closed_caption_channel;
	c--;
	if (c < 0) c = 4;
	changeClosedCaptionChannel(c);
}
*/

#if DVDNAV_SUPPORT
// dvdnav buttons
void Core::dvdnavUp() {
	qDebug("Core::dvdnavUp");
	proc->discButtonPressed("up");
}

void Core::dvdnavDown() {
	qDebug("Core::dvdnavDown");
	proc->discButtonPressed("down");
}

void Core::dvdnavLeft() {
	qDebug("Core::dvdnavLeft");
	proc->discButtonPressed("left");
}

void Core::dvdnavRight() {
	qDebug("Core::dvdnavRight");
	proc->discButtonPressed("right");
}

void Core::dvdnavMenu() {
	qDebug("Core::dvdnavMenu");
	proc->discButtonPressed("menu");
}

void Core::dvdnavSelect() {
	qDebug("Core::dvdnavSelect");
	proc->discButtonPressed("select");
}

void Core::dvdnavPrev() {
	qDebug("Core::dvdnavPrev");
	proc->discButtonPressed("prev");
}

void Core::dvdnavMouse() {
	qDebug("Core::dvdnavMouse");

	if ((state() == Playing) && (mdat.filename.startsWith("dvdnav:"))) {
		proc->discButtonPressed("mouse");
	}
}
#endif

void Core::displayMessage(QString text) {
	qDebug("Core::displayMessage");
	emit showMessage(text);

	if ((pref->fullscreen) && (state() != Paused)) {
		displayTextOnOSD( text );
	}
}

void Core::displayScreenshotName(QString filename) {
	qDebug("Core::displayScreenshotName: %s", filename.toUtf8().constData());

	QFileInfo fi(filename);

	QString text = tr("Screenshot saved as %1").arg(fi.fileName());
	//QString text = QString("Screenshot saved as %1").arg(fi.fileName());

#ifdef MPLAYER2_SUPPORT
	if (MplayerVersion::isMplayer2()) {
		displayTextOnOSD(text, 3000, 1, "");
	}
	else
#endif
	if (MplayerVersion::isMplayerAtLeast(27665)) {
		displayTextOnOSD(text, 3000, 1, "pausing_keep_force");
	}
	else
	if (state() != Paused) {
		// Dont' show the message on OSD while in pause, otherwise
		// the video goes forward a frame.
		displayTextOnOSD(text, 3000, 1, "pausing_keep");
	}

	emit showMessage(text);
}

void Core::displayUpdatingFontCache() {
	qDebug("Core::displayUpdatingFontCache");
	emit showMessage( tr("Updating the font cache. This may take some seconds...") );
}

void Core::displayBuffering() {
	setState(Buffering);
//	emit showMessage(tr("Buffering..."));
}

void Core::displayPlaying() {
	qDebug("Core::displayPlaying");
	setState(Buffering);
	emit showMessage(tr("Starting..."), 60000);
}

void Core::gotWindowResolution(int w, int h) {
	qDebug("Core::gotWindowResolution: %d, %d", w, h);
	//double aspect = (double) w/h;

	if (pref->use_mplayer_window) {
		emit noVideo();
	} else {
		if ((pref->resize_method==Preferences::Afterload) && (we_are_restarting)) {
			// Do nothing
		} else {
			emit needResize(w,h);
		}
	}

	mset.win_width = w;
	mset.win_height = h;

	//Override aspect ratio, is this ok?
	//mdat.video_aspect = mset.win_aspect();

	mplayerwindow->setResolution( w, h );
	mplayerwindow->setAspect( mset.win_aspect() );
}

void Core::gotNoVideo() {
	// File has no video (a sound file)

	// Reduce size of window
	/*
	mset.win_width = mplayerwindow->size().width();
	mset.win_height = 0;
	mplayerwindow->setResolution( mset.win_width, mset.win_height );
	emit needResize( mset.win_width, mset.win_height );
	*/
	//mplayerwindow->showLogo(true);
	emit noVideo();
}

void Core::gotVO(QString vo) {
	qDebug() << "Core::gotVO:" << vo;

	/*
	if (pref->vo.isEmpty()) {
		qDebug("Core::gotVO: saving vo");
		pref->vo = vo;
	}
	*/
}

void Core::gotAO(QString ao) {
	qDebug() << "Core::gotAO:" << ao;

	/*
	if (pref->ao.isEmpty()) {
		qDebug("Core::gotAO: saving ao");
		pref->ao = ao;
	}
	*/
}

void Core::streamTitleChanged(QString title) {
	mdat.stream_title = title;
	emit mediaInfoChanged();
}

void Core::streamTitleAndUrlChanged(QString title, QString url) {
	mdat.stream_title = title;
	mdat.stream_url = url;
	emit mediaInfoChanged();
}

void Core::sendMediaInfo() {
	qDebug("Core::sendMediaInfo");
	emit mediaPlaying(mdat.filename, mdat.displayName(pref->show_tag_in_window_title));
}

//!  Called when the state changes
void Core::watchState(Core::State state) {
#ifdef SCREENSAVER_OFF
	#if 0
	qDebug("Core::watchState: %d", state);
	//qDebug("Core::watchState: has video: %d", !mdat.novideo);

	if ((state == Playing) /* && (!mdat.novideo) */) {
		disableScreensaver();
	} else {
		enableScreensaver();
	}
	#endif
#endif

	if ((proc->isMPlayer()) && (state == Playing) && (change_volume_after_unpause)) {
		// Delayed volume change
		qDebug("Core::watchState: delayed volume change");
		int volume = (pref->global_volume ? pref->volume : mset.volume);
		proc->setVolume(volume);
		change_volume_after_unpause = false;
	}
}

void Core::checkIfVideoIsHD() {
	qDebug("Core::checkIfVideoIsHD");

	// Check if the video is in HD and uses ffh264 codec.
	if ((mdat.video_codec=="ffh264") && (mset.win_height >= pref->HD_height)) {
		qDebug("Core::checkIfVideoIsHD: video == ffh264 and height >= %d", pref->HD_height);
		if (!mset.is264andHD) {
			mset.is264andHD = true;
			if (pref->h264_skip_loop_filter == Preferences::LoopDisabledOnHD) {
				qDebug("Core::checkIfVideoIsHD: we're about to restart the video");
				restartPlay();
			}
		}
	} else {
		mset.is264andHD = false;
		// FIXME: if the video was previously marked as HD, and now it's not
		// then the video should restart too.
	}
}

#if DELAYED_AUDIO_SETUP_ON_STARTUP && NOTIFY_AUDIO_CHANGES
#error "DELAYED_AUDIO_SETUP_ON_STARTUP and NOTIFY_AUDIO_CHANGES can't be both defined"
#endif

#if DELAYED_AUDIO_SETUP_ON_STARTUP
void Core::initAudioTrack() {
	qDebug("Core::initAudioTrack");

	// First audio if none selected
	if ( (mset.current_audio_id == MediaSettings::NoneSelected) && 
         (mdat.audios.numItems() > 0) ) 
	{
		// Don't set mset.current_audio_id here! changeAudio will do. 
		// Otherwise changeAudio will do nothing.

		int audio = mdat.audios.itemAt(0).ID(); // First one
		if (mdat.audios.existsItemAt(pref->initial_audio_track-1)) {
			audio = mdat.audios.itemAt(pref->initial_audio_track-1).ID();
		}

		// Check if one of the audio tracks is the user preferred.
		if (!pref->audio_lang.isEmpty()) {
			int res = mdat.audios.findLang( pref->audio_lang );
			if (res != -1) audio = res;
		}

		changeAudio( audio );
	}
}
#endif

#if NOTIFY_VIDEO_CHANGES
void Core::initVideoTrack(const Tracks & videos) {
	qDebug("Core::initVideoTrack");
	mdat.videos = videos;
	initializeMenus();
	updateWidgets();
}
#endif

#if NOTIFY_AUDIO_CHANGES
#ifdef SIMPLE_TRACK_SELECTION
void Core::initAudioTrack(const Tracks & audios) {
	mdat.audios = audios;

	qDebug() << "Core::initAudioTrack: num_items:" << mdat.audios.numItems();
	qDebug("Core::initAudioTrack: list of audios:");
	mdat.audios.list();

	initializeMenus();
	updateWidgets();
}
#else

void Core::initAudioTrack(const Tracks & audios) {
	qDebug("Core::initAudioTrack");
	qDebug("Core::initAudioTrack: num_items: %d", mdat.audios.numItems());

	bool is_osd_enabled = proc->isOSDInCommandsEnabled();
	proc->enableOSDInCommands(false);

	bool restore_audio = ((mdat.audios.numItems() > 0) || 
                          (mset.current_audio_id != MediaSettings::NoneSelected));

	mdat.audios = audios;

	qDebug("Core::initAudioTrack: list of audios:");
	mdat.audios.list();

	initializeMenus();

	if (!restore_audio) {
		// Select initial track
		qDebug("Core::initAudioTrack: selecting initial track");

		bool change_audio = (mdat.type != TYPE_STREAM); // Don't change audio with streams unless strictly necessary

		int audio = mdat.audios.itemAt(0).ID(); // First one
		if (mdat.audios.existsItemAt(pref->initial_audio_track-1)) {
			audio = mdat.audios.itemAt(pref->initial_audio_track-1).ID();
		}

		// Check if one of the audio tracks is the user preferred.
		if (!pref->audio_lang.isEmpty()) {
			int res = mdat.audios.findLang( pref->audio_lang );
			if (res != -1) {
				audio = res;
				change_audio = true;
			}
		}

		if (change_audio) changeAudio(audio);
	} else {
		// Try to restore previous audio track
		qDebug("Core::initAudioTrack: restoring audio");
		// Nothing to do, the audio is already set with -aid
	}

	proc->enableOSDInCommands(is_osd_enabled);

	updateWidgets();

	emit audioTracksChanged();
}
#endif // SIMPLE_TRACK_SELECTION
#endif

#if NOTIFY_SUB_CHANGES
#ifdef SIMPLE_TRACK_SELECTION
void Core::initSubtitleTrack(const SubTracks & subs) {
	mdat.subs = subs;

	qDebug() << "Core::initSubtitleTrack: num_items:" << mdat.subs.numItems();
	qDebug("Core::initSubtitleTrack: list of subtitles:");
	mdat.subs.list();

	initializeMenus();
	updateWidgets();
}
#else

void Core::initSubtitleTrack(const SubTracks & subs) {
	qDebug("Core::initSubtitleTrack");
	qDebug("Core::initSubtitleTrack: num_items: %d", mdat.subs.numItems());

	proc->enableOSDInCommands(false);

	bool restore_subs = ((mdat.subs.numItems() > 0) || 
                         (mset.current_sub_id != MediaSettings::NoneSelected));

	// Save current sub
	SubData::Type previous_sub_type = SubData::Sub;
	int previous_sub_id = -1;
	if (mdat.subs.numItems() > 0) {
		if ((mset.current_sub_id != MediaSettings::SubNone) && 
	        (mset.current_sub_id != MediaSettings::NoneSelected)) 
		{
			previous_sub_type = mdat.subs.itemAt(mset.current_sub_id).type();
			previous_sub_id = mdat.subs.itemAt(mset.current_sub_id).ID();
		}
	}
	qDebug("Core::initSubtitleTrack: previous subtitle: type: %d id: %d", previous_sub_type, previous_sub_id);

	mdat.subs = subs;

	qDebug("Core::initSubtitleTrack: list of subtitles:");
	mdat.subs.list();

	initializeMenus();

	if (just_unloaded_external_subs) {
		qDebug("Core::initSubtitleTrack: just_unloaded_external_subs: true");
		restore_subs = false;
		just_unloaded_external_subs = false;
	}
	if (just_loaded_external_subs) {
		qDebug("Core::initSubtitleTrack: just_loaded_external_subs: true");
		restore_subs = false;
		just_loaded_external_subs = false;

		QFileInfo fi(mset.external_subtitles);
		bool is_idx = (fi.suffix().toLower() == "idx");
		if (proc->isMPV()) is_idx = false; // Hack to ignore the idx extension with mpv

		if (!is_idx) {
			// The loaded subtitle file is the last one, so
			// try to select that one.
			if (mdat.subs.numItems() > 0) {
				int selected_subtitle = mdat.subs.numItems()-1; // If everything fails, use the last one

				// Try to find the subtitle file in the list
				for (int n = 0; n < mdat.subs.numItems(); n++) {
					SubData sub = mdat.subs.itemAt(n);
					if ((sub.type() == SubData::File) && (sub.filename() == mset.external_subtitles)) {
						selected_subtitle = n;
						qDebug("Core::initSubtitleTrack: external subtitle found: #%d", n);
						break;
					}
				}
				changeSubtitle(selected_subtitle);
				goto end;
			}
		}
	}

	if (!restore_subs) {
		// Select initial track
		qDebug("Core::initSubtitleTrack: selecting initial track");

		if (!pref->autoload_sub) {
			changeSubtitle(MediaSettings::SubNone);
		} else {
			//Select first subtitle
			int sub = mdat.subs.selectOne( pref->subtitle_lang, pref->initial_subtitle_track-1 );
			changeSubtitle(sub);
		}
	} else {
		// Try to restore previous subtitle track
		qDebug("Core::initSubtitleTrack: restoring subtitle");

		if (mset.current_sub_id == MediaSettings::SubNone) {
			changeSubtitle(MediaSettings::SubNone);
		}
		else
		if (mset.current_sub_id != MediaSettings::NoneSelected) {
			// Try to find old subtitle
			int item = mset.current_sub_id;
			if (previous_sub_id != -1) {
				int sub_item = mdat.subs.find(previous_sub_type, previous_sub_id);
				if (sub_item > -1) {
					item = sub_item;
					qDebug("Core::initSubtitleTrack: previous subtitle found: %d", sub_item);
				}
			}
			if (item > -1) {
				changeSubtitle(item);
			} else {
				qDebug("Core::initSubtitleTrack: previous subtitle not found!");
			}
		}
	}
end:

#ifdef MPV_SUPPORT
	changeSecondarySubtitle(mset.current_secondary_sub_id);
#endif

	proc->enableOSDInCommands(true);
	updateWidgets();
}
#endif // SIMPLE_TRACK_SELECTION

void Core::setSubtitleTrackAgain(const SubTracks &) {
	qDebug("Core::setSubtitleTrackAgain");
	changeSubtitle( mset.current_sub_id );
}
#endif

#if NOTIFY_CHAPTER_CHANGES
void Core::updateChapterInfo(const Chapters & chapters) {
	qDebug("Core::updateChapterInfo");
	mdat.chapters = chapters;
	initializeMenus();
	updateWidgets();
}
#endif

#if DVDNAV_SUPPORT
void Core::dvdTitleChanged(int title) {
	qDebug("Core::dvdTitleChanged: %d", title);
}

void Core::durationChanged(double length) {
	qDebug("Core::durationChanged: %f", length);
	if (mdat.duration != length) {
		mdat.duration = length;
		emit newDuration(length);
	}
}

void Core::askForInfo() {
	if (proc->isMPlayer()) {
		if ((state() == Playing) && (mdat.filename.startsWith("dvdnav:"))) {
			proc->setPausingPrefix(pausing_prefix());
			proc->askForLength();
		}
	}
}

void Core::dvdnavUpdateMousePos(QPoint pos) {
#if 0
	bool under_mouse = mplayerwindow->videoLayer()->underMouse();

	qDebug("Core::dvdnavUpdateMousePos: %d %d", pos.x(), pos.y());
	qDebug("Core::dvdnavUpdateMousePos: state: %d", state());
	qDebug("Core::dvdnavUpdateMousePos: filename: %s", mdat.filename.toUtf8().constData());
	qDebug("Core::dvdnavUpdateMousePos: dvdnav_title_is_menu: %d", dvdnav_title_is_menu);
	qDebug("Core::dvdnavUpdateMousePos: under mouse: %d", under_mouse);
#endif
	if ((state() == Playing) && (mdat.filename.startsWith("dvdnav:")) && (dvdnav_title_is_menu)) {
		//if (under_mouse) {
			proc->discSetMousePos(pos.x(), pos.y());
		//}
	}
}

void Core::dvdTitleIsMenu() {
	qDebug("Core::dvdTitleIsMenu");
	dvdnav_title_is_menu = true;
}

void Core::dvdTitleIsMovie() {
	qDebug("Core::dvdTitleIsMovie");
	dvdnav_title_is_menu = false;
}
#endif

QString Core::pausing_prefix() {
	qDebug("Core::pausing_prefix");

#ifdef MPLAYER2_SUPPORT
	if (MplayerVersion::isMplayer2()) {
		return QString::null;
	}
	else
#endif
	if ( (pref->use_pausing_keep_force) && 
         (MplayerVersion::isMplayerAtLeast(27665)) ) 
	{
		return "pausing_keep_force";
	} else {
		return "pausing_keep";
	}
}

#ifdef BOOKMARKS
void Core::prevBookmark() {
	qDebug("Core::prevBookmark");

	if (mset.bookmarks.count() > 0) {
		QMapIterator<int, QString> i(mset.bookmarks);
		i.toBack();
		int last_time = i.peekPrevious().key();
		while (i.hasPrevious()) {
			i.previous();
			int time = i.key();
			if (time < (mset.current_sec -2)) {
				goToSec(time);
				return;
			}
		}
		// Go to last bookmark
		goToSec(last_time);
	}
}

void Core::nextBookmark() {
	qDebug("Core::nextBookmark");

	if (mset.bookmarks.count() > 0) {
		QMapIterator<int, QString> i(mset.bookmarks);
		int first_time = i.peekNext().key();
		while (i.hasNext()) {
			i.next();
			int time = i.key();
			if (time > mset.current_sec) {
				goToSec(time);
				return;
			}
		}
		// Go to first bookmark
		goToSec(first_time);
	}
}

void Core::saveBookmarks() {
	qDebug("Core::saveBookmarks");
	saveMediaInfo();
}
#endif

#include "moc_core.cpp"
