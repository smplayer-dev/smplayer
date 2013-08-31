/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>
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

RetrieveYoutubeUrl::RetrieveYoutubeUrl( QObject* parent ) : QObject(parent)
{
	reply = 0;
	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotResponse(QNetworkReply*)));

	preferred_quality = FLV_360p;
	user_agent = "Mozilla/5.0 (X11; Linux x86_64; rv:5.0.1) Gecko/20100101 Firefox/5.0.1";
}

RetrieveYoutubeUrl::~RetrieveYoutubeUrl() {
}

void RetrieveYoutubeUrl::fetchPage(const QString & url) {
	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", user_agent.toLatin1());
	reply = manager->get(req);
	orig_url = url;

	emit connecting(url);
}

void RetrieveYoutubeUrl::close() {
	if (reply) reply->abort();
}

void RetrieveYoutubeUrl::gotResponse(QNetworkReply* reply) {
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

	QRegExp regex("\\\"url_encoded_fmt_stream_map\\\"\\s*:\\s*\\\"([^\\\"]*)");
	regex.indexIn(replyString);
	QString fmtArray = regex.cap(1);
	fmtArray = sanitizeForUnicodePoint(fmtArray);
	fmtArray.replace(QRegExp("\\\\(.)"), "\\1");

	bool signature_not_found = false;

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
			#if QT_VERSION >= 0x050000
			q->setQuery( q->query() + "&" + url.query() );
			#else
			q->setEncodedQuery( q->encodedQuery() + "&" + url.encodedQuery() );
			#endif
			q->removeQueryItem("url");

			if (q->hasQueryItem("sig")) {
				q->addQueryItem("signature", q->queryItemValue("sig"));
				q->removeQueryItem("sig");
			}
			else
			if (q->hasQueryItem("s")) {
				QString signature = YTSig::aclara(q->queryItemValue("s"));
				if (!signature.isEmpty()) {
					q->addQueryItem("signature", signature);
				} else {
					signature_not_found = true;
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
				line.setQuery(q->query());
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

	if ((urlMap.count() == 0) && (signature_not_found)) {
		qDebug("RetrieveYoutubeUrl::parse: no url found with valid signature");
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
