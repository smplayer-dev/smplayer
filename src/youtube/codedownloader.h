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

#ifndef CODEDOWNLOADER_H
#define CODEDOWNLOADER_H

#include <QProgressDialog>
#include <QUrl>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CodeDownloader : public QProgressDialog
{
	Q_OBJECT

public:
	enum ErrorMessage { NoError = 0, FailedToRun = 1, UrlNotFound = 2 };

	CodeDownloader(QWidget *parent = 0);
	~CodeDownloader();

	void saveAs(const QString & output) { output_filename = output; };
	void setProxy(QNetworkProxy proxy);

	void setUserAgent(const QByteArray & s) { user_agent = s; }
	QByteArray userAgent() { return user_agent; }

public slots:
	void download(QUrl url);
	void cancelDownload();

signals:
	void downloadFinished();
	void errorOcurred(int error_number, QString error_str);
	void fileSaved(const QString &);
	void saveFailed(const QString &);

public:
	static void askAndDownload(QWidget * parent, ErrorMessage e = NoError);

private slots:
	void gotResponse(QNetworkReply* reply);
	void updateDataReadProgress(qint64 bytes_read, qint64 total_bytes);
	void save(QByteArray bytes);

	void reportFileSaved(const QString &);
	void reportSaveFailed(const QString &);
	void reportError(int error_number, QString error_str);

private:
	QNetworkAccessManager* manager;
	QNetworkReply* reply;

	QString output_filename;
	QByteArray user_agent;

	static CodeDownloader * downloader;
	QWidget * parent_widget;
	QString version;
};

#endif
