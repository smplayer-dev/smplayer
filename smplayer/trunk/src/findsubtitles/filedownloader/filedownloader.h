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

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QProgressDialog>
#include <QUrl>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class FileDownloader : public QProgressDialog
{
	Q_OBJECT

public:
	FileDownloader(QWidget *parent = 0);
	~FileDownloader();

	void setProxy(QNetworkProxy proxy);

	void setUserAgent(const QByteArray & s) { user_agent = s; }
	QByteArray userAgent() { return user_agent; }

public slots:
	void download(QUrl url);
	void cancelDownload();

signals:
	void downloadFinished(const QByteArray & buffer);
	void downloadFailed(const QString & reason);

private slots:
	void gotResponse(QNetworkReply* reply);
	void updateDataReadProgress(qint64 bytes_read, qint64 total_bytes);

private:
	QNetworkAccessManager* manager;
	QNetworkReply* reply;
	QByteArray user_agent;
};

#endif
