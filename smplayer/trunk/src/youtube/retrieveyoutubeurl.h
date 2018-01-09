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

//#define YT_GET_VIDEOINFO
//#define YT_DASH_SUPPORT
#define YT_LIVE_STREAM

#ifdef YT_GET_VIDEOINFO
#define YT_DISCARD_HTTPS
#endif

typedef QMap<int,QString> UrlMap;

class RetrieveYoutubeUrl : public QObject
{
	Q_OBJECT

public:
	enum Quality { FLV_240p = 5, MP4_360p = 18, MP4_720p = 22, FLV_360p = 34,
                   FLV_480p = 35, MP4_1080p = 37, WEBM_360p = 43,
                   WEBM_480p = 44, WEBM_720p = 45, WEBM_1080p = 46,
                   DASH_AUDIO_MP4_48 = 139, DASH_AUDIO_MP4_128 = 140, DASH_AUDIO_MP4_256 = 141,
                   DASH_AUDIO_WEBM_128 = 171, DASH_AUDIO_WEBM_192 = 172,
                   DASH_VIDEO_1080p = 137, DASH_VIDEO_720p = 136,
                   DASH_VIDEO_480p = 135, DASH_VIDEO_360p = 134,
                   DASH_VIDEO_240p = 133 };

	RetrieveYoutubeUrl( QObject* parent = 0 );
	~RetrieveYoutubeUrl();

	void setPreferredQuality(Quality q) { preferred_quality = q; }
	Quality preferredQuality() { return preferred_quality; }

	void setUserAgent(const QString & s) { LoadPage::setDefaultUserAgent(s); };
	QString userAgent() { return LoadPage::defaultUserAgent(); };

	void fetchPage(const QString & url);

#ifdef YT_USE_SIG
	void setSettings(QSettings * settings);
#endif

#ifdef YT_DASH_SUPPORT
	static int findBestAudio(const QMap<int, QString>& url_map); // Returns the itag
#endif

	QString urlTitle() { return url_title; }
	QString origUrl() { return yt_url; }

	QString latestPreferredUrl() { return latest_preferred_url; }

	bool isUrlSupported(const QString & url);
	QString fullUrl(const QString & url);

	void setUseHttpsMain(bool b) { use_https_main = b; };
	void setUseHttpsVi(bool b) { use_https_vi = b; };
	bool useHttpsMain() { return use_https_main; };
	bool useHttpsVi() { return use_https_vi; };

	static int findPreferredUrl(const UrlMap & url_map, Quality q); // Returns the itag
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

	QString getVideoID(QString video_url);
	UrlMap extractURLs(QString fmtArray, bool allow_https, bool use_player);

	void finish(const UrlMap & url_map);

#ifdef YT_USE_SCRIPT
	QString aclara(const QString & text, const QString & player = "");
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
#else
	QString html5_player;
#endif

#ifdef YT_LIVE_STREAM
	LoadPage * dl_stream_page;
#endif

	QString video_page;
	QString url_title;

	Quality preferred_quality;
	bool use_https_main;
	bool use_https_vi;

	QString yt_url;
	QString video_id;

	QString latest_preferred_url;

	bool failed_to_decrypt_signature;
};

#endif

