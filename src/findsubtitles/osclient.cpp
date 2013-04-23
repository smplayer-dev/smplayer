/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

OSClient::OSClient(QObject* parent) : QObject(parent), logged_in(false), search_size(0) {
	rpc = new MaiaXmlRpcClient(QUrl("http://api.opensubtitles.org/xml-rpc"), this);
}

void OSClient::setServer(const QString & server) {
	rpc->setUrl(QUrl(server));
}

void OSClient::login() {
	qDebug("OSClient::login");

	QVariantList args;

	args << "" << "" << "" << "OS Test User Agent";

	rpc->call("LogIn", args,
			  this, SLOT(responseLogin(QVariant &)),
			  this, SLOT(gotFault(int, const QString &)));
}

void OSClient::search(const QString & hash, qint64 file_size) {
	qDebug("OSClient::search: hash: %s, file_size: %lld", hash.toUtf8().constData(), file_size);

	search_hash = hash;
	search_size = file_size;

	if (logged_in) {
		doSearch();
	} else {
		connect(this, SIGNAL(loggedIn()), this, SLOT(doSearch()));
		login();
	}
}

void OSClient::doSearch() {
	qDebug("OSClient::doSearch");

	QVariantMap m;
	m["sublanguageid"] = "all";
	m["moviehash"] = search_hash;
	m["moviebytesize"] = (int) search_size;
	//m["query"] = "Star wars";

	QVariantList list;
	list.append(m);

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

	QVariantList data = m["data"].toList();
	qDebug("OSClient::responseSearch: data count: %d", data.count());

	for (int n = 0; n < data.count(); n++) {
		OSSubtitle sub;

		//qDebug("%d: type: %d (%s)", n, data[n].type(), data[n].typeName());
		QVariantMap m = data[n].toMap();

		sub.releasename = m["MovieReleaseName"].toString();
		sub.movie = m["MovieName"].toString();
		sub.link = m["ZipDownloadLink"].toString();
		sub.date = m["SubAddDate"].toString();
		sub.iso639 = m["ISO639"].toString();
		sub.rating = m["SubRating"].toString();
		sub.comments = m["SubAuthorComment"].toString();
		sub.format = m["SubFormat"].toString();
		sub.language = m["LanguageName"].toString();
		sub.user = m["UserNickName"].toString();

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
