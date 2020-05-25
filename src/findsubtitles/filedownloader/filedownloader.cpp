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

#include "filedownloader.h"
#include <QFile>
#include <QMessageBox>

FileDownloader::FileDownloader(QWidget *parent)
	: QProgressDialog(parent)
	, reply(0)
{
	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotResponse(QNetworkReply*)));

	setMinimumDuration(0);
	setRange(0,0);

	reset(); // Prevent the dialog to be shown on initialization (Qt 5.5)

	connect(this, SIGNAL(canceled()), this, SLOT(cancelDownload()));

	setWindowTitle(tr("Downloading..."));

	#ifdef Q_OS_WIN
	user_agent = "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:56.0) Gecko/20100101 Firefox/56.0";
	#else
	user_agent = "Mozilla/5.0 (X11; Linux i686; rv:62.0) Gecko/20100101 Firefox/62.0";
	#endif
}

FileDownloader::~FileDownloader() {
	delete manager;
}

void FileDownloader::setProxy(QNetworkProxy proxy) {
	manager->setProxy(proxy);

	qDebug() << "FileDownloader::setProxy: host:" << proxy.hostName() << "port:" << proxy.port() << "type:" << proxy.type();

}

void FileDownloader::download(QUrl url) {
	QNetworkRequest req(url);
	if (!user_agent.isEmpty()) req.setRawHeader("User-Agent", user_agent);
	reply = manager->get(req);
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(updateDataReadProgress(qint64, qint64)));

	setLabelText(tr("Connecting to %1").arg(url.host()));
}

void FileDownloader::cancelDownload() {
	if (reply) reply->abort();
}

void FileDownloader::gotResponse(QNetworkReply* reply) {
	if (reply->error() == QNetworkReply::NoError) {
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		qDebug("FileDownloader::gotResponse: status: %d", status);
		switch (status) {
			case 301:
			case 302:
			case 307:
				QString r_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
				qDebug() << "FileDownloader::gotResponse: redirected:" << r_url;
				download(r_url);
				return;
		}
	} else {
		hide();
		emit downloadFailed(reply->errorString());
		return;
	}

	hide();
	emit downloadFinished(reply->readAll());
}

void FileDownloader::updateDataReadProgress(qint64 bytes_read, qint64 total_bytes) {
	//qDebug() << "FileDownloader::updateDataReadProgress: " << bytes_read << " " << total_bytes;
	if (total_bytes > -1) {
		setMaximum(total_bytes);
		setValue(bytes_read);
	}
}

#include "moc_filedownloader.cpp"

