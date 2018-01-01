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

#ifndef LOADPAGE_H
#define LOADPAGE_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class LoadPage : public QObject
{
	Q_OBJECT

public:
	LoadPage(QNetworkAccessManager * man, QObject* parent = 0);
	~LoadPage();

	void fetchPage(const QString & url);

	static void setDefaultUserAgent(const QString & ua) { default_user_agent = ua; };
	static QString defaultUserAgent() { return default_user_agent; };

	void setUserAgent(const QString & ua) { user_agent = ua; };
	QString userAgent() {
		if (user_agent.isNull()) return default_user_agent; else return user_agent;
	}

signals:
	void errorOcurred(int error_number, QString error_str);
	void pageLoaded(QByteArray page);

protected slots:
	void gotResponse();

private:
	QNetworkAccessManager * manager;
	QNetworkReply * reply;
	QString user_agent;

	static QString default_user_agent;
};

#endif
