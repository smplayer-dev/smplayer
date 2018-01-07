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

#ifndef PLAYERPROCESS_H
#define PLAYERPROCESS_H

#include "myprocess.h"
#include "mediadata.h"
#include "playerid.h"
#include "config.h"
#include "assstyles.h"
#include <QVariant>

class PlayerProcess : public MyProcess
{
	Q_OBJECT

public:
	enum ScreenshotType { Single = 0, Multiple = 1 };

	PlayerProcess(QObject * parent = 0);

	PlayerID::Player player() { return player_id; }
	bool isMPlayer() { return (player_id == PlayerID::MPLAYER); }
	bool isMPV() { return (player_id == PlayerID::MPV); }

	virtual bool start() = 0;

	void writeToStdin(QString text);
	MediaData mediaData() { return md; };

	// Command line options
	virtual void setMedia(const QString & media, bool is_playlist = false) = 0;
	virtual void setFixedOptions() = 0;
	virtual void disableInput() = 0;
	virtual void setOption(const QString & option_name, const QVariant & value = QVariant()) = 0;
	virtual void addUserOption(const QString & option) = 0;
	virtual void addVF(const QString & filter_name, const QVariant & value = QVariant()) = 0;
	virtual void addAF(const QString & filter_name, const QVariant & value = QVariant()) = 0;
	virtual void addStereo3DFilter(const QString & in, const QString & out) = 0;
	virtual void setSubStyles(const AssStyles & styles, const QString & assStylesFile = QString::null) = 0;
	virtual void setSubEncoding(const QString & codepage, const QString & enca_lang) = 0;
	virtual void setVideoEqualizerOptions(int contrast, int brightness, int hue, int saturation, int gamma, bool soft_eq) = 0;

	// Slave commands
	virtual void quit() = 0;
	virtual void setVolume(int v) = 0;
	virtual void setOSD(int o) = 0;
	virtual void setAudio(int ID) = 0;
	virtual void setVideo(int ID) = 0;
	virtual void setSubtitle(int type, int ID) = 0;
	virtual void disableSubtitles() = 0;
	virtual void setSecondarySubtitle(int ID) = 0;
	virtual void disableSecondarySubtitles() = 0;
	virtual void setSubtitlesVisibility(bool b) = 0;
	virtual void seek(double secs, int mode, bool precise) = 0;
	virtual void mute(bool b) = 0;
	virtual void setPause(bool b) = 0;
	virtual void frameStep() = 0;
	virtual void frameBackStep() = 0;
	virtual void showOSDText(const QString & text, int duration, int level) = 0;
	virtual void showFilenameOnOSD() = 0;
	virtual void showMediaInfoOnOSD() = 0;
	virtual void showTimeOnOSD() = 0;

	virtual void setContrast(int value) = 0;
	virtual void setBrightness(int value) = 0;
	virtual void setHue(int value) = 0;
	virtual void setSaturation(int value) = 0;
	virtual void setGamma(int value) = 0;

	virtual void setChapter(int ID) = 0;
	virtual void nextChapter() = 0;
	virtual void previousChapter() = 0;
	virtual void setExternalSubtitleFile(const QString & filename) = 0;
	virtual void setSubPos(int pos) = 0;
	virtual void setSubScale(double value) = 0;
	virtual void setSubStep(int value) = 0;
#ifdef MPV_SUPPORT
	virtual void seekSub(int value) = 0;
#endif
	virtual void setSubForcedOnly(bool b) = 0;
	virtual void setSpeed(double value) = 0;
	virtual void enableKaraoke(bool b) = 0;
	virtual void enableExtrastereo(bool b) = 0;
	virtual void enableVolnorm(bool b, const QString & option) = 0;
#ifdef MPV_SUPPORT
	virtual void enableEarwax(bool b) = 0;
#endif
	virtual void setAudioEqualizer(AudioEqualizerList) = 0;
	virtual void setAudioDelay(double delay) = 0;
	virtual void setSubDelay(double delay) = 0;
	virtual void setLoop(int v) = 0;
	virtual void setAMarker(int sec) = 0;
	virtual void setBMarker(int sec) = 0;
	virtual void clearABMarkers() = 0;
	virtual void takeScreenshot(ScreenshotType t, bool include_subtitles = false) = 0;
#ifdef CAPTURE_STREAM
	virtual void switchCapturing() = 0;
#endif
	virtual void setTitle(int ID) = 0;
	virtual void changeVF(const QString & filter, bool enable, const QVariant & option = QVariant()) = 0;
	virtual void changeAF(const QString & filter, bool enable, const QVariant & option = QVariant()) = 0;
	virtual void changeStereo3DFilter(bool enable, const QString & in, const QString & out) = 0;
#if DVDNAV_SUPPORT
	virtual void discSetMousePos(int x, int y) = 0;
	virtual void discButtonPressed(const QString & button_name) = 0;
#endif
	virtual void setAspect(double aspect) = 0;
	virtual void setFullscreen(bool b) = 0;
#if PROGRAM_SWITCH
	virtual void setTSProgram(int ID) = 0;
#endif
	virtual void toggleDeinterlace() = 0;
	virtual void askForLength() = 0;
	virtual void setOSDScale(double value) = 0;
	virtual void setOSDFractions(bool active) = 0;
	virtual void setChannelsFile(const QString &) = 0;

	virtual void enableScreenshots(const QString & dir, const QString & templ = QString::null, const QString & format = QString::null) = 0;

	void setPausingPrefix(const QString & prefix) { pausing_prefix = prefix; };

	void setOSDMediaInfo(const QString & s) { osd_media_info = s; };
	QString OSDMediaInfo() { return osd_media_info; };

	virtual void enableOSDInCommands(bool b) = 0;
	virtual bool isOSDInCommandsEnabled() = 0;

#ifdef CAPTURE_STREAM
	virtual void setCaptureDirectory(const QString & dir);
#endif

	static PlayerProcess * createPlayerProcess(const QString & player_bin, QObject * parent = 0);


// Signals
signals:
	void processExited();
	void lineAvailable(QString line);

	void receivedCurrentSec(double sec);
	void receivedCurrentFrame(int frame);
	void receivedPause();
	void receivedWindowResolution(int,int);
	void receivedNoVideo();
	void receivedVO(QString);
	void receivedAO(QString);
	void receivedEndOfFile();
	void mplayerFullyLoaded();
	void receivedStartingTime(double sec);

	void receivedCacheMessage(QString);
	void receivedCacheEmptyMessage(QString);
	void receivedCreatingIndex(QString);
	void receivedConnectingToMessage(QString);
	void receivedResolvingMessage(QString);
	void receivedBuffering();
	void receivedPlaying();
	void receivedScreenshot(QString);
	void receivedUpdatingFontCache();
	void receivedScanningFont(QString);
	void receivedForbiddenText();

	void receivedStreamTitle(QString);
	void receivedStreamTitleAndUrl(QString,QString);

	void failedToParseMplayerVersion(QString line_with_mplayer_version);

#if NOTIFY_SUB_CHANGES
	//! Emitted if a new subtitle has been added or an old one changed
	void subtitleInfoChanged(const SubTracks &, int selected_id);

	//! Emitted when subtitle info has been received but there wasn't anything new
	void subtitleInfoReceivedAgain(const SubTracks &);
#endif
#if NOTIFY_AUDIO_CHANGES
	//! Emitted if a new audio track been added or an old one changed
    void audioInfoChanged(const Tracks &, int selected_id);
#endif
#if NOTIFY_VIDEO_CHANGES
	//! Emitted if a new video track been added or an old one changed
	void videoInfoChanged(const Tracks &, int selected_id);
#endif
#if NOTIFY_CHAPTER_CHANGES
	void chaptersChanged(const Chapters &);
#endif

#if DVDNAV_SUPPORT
	void receivedDVDTitle(int);
	void receivedDuration(double);
	void receivedTitleIsMenu();
	void receivedTitleIsMovie();
#endif

	void receivedVideoBitrate(int);
	void receivedAudioBitrate(int);

protected:
	virtual void initializeOptionVars() {};

	MediaData md;
	QString pausing_prefix;

#ifdef CAPTURE_STREAM
	QString capture_filename;
#endif

	PlayerID::Player player_id;

	QString osd_media_info;
};

#endif
