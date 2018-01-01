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

#include "osclient.h"
#include "version.h"

OSClient::OSClient(QObject* parent) : 
	QObject(parent)
	, logged_in(false)
	, search_size(0) 
#ifdef OS_SEARCH_WORKAROUND
	, best_search_count(0)
	, search_retries(8)
#endif
{
	rpc = new MaiaXmlRpcClient(QUrl("http://api.opensubtitles.org/xml-rpc"), this);
}

void OSClient::setServer(const QString & server) {
	rpc->setUrl(QUrl(server));
}

#ifdef FS_USE_PROXY
void OSClient::setProxy(const QNetworkProxy & proxy) {
	rpc->setProxy(proxy);
}
#endif

void OSClient::login() {
	qDebug("OSClient::login");

	QString user_agent = "SMPlayer v" + Version::stable();
	qDebug("OSClient::login: user agent: %s", user_agent.toUtf8().constData());

	QVariantList args;

	args << "" << "" << "" << user_agent;

	rpc->call("LogIn", args,
			  this, SLOT(responseLogin(QVariant &)),
			  this, SLOT(gotFault(int, const QString &)));
}

void OSClient::search(const QString & hash, qint64 file_size, const QString & filename) {
	qDebug() << "OSClient::search: hash:" << hash << "file_size:" << file_size << "filename:" << filename;

	search_hash = hash;
	search_size = file_size;
	search_filename = filename;

	disconnect(this, SIGNAL(loggedIn()), this, SLOT(doSearch()));

	#if 0
	if (logged_in) {
		doSearch();
	} else {
		connect(this, SIGNAL(loggedIn()), this, SLOT(doSearch()));
		login();
	}
	#else
	connect(this, SIGNAL(loggedIn()), this, SLOT(doSearch()));
	login();
	#endif
}

#ifdef OS_SEARCH_WORKAROUND
void OSClient::doSearch() {
	best_search_count = -1;
	for (int n = 1; n <= search_retries; n++) doSearch(n);
}

void OSClient::doSearch(int nqueries) {
#else
void OSClient::doSearch() {
#endif
	qDebug("OSClient::doSearch");

	QVariantMap m;
	m["sublanguageid"] = "all";
	if (!search_filename.isEmpty()) {
		m["query"] = search_filename;
	} else {
		m["moviehash"] = search_hash;
		m["moviebytesize"] = QString::number(search_size);
	}

	QVariantList list;
#ifdef OS_SEARCH_WORKAROUND
	// Sometimes opensubtitles return 0 subtitles
	// A workaround seems to add the query several times
	qDebug("OSClient::doSearch: nqueries: %d", nqueries);
	for (int count = 0; count < nqueries; count++) list.append(m);
	//qDebug("OSClient::doSearch: list count: %d", list.count());
#else
	list.append(m);
#endif

	QVariantList args;
	args << token << QVariant(list);

	/*
	for (int n=0; n < args.count(); n++) {
		qDebug("%d = %d (%s)", n, args[n].type(), args[n].typeName());
	}
	*/

	rpc->call("SearchSubtitles", args,
			  this, SLOT(responseSearch(QVariant &)),
			  this, SLOT(gotFault(int, const QString &)));
}

void OSClient::responseLogin(QVariant &arg) {
	qDebug("OSClient::responseLogin");

	QVariantMap m = arg.toMap();
	QString status = m["status"].toString();
	QString t = m["token"].toString();

	qDebug("OSClient::responseLogin: status: %s", status.toLatin1().constData());
	qDebug("OSClient::responseLogin: token: %s", t.toLatin1().constData());

	if (status == "200 OK") {
		token = t;
		logged_in = true;
		emit loggedIn();
	} else {
		emit loginFailed();
	}
}

void OSClient::responseSearch(QVariant &arg) {
	qDebug("OSClient::responseSearch");

	QVariantMap m = arg.toMap();
	QString status = m["status"].toString();

	qDebug("OSClient::responseSearch: status: %s", status.toLatin1().constData());
	//qDebug("count: %d", m.count());

	/*
	QMapIterator<QString, QVariant> i(m);
	 while (i.hasNext()) {
		i.next();
		qDebug("key: %s", i.key().toLatin1().constData());
	}
	*/

	if (status != "200 OK") {
		emit searchFailed();
		return;
	}

	s_list.clear();

	QVariantList data = m["data"].toList();
	qDebug("OSClient::responseSearch: data count: %d", data.count());

#ifdef OS_SEARCH_WORKAROUND
	if (best_search_count >= data.count()) {
		qDebug("OSClient::responseSearch: we already have a better search (%d). Ignoring this one.", best_search_count);
		return;
	}
	best_search_count = data.count();
#endif

	for (int n = 0; n < data.count(); n++) {
		OSSubtitle sub;

		//qDebug("%d: type: %d (%s)", n, data[n].type(), data[n].typeName());
		QVariantMap m = data[n].toMap();

		sub.releasename = m["MovieReleaseName"].toString();
		sub.movie = m["MovieName"].toString();
#ifdef USE_QUAZIP
		sub.link = m["ZipDownloadLink"].toString();
#else
		sub.link = m["SubDownloadLink"].toString();
#endif
		sub.date = m["SubAddDate"].toString();
		sub.iso639 = m["ISO639"].toString();
		sub.rating = m["SubRating"].toString();
		sub.comments = m["SubAuthorComment"].toString();
		sub.format = m["SubFormat"].toString();
		sub.language = m["LanguageName"].toString();
		sub.user = m["UserNickName"].toString();
		sub.files = "1";

		s_list.append(sub);

		/*
		qDebug("MovieName: %s", sub.movie.toLatin1().constData());
		qDebug("MovieReleaseName: %s", sub.releasename.toLatin1().constData());
		//qDebug("SubFileName: %s", m["SubFileName"].toString().toLatin1().constData());
		//qDebug("SubDownloadLink: %s", m["SubDownloadLink"].toString().toLatin1().constData());
		qDebug("ZipDownloadLink: %s", sub.link.toLatin1().constData());
		qDebug("SubAddDate: %s", sub.date.toLatin1().constData());
		qDebug("ISO639: %s", sub.iso639.toLatin1().constData());
		qDebug("SubRating: %s", sub.rating.toLatin1().constData());
		qDebug("SubAuthorComment: %s", sub.comments.toLatin1().constData());
		qDebug("SubFormat: %s", sub.format.toLatin1().constData());
		qDebug("LanguageName: %s", sub.language.toLatin1().constData());
		qDebug("UserNickName: %s", sub.user.toLatin1().constData());
		qDebug("=======");
		*/
	}

	emit searchFinished();
}

void OSClient::gotFault(int error, const QString &message) {
	qDebug("OSClient::gotFault: error: %d, message: %s", error, message.toUtf8().constData());
	emit errorFound(error, message);
}

#include "moc_osclient.cpp"
