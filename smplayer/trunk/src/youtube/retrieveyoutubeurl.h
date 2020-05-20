/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef RETRIEVEYOUTUBEURL_H
#define RETRIEVEYOUTUBEURL_H

#include <QProcess>

#define YT_USE_YTDL
#define YT_OBSOLETE

#ifdef YT_OBSOLETE
#define YT_DASH_SUPPORT
class QSettings;
#endif

class RetrieveYoutubeUrl : public QObject
{
	Q_OBJECT

public:
	enum Quality { None = -1,
                   FLV_240p = 5, FLV_270p = 6, FLV_360p = 34, FLV_480p = 35,
                   MP4_360p = 18, MP4_480p = 59, MP4_480p2 = 78,
                   MP4_720p = 22,
                   MP4_1080p = 37, MP4_4096 = 38,
                   WEBM_360p = 43, WEBM_480p = 44, WEBM_720p = 45,
                   WEBM_1080p = 46,

                   DASH_AUDIO_MP4_48 = 139, DASH_AUDIO_MP4_128 = 140,
                   DASH_AUDIO_MP4_256 = 141,
                   DASH_AUDIO_WEBM_128 = 171, DASH_AUDIO_WEBM_192 = 172,

                   DASH_VIDEO_240p = 133, DASH_VIDEO_360p = 134,
                   DASH_VIDEO_480p = 135, DASH_VIDEO_480p2 = 212,
                   DASH_VIDEO_720p = 136,
                   DASH_VIDEO_720p60 = 298,
                   DASH_VIDEO_1080p = 137,
                   DASH_VIDEO_1080p60 = 299,
                   DASH_VIDEO_1440p = 264,
                   DASH_VIDEO_2160p2 = 138, DASH_VIDEO_2160p = 266,

                   DASH_VIDEO_WEBM_240p = 242,
                   DASH_VIDEO_WEBM_360p = 167,
                   DASH_VIDEO_WEBM_360p2 = 243,

                   DASH_VIDEO_WEBM_480p = 168,
                   DASH_VIDEO_WEBM_480p2 = 218,
                   DASH_VIDEO_WEBM_480p3 = 219,
                   DASH_VIDEO_WEBM_480p4 = 244,
                   DASH_VIDEO_WEBM_480p5 = 245,
                   DASH_VIDEO_WEBM_480p6 = 246,
                   DASH_VIDEO_WEBM_720p = 169,
                   DASH_VIDEO_WEBM_720p2 = 247,
                   DASH_VIDEO_WEBM_720p60 = 302,

                   DASH_VIDEO_WEBM_1080p = 170,
                   DASH_VIDEO_WEBM_1080p2 = 248,
                   DASH_VIDEO_WEBM_1080p60 = 303,
                   DASH_VIDEO_WEBM_1440p = 271,
                   DASH_VIDEO_WEBM_1440p60 = 308,
                   DASH_VIDEO_WEBM_2160p = 313,
                   DASH_VIDEO_WEBM_2160p2 = 272,
                   DASH_VIDEO_WEBM_2160p60 = 315,

                   DASH_VIDEO_WEBM_240p60hdr = 331,
                   DASH_VIDEO_WEBM_360p60hdr = 332,
                   DASH_VIDEO_WEBM_480p60hdr = 333,
                   DASH_VIDEO_WEBM_720p60hdr = 334,
                   DASH_VIDEO_WEBM_1080p60hdr = 335,
                   DASH_VIDEO_WEBM_1440p60hdr = 336,
                   DASH_VIDEO_WEBM_2160p60hdr = 337 };

	enum Resolution { R240p = 1, R360p = 2, R480p = 3, R720p = 4, R1080p = 5, R1440p = 6, R2160p = 7 };

	RetrieveYoutubeUrl( QObject* parent = 0 );
	~RetrieveYoutubeUrl();

	void fetchPage(const QString & url);
	void close();

	void setPreferredResolution(Resolution r) { preferred_resolution = r; }
	Resolution preferredResolution() { return preferred_resolution; }

	void setUseDASH(bool b) { use_dash = b; }
	bool useDASH() { return use_dash; }

	void enable60fps(bool b) { use_60fps = b; }
	bool is60fpsEnabled() { return use_60fps; }

	void enableAv1(bool b) { use_av1 = b; }
	bool isAv1Enabled() { return use_av1; }

	void setYtdlBin(const QString & path) { ytdl_bin = path; };
	QString ytdlBin() { return ytdl_bin; };

	void setUserAgent(const QString & s) { user_agent = s; }
	QString userAgent() { return user_agent; };

	void setUserFormat(const QString & s) { user_format = s; }
	QString userFormat() { return user_format; }

	QString origUrl() { return yt_url; }

	QString videoTitle() { return video_title; }

	QString selectedVideoUrl() { return selected_video_url; }
	Quality selectedVideoQuality() { return selected_video_quality; }

	QString selectedAudioUrl() { return selected_audio_url; }
	Quality selectedAudioQuality() { return selected_audio_quality; }

	static bool isUrlSupported(const QString & url);
	QString fullUrl(const QString & url);

	static QString extensionForItag(int itag);

#ifdef YT_OBSOLETE
	#ifdef YT_USE_SIG
	void setSettings(QSettings * /*settings*/) {};
	#endif

	QString urlTitle() { return video_title; }
	QString selectedUrl() { return selected_video_url; }
	Quality selectedQuality() { return selected_video_quality; }

	void setUseHttpsMain(bool b) { use_https_main = b; };
	bool useHttpsMain() { return use_https_main; };
#endif

signals:
	void gotPreferredUrl(const QString & url, int itag);
	void gotEmptyList();
	void connecting(QString host);
	void processFailedToStart();
#ifdef YT_OBSOLETE
	void errorOcurred(int error_number, QString error_str);
	void signatureNotFound(const QString & title);
	void noSslSupport();
#endif

protected slots:
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void processError(QProcess::ProcessError error);

protected:
	void clearData();
	void runYtdl(const QString & url);
	static QString getVideoID(QString video_url);
	static QString resolutionToText(Resolution r);
	static int getItagFromFormat(const QByteArray & t);

private:
	Resolution preferred_resolution;

#ifdef YT_OBSOLETE
	bool use_https_main;
#endif

	QString yt_url;
	QString video_title;

	QString selected_video_url;
	Quality selected_video_quality;

	QString selected_audio_url;
	Quality selected_audio_quality;

	bool use_dash;
	bool use_60fps;
	bool use_av1;

	QString user_agent;
	QString user_format;

	QProcess * process;
	QString ytdl_bin;
};

#endif
