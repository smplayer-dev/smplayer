/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#include "osclient.h"
#include "version.h"

#include "qrestapi/qGirderAPI.h"
#include "qrestapi/qRestResult.h"
#include <QDebug>

#define API_SERVER "https://api.opensubtitles.com/api/v1"
#define API_KEY "jdXcE3ZTnuKJu6IrrX7XoJJRfsQ43fwM"

OSClient::OSClient(QObject* parent) :
	QObject(parent),
	search_method(Hash),
	logged_in(false)
{
	headers["Accept"] = "application/json";
	headers["Api-Key"] = API_KEY;
	headers["Content-Type"] = "application/json";
	headers["User-Agent"] = "SMPlayer v" + Version::stable().toLatin1();
	qDebug() << headers;

	api = new qGirderAPI();
	//api->setSuppressSslErrors(false);
	api->setServerUrl(API_SERVER);
	api->setDefaultRawHeaders(headers);
}

void OSClient::setProxy(const QNetworkProxy & proxy) {
	api->setHttpNetworkProxy(proxy);
}

void OSClient::login() {
	qDebug() << "OSClient::login";

	qRestAPI::Parameters par;
	par["username"] = os_username;
	par["password"] = os_password;

	QUuid query_id = api->post("/login", par);
	qDebug() << "OSClient::login: query_id:" << query_id;

	QList<QVariantMap> result;
	bool ok = api->sync(query_id, result);

	if (!ok) {
		qDebug() << "OSClient::login: error code:" << api->error();
		qDebug() << "OSClient::login: error:" << api->errorString();
		emit loginFailed();
		emit errorFound(api->error(), api->errorString());
		return;
	}

	if (result.count() > 0) {
		QVariantMap map = result[0];
		if (map.contains("token")) {
			os_token = map["token"].toByteArray();
			logged_in = true;
			emit loggedIn();
			qDebug() << "OSClient::login: token:" << os_token;
		}
	}
}

void OSClient::search(const QString & hash, qint64 file_size, QString search_term) {
	qDebug() << "OSClient::search: hash:" << hash << "file_size:" << file_size << "search_term:" << search_term;
	emit connecting();

	qRestAPI::Parameters par;
	//par["ai_translated"] = "exclude";

	search_term = search_term.replace(" ", "+").toLower();

	switch (search_method) {
		case Filename:
			par["query"] = search_term;
			break;
		case Hash:
			par["moviehash"] = hash;
			break;
		case HashAndFilename:
			par["query"] = search_term;
			par["moviehash"] = hash;
			break;
		default:
			return;
	}

	QUuid query_id = api->get("/subtitles", par);
	qDebug() << "OSClient::search: query_id:" << query_id;

	QList<QVariantMap> result;
	bool ok = api->sync(query_id, result);

	if (!ok) {
		qDebug() << "OSClient::search: error code:" << api->error();
		qDebug() << "OSClient::search: error:" << api->errorString();
		emit searchFailed();
		emit errorFound(api->error(), api->errorString());
		return;
	}

	sub_list.clear();

	foreach (const QVariantMap &map, result) {
		if (map.contains("data")) {
			QVariantList data = map["data"].toList();
			foreach(const QVariant &variant, data) {
				QVariantMap item = variant.toMap();
				QVariantMap att = item["attributes"].toMap();
				QVariantMap det = att["feature_details"].toMap();
				QVariantMap uploader = att["uploader"].toMap();
				QList<QVariant> files = att["files"].toList();

				//qDebug() << att["language"];
				//qDebug() << att["feature_details"];
				//qDebug() << att["release"];
				//qDebug() << att["files"];

				for (int n=0; n < files.count(); n++) {
					OSSubtitle sub;
					QVariantMap file = files[n].toMap();

					sub.releasename = att["release"].toString();
					sub.movie = det["movie_name"].toString();
					if (files.count() > 1) sub.movie += QString(" (CD %1)").arg(file["cd_number"].toString());
					sub.link = "";
					sub.file_id = file["file_id"].toString();
					sub.date = att["upload_date"].toString();
					sub.iso639 = att["language"].toString();
					sub.rating = att["ratings"].toString();
					sub.comments = att["comments"].toString();
					sub.format = "srt";
					sub.language = sub.iso639;
					sub.user = uploader["name"].toString();
					sub.files = "1"; //QString::number(files.count());

					sub_list.append(sub);

					#if 0
					qDebug() << "Release:" << sub.releasename;
					qDebug() << "Name:" << sub.movie;
					qDebug() << "Date:" << sub.date;
					qDebug() << "ISO639:" << sub.iso639;
					qDebug() << "Rating:" << sub.rating;
					qDebug() << "Comments:" << sub.comments;
					qDebug() << "Uploader:" << sub.user;
					qDebug() << "File ID:" << sub.file_id;
					qDebug() << "=========";
					#endif
				}
			}
		}
	}
	emit searchFinished();
}

QString OSClient::getDownloadLink(const QString & file_id) {
	qDebug() << "OSClient::getDownloadLink:" << file_id;

	// Try to login
	if (!logged_in && !os_username.isEmpty() && ! os_password.isEmpty()) {
		login();
	}

	QString link;

	qRestAPI::Parameters par;
	par["file_id"] = file_id;
	par["sub_format"] = "srt";

	qRestAPI::RawHeaders header;
	if (!os_token.isEmpty()) {
		header["Authorization"] = "Bearer " + os_token;
	}

	QUuid query_id = api->post("/download", par, header);
	qDebug() << "OSClient::getDownloadLink: query_id:" << query_id;

	QList<QVariantMap> result;
	bool ok = api->sync(query_id, result);

	if (!ok) {
		qDebug() << "OSClient::getDownloadLink: error code:" << api->error();
		qDebug() << "OSClient::getDownloadLink: error:" << api->errorString();
		emit getDownloadLinkFailed();
		emit errorFound(api->error(), api->errorString());
		return link;
	}

	qDebug() << "OSClient::getDownloadLink: result:" << result;

	if (result.count() > 0) {
		QVariantMap map = result[0];
		if (map.contains("link")) {
			link = map["link"].toString();
		}
	}

	return link;
}

#include "moc_osclient.cpp"
