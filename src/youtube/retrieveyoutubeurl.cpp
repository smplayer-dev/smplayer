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

#include "retrieveyoutubeurl.h"
#include "loadpage.h"

#include <QUrl>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QSslSocket>

#ifdef YT_USE_YTSIG
#include "ytsig.h"
#endif

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif


RetrieveYoutubeUrl::RetrieveYoutubeUrl(QObject* parent)
	: QObject(parent)
#ifdef YT_USE_SIG
	, set(0)
#endif
	, preferred_quality(MP4_360p)
	, use_https_main(false)
	, use_https_vi(false)
{
	manager = new QNetworkAccessManager(this);

	dl_video_page = new LoadPage(manager, this);
	connect(dl_video_page, SIGNAL(pageLoaded(QByteArray)), this, SLOT(videoPageLoaded(QByteArray)));
	connect(dl_video_page, SIGNAL(errorOcurred(int, QString)), this, SIGNAL(errorOcurred(int, QString)));

#ifdef YT_GET_VIDEOINFO
	dl_video_info_page = new LoadPage(manager, this);
	connect(dl_video_info_page, SIGNAL(pageLoaded(QByteArray)), this, SLOT(videoInfoPageLoaded(QByteArray)));
	connect(dl_video_info_page, SIGNAL(errorOcurred(int, QString)), this, SIGNAL(errorOcurred(int, QString)));
#endif

#ifdef YT_USE_SIG
	dl_player_page = new LoadPage(manager, this);
	connect(dl_player_page, SIGNAL(pageLoaded(QByteArray)), this, SLOT(playerPageLoaded(QByteArray)));
	connect(dl_player_page, SIGNAL(errorOcurred(int, QString)), this, SIGNAL(errorOcurred(int, QString)));
#endif

#ifdef YT_LIVE_STREAM
	dl_stream_page = new LoadPage(manager, this);
	connect(dl_stream_page, SIGNAL(pageLoaded(QByteArray)), this, SLOT(streamPageLoaded(QByteArray)));
	connect(dl_stream_page, SIGNAL(errorOcurred(int, QString)), this, SIGNAL(errorOcurred(int, QString)));
#endif
}

RetrieveYoutubeUrl::~RetrieveYoutubeUrl() {
}

#ifdef YT_USE_SIG
void RetrieveYoutubeUrl::setSettings(QSettings * settings) {
	set = settings;
	sig.load(set);
}
#endif

bool RetrieveYoutubeUrl::isUrlSupported(const QString & url) {
	return (!getVideoID(url).isEmpty());
}

QString RetrieveYoutubeUrl::fullUrl(const QString & url) {
	QString r;
	QString ID = getVideoID(url);
	if (!ID.isEmpty()) {
		QString scheme = use_https_main ? "https" : "http";
		r = scheme + "://www.youtube.com/watch?v=" + ID;
	}
	return r;
}

QString RetrieveYoutubeUrl::getVideoID(QString video_url) {
	//qDebug() << "RetrieveYoutubeUrl::getVideoID: video_url:" << video_url;

	if (video_url.contains("m.youtube.com")) {
		video_url.replace("m.youtube.com", "www.youtube.com");
	}

	if (video_url.startsWith("youtube.com") || video_url.startsWith("www.youtube.com") ||
		video_url.startsWith("youtu.be") || video_url.startsWith("www.youtu.be") || 
		video_url.startsWith("y2u.be") || video_url.startsWith("www.y2u.be"))
	{
		video_url = "http://" + video_url;
	}

	//qDebug() << "RetrieveYoutubeUrl::getVideoID: fixed url:" << video_url;

	QUrl url(video_url);

	QString ID;

	#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery(url);
	#else
	const QUrl * q = &url;
	#endif

	/*
	qDebug() << "host:" << url.host();
	qDebug() << "path:" << url.path();
	*/

	if (url.host() == "youtu.be" || url.host() == "y2u.be") {
		ID = url.path();
		if (ID.startsWith("/")) ID = ID.mid(1);
	}
	else
	if ((url.host().contains("youtube")) && (url.path().contains("watch_videos"))) {
		if (q->hasQueryItem("video_ids")) {
			int index = 0;
			if (q->hasQueryItem("index")) index = q->queryItemValue("index").toInt();
			QStringList list = q->queryItemValue("video_ids").split(",");
			if (index < list.count()) ID = list[index];
		}
	}
	else
	if ((url.host().contains("youtube")) && (url.path().contains("watch"))) {
		if (q->hasQueryItem("v")) {
			ID = q->queryItemValue("v");
		}
	}

	#if QT_VERSION >= 0x050000
	delete q;
	#endif

	qDebug() << "RetrieveYoutubeUrl::getVideoID: ID:" << ID;

	return ID;
}

void RetrieveYoutubeUrl::fetchPage(const QString & url) {
	yt_url = url;
	fetchVideoPage(url);
	//fetchVideoInfoPage(url);
}

void RetrieveYoutubeUrl::fetchVideoPage(const QString & url) {
	qDebug() << "RetrieveYoutubeUrl::fetchVideoPage: url:" << url;

	if (url.toLower().startsWith("https") && !QSslSocket::supportsSsl()) {
		qDebug() << "RetrieveYoutubeUrl::fetchVideoPage: no support for ssl";
		emit noSslSupport();
		return;
	}

	dl_video_page->fetchPage(url);

	emit connecting(QUrl(url).host());
};

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::fetchVideoInfoPage(const QString & url) {
	video_id = getVideoID(url);

	QString scheme = use_https_vi ? "https" : "http";
	QString u = QString("%2://www.youtube.com/get_video_info?video_id=%1&el=leanback&ps=default&eurl=&gl=US&hl=en").arg(video_id).arg(scheme);

	qDebug() << "RetrieveYoutubeUrl::fetchVideoInfoPage: url:" << url.left(20);

	if (u.toLower().startsWith("https") && !QSslSocket::supportsSsl()) {
		qDebug() << "RetrieveYoutubeUrl::fetchVideoInfoPage: no support for ssl";
		emit noSslSupport();
		return;
	}

	#if defined(YT_USE_YTSIG) && !defined(YT_USE_SIG)
	YTSig::check(u);
	#endif

	#ifdef YT_USE_SIG
	/*
	if (!sig.sts.isEmpty()) {
		u = u + "&amp;sts=" + sig.sts;
	}
	*/
	#ifdef YT_USE_YTSIG
	else {
		YTSig::check(u);
	}
	#endif
	#endif

	dl_video_info_page->fetchPage(u);

	emit connecting(QUrl(u).host());
}
#endif

#ifdef YT_USE_SIG
void RetrieveYoutubeUrl::fetchPlayerPage(const QString & player_name) {
	qDebug() << "RetrieveYoutubeUrl::fetchPlayerPage:" << player_name;

	if (!player_name.isEmpty()) {
		QString url;
		/*
		if (player_name.startsWith("new-")) {
			url = QString("http://s.ytimg.com/yts/jsbin/html5player-%1/html5player-new.js").arg(player_name);
		} else {
			url = QString("http://s.ytimg.com/yts/jsbin/html5player-%1/html5player.js").arg(player_name);
		}
		*/
		url = QString("http://s.ytimg.com/yts/jsbin/player-%1/base.js").arg(player_name);
		qDebug() << "RetrieveYoutubeUrl::fetchPlayerPage: url:" << url;
		dl_player_page->fetchPage(url);
	}
}
#endif

#ifdef YT_LIVE_STREAM
void RetrieveYoutubeUrl::fetchStreamPage(const QString & url) {
	qDebug() << "RetrieveYoutubeUrl::fetchStreamPage:" << url;
	dl_stream_page->fetchPage(url);
}
#endif

void RetrieveYoutubeUrl::videoPageLoaded(QByteArray page) {
	qDebug() << "RetrieveYoutubeUrl::videoPageLoaded";

	QString replyString = QString::fromUtf8(page);

	QRegExp rx_title(".*<title>(.*)</title>.*");
	if (rx_title.indexIn(replyString) != -1) {
		url_title = rx_title.cap(1).simplified();
		url_title = QString(url_title).replace("&amp;","&").replace("&gt;", ">").replace("&lt;", "<").replace("&quot;","\"").replace("&#39;","'");
		qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: title:" << url_title;
	} else {
		url_title = "YouTube video";
	}

	#ifdef YT_USE_SIG
	QString html5_player;
	#endif
	QRegExp rxplayer("jsbin\\/player-([\\d\\w-]+)\\/([a-z]{2}_[A-Z]{2})\\/base\\.js");
	if (rxplayer.indexIn(replyString) != -1) {
		QString player = rxplayer.cap(1);
		QString locale = rxplayer.cap(2);
		qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: html5player:" << player << "locale:" << locale;
		html5_player = player +"/"+ locale;
	}

	video_page = replyString;

	#ifdef YT_USE_SIG
	if (!html5_player.isEmpty() && html5_player != sig.html5_player) {
		sig.clear();
		sig.html5_player = html5_player;
		fetchPlayerPage(html5_player);
	} else {
		processVideoPage();
	}
	#else
	processVideoPage();
	#endif
}

void RetrieveYoutubeUrl::processVideoPage() {
	QString replyString = video_page;

#ifdef YT_LIVE_STREAM
	QRegExp rxhlsvp("\"hlsvp\":\"([a-zA-Z0-9\\\\\\/_%\\+:\\.-]+)\"");
	if (rxhlsvp.indexIn(replyString) != -1) {
		QString hlsvp = QUrl::fromPercentEncoding(rxhlsvp.cap(1).toLatin1()).replace("\\/", "/");
		qDebug() << "RetrieveYoutubeUrl::processVideoPage: hlsvp:" << hlsvp;

		if (!hlsvp.isEmpty()) {
			fetchStreamPage(hlsvp);
			return;
		}
	}
#endif

	QString fmtArray;
	QRegExp regex("\\\"url_encoded_fmt_stream_map\\\"\\s*:\\s*\\\"([^\\\"]*)");
	if (regex.indexIn(replyString) != -1) {
		fmtArray = regex.cap(1);
	}

	#ifdef YT_DASH_SUPPORT
	QRegExp regex2("\\\"adaptive_fmts\\\"\\s*:\\s*\\\"([^\\\"]*)");
	if (regex2.indexIn(replyString) != -1) {
		if (!fmtArray.isEmpty()) fmtArray += ",";
		fmtArray += regex2.cap(1);
	}
	#endif

	fmtArray = sanitizeForUnicodePoint(fmtArray);
	fmtArray.replace(QRegExp("\\\\(.)"), "\\1");

	//qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: fmtArray:" << fmtArray;

	#ifdef YT_DISCARD_HTTPS
	bool allow_https = false;
	#else
	bool allow_https = true;
	#endif

	UrlMap url_map = extractURLs(fmtArray, allow_https, true);
	#ifdef YT_GET_VIDEOINFO
	if (url_map.isEmpty()) {
		fetchVideoInfoPage(yt_url);
	} else {
		finish(url_map);
	}
	#else
	finish(url_map);
	#endif
}

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::videoInfoPageLoaded(QByteArray page) {
	qDebug() << "RetrieveYoutubeUrl::videoInfoPageLoaded";

	#if QT_VERSION >= 0x050000
	QUrlQuery all;
	all.setQuery(page);
	#else
	QUrl all;
	all.setEncodedQuery(page);
	#endif

	QByteArray fmtArray;
	#if QT_VERSION >= 0x050000
	fmtArray = all.queryItemValue("url_encoded_fmt_stream_map", QUrl::FullyDecoded).toLatin1();
	#else
	fmtArray = all.queryItemValue("url_encoded_fmt_stream_map").toLatin1();
	#endif

#ifdef YT_DASH_SUPPORT
	if (!fmtArray.isEmpty()) fmtArray += ",";
	#if QT_VERSION >= 0x050000
	fmtArray += all.queryItemValue("adaptive_fmts", QUrl::FullyDecoded).toLatin1();
	#else
	fmtArray += all.queryItemValue("adaptive_fmts").toLatin1();
	#endif
#endif

	//qDebug() <<"RetrieveYoutubeUrl::videoInfoPageLoaded: fmtArray:" << fmtArray;

	UrlMap url_map = extractURLs(fmtArray, true, false);

	if ((url_map.count() == 0) && (failed_to_decrypt_signature)) {
		qDebug() << "RetrieveYoutubeUrl::videoInfoPageLoaded: no url found with valid signature";
		emit signatureNotFound(url_title);
		return;
	}

	finish(url_map);
}
#endif

#ifdef YT_USE_SIG
void RetrieveYoutubeUrl::playerPageLoaded(QByteArray page) {
	qDebug() << "RetrieveYoutubeUrl::playerPageLoaded";

	QString replyString = QString::fromUtf8(page);
	QString signature_code = sig.findFunctions(replyString);
	qDebug() << "RetrieveYoutubeUrl::playerPageLoaded: signature_code:" << signature_code;

	if (!signature_code.isEmpty() && set) sig.save(set);

	processVideoPage();
}
#endif

#ifdef YT_LIVE_STREAM
void RetrieveYoutubeUrl::streamPageLoaded(QByteArray page) {
	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded";

	//qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: page:" << page;

	QRegExp rx("#EXT-X-STREAM-INF:.*RESOLUTION=\\d+x(\\d+)");

	QMap<int, QString> url_map;
	int best_resolution = 0;
	int res_height = 0;

	QTextStream stream(page);
	QString line;
	do {
		line = stream.readLine();
		if (!line.isEmpty()) {
			//qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: line:" << line;
			if (rx.indexIn(line) != -1) {
				res_height = rx.cap(1).toInt();
				qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: height:" << res_height;
				if (res_height > best_resolution) best_resolution = res_height;
			}
			else
			if (!line.startsWith("#") && res_height != 0) {
				url_map[res_height] = line;
				res_height = 0;
			}
		}
	} while (!line.isNull());

	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: best_resolution:" << best_resolution;

	// Try to find a URL with the user's preferred quality
	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: preferred_quality:" << preferred_quality;

	int selected_quality = 0;
	int q = preferred_quality;

	if (q == WEBM_1080p || q == MP4_1080p) {
		if (url_map.contains(1080)) {
			selected_quality = 1080;
		} else q = MP4_720p;
	}

	if (q == WEBM_720p || q == MP4_720p) {
		if (url_map.contains(720)) {
			selected_quality = 720;
		} else q = WEBM_480p;
	}

	if (q == WEBM_480p || q == FLV_480p) {
		if (url_map.contains(480)) {
			selected_quality = 480;
		} else q = MP4_360p;
	}

	if (q == WEBM_360p || q == FLV_360p || q == MP4_360p) {
		if (url_map.contains(360)) {
			selected_quality = 360;
		} else q = FLV_240p;
	}

	if (q == FLV_240p) {
		if (url_map.contains(240)) {
			selected_quality = 240;
		}
	}

	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: selected_quality:" << selected_quality;

	if (selected_quality == 0) selected_quality = best_resolution;

	if (url_map.contains(selected_quality)) {
		QString p_url = url_map.value(selected_quality);
		qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: p_url:" << p_url;
		emit gotPreferredUrl(p_url, 0);
		latest_preferred_url = p_url;
	} else {
		 emit gotEmptyList();
	}
}
#endif

void RetrieveYoutubeUrl::finish(const UrlMap & url_map) {
	qDebug() << "RetrieveYoutubeUrl::finish";

	int itag = findPreferredUrl(url_map, preferred_quality);

	QString p_url;
	if (itag != -1) p_url = url_map[itag];
	qDebug() << "RetrieveYoutubeUrl::finish: p_url:" << p_url;

	latest_preferred_url = p_url;

	#if 0 && defined(YT_DASH_SUPPORT)
	// Test findBestAudio
	{
		int itag = findBestAudio(url_map);
		QString audio_url;
		if (itag != -1) audio_url = url_map[itag];
		qDebug() << "RetrieveYoutubeUrl::finish: audio itag:" << itag;
		qDebug() << "RetrieveYoutubeUrl::finish: audio url:" << audio_url;
	}
	#endif

	if (!p_url.isNull()) {
		emit gotUrls(url_map);
		//emit gotPreferredUrl(p_url);
		emit gotPreferredUrl(p_url, itag);
	} else {
		 emit gotEmptyList();
	}
}

#ifdef YT_USE_SCRIPT
QString RetrieveYoutubeUrl::aclara(const QString & text, const QString & player) {
	QString res;

	#if defined(YT_USE_YTSIG) && !defined(YT_USE_SIG)
	if (!player.isNull()) {
		res = YTSig::aclara(text, player);
	} else {
		res = YTSig::aclara(text, "", "aclara_f");
	}
	#endif

	#ifdef YT_USE_SIG
	if (!sig.decrypt_function.isEmpty()) {
		res = sig.aclara(text);
	}
	#ifdef YT_USE_YTSIG
	else {
		if (!player.isNull()) {
			res = YTSig::aclara(text, player);
		} else {
			res = YTSig::aclara(text, "", "aclara_f");
		}
	}
	#endif
	#endif

	return res;
}
#endif

UrlMap RetrieveYoutubeUrl::extractURLs(QString fmtArray, bool allow_https, bool use_player) {
	UrlMap url_map;

	failed_to_decrypt_signature = false;

	#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery();
	#endif

	//qDebug() << "RetrieveYoutubeUrl::extractURLs: fmtArray:" << fmtArray;

	QList<QByteArray> codeList = fmtArray.toLatin1().split(',');
	//qDebug() << "RetrieveYoutubeUrl::extractURLs: codeList.count:" << codeList.count();

	foreach(QByteArray code, codeList) {
		code = QUrl::fromPercentEncoding(code).toLatin1();
		//qDebug() << "RetrieveYoutubeUrl::extractURLs: code:" << code;

		QUrl line;
		#if QT_VERSION >= 0x050000
		q->setQuery(code);
		#else
		QUrl * q = &line;
		q->setEncodedQuery(code);
		#endif

		if (q->hasQueryItem("url")) {
			QUrl url( q->queryItemValue("url") );
			line.setScheme(url.scheme());
			line.setHost(url.host());
			line.setPath(url.path());
			q->removeQueryItem("url");
			#if QT_VERSION >= 0x050000
			q->setQuery( q->query(QUrl::FullyDecoded) + "&" + url.query(QUrl::FullyDecoded) );
			#else
			q->setEncodedQuery( q->encodedQuery() + "&" + url.encodedQuery() );
			#endif

			if (q->hasQueryItem("sig")) {
				q->addQueryItem("signature", q->queryItemValue("sig"));
				q->removeQueryItem("sig");
			}
			else
			if (q->hasQueryItem("s")) {
				#ifdef YT_USE_SCRIPT
				#ifdef YT_USE_SIG
				QString player = sig.html5_player;
				#else
				QString player = html5_player;
				#endif
				QString signature = aclara(q->queryItemValue("s"), use_player ? player : QString::null);
				if (!signature.isEmpty()) {
					q->addQueryItem("signature", signature);
				} else {
					failed_to_decrypt_signature = true;
				}
				#else // YT_USE_SCRIPT
				failed_to_decrypt_signature = true;
				#endif
				q->removeQueryItem("s");
			}
			q->removeAllQueryItems("fallback_host");
			q->removeAllQueryItems("type");

			if (!q->hasQueryItem("ratebypass")) q->addQueryItem("ratebypass", "yes");

			if ((q->hasQueryItem("itag")) && (q->hasQueryItem("signature"))) {
				QString itag = q->queryItemValue("itag");

				// Remove duplicated queries
				QPair <QString,QString> item;
				QList<QPair<QString, QString> > items = q->queryItems();
				foreach(item, items) {
					q->removeAllQueryItems(item.first);
					q->addQueryItem(item.first, item.second);
				}

				#if QT_VERSION >= 0x050000
				line.setQuery(q->query(QUrl::FullyDecoded));
				#endif

				if (!line.toString().startsWith("https") || allow_https) {
					url_map[itag.toInt()] = line.toString();
				} else {
					qDebug() << "RetrieveYoutubeUrl::extractURLs: discarted url with itag:" << itag.toInt();
				}

				//qDebug() << "RetrieveYoutubeUrl::extractURLs: itag:" << itag << "line:" << line.toString();
			}
		}
	}

	#if QT_VERSION >= 0x050000
	delete q;
	#endif

	qDebug() << "RetrieveYoutubeUrl::extractURLs: url count:" << url_map.count();

	return url_map;
}

int RetrieveYoutubeUrl::findPreferredUrl(const UrlMap & url_map, Quality q) {
	// Choose a url according to preferred quality
	QString p_url;
	//Quality q = preferred_quality;

	int chosen_quality = -1;

	#define SETPURL(QUALITY) { \
			p_url= url_map.value(QUALITY, QString()); \
			if (!p_url.isNull()) chosen_quality = QUALITY; \
		}


	if (q==MP4_1080p) {
		SETPURL(MP4_1080p)
		if (p_url.isNull()) SETPURL(WEBM_1080p)
		if (p_url.isNull()) q = MP4_720p;
	}

	if (q==WEBM_1080p) {
		SETPURL(WEBM_1080p)
		if (p_url.isNull()) SETPURL(MP4_1080p)
		if (p_url.isNull()) q = WEBM_720p;
	}

	if (q==MP4_720p) {
		SETPURL(MP4_720p)
		if (p_url.isNull()) SETPURL(WEBM_720p)
		if (p_url.isNull()) SETPURL(WEBM_480p)
		if (p_url.isNull()) q = MP4_360p;
	}

	if (q==WEBM_720p) {
		SETPURL(WEBM_720p)
		if (p_url.isNull()) SETPURL(MP4_720p)
		if (p_url.isNull()) q = WEBM_480p;
	}

	if (q==WEBM_480p) {
		SETPURL(WEBM_480p)
		if (p_url.isNull()) q = WEBM_360p;
	}

	if (q==MP4_360p) {
		SETPURL(MP4_360p)
		if (p_url.isNull()) SETPURL(WEBM_360p)
		if (p_url.isNull()) q = FLV_360p;
	}

	if (q==WEBM_360p) {
		SETPURL(WEBM_360p)
		if (p_url.isNull()) SETPURL(MP4_360p)
		if (p_url.isNull()) q = FLV_360p;
	}

	// FLV, low priority
	if (q==FLV_480p) {
		SETPURL(FLV_480p)
		if (p_url.isNull()) q = FLV_360p;
	}

	if (q==FLV_360p) {
		SETPURL(FLV_360p)
		if (p_url.isNull()) q = FLV_240p;
	}

	if (q==FLV_240p) {
		SETPURL(q)
	}

	// If everything fails, take the first url in the map
	if (p_url.isEmpty()) {
		QList<int> keys = url_map.keys();
		if (!keys.isEmpty()) SETPURL(keys[0])
	}
	
	qDebug("RetrieveYoutubeUrl::findPreferredUrl: chosen_quality: %d", chosen_quality);
	return chosen_quality;
}

QString RetrieveYoutubeUrl::sanitizeForUnicodePoint(QString string) {
	QRegExp rx("\\\\u(\\d{4})");
	while (rx.indexIn(string) != -1) {
		string.replace(rx.cap(0), QString(QChar(rx.cap(1).toInt(0,16))));
	}
	return string;
}

QString RetrieveYoutubeUrl::extensionForItag(int itag) {
	QString ext = ".mp4";
	switch (itag) {
		case RetrieveYoutubeUrl::FLV_240p:
		case RetrieveYoutubeUrl::FLV_360p:
		case RetrieveYoutubeUrl::FLV_480p:
			ext = ".flv";
			break;
		case RetrieveYoutubeUrl::WEBM_360p:
		case RetrieveYoutubeUrl::WEBM_480p:
		case RetrieveYoutubeUrl::WEBM_720p:
		case RetrieveYoutubeUrl::WEBM_1080p:
			ext = ".webm";
			break;
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_48:
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_128:
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_256:
			ext = ".m4a";
			break;
		case RetrieveYoutubeUrl::DASH_AUDIO_WEBM_128:
		case RetrieveYoutubeUrl::DASH_AUDIO_WEBM_192:
			ext = ".webm";
			break;
	}

	return ext;
}

#ifdef YT_DASH_SUPPORT
int RetrieveYoutubeUrl::findBestAudio(const QMap<int, QString>& url_map) {
	QString url;

	#define CHECKAQ(QUALITY) { \
			url = url_map.value(QUALITY, QString()); \
			if (!url.isNull()) return QUALITY; \
		}

	CHECKAQ(DASH_AUDIO_MP4_256);
	CHECKAQ(DASH_AUDIO_WEBM_192);
	CHECKAQ(DASH_AUDIO_MP4_128);
	CHECKAQ(DASH_AUDIO_WEBM_128);
	CHECKAQ(DASH_AUDIO_MP4_48);

	return -1;
}
#endif

#include "moc_retrieveyoutubeurl.cpp"
