/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2010 Ori Rejwan

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
#include <QUrl>
#include <QRegExp>
#include <QStringList>
#include <QFile>
#include "ytsig.h"

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#define USE_PLAYER_NAME

QString RetrieveYoutubeUrl::user_agent;

RetrieveYoutubeUrl::RetrieveYoutubeUrl( QObject* parent ) : QObject(parent)
{
	reply = 0;
	manager = new QNetworkAccessManager(this);

	preferred_quality = FLV_360p;
}

RetrieveYoutubeUrl::~RetrieveYoutubeUrl() {
}

void RetrieveYoutubeUrl::fetchPage(const QString & url) {
	QString agent = user_agent;
	if (agent.isEmpty()) agent = "Mozilla/5.0 (X11; Linux x86_64; rv:5.0.1) Gecko/20100101 Firefox/5.0.1";
	qDebug("RetrieveYoutubeUrl::fetchPage: user agent: %s", agent.toLatin1().constData());

	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", agent.toLatin1());
	req.setRawHeader("Accept-Language", "en-us,en;q=0.5");
	reply = manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotResponse()));
	orig_url = url;

	emit connecting(QUrl(url).host());

#ifdef YT_GET_VIDEOINFO
	video_id = getVideoID(url);
#endif
}

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::fetchVideoInfoPage() {
	QString url = QString("http://www.youtube.com/get_video_info?el=detailpage&ps=default&eurl=&gl=US&hl=en&video_id=%1").arg(video_id);
	//qDebug("RetrieveYoutubeUrl::fetchVideoInfoPage: url: %s", url.toUtf8().constData());

	YTSig::check(url);
	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", user_agent.toLatin1());
	req.setRawHeader("Accept-Language", "en-us,en;q=0.5");
	reply = manager->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(gotVideoInfoResponse()));

	emit connecting(QUrl(url).host());
}
#endif

void RetrieveYoutubeUrl::close() {
	if (reply) reply->abort();
}

void RetrieveYoutubeUrl::gotResponse() {
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply->error() == QNetworkReply::NoError) {
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		qDebug("RetrieveYoutubeUrl::gotResponse: status: %d", status);
		switch (status) {
			case 301:
			case 302:
			case 307:
				QString r_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
				qDebug("RetrieveYoutubeUrl::gotResponse: redirected: %s", r_url.toLatin1().constData());
				fetchPage(r_url);
				return;
		}
	} else {
		emit errorOcurred((int)reply->error(), reply->errorString());
		return;
	}
	parse(reply->readAll());
}

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::gotVideoInfoResponse() {
	qDebug("RetrieveYoutubeUrl::gotVideoInfoResponse");

	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if (reply->error() != QNetworkReply::NoError) {
		emit errorOcurred((int)reply->error(), reply->errorString());
		return;
	}
	parseVideoInfo(reply->readAll());
}
#endif

void RetrieveYoutubeUrl::parse(QByteArray text) {
	qDebug("RetrieveYoutubeUrl::parse");

	urlMap.clear();

	QString replyString = QString::fromUtf8(text);

	QRegExp rx_title(".*<title>(.*)</title>.*");
	if (rx_title.indexIn(replyString) != -1) {
		url_title = rx_title.cap(1).simplified();
		url_title = QString(url_title).replace("&amp;","&").replace("&gt;", ">").replace("&lt;", "<").replace("&quot;","\"").replace("&#39;","'")/*.replace(" - YouTube", "")*/;
		qDebug("RetrieveYoutubeUrl::parse: title '%s'", url_title.toUtf8().constData());
	} else {
		url_title = "Youtube video";
	}

	//qDebug("RetrieveYoutubeUrl::parse: replyString: %s",replyString.toLatin1().constData());

	QString player;
	QRegExp rxplayer("html5player-([\\d,\\w,-]+)\\.js");
	if (rxplayer.indexIn(replyString) != -1) {
		player = rxplayer.cap(1);
		qDebug("RetrieveYoutubeUrl::parse: html5player: %s", player.toLatin1().constData());
	}

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

#ifndef YT_GET_VIDEOINFO
	bool failed_to_decrypt_signature = false;
#endif

	#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery();
	#endif

	QList<QByteArray> codeList = fmtArray.toLatin1().split(',');
	foreach(QByteArray code, codeList) {
		code = QUrl::fromPercentEncoding(code).toLatin1();
		//qDebug("code: %s", code.constData());

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
			/*
			QList < QPair < QString,QString > > l = q->queryItems();
			for (int n=0; n < l.count(); n++) {
				qDebug("n: %d, %s = %s", n, l[n].first.toLatin1().constData(), l[n].second.toLatin1().constData());
			}
			*/
			#else
			q->setEncodedQuery( q->encodedQuery() + "&" + url.encodedQuery() );
			/*
			QList < QPair < QString,QString > > l = q->queryItems();
			for (int n=0; n < l.count(); n++) {
				qDebug("n: %d, %s = %s", n, l[n].first.toLatin1().constData(), l[n].second.toLatin1().constData());
			}
			*/
			#endif

			if (q->hasQueryItem("sig")) {
				q->addQueryItem("signature", q->queryItemValue("sig"));
				q->removeQueryItem("sig");
			}
			else
			if (q->hasQueryItem("s")) {
				#ifdef USE_PLAYER_NAME
				QString signature = YTSig::aclara(q->queryItemValue("s"), player);
				#else
				QString signature = YTSig::aclara(q->queryItemValue("s"));
				#endif
				if (!signature.isEmpty()) {
					q->addQueryItem("signature", signature);
				} else {
				#ifndef YT_GET_VIDEOINFO
					failed_to_decrypt_signature = true;
				#endif
				}
				q->removeQueryItem("s");
			}
			q->removeAllQueryItems("fallback_host");
			q->removeAllQueryItems("type");

			if ((q->hasQueryItem("itag")) && (q->hasQueryItem("signature"))) {
				QString itag = q->queryItemValue("itag");
				q->removeAllQueryItems("itag"); // Remove duplicated itag
				q->addQueryItem("itag", itag);
				#if QT_VERSION >= 0x050000
				line.setQuery(q->query(QUrl::FullyDecoded));
				#endif
				urlMap[itag.toInt()] = line.toString();
				//qDebug("line: %s", line.toString().toLatin1().constData());
			}
		}
	}

	#if QT_VERSION >= 0x050000
	delete q;
	#endif

	qDebug("RetrieveYoutubeUrl::parse: url count: %d", urlMap.count());

#ifndef YT_GET_VIDEOINFO
	if ((urlMap.count() == 0) && (failed_to_decrypt_signature)) {
		qDebug("RetrieveYoutubeUrl::parse: no url found with valid signature");
		emit signatureNotFound(url_title);
		return;
	}
#else
	if (urlMap.count() == 0) {
		qDebug("RetrieveYoutubeUrl::parse: no url found with valid signature");
		fetchVideoInfoPage();
		return;
	}
#endif

	QString p_url = findPreferredUrl();
	//qDebug("p_url: '%s'", p_url.toLatin1().constData());

	if (!p_url.isNull()) {
		emit gotUrls(urlMap);
		emit gotPreferredUrl(p_url);
	} else {
		 emit gotEmptyList();
	}
}

QString RetrieveYoutubeUrl::getVideoID(QString video_url) {
	if (video_url.contains("youtu.be/")) {
		video_url.replace("youtu.be/", "youtube.com/watch?v=");
	}
	else
	if (video_url.contains("y2u.be/")) {
		video_url.replace("y2u.be/", "youtube.com/watch?v=");
	}
	else
	if (video_url.contains("m.youtube.com")) {
		video_url.replace("m.youtube.com", "www.youtube.com");
	}

	if ((video_url.startsWith("youtube.com")) || (video_url.startsWith("www.youtube.com"))) video_url = "http://" + video_url;

	//qDebug("RetrieveYoutubeUrl::getVideoID: video_url: %s", video_url.toLatin1().constData());

	QUrl url(video_url);

	QString ID;

#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery(url);
#else
	const QUrl * q = &url;
#endif

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

	//qDebug("RetrieveYoutubeUrl::getVideoID: ID: %s", ID.toLatin1().constData());

	return ID;
}

bool RetrieveYoutubeUrl::isUrlSupported(const QString & url) {
	return (!getVideoID(url).isEmpty());
}

QString RetrieveYoutubeUrl::fullUrl(const QString & url) {
	QString r;
	QString ID = getVideoID(url);
	if (!ID.isEmpty()) r = "http://www.youtube.com/watch?v=" + ID;
	return r;
}

#ifdef YT_GET_VIDEOINFO
void RetrieveYoutubeUrl::parseVideoInfo(QByteArray text) {
	urlMap.clear();

	#if QT_VERSION >= 0x050000
	QUrlQuery all;
	all.setQuery(text);
	#else
	QUrl all;
	all.setEncodedQuery(text);
	#endif

	QByteArray fmtArray;
	fmtArray = all.queryItemValue("url_encoded_fmt_stream_map").toLatin1();

#ifdef YT_DASH_SUPPORT
	if (!fmtArray.isEmpty()) fmtArray += ",";
	fmtArray += all.queryItemValue("adaptive_fmts").toLatin1();
#endif

	/*
	qDebug("fmtArray: %s", fmtArray.constData());
	return;
	*/

	bool failed_to_decrypt_signature = false;

	#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery();
	#endif

	QList<QByteArray> codeList = fmtArray.split(',');
	foreach(QByteArray code, codeList) {
		code = QUrl::fromPercentEncoding(code).toLatin1();
		//qDebug("code: %s", code.constData());

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
			/*
			QList < QPair < QString,QString > > l = q->queryItems();
			for (int n=0; n < l.count(); n++) {
				qDebug("n: %d, %s = %s", n, l[n].first.toLatin1().constData(), l[n].second.toLatin1().constData());
			}
			*/
			#else
			q->setEncodedQuery( q->encodedQuery() + "&" + url.encodedQuery() );
			/*
			QList < QPair < QString,QString > > l = q->queryItems();
			for (int n=0; n < l.count(); n++) {
				qDebug("n: %d, %s = %s", n, l[n].first.toLatin1().constData(), l[n].second.toLatin1().constData());
			}
			*/
			#endif

			if (q->hasQueryItem("sig")) {
				q->addQueryItem("signature", q->queryItemValue("sig"));
				q->removeQueryItem("sig");
			}
			else
			if (q->hasQueryItem("s")) {
				QString signature = YTSig::aclara(q->queryItemValue("s"), "", "aclara_f");
				if (!signature.isEmpty()) {
					q->addQueryItem("signature", signature);
				} else {
					failed_to_decrypt_signature = true;
				}
				q->removeQueryItem("s");
			}
			q->removeAllQueryItems("fallback_host");
			q->removeAllQueryItems("type");
			if ((q->hasQueryItem("itag")) && (q->hasQueryItem("signature"))) {
				QString itag = q->queryItemValue("itag");
				q->removeAllQueryItems("itag"); // Remove duplicated itag
				q->addQueryItem("itag", itag);
				#if QT_VERSION >= 0x050000
				line.setQuery(q->query(QUrl::FullyDecoded));
				#endif
				urlMap[itag.toInt()] = line.toString();
				//qDebug("line: %s", line.toString().toLatin1().constData());
			}
		}
	}

	#if QT_VERSION >= 0x050000
	delete q;
	#endif

	qDebug("RetrieveYoutubeUrl::parseVideoInfo: url count: %d", urlMap.count());

	if ((urlMap.count() == 0) && (failed_to_decrypt_signature)) {
		qDebug("RetrieveYoutubeUrl::parseVideoInfo: no url found with valid signature");
		emit signatureNotFound(url_title);
		return;
	}

	QString p_url = findPreferredUrl();
	//qDebug("p_url: '%s'", p_url.toLatin1().constData());

	if (!p_url.isNull()) {
		emit gotUrls(urlMap);
		emit gotPreferredUrl(p_url);
	} else {
		 emit gotEmptyList();
	}
}
#endif

QString RetrieveYoutubeUrl::findPreferredUrl() {
	latest_preferred_url = findPreferredUrl(urlMap, preferred_quality);
	return latest_preferred_url;
}

QString RetrieveYoutubeUrl::findPreferredUrl(const QMap<int, QString>& urlMap, Quality q) {
	// Choose a url according to preferred quality
	QString p_url;
	//Quality q = preferred_quality;

	if (q==MP4_1080p) {
		p_url = urlMap.value(MP4_1080p, QString());
		if (p_url.isNull()) p_url= urlMap.value(WEBM_1080p, QString());
		if (p_url.isNull()) q = MP4_720p;
	}

	if (q==WEBM_1080p) {
		p_url = urlMap.value(WEBM_1080p, QString());
		if (p_url.isNull()) p_url= urlMap.value(MP4_1080p, QString());
		if (p_url.isNull()) q = WEBM_720p;
	}

	if (q==MP4_720p) {
		p_url = urlMap.value(MP4_720p, QString());
		if (p_url.isNull()) p_url= urlMap.value(WEBM_720p, QString());
		if (p_url.isNull()) p_url = urlMap.value(WEBM_480p, QString());
		if (p_url.isNull()) q = MP4_360p;
	}

	if (q==WEBM_720p) {
		p_url = urlMap.value(WEBM_720p, QString());
		if (p_url.isNull()) p_url= urlMap.value(MP4_720p, QString());
		if (p_url.isNull()) q = WEBM_480p;
	}

	if (q==WEBM_480p) {
		p_url = urlMap.value(WEBM_480p, QString());
		if (p_url.isNull()) q = WEBM_360p;
	}

	if (q==MP4_360p) {
		p_url = urlMap.value(MP4_360p, QString());
		if (p_url.isNull()) p_url= urlMap.value(WEBM_360p, QString());
		if (p_url.isNull()) q = FLV_360p;
	}

	if (q==WEBM_360p) {
		p_url = urlMap.value(WEBM_360p, QString());
		if (p_url.isNull()) p_url= urlMap.value(MP4_360p, QString());
		if (p_url.isNull()) q = FLV_360p;
	}

	// FLV, low priority
	if (q==FLV_480p) {
		p_url = urlMap.value(FLV_480p, QString());
		if (p_url.isNull()) q = FLV_360p;
	}

	if (q==FLV_360p) {
		p_url = urlMap.value(FLV_360p, QString());
		if (p_url.isNull()) q = FLV_240p;
	}

	if (q==FLV_240p) {
		p_url = urlMap.value(q, QString());
	}

	return p_url;
}

#ifdef YT_DASH_SUPPORT
QString RetrieveYoutubeUrl::findBestAudio(const QMap<int, QString>& urlMap) {
	QString url;

	url = urlMap.value(DASH_AUDIO_MP4_256, QString());
	if (!url.isEmpty()) return url;

	url = urlMap.value(DASH_AUDIO_WEBM_192, QString());
	if (!url.isEmpty()) return url;

	url = urlMap.value(DASH_AUDIO_MP4_128, QString());
	if (!url.isEmpty()) return url;

	url = urlMap.value(DASH_AUDIO_WEBM_128, QString());
	if (!url.isEmpty()) return url;

	url = urlMap.value(DASH_AUDIO_MP4_48, QString());
	if (!url.isEmpty()) return url;

	return url;
}
#endif

QString RetrieveYoutubeUrl::sanitizeForUnicodePoint(QString string) {
	QRegExp rx("\\\\u(\\d{4})");
	while (rx.indexIn(string) != -1) {
		string.replace(rx.cap(0), QString(QChar(rx.cap(1).toInt(0,16))));
	}
	return string;
}

void RetrieveYoutubeUrl::htmlDecode(QString& string) {
	string.replace("%3A", ":", Qt::CaseInsensitive);
	string.replace("%2F", "/", Qt::CaseInsensitive);
	string.replace("%3F", "?", Qt::CaseInsensitive);
	string.replace("%3D", "=", Qt::CaseInsensitive);
	string.replace("%25", "%", Qt::CaseInsensitive);
	string.replace("%26", "&", Qt::CaseInsensitive);
	string.replace("%3D", "=", Qt::CaseInsensitive);
}

#include "moc_retrieveyoutubeurl.cpp"
