/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _MPLAYERPROCESS_H_
#define _MPLAYERPROCESS_H_

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
	void setMedia(const QString & media);
	void setFixedOptions();
	void disableInput();
	void setOption(const QString & option_name, const QVariant & value = QVariant());
	void addUserOption(const QString & option);
	void addVF(const QString & filter_name, const QVariant & value = QVariant());
	void addAF(const QString & filter_name, const QVariant & value = QVariant());

	// Slave commands
	void quit();
	void setVolume(int v);
	void setOSD(int o);
	void setAudio(int ID);
	void setVideo(int ID);
	void setSubtitle(int type, int ID);
	void disableSubtitles();
	void setSubtitlesVisibility(bool b);
	void seek(double secs, int mode, bool precise);
	void mute(bool b);
	void setPause(bool b);
	void frameStep();
	void frameBackStep();
	void showOSDText(const QString & text, int duration, int level);
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

#if DVDNAV_SUPPORT
	void discSetMousePos(int x, int y);
	void discButtonPressed(const QString & button_name);
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
