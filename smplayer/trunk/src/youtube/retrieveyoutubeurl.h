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

#ifndef RETRIEVEYOUTUBEURL_H
#define RETRIEVEYOUTUBEURL_H

#include <QNetworkAccessManager>
#include <QMap>
#include "loadpage.h"

#ifdef YT_USE_SIG
#include "sig.h"
#endif

class QSettings;

#define YT_GET_VIDEOINFO
#define YT_DASH_SUPPORT
#define YT_LIVE_STREAM

#ifdef YT_GET_VIDEOINFO
//#define YT_DISCARD_HTTPS
#endif

typedef QMap<int,QString> UrlMap;

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

	void clearData();

	void setPreferredResolution(Resolution r) { preferred_resolution = r; }
	Resolution preferredResolution() { return preferred_resolution; }

	void setUserAgent(const QString & s) { LoadPage::setDefaultUserAgent(s); };
	QString userAgent() { return LoadPage::defaultUserAgent(); };

	void fetchPage(const QString & url);

	#ifdef YT_USE_SIG
	void setSettings(QSettings * settings);
	#endif

	QString urlTitle() { return url_title; }
	QString origUrl() { return yt_url; }

	QString selectedUrl() { return selected_url; }
	Quality selectedQuality() { return selected_quality; }

	#ifdef YT_DASH_SUPPORT
	void setUseDASH(bool b) { use_dash = b; }
	bool useDASH() { return use_dash; }

	QString selectedAudioUrl() { return selected_audio_url; }
	Quality selectedAudioQuality() { return selected_audio_quality; }
	#endif

	UrlMap urlMap() { return urlmap; }

	bool isUrlSupported(const QString & url);
	QString fullUrl(const QString & url);

	void setUseHttpsMain(bool b) { use_https_main = b; };
	void setUseHttpsVi(bool b) { use_https_vi = b; };
	bool useHttpsMain() { return use_https_main; };
	bool useHttpsVi() { return use_https_vi; };

	static QString extensionForItag(int itag);

	void close() { /* FIXME: do something */ };

signals:
	void gotUrls(const QMap<int, QString>&);
	//void gotPreferredUrl(const QString &);
	void gotPreferredUrl(const QString & url, int itag);
	void gotEmptyList();
	void connecting(QString host);
	void errorOcurred(int error_number, QString error_str);
	void signatureNotFound(const QString & title);
	void noSslSupport();

protected slots:
	void videoPageLoaded(QByteArray page);
	#ifdef YT_GET_VIDEOINFO
	void videoInfoPageLoaded(QByteArray page);
	#endif
	#ifdef YT_USE_SIG
	void playerPageLoaded(QByteArray page);
	#endif
	#ifdef YT_LIVE_STREAM
	void streamPageLoaded(QByteArray page);
	#endif
	/* void receivedResponse303(QString); */

	void processVideoPage();

protected:
	void fetchVideoPage(const QString & url);
	#ifdef YT_GET_VIDEOINFO
	void fetchVideoInfoPage(const QString & url);
	#endif
	#ifdef YT_USE_SIG
	void fetchPlayerPage(const QString & player_name);
	#endif
	#ifdef YT_LIVE_STREAM
	void fetchStreamPage(const QString & url);
	#endif

	void setUrlMap(const UrlMap & map) { urlmap = map; }

	QString getVideoID(QString video_url);
	UrlMap extractURLs(QString fmtArray, bool allow_https, bool use_player);

	void finish(const UrlMap & url_map);

	#ifdef YT_USE_SIG
	QString aclara(const QString & text, const QString & player = "");
	#endif

	static Quality findResolution(const UrlMap & url_map, QList<Quality> l);
	static Quality findPreferredResolution(const UrlMap & url_map, Resolution res, bool use_dash = false);
	#ifdef YT_DASH_SUPPORT
	static Quality findBestAudio(const QMap<int, QString>& url_map);
	#endif

	static QString sanitizeForUnicodePoint(QString string);

private:
	QNetworkAccessManager* manager;
	LoadPage * dl_video_page;

	#ifdef YT_GET_VIDEOINFO
	LoadPage * dl_video_info_page;
	#endif

	#ifdef YT_USE_SIG
	LoadPage * dl_player_page;
	Sig sig;
	QSettings * set;
	#endif

	#ifdef YT_LIVE_STREAM
	LoadPage * dl_stream_page;
	#endif

	Resolution preferred_resolution;

	bool use_https_main;
	bool use_https_vi;

	QString yt_url;
	QString url_title;

	QString selected_url;
	Quality selected_quality;

	#ifdef YT_DASH_SUPPORT
	QString selected_audio_url;
	Quality selected_audio_quality;
	bool use_dash;
	#endif

	UrlMap urlmap;

	QString video_page;
	bool failed_to_decrypt_signature;
};

#endif

