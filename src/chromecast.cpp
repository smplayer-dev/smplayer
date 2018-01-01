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

#include "chromecast.h"
#include "links.h"
#include <QUrl>
#include <QFileInfo>
#include <QDesktopServices>
#include <QNetworkInterface>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include "helper.h"

#ifdef CONVERT_TO_VTT
#include "subreader.h"
#endif

//#define CHROMECAST_USE_SERVER_WEBFSD
//#define CHROMECAST_USE_SERVER_SIMPLE_WEB_SERVER

#if !defined(CHROMECAST_USE_SERVER_WEBFSD) && !defined(CHROMECAST_USE_SERVER_SIMPLE_WEB_SERVER)
  #define CHROMECAST_USE_SERVER_SIMPLE_WEB_SERVER
#endif

Chromecast * Chromecast::instance_obj = 0;

Chromecast * Chromecast::instance() {
	if (instance_obj == 0) {
		instance_obj = new Chromecast();
	}
	return instance_obj;
}

void Chromecast::deleteInstance() {
	if (instance_obj) {
		delete instance_obj;
		instance_obj = 0;
	}
}


Chromecast::Chromecast(QObject * parent)
	: QObject(parent)
	, server_process(0)
	, settings(0)
	, server_port(8010)
	, directory_listing(true)
	, server_needs_restart(false)
{
#ifdef CONVERT_TO_VTT
	autoconvert_to_vtt = true;
	sub_encoding = "ISO-8859-1";
	sub_position = -1;
	overwrite_vtt = false;
	use_sub_filter = true;
#endif
}

Chromecast::~Chromecast() {
	if (server_process) {
		stopServer();
	}
	saveSettings();
}

void Chromecast::openStream(const QString & url, const QString & title) {
	QDesktopServices::openUrl(QUrl(URL_CHROMECAST "/?title=" + title.toUtf8().toBase64() +
		"&url=" + url.toUtf8().toBase64()));
}

void Chromecast::openLocal(const QString & file, const QString & title, const QString & subtitle) {
	qDebug() << "Chromecast::openLocal: file:" << file << "subtitle:" << subtitle;

	QFileInfo fi(file);
	QString dir = fi.absolutePath();
	QString filename = fi.fileName();

	qDebug() << "Chromecast::openLocal: dir:" << dir;
	qDebug() << "Chromecast::openLocal: filename:" << filename;

	QString local_address = localAddress();
	if (local_address.isEmpty()) local_address = findLocalAddress();
	qDebug() << "Chromecast::openLocal: chosen address:" << local_address;

	// Find subtitle file
#ifdef SERVE_FILE_DIR_ONLY
	QString sub_filename;
	if (!subtitle.isEmpty()) {
		// Check if subtitle is in the video directory
		if (QFile::exists(dir +"/"+ subtitle)) {
			sub_filename = subtitle;
			qDebug() << "Chromecast::openLocal: sub_filename:" << sub_filename;
		} else {
			// Check if it's an absolute path
			if (QFile::exists(subtitle)) {
				QFileInfo sub_fi(subtitle);
				if (QFile::exists(dir +"/"+ sub_fi.fileName())) {
					sub_filename = sub_fi.fileName();
				}
			}
		}
		sub_filename = checkForVTT(dir, sub_filename);
		qDebug() << "Chromecast::openLocal: sub_filename:" << sub_filename;
	}
#else
	QString sub_filepath;
	if (!subtitle.isEmpty()) {
		if (QFile::exists(subtitle)) {
			sub_filepath = subtitle;
		} else {
			if (QFile::exists(dir +"/"+ subtitle)) {
				sub_filepath = dir +"/"+ subtitle;
			}
		}
		qDebug() << "Chromecast::openLocal: sub_filepath:" << sub_filepath;
	}
#endif

	if (!local_address.isEmpty()) {
		// Run web server

		#ifdef SERVE_FILE_DIR_ONLY
		startServer(dir);
		QString url = "http://" + local_address + ":" + QString::number(server_port) + "/" + filename;
		QString sub_url;
		if (!sub_filename.isEmpty()) {
			sub_url = "http://" + local_address + ":" + QString::number(server_port) + "/" + sub_filename;
		}
		#else
		QString root = QDir::rootPath();
		startServer(root);

		QString abs_filename = filepathWithoutRoot(file);
		qDebug() << "Chromecast::openLocal: abs_filename:" << abs_filename;
		QString url = "http://" + local_address + ":" + QString::number(server_port) + "/" + abs_filename;

		QString abs_sub_filename = filepathWithoutRoot(sub_filepath);
		qDebug() << "Chromecast::openLocal: abs_sub_filename:" << abs_sub_filename;
		QString sub_url = "http://" + local_address + ":" + QString::number(server_port) + "/" + abs_sub_filename;
		#endif

		qDebug() << "Chromecast::openLocal: url:" << url;
		qDebug() << "Chromecast::openLocal: sub_url:" << sub_url;

		#if 1
		QDesktopServices::openUrl(QUrl(URL_CHROMECAST "/?title=" + title.toUtf8().toBase64() +
			"&url=" + url.toUtf8().toBase64() + "&subtitles=" + sub_url.toUtf8().toBase64()));
		#endif
	}
}

#ifndef SERVE_FILE_DIR_ONLY
QString Chromecast::filepathWithoutRoot(const QString & filepath) {
	QString root = QDir::rootPath();
	QFileInfo fi(filepath);
	QString abs_filename = fi.absoluteFilePath();
	if (abs_filename.startsWith(root)) {
		abs_filename = abs_filename.mid(root.length());
	}
	return abs_filename;
}
#endif

QString Chromecast::checkForVTT(const QString & video_path, const QString & subtitle_file) {
	QString actual_subtitle = subtitle_file;
	QString dir = video_path;

	QString subtitle_path = dir +"/"+ subtitle_file;
	QFileInfo fi(subtitle_path);
	if (fi.suffix().toLower() == "srt") {
		qDebug() << "Chromecast::checkForVTT: subtitle is in srt format";

		#ifdef CONVERT_TO_VTT
		if (autoconvert_to_vtt) {
			SubReader sr;
			sr.setInputCodec(sub_encoding.toLatin1());
			sr.setVTTLinePosition(sub_position);
			if (use_sub_filter) {
				QString filter = sub_filter;
				if (filter.isEmpty()) filter = "opensubtitles|subtitulos\\.es|tusubtitulo|osdb|addic7ed";
				sr.setTextFilter(filter);
			}
			sr.setOverwriteVTT(overwrite_vtt);
			sr.autoConvertToVTT(subtitle_path);
		}
		#endif

		// Check if a subtitle file with vtt extension exists
		QString vtt_subtitle = fi.completeBaseName() + ".vtt";
		QString vtt_subtitle_path = dir +"/"+ vtt_subtitle;
		if (QFile::exists(vtt_subtitle_path)) {
			qDebug() << "Chromecast::checkForVTT: using" << vtt_subtitle;
			actual_subtitle = vtt_subtitle;
		}
	}

	return actual_subtitle;
}

QStringList Chromecast::localAddresses() {
	QStringList l;

	foreach(const QHostAddress &address, QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
			l << address.toString();
		}
	}

	return l;
}

QString Chromecast::findLocalAddress() {
	QString local_address;

	QStringList addresses = localAddresses();
	qDebug() << "Chromecast::defaultLocalAddress: all IPv4 addresses:" << addresses;

	foreach(QString address, addresses) {
		//qDebug() << "Chromecast::defaultLocalAddress: address:" << address;
		if (address.startsWith("192.") && !address.endsWith(".1")) {
			local_address = address;
			break;
		}
	}

	if (local_address.isEmpty() && !addresses.isEmpty()) local_address = addresses[0];
	return local_address;
}

void Chromecast::startServer(QString doc_root) {
	qDebug("Chromecast::startServer");

	static QString last_doc_root;

	if (server_process == 0) {
		server_process = new QProcess(this);
		server_process->setProcessChannelMode( QProcess::MergedChannels );
		connect(server_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
		connect(server_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
		connect(server_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
	}

	if (server_process->state() == QProcess::Running) {
		if (!server_needs_restart && last_doc_root == doc_root) {
			return;
		} else {
			stopServer();
		}
	}

	QString prog;
	QStringList args;

#if defined(Q_OS_WIN) && defined(SERVE_FILE_DIR_ONLY)
	doc_root = Helper::shortPathName(doc_root);
#endif

#if defined(CHROMECAST_USE_SERVER_WEBFSD)
	prog = "webfsd";
	args << "-F" << "-d" << "-4" << "-p" << QString::number(server_port) << "-r" << doc_root;
	if (!directoryListing()) args << "-j";
#elif defined(CHROMECAST_USE_SERVER_SIMPLE_WEB_SERVER)
	prog = "simple_web_server";
	#ifdef Q_OS_WIN
	prog += ".exe";
	#endif
	args << "-p" << QString::number(server_port) << "-r" << doc_root;
	if (!directoryListing()) args << "-j";
#else
	#error "No server defined"
#endif

	{
		QString command = prog + " " + args.join(" ");
		qDebug() << "Chromecast::startServer: command:" << command;
	}

	server_process->start(prog, args);

	last_doc_root = doc_root;
	server_needs_restart = false;
}


void Chromecast::stopServer() {
	qDebug("Chromecast::stopServer");

	if (server_process) {
		server_process->terminate();
		if (!server_process->waitForFinished(5000)) {
			server_process->kill();
			server_process->waitForFinished();
		}
	}
}

// Slots
void Chromecast::readProcessOutput() {
	QByteArray line;
	while (server_process->canReadLine()) {
		line = server_process->readLine().trimmed();
		qDebug() << "Chromecast::readProcessOutput: line:" << line;
	}
}

void Chromecast::processFinished(int exit_code, QProcess::ExitStatus exit_status) {
	qDebug() << "Chromecast::processFinished: exit_code:" << exit_code << "exit_status:" << exit_status;
}

void Chromecast::processError(QProcess::ProcessError error) {
	qDebug() << "Chromecast::processError:" << error;
	if (error == QProcess::FailedToStart) {
		qDebug("Chromecast::processError: process failed to start");
	}
}

void Chromecast::loadSettings() {
	qDebug("Chromecast::loadSettings");

	if (settings) {
		settings->beginGroup("chromecast/server");
		setServerPort(settings->value("port", serverPort()).toInt());
		setLocalAddress(settings->value("local_address", localAddress()).toString());
		setDirectoryListing(settings->value("directory_listing", directoryListing()).toBool());
		settings->endGroup();

		#ifdef CONVERT_TO_VTT
		settings->beginGroup("chromecast/subtitles");
		setAutoConvertToVTT(settings->value("autoconvert_to_vtt", autoConvertToVTT()).toBool());
		//setSubtitleEncoding(settings->value("encoding", subtitleEncoding()).toString());
		setSubtitlePosition(settings->value("position_on_screen", 93).toInt());
		setSubtitleFilter(settings->value("text_filter", subtitleFilter()).toString());
		setOverwriteVTT(settings->value("overwrite_vtt", overwriteVTT()).toBool());
		enableSubtitleFilter(settings->value("use_sub_filter", isSubtitleFilterEnabled()).toBool());
		settings->endGroup();
		#endif
	}
}

void Chromecast::saveSettings() {
	qDebug("Chromecast::saveSettings");

	if (settings) {
		settings->beginGroup("chromecast/server");
		settings->setValue("port", serverPort());
		settings->setValue("local_address", localAddress());
		settings->setValue("directory_listing", directoryListing());
		settings->endGroup();

		#ifdef CONVERT_TO_VTT
		settings->beginGroup("chromecast/subtitles");
		settings->setValue("autoconvert_to_vtt", autoConvertToVTT());
		//settings->setValue("encoding", subtitleEncoding());
		settings->setValue("position_on_screen", subtitlePosition());
		settings->setValue("text_filter", subtitleFilter());
		settings->setValue("overwrite_vtt", overwriteVTT());
		settings->setValue("use_sub_filter", isSubtitleFilterEnabled());
		settings->endGroup();
		#endif
	}
}

#include "moc_chromecast.cpp"
