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

#ifndef MPLAYERPROCESS_H
#define MPLAYERPROCESS_H

#include <QString>
#include "playerprocess.h"
#include "mediadata.h"
#include "config.h"

class QStringList;

class MplayerProcess : public PlayerProcess
{
	Q_OBJECT

public:
	MplayerProcess(QObject * parent = 0);
	~MplayerProcess();

	bool start();

	// Command line options
	void setMedia(const QString & media, bool is_playlist = false);
	void setFixedOptions();
	void disableInput();
	void setOption(const QString & option_name, const QVariant & value = QVariant());
	void addUserOption(const QString & option);
	void addVF(const QString & filter_name, const QVariant & value = QVariant());
	void addAF(const QString & filter_name, const QVariant & value = QVariant());
	void addStereo3DFilter(const QString & in, const QString & out);
	void setSubStyles(const AssStyles & styles, const QString & assStylesFile = QString::null);
	void setSubEncoding(const QString & codepage, const QString & enca_lang);

	// Slave commands
	void quit();
	void setVolume(int v);
	void setOSD(int o);
	void setAudio(int ID);
	void setVideo(int ID);
	void setSubtitle(int type, int ID);
	void disableSubtitles();
	void setSecondarySubtitle(int /*ID*/) {};
	void disableSecondarySubtitles() {};
	void setSubtitlesVisibility(bool b);
	void seek(double secs, int mode, bool precise);
	void mute(bool b);
	void setPause(bool b);
	void frameStep();
	void frameBackStep();
	void showOSDText(const QString & text, int duration, int level);
	void showFilenameOnOSD();
	void showTimeOnOSD();
	void setContrast(int value, bool soft_eq);
	void setBrightness(int value, bool soft_eq);
	void setHue(int value, bool soft_eq);
	void setSaturation(int value, bool soft_eq);
	void setGamma(int value, bool soft_eq);
	void setChapter(int ID);
	void nextChapter();
	void previousChapter();
	void setExternalSubtitleFile(const QString & filename);
	void setSubPos(int pos);
	void setSubScale(double value);
	void setSubStep(int value);
#ifdef MPV_SUPPORT
	void seekSub(int value);
#endif
	void setSubForcedOnly(bool b);
	void setSpeed(double value);
	void enableKaraoke(bool b);
	void enableExtrastereo(bool b);
	void enableVolnorm(bool b, const QString & option);
#ifdef MPV_SUPPORT
	void enableEarwax(bool /*b*/) {};
#endif
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
	void setOSDFractions(bool) {};
	void setChannelsFile(const QString &) {};

	void enableScreenshots(const QString & dir, const QString & templ = QString::null, const QString & format = QString::null);

#ifdef CAPTURE_STREAM
	void setCaptureDirectory(const QString & dir);
#endif

protected slots:
	void parseLine(QByteArray ba);
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void gotError(QProcess::ProcessError);

private:
	bool notified_mplayer_is_running;
	bool received_end_of_file;

	int last_sub_id;

	int mplayer_svn;

#if NOTIFY_SUB_CHANGES
	SubTracks subs;

	bool subtitle_info_received;
	bool subtitle_info_changed;
#endif

#if NOTIFY_AUDIO_CHANGES
	Tracks audios;
	bool audio_info_changed;
#endif

	int dvd_current_title;
	int br_current_title;
};

#endif
