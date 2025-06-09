/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#include "codedownloader.h"
#include "retrieveyoutubeurl.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>

#include <QDir>

#define APPNAME "SMPlayer"
//#define APPNAME "SMTube"

CodeDownloader * CodeDownloader::downloader = 0;

CodeDownloader::CodeDownloader(QWidget * parent)
	: QProgressDialog(parent)
	, reply(0)
	, parent_widget(parent)
{
	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotResponse(QNetworkReply*)));

	setMinimumDuration(0);
	setRange(0,0);

	reset(); // Prevent the dialog to be shown on initialization (Qt 5.5)

	connect(this, SIGNAL(canceled()), this, SLOT(cancelDownload()));
	connect(this, SIGNAL(fileSaved(const QString &)), this, SLOT(reportFileSaved(const QString &)));
	connect(this, SIGNAL(saveFailed(const QString &)), this, SLOT(reportSaveFailed(const QString &)));
	connect(this, SIGNAL(errorOcurred(int,QString)), this, SLOT(reportError(int,QString)));

	setWindowTitle(tr("Downloading..."));

	#ifdef Q_OS_WIN
	user_agent = "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:56.0) Gecko/20100101 Firefox/56.0";
	#else
	user_agent = "Mozilla/5.0 (X11; Linux i686; rv:62.0) Gecko/20100101 Firefox/62.0";
	#endif
}

CodeDownloader::~CodeDownloader() {
	delete manager;
}

void CodeDownloader::setProxy(QNetworkProxy proxy) {
	manager->setProxy(proxy);

	qDebug() << "CodeDownloader::setProxy: host:" << proxy.hostName() << "port:" << proxy.port() << "type:" << proxy.type();
}

void CodeDownloader::download(QUrl url) {
	qDebug() << "CodeDownloader::download:" << url;
	QNetworkRequest req(url);
	if (!user_agent.isEmpty()) req.setRawHeader("User-Agent", user_agent);
	reply = manager->get(req);
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(updateDataReadProgress(qint64, qint64)));

	setLabelText(tr("Connecting to %1").arg(url.host()));

	QRegExp rx("download\\/([\\d\\.]+)\\/(youtube|yt-dlp)");
	if (rx.indexIn(url.toString()) > -1) {
		version = rx.cap(1);
		qDebug() << "CodeDownloader::download: version:" << version;
	}
}

void CodeDownloader::cancelDownload() {
	if (reply) reply->abort();
}

void CodeDownloader::gotResponse(QNetworkReply* reply) {
	if (reply->error() == QNetworkReply::NoError) {
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		qDebug("CodeDownloader::gotResponse: status: %d", status);
		switch (status) {
			case 301:
			case 302:
			case 307:
				QString r_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
				qDebug() << "CodeDownloader::gotResponse: redirected:" << r_url;
				download(r_url);
				return;
		}
	} else {
		emit errorOcurred((int)reply->error(), reply->errorString());
		return;
	}

	emit downloadFinished();
	save(reply->readAll());
}

void CodeDownloader::save(QByteArray bytes) {
	if (output_filename.isEmpty()) {
		qWarning("CodeDownloader::save: output filename is empty");
		emit saveFailed(output_filename);
		return;
	}

	QFile file(output_filename);
	if (!file.open(QIODevice::WriteOnly))  {
		qWarning() << "CodeDownloader::save: could not open" << output_filename << "for writing";
		emit saveFailed(output_filename);
		return;
	}

	file.write(bytes);
	file.close();

#ifndef Q_OS_WIN
	file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther);
#endif

	emit fileSaved(output_filename);
}

void CodeDownloader::updateDataReadProgress(qint64 bytes_read, qint64 total_bytes) {
	//qDebug() << "CodeDownloader::updateDataReadProgress: " << bytes_read << " " << total_bytes;
	if (total_bytes > -1) {
		setMaximum(total_bytes);
		setValue(bytes_read);
	}
}

void CodeDownloader::reportFileSaved(const QString &) {
	hide();
	QString t = tr("The YouTube code has been installed successfully.");
	if (!version.isEmpty()) t += "<br>"+ tr("Installed version: %1").arg(version);
	QMessageBox::information(parent_widget, tr("Success"),t);
}

void CodeDownloader::reportSaveFailed(const QString & file) {
	hide();
	QMessageBox::warning(parent_widget, tr("Error"), tr("It's not possible to save %1.").arg(file));
}

void CodeDownloader::reportError(int, QString error_str) {
	hide();
	QMessageBox::warning(parent_widget, tr("Error"), tr("An error happened while downloading the file:<br>%1").arg(error_str));
}

void CodeDownloader::askAndDownload(QWidget * parent, ErrorMessage e, const QString & download_path, QString app_name) {
	QString message;

	switch (e) {
		case FailedToRun:
			message += "<b>" + tr("%1 failed to communicate with the external YouTube application. "
                             "Either it's not installed or it doesn't work correctly.").arg(APPNAME) +"</b><br><br>";
			break;
		case UrlNotFound:
			message += "<b>" + tr("It wasn't possible to find the URL for this video.") + " "+
							tr("Maybe you need to update the YouTube code.") +"</b><br><br>";
		case NoError: ;
	}

#if defined(Q_OS_WIN) && !defined(PORTABLE_APP) && !defined(YT_BIN_ON_CONFIG_DIR)
	message +=  tr("In order to play YouTube videos, %1 needs an external application called youtube-dl.").arg(APPNAME) + "<br><br>"+
				tr("This component needs to be updated frequently.") +" "+
				tr("You can update it just by reinstalling SMPlayer. The installer will download and install the very latest version.");

	QMessageBox::information(parent, tr("Install / Update YouTube support"),message);
#else
	if (app_name.isEmpty()) app_name = YTDL_DEFAULT_BIN;
	QFileInfo fi(app_name);
	QString app_basename = fi.baseName().toLower();

	QString url;
	QString output_file;
	//output_file = fi.fileName();

	qDebug() << "CodeDownloader::askAndDownload: app_basename:" << app_basename;

	#ifdef Q_OS_WIN
	if (app_basename == "yt-dlp") {
		#if defined(_WIN64)
		url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe";
		#else
		url = "https://github.com/nicolaasjan/yt-dlp/releases/latest/download/yt-dlp_win7.exe";
		#endif
		output_file = "yt-dlp.exe";
	} else {
		url = "https://github.com/ytdl-org/ytdl-nightly/releases/latest/download/youtube-dl.exe";
		output_file = "youtube-dl.exe";
	}
	#else
	if (app_basename == "yt-dlp") {
		#ifdef Q_OS_MACX
		url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp_macos";
		#elif defined(Q_OS_LINUX)
			#if defined(__aarch64__)
			url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp_linux_aarch64";
			#else
			url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp_linux";
			#endif
		#else
		url = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp";
		#endif
		output_file = "yt-dlp";
	} else {
		url = "https://github.com/ytdl-org/ytdl-nightly/releases/latest/download/youtube-dl";
		output_file = "youtube-dl";
	}
	#endif

	qDebug() << "CodeDownloader::askAndDownload: output_file:" << output_file;

	#ifdef Q_OS_WIN
	QString output_dir = download_path;
	if (output_dir.isEmpty()) output_dir = "mpv";
	#else
	QString output_dir = download_path;
	if (output_dir.isEmpty()) output_dir = QDir::homePath() + "/bin";

	QDir d;
	if (!d.exists(output_dir)) {
		if (!d.mkdir(output_dir)) {
			qDebug() << "CodeDownloader::askAndDownload: fail to create" << output_dir;
		}
	}
	#endif

	QString output = output_dir + "/" + output_file;

	qDebug() << "CodeDownloader::askAndDownload: url:" << url;
	qDebug() << "CodeDownloader::askAndDownload: output" << output;

	message +=  tr("In order to play YouTube videos, %1 needs the help of an external application.").arg(APPNAME) + "<br><br>"+
				tr("%1 can download and install this application for you.").arg(APPNAME) +" "+
				tr("It will be downloaded from the official website and installed as %1.").arg("<i>" + output +"</i>") + "<br><br>"+
				tr("Would you like to proceed?");

	int ret = 0;
	if (e != NoError) {
		ret = QMessageBox::warning(parent, tr("Install YouTube support?"),
				message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	} else {
		ret = QMessageBox::question(parent, tr("Install YouTube support?"),
				message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

	if (ret == QMessageBox::Yes) {
 		if (!downloader) downloader = new CodeDownloader(parent);
		downloader->saveAs(output);
		downloader->show();
		downloader->download(url);
	}
#endif
}

#include "moc_codedownloader.cpp"

