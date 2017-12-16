/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef MPVPROCESS_H
#define MPVPROCESS_H

#include <QString>
#include <QRegExp>
#include "playerprocess.h"
#include "config.h"

#define OSD_WITH_TIMER
//#define USE_OLD_VIDEO_EQ

#ifndef USE_OLD_VIDEO_EQ
class SoftVideoEq
{
public:
	SoftVideoEq() { contrast = brightness = hue = saturation = gamma = 0; };
	SoftVideoEq(int c, int b, int h, int s, int g) {
		contrast = c; brightness = b; hue = h, saturation = s; gamma = g;
	}
	int contrast, brightness, hue, saturation, gamma;
};
#endif

class QStringList;

class MPVProcess : public PlayerProcess
{
	Q_OBJECT

public:
	MPVProcess(QObject * parent = 0);
	~MPVProcess();

	bool start();

	// Command line options
	void addArgument(const QString & a);
	void setMedia(const QString & media, bool is_playlist = false);
	void disableInput();
	void setFixedOptions();
	void setOption(const QString & option_name, const QVariant & value = QVariant());
	void addUserOption(const QString & option);
	void addVF(const QString & filter_name, const QVariant & value = QVariant());
	void addAF(const QString & filter_name, const QVariant & value = QVariant());
	void addStereo3DFilter(const QString & in, const QString & out);
	void setSubStyles(const AssStyles & styles, const QString & assStylesFile = QString::null);
	void setSubEncoding(const QString & codepage, const QString & enca_lang);
	void setVideoEqualizerOptions(int contrast, int brightness, int hue, int saturation, int gamma, bool soft_eq);

	// Slave commands
	void quit();
	void setVolume(int v);
	void setOSD(int o);
	void setAudio(int ID);
	void setVideo(int ID);
	void setSubtitle(int type, int ID);
	void disableSubtitles();
	void setSecondarySubtitle(int ID);
	void disableSecondarySubtitles();
	void setSubtitlesVisibility(bool b);
	void seek(double secs, int mode, bool precise);
	void mute(bool b);
	void setPause(bool b);
	void frameStep();
	void frameBackStep();
	void showOSDText(const QString & text, int duration, int level);
	void showFilenameOnOSD();
	void showTimeOnOSD();

	void setContrast(int value);
	void setBrightness(int value);
	void setHue(int value);
	void setSaturation(int value);
	void setGamma(int value);

	void setChapter(int ID);
	void nextChapter();
	void previousChapter();
	void setExternalSubtitleFile(const QString & filename);
	void setSubPos(int pos);
	void setSubScale(double value);
	void setSubStep(int value);
	void seekSub(int value);
	void setSubForcedOnly(bool b);
	void setSpeed(double value);
	void enableKaraoke(bool b);
	void enableExtrastereo(bool b);
	void enableVolnorm(bool b, const QString & option);
	void enableEarwax(bool b);
	void setAudioEqualizer(AudioEqualizerList);
	void setAudioDelay(double delay);
	void setSubDelay(double delay);
	void setLoop(int v);
	void setAMarker(int sec);
	void setBMarker(int sec);
	void clearABMarkers();
	void takeScreenshot(ScreenshotType t, bool include_subtitles = false);
#ifdef CAPTURE_STREAM
	void switchCapturing();
#endif
	void setTitle(int ID);
	void changeVF(const QString & filter, bool enable, const QVariant & option = QVariant());
	void changeAF(const QString & filter, bool enable, const QVariant & option = QVariant());
	void changeStereo3DFilter(bool enable, const QString & in, const QString & out);
#if DVDNAV_SUPPORT
	void discSetMousePos(int x, int y);
	void discButtonPressed(const QString & button_name);
#endif
	void setAspect(double aspect);
	void setFullscreen(bool b);
#if PROGRAM_SWITCH
	void setTSProgram(int ID);
#endif
	void toggleDeinterlace();
	void askForLength();
	void setOSDScale(double value);
	void setOSDFractions(bool active);
	void setChannelsFile(const QString &);

	void enableScreenshots(const QString & dir, const QString & templ = QString::null, const QString & format = QString::null);

	void enableOSDInCommands(bool b) { use_osd_in_commands = b; };
	bool isOSDInCommandsEnabled() { return use_osd_in_commands; };

	QString mpvVersion() { return mpv_version; };

protected:
	bool isOptionAvailable(const QString & option);
	void addVFIfAvailable(const QString & vf, const QString & value = QString::null);
	void messageFilterNotSupported(const QString & filter_name);
	QString lavfi(const QString & filter_name, const QVariant & option = QVariant());
	QString audioEqualizerFilter(AudioEqualizerList);
#ifndef USE_OLD_VIDEO_EQ
	QString videoEqualizerFilter(SoftVideoEq);
	void updateSoftVideoEqualizerFilter();
#endif

#ifdef OSD_WITH_TIMER
	void toggleInfoOnOSD();
#endif

protected slots:
	void parseLine(QByteArray ba);
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void gotError(QProcess::ProcessError);
	void requestChapterInfo();
	/* void requestBitrateInfo(); */

#ifdef OSD_WITH_TIMER
	void displayInfoOnOSD();
#endif

protected:
	virtual void initializeOptionVars();

#if NOTIFY_AUDIO_CHANGES
	void updateAudioTrack(int ID, const QString & name, const QString & lang);
#endif
#if NOTIFY_SUB_CHANGES
	void updateSubtitleTrack(int ID, const QString & name, const QString & lang);
#endif

private:
	#if 0
	// For some reason lupdate doesn't get the translations from mpvoptions.cpp
	void translations() {
		QString s = tr("the '%1' filter is not supported by mpv");
		s = tr("File:");
		s = tr("Video:");
		s = tr("Resolution:");
		s = tr("Frames per second:");
		s = tr("Estimated:");
		s = tr("Aspect Ratio:");
		s = tr("Bitrate:");
		s = tr("Dropped frames:");
		s = tr("Audio:");
		s = tr("Bitrate:");
		s = tr("Sample Rate:");
		s = tr("Channels:");
		s = tr("Audio/video synchronization:");
		s = tr("Cache fill:");
		s = tr("Used cache:");
	}
	#endif

private:
	bool notified_mplayer_is_running;
	bool notified_pause;
	bool received_end_of_file;

	int last_sub_id;

	int mplayer_svn;

	QString mpv_version;
	bool verbose;

#if NOTIFY_SUB_CHANGES
	SubTracks subs;

	bool subtitle_info_received;
	bool subtitle_info_changed;
#endif

#if NOTIFY_AUDIO_CHANGES
	Tracks audios;
	bool audio_info_changed;
#endif

#if NOTIFY_VIDEO_CHANGES
	Tracks videos;
	bool video_info_changed;
#endif

#if NOTIFY_CHAPTER_CHANGES
	Chapters chapters;
	bool chapter_info_changed;
#endif

	int dvd_current_title;
	int br_current_title;

	QString previous_eq;
	AudioEqualizerList previous_eq_list;

#ifndef USE_OLD_VIDEO_EQ
	bool use_soft_eq;
	SoftVideoEq current_soft_eq;
	SoftVideoEq previous_soft_eq;
#endif

#ifdef CAPTURE_STREAM
	bool capturing;
#endif

#ifdef OSD_WITH_TIMER
	QTimer * osd_timer;
#endif

	bool use_osd_in_commands;

	// Regular expressions
	QRegExp rx_av;
	QRegExp rx_dsize;
	QRegExp rx_vo;
	QRegExp rx_ao;
	QRegExp rx_paused;
	QRegExp rx_endoffile;

	QRegExp rx_audio;
	QRegExp rx_subs;

	QRegExp rx_videocodec;
	QRegExp rx_audiocodec;

#if !NOTIFY_VIDEO_CHANGES
	QRegExp rx_video;
#endif

	QRegExp rx_chaptername;
	QRegExp rx_trackinfo;
	QRegExp rx_forbidden;

#if DVDNAV_SUPPORT
	QRegExp rx_switch_title;
#endif

	QRegExp rx_playing;
	QRegExp rx_generic;
	QRegExp rx_stream_title;
	QRegExp rx_debug;

	void initializeRX();
};

#endif
