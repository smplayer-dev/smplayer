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

//#define DEBUG_OUTPUT_PAGE
#ifdef DEBUG_OUTPUT_PAGE
#include <QFile>
 #if QT_VERSION >= 0x050000
 #include <QStandardPaths>
 #else
 #include <QDesktopServices>
 #endif
#endif

#define SUPPORT_OLD_FORMAT


RetrieveYoutubeUrl::RetrieveYoutubeUrl(QObject* parent)
	: QObject(parent)
#ifdef YT_USE_SIG
	, set(0)
#endif
	, preferred_resolution(R720p)
	, use_https_main(false)
#ifdef YT_DASH_SUPPORT
	, use_dash(false)
	, use_60fps(false)
#endif
{
	clearData();
	manager = new QNetworkAccessManager(this);

	dl_video_page = new LoadPage(manager, this);
	connect(dl_video_page, SIGNAL(pageLoaded(QByteArray)), this, SLOT(videoPageLoaded(QByteArray)));
	connect(dl_video_page, SIGNAL(errorOcurred(int, QString)), this, SIGNAL(errorOcurred(int, QString)));
	/* connect(dl_video_page, SIGNAL(response303(QString)), this, SLOT(receivedResponse303(QString))); */

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

void RetrieveYoutubeUrl::clearData() {
	selected_url = "";
	selected_quality = None;

#ifdef YT_DASH_SUPPORT
	selected_audio_url = "";
	selected_audio_quality = None;
#endif

	yt_url = "";
	url_title = "";

	urlmap.clear();
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
	clearData();

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

/*
void RetrieveYoutubeUrl::receivedResponse303(QString url) {
	qDebug() << "RetrieveYoutubeUrl::receivedResponse303:" << url;
	fetchPage(url);
}
*/

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::fetchVideoInfoPage(const QString & url) {
	QString video_id = getVideoID(url);

	QString u = QString("https://www.youtube.com/get_video_info?video_id=%1&disable_polymer=true&eurl=https://youtube.googleapis.com/v/%1&gl=US&hl=en").arg(video_id);
	qDebug() << "RetrieveYoutubeUrl::fetchVideoInfoPage: url:" << u;

	if (u.toLower().startsWith("https") && !QSslSocket::supportsSsl()) {
		qDebug() << "RetrieveYoutubeUrl::fetchVideoInfoPage: no support for ssl";
		emit noSslSupport();
		return;
	}

	#ifdef YT_USE_SIG
	if (!sig.sts.isEmpty()) {
		u += "&sts=" + sig.sts;
	}
	#endif

	dl_video_info_page->fetchPage(u);

	emit connecting(QUrl(u).host());
}
#endif

#ifdef YT_USE_SIG
void RetrieveYoutubeUrl::fetchPlayerPage(const QString & player_name) {
	qDebug() << "RetrieveYoutubeUrl::fetchPlayerPage:" << player_name;

	if (!player_name.isEmpty()) {
		QString url = Sig::playerURL(player_name);
		qDebug() << "RetrieveYoutubeUrl::fetchPlayerPage: url:" << url;
		#ifdef SIG_FROM_SMPLAYER_SITE
		if (dl_player_page->userAgent().isEmpty()) dl_player_page->setUserAgent("SMPlayer");
		#endif
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

	// Find video title
	url_title = "";
	QStringList rx_title_patterns;
	rx_title_patterns << "\"title\":\"(.*)\"" << ".*<title>(.*)</title>.*";
	for (int n = 0; n < rx_title_patterns.count(); n++) {
		QRegExp rx(rx_title_patterns[n]);
		rx.setMinimal(true);
		if (rx.indexIn(replyString) != -1) {
			url_title = rx.cap(1);
			qDebug("RetrieveYoutubeUrl::videoPageLoaded: title found with pattern %d", n+1);
			break;
		}
	}

	if (!url_title.isEmpty()) {
		url_title = sanitizeForUnicodePoint(url_title);
		url_title = url_title.simplified();
		url_title = QString(url_title).replace("&amp;","&").replace("&gt;", ">").replace("&lt;", "<").replace("&quot;","\"").replace("&#39;","'");
	} else {
		url_title = "YouTube video";
	}
	qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: title:" << url_title;

	// Find video player
	QString html5_player;
	QStringList rx_player_patterns;
	rx_player_patterns << "\"(?:js|jsUrl)\":\"(\\S+)\\/player(.*)base\\.js\"" << "script\\s+src=\"(\\S+)\\/player(.*)base\\.js\"";
	for (int n = 0; n < rx_player_patterns.count(); n++) {
		QRegExp rx(rx_player_patterns[n]);
		rx.setMinimal(true);
		if (rx.indexIn(replyString) != -1) {
			html5_player = rx.cap(1) + "/player"+ rx.cap(2);
			qDebug("RetrieveYoutubeUrl::videoPageLoaded: player found with pattern %d", n+1);
			break;
		}
	}

	if (!html5_player.isEmpty()) {
		html5_player = html5_player.replace("\\/", "/");
		qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: html5player:" << html5_player;
	} else {
		qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: player not found!";
		//qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: page:" << page;
		#ifdef DEBUG_OUTPUT_PAGE
		#if QT_VERSION >= 0x050000
		QString tmp_path = QStandardPaths::standardLocations(QStandardPaths::TempLocation)[0];
		#else
		QString tmp_path = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
		#endif
		QString output = tmp_path + "/smplayer_yt_page.html";
		QFile f(output);
		if (f.open(QIODevice::ReadWrite)) {
			f.write(page);
			f.close();
			qDebug() << "RetrieveYoutubeUrl::videoPageLoaded: page saved to" << output;
		}
		#endif
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

	QRegExp manifest_rx("\\\\\"hlsManifestUrl\\\\\":\\\\\"(.*)\\\\\"");
	manifest_rx.setMinimal(true);
	if (manifest_rx.indexIn(replyString) != -1) {
		QString manifest_url = QUrl::fromPercentEncoding(manifest_rx.cap(1).toLatin1()).replace("\\/", "/");
		qDebug() << "RetrieveYoutubeUrl::processVideoPage:" << manifest_url;

		if (!manifest_url.isEmpty()) {
			fetchStreamPage(manifest_url);
			return;
		}
	}
#endif

	QString fmtArray;
#ifdef SUPPORT_OLD_FORMAT
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
#endif

	if (fmtArray.isEmpty()) fmtArray = getArrayFromStreamingData(replyString);

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

QString RetrieveYoutubeUrl::getArrayFromStreamingData(const QString & page) {
	qDebug("RetrieveYoutubeUrl::getArrayFromStreamingData");

	QString fmtArray;

	QStringList exps;
	exps << "streamingData.*formats\\\\\":\\[(.*)\\]";
	#ifdef YT_DASH_SUPPORT
	exps << "streamingData.*adaptiveFormats\\\\\":\\[(.*)\\]";
	#endif

	foreach(QString expression, exps) {
		QString text;
		QRegExp rx_block(expression);
		rx_block.setMinimal(true);
		if (rx_block.indexIn(page) != -1) {
			text = rx_block.cap(1);
			//qDebug() << "text:" << text;

			QRegExp regex("(cipher|url)\\\\\":\\\\\"(.*)\\\\\"");
			regex.setMinimal(true);

			int pos = 0;
			while ((pos = regex.indexIn(text, pos)) != -1) {
				if (!fmtArray.isEmpty()) fmtArray += ",";
				if (regex.cap(1) == "url") {
					QString url = regex.cap(2);
					url = QUrl::fromPercentEncoding(url.toUtf8());
					url = sanitizeForUnicodePoint(url);
					url.replace(QRegExp("\\\\(.)"), "\\1");
					url = QUrl::toPercentEncoding(url);
					fmtArray += "url=";
					fmtArray += url;
				} else {
					fmtArray += regex.cap(2);
				}
				pos += regex.matchedLength();
			}
		}
	}

	//qDebug() << "RetrieveYoutubeUrl::getArrayFromStreamingData: fmtArray:" << fmtArray;
	return fmtArray;
}

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::videoInfoPageLoaded(QByteArray page) {
	qDebug() << "RetrieveYoutubeUrl::videoInfoPageLoaded";

	QByteArray fmtArray;

#ifdef SUPPORT_OLD_FORMAT
	#if QT_VERSION >= 0x050000
	QUrlQuery all;
	all.setQuery(page);
	#else
	QUrl all;
	all.setEncodedQuery(page);
	#endif

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
#endif

	if (fmtArray.isEmpty()) {
		QString s = QUrl::fromPercentEncoding(page);
		s = sanitizeForUnicodePoint(s);
		s.replace("\"", "\\\"");
		fmtArray = getArrayFromStreamingData(s).toLatin1();
	}

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

	UrlMap url_map;
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

	// Try to find a URL with the user's preferred resolution
	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: preferred_resolution:" << preferred_resolution;

	int chosen_quality = 0;
	int r = preferred_resolution;

	if (r == R1080p) {
		if (url_map.contains(1080)) {
			chosen_quality = 1080;
		} else r = R720p;
	}

	if (r == R720p) {
		if (url_map.contains(720)) {
			chosen_quality = 720;
		} else r = R480p;
	}

	if (r == R480p) {
		if (url_map.contains(480)) {
			chosen_quality = 480;
		} else r = R360p;
	}

	if (r == R360p) {
		if (url_map.contains(360)) {
			chosen_quality = 360;
		} else r = R240p;
	}

	if (r == R240p) {
		if (url_map.contains(240)) {
			chosen_quality = 240;
		}
	}

	qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: chosen_quality:" << chosen_quality;

	if (chosen_quality == 0) chosen_quality = best_resolution;

	setUrlMap(url_map);

	if (url_map.contains(chosen_quality)) {
		selected_url = url_map.value(chosen_quality);
		qDebug() << "RetrieveYoutubeUrl::streamPageLoaded: selected_url:" << selected_url;
		emit gotPreferredUrl(selected_url, 0);
	} else {
		 emit gotEmptyList();
	}
}
#endif

void RetrieveYoutubeUrl::finish(const UrlMap & url_map) {
	qDebug() << "RetrieveYoutubeUrl::finish";

	setUrlMap(url_map);

	#ifdef YT_DASH_SUPPORT
	selected_quality = findPreferredResolution(url_map, preferred_resolution, use_dash, use_60fps);
	#else
	selected_quality = findPreferredResolution(url_map, preferred_resolution);
	#endif

	selected_url = "";
	if (selected_quality != None) selected_url = url_map[selected_quality];
	qDebug() << "RetrieveYoutubeUrl::finish: selected_url:" << selected_url;

	#ifdef YT_DASH_SUPPORT
	selected_audio_quality = findBestAudio(url_map);
	selected_audio_url = "";
	if (use_dash) {
		if (selected_audio_quality != None) selected_audio_url = url_map[selected_audio_quality];
		qDebug() << "RetrieveYoutubeUrl::finish: audio itag:" << selected_audio_quality;
		qDebug() << "RetrieveYoutubeUrl::finish: audio url:" << selected_audio_url;
	}
	#endif

	if (selected_quality != None) {
		emit gotUrls(url_map);
		//emit gotPreferredUrl(p_url);
		emit gotPreferredUrl(selected_url, selected_quality);
	} else {
		 emit gotEmptyList();
	}
}

#ifdef YT_USE_SIG
QString RetrieveYoutubeUrl::aclara(const QString & text, const QString & player) {
	Q_UNUSED(player);

	QString res;

	#ifdef YT_USE_SIG
	if (!sig.decrypt_function.isEmpty()) {
		res = sig.aclara(text);
	}
	#endif

	return res;
}
#endif

OptMap RetrieveYoutubeUrl::extractOptions(const QByteArray & urldata) {
	OptMap data;
	QList<QByteArray> l = urldata.split('&');
	foreach(QByteArray o, l) {
		QList<QByteArray> p = o.split('=');
		if (p.count() > 1) {
			QString tag = QUrl::fromPercentEncoding(p[0]);
			QString value = QUrl::fromPercentEncoding(p[1]);
			data[tag] = value;
		}
	}
	return data;
}

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
		//qDebug() << "RetrieveYoutubeUrl::extractURLs: code:" << code;
		OptMap opt_map = extractOptions(code);
		//qDebug() << "RetrieveYoutubeUrl::extractURLs: opt_map:" << opt_map;

		if (opt_map.contains("url")) {
			QUrl url(opt_map["url"]);
			//qDebug() << "RetrieveYoutubeUrl::extractURLs: url:" << url;

			#if QT_VERSION >= 0x050000
			q->setQuery(url.query(QUrl::FullyDecoded));
			#else
			QUrl * q = &url;
			#endif

			QString signature_name = "signature";
			if (opt_map.contains("sp")) {
				signature_name = opt_map["sp"];
				//qDebug() << "RetrieveYoutubeUrl::extractURLs: signature field:" << signature_name;
			}

			if (opt_map.contains("sig")) {
				q->addQueryItem(signature_name, opt_map["sig"]);
			}
			else
			if (opt_map.contains("s")) {
				#ifdef YT_USE_SIG
				QString player = sig.html5_player;
				QString enc_sig = opt_map["s"];
				QString signature = aclara(enc_sig, use_player ? player : QString::null);
				if (!signature.isEmpty()) {
					q->addQueryItem(signature_name, signature);
				} else {
					failed_to_decrypt_signature = true;
				}
				#else
				failed_to_decrypt_signature = true;
				#endif
				q->removeQueryItem("s");
			}
			q->removeAllQueryItems("fallback_host");
			q->removeAllQueryItems("type");
			q->removeAllQueryItems("xtags");

			if (!q->hasQueryItem("ratebypass")) q->addQueryItem("ratebypass", "yes");

			if (opt_map.contains("clen")) {
				if (opt_map["clen"] == "0") {
					qDebug() << "RetrieveYoutubeUrl::extractURLs: discarted url with empty clen";
					continue;
				}
			} else {
				if (!q->hasQueryItem("clen")) {
					qDebug() << "RetrieveYoutubeUrl::extractURLs: discarted url with no clen";
					continue;
				}
			}

			if ((q->hasQueryItem("itag")) /*&& (q->hasQueryItem(signature_name))*/) {
				QString itag = q->queryItemValue("itag");

				// Remove duplicated queries
				QPair <QString,QString> item;
				QList<QPair<QString, QString> > items = q->queryItems();
				foreach(item, items) {
					q->removeAllQueryItems(item.first);
					q->addQueryItem(item.first, item.second);
				}

				QUrl line = url;
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

RetrieveYoutubeUrl::Quality RetrieveYoutubeUrl::findResolution(const UrlMap & url_map, QList<Quality> l) {
	foreach(Quality q, l) {
		QString url = url_map.value(q, QString()); \
		if (!url.isNull()) return q;
	}
	return None;
}

RetrieveYoutubeUrl::Quality RetrieveYoutubeUrl::findPreferredResolution(const UrlMap & url_map, Resolution res, bool use_dash, bool use_60fps) {
	Quality chosen_quality = None;

	QList<Quality> l2160p, l1440p, l1080p, l720p, l480p, l360p, l240p;

	if (!use_dash) {
		l1080p << MP4_1080p << WEBM_1080p;
		l720p << MP4_720p << WEBM_720p;
		l480p << MP4_480p << MP4_480p2 << WEBM_480p << FLV_480p;
		l360p << MP4_360p << WEBM_360p << FLV_360p;
		l240p << FLV_240p << FLV_270p;
	}

#ifdef YT_DASH_SUPPORT
	else {
		#if 0
		l240p << DASH_VIDEO_WEBM_240p60hdr;
		l360p << DASH_VIDEO_WEBM_360p60hdr;
		l480p << DASH_VIDEO_WEBM_480p60hdr;
		l720p << DASH_VIDEO_WEBM_720p60hdr;
		l1080p << DASH_VIDEO_WEBM_1080p60hdr;
		l1440p << DASH_VIDEO_WEBM_1440p60hdr;
		l2160p << DASH_VIDEO_WEBM_2160p60hdr;
		#endif

		if (use_60fps) {
			l720p << DASH_VIDEO_720p60 << DASH_VIDEO_WEBM_720p60;
			l1080p << DASH_VIDEO_1080p60 << DASH_VIDEO_WEBM_1080p60;
			l1440p << DASH_VIDEO_WEBM_1440p60;
			l2160p << DASH_VIDEO_WEBM_2160p60;
		}

		l2160p << DASH_VIDEO_2160p << DASH_VIDEO_2160p2 << DASH_VIDEO_WEBM_2160p << DASH_VIDEO_WEBM_2160p2;
		l1440p << DASH_VIDEO_1440p << DASH_VIDEO_WEBM_1440p;
		l1080p << DASH_VIDEO_1080p << DASH_VIDEO_WEBM_1080p << DASH_VIDEO_WEBM_1080p2;
		l720p << DASH_VIDEO_720p << DASH_VIDEO_WEBM_720p << DASH_VIDEO_WEBM_720p2;
		l480p << DASH_VIDEO_480p << DASH_VIDEO_480p2 << DASH_VIDEO_WEBM_480p << DASH_VIDEO_WEBM_480p2 
              << DASH_VIDEO_WEBM_480p3 << DASH_VIDEO_WEBM_480p4 << DASH_VIDEO_WEBM_480p5 << DASH_VIDEO_WEBM_480p6;
		l360p << DASH_VIDEO_360p << DASH_VIDEO_WEBM_360p << DASH_VIDEO_WEBM_360p2;
		l240p << DASH_VIDEO_240p << DASH_VIDEO_WEBM_240p;
	}
#endif

	if (res == R2160p) {
		chosen_quality = findResolution(url_map, l2160p);
		if (chosen_quality == None) res = R1440p;
	}

	if (res == R1440p) {
		chosen_quality = findResolution(url_map, l1440p);
		if (chosen_quality == None) res = R1080p;
	}

	if (res == R1080p) {
		chosen_quality = findResolution(url_map, l1080p);
		if (chosen_quality == None) res = R720p;
	}

	if (res == R720p) {
		chosen_quality = findResolution(url_map, l720p);
		if (chosen_quality == None) res = R480p;
	}

	if (res == R480p) {
		chosen_quality = findResolution(url_map, l480p);
		if (chosen_quality == None) res = R360p;
	}

	if (res == R360p) {
		chosen_quality = findResolution(url_map, l360p);
		if (chosen_quality == None) res = R240p;
	}

	if (res == R240p) {
		chosen_quality = findResolution(url_map, l240p);
	}

	// If everything fails, take the first url in the map
	if (chosen_quality == None) {
		QList<int> keys = url_map.keys();
		if (!keys.isEmpty()) {
			QString url = url_map.value(keys[0], QString());
			if (!url.isNull()) chosen_quality = (Quality) keys[0];
		}
	}

	qDebug() << "RetrieveYoutubeUrl::findResolution: chosen_quality:" << chosen_quality;
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
RetrieveYoutubeUrl::Quality RetrieveYoutubeUrl::findBestAudio(const QMap<int, QString>& url_map) {
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

	return None;
}
#endif

#include "moc_retrieveyoutubeurl.cpp"
