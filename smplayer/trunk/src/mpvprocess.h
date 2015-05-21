/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "playerprocess.h"
#include "config.h"

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
	void setExternalSubtitleFile(const QString & filename);
	void setSubPos(int pos);
	void setSubScale(double value);
	void setSubStep(int value);
	void setSubForcedOnly(bool b);
	void setSpeed(double value);
	void enableKaraoke(bool b);
	void enableExtrastereo(bool b);
	void enableVolnorm(bool b, const QString & option);
	void setAudioEqualizer(const QString & values);
	void setAudioDelay(double delay);
	void setSubDelay(double delay);
	void setLoop(int v);
	void takeScreenshot(ScreenshotType t, bool include_subtitles = false);
	void setTitle(int ID);
	void changeVF(const QString & filter, bool enable, const QVariant & option = QVariant());
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
	void setChannelsFile(const QString &);

	QString mpvVersion() { return mpv_version; };

protected:
	bool isOptionAvailable(const QString & option);
	void addVFIfAvailable(const QString & vf, const QString & value = QString::null);

protected slots:
	void parseLine(QByteArray ba);
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void gotError(QProcess::ProcessError);
	void requestChapterInfo();
	void requestBitrateInfo();

protected:
#if NOTIFY_AUDIO_CHANGES
	void updateAudioTrack(int ID, const QString & name, const QString & lang);
#endif
#if NOTIFY_SUB_CHANGES
	void updateSubtitleTrack(int ID, const QString & name, const QString & lang);
#endif

private:
	bool notified_mplayer_is_running;
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
};

#endif
