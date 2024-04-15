/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2024 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef OSCLIENT_H
#define OSCLIENT_H

#include <QObject>
#include <QString>
#include "qrestapi/qRestAPI.h"

class OSSubtitle {
public:
	QString movie, releasename, link, iso639, language, date;
	QString format, comments, detail, rating, files, user;
	QString file_id;
};

class OSClient : public QObject {
	Q_OBJECT

public:
	enum SearchMethod { Hash = 0, Filename = 1, HashAndFilename = 2 };

	OSClient(QObject* parent = 0);
	QList<OSSubtitle> subtitleList() { return sub_list; };

	void setProxy(const QNetworkProxy & proxy);

	#ifdef FS_USE_SERVER_CONFIG
	void setServer(const QString & server) {};
	#endif

	void setSearchMethod(SearchMethod m) { search_method = m; };
	int searchMethod() { return search_method; };

	void setUsername(QString username) { os_username = username; };
	QString username() { return os_username; };

	void setPassword(QString password) { os_password = password; };
	QString password() { return os_password; };

	QString getDownloadLink(const QString & file_id, int * remaining_downloads = 0);

public slots:
	void search(const QString & hash, qint64 file_size, QString search_term = QString(), QString languages = QString());
	void login();

signals:
	void connecting();
	void loggedIn();
	void loginFailed();
	void searchFinished();
	void searchFailed();
	void getDownloadLinkFailed();
	void errorFound(int, const QString &);

private:
	qRestAPI::RawHeaders headers;
	qRestAPI * api;
	QList <OSSubtitle> sub_list;
	int search_method;
	QString os_username;
	QString os_password;
	QByteArray os_token;
	bool logged_in;
};

#endif
