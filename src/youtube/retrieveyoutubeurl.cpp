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

#include "retrieveyoutubeurl.h"

#if QT_VERSION >= 0x050000
#define YT_USE_JSON
#define DEBUG_OUTPUT_JSON
#endif

#include <QUrl>
#include <QDebug>
#include <QStringList>
#include <QProcess>
#include <QFileInfo>
#include <QDir>

#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#ifdef YT_USE_JSON
#include <QJsonDocument>
#include <QJsonObject>
#ifdef DEBUG_OUTPUT_JSON
#include <QFile>
#endif
#endif

RetrieveYoutubeUrl::RetrieveYoutubeUrl(QObject* parent)
	: QObject(parent)
	, preferred_resolution(R720p)
#ifdef YT_OBSOLETE
	, use_https_main(false)
#endif
	, use_dash(false)
	, use_60fps(false)
	, use_av1(false)
{
	process = new QProcess(this);
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
	connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

	clearData();
}

RetrieveYoutubeUrl::~RetrieveYoutubeUrl() {
}

QString RetrieveYoutubeUrl::ytdlBin() {
	if (!ytdl_bin.isEmpty()) {
		return ytdl_bin;
	}

#ifdef Q_OS_WIN
	ytdl_bin = "mpv/youtube-dl.exe";
#else
	ytdl_bin = QDir::homePath() + "/bin/youtube-dl";
	QFileInfo fi(ytdl_bin);
	if (!fi.exists() || !fi.isExecutable()) {
		ytdl_bin = "youtube-dl";
	}
#endif
	return ytdl_bin;
}

void RetrieveYoutubeUrl::close() {
	qDebug("RetrieveYoutubeUrl::close");
	process->close();
}

void RetrieveYoutubeUrl::clearData() {
	selected_video_url = "";
	selected_video_quality = None;

	selected_audio_url = "";
	selected_audio_quality = None;

	yt_url = "";
	video_title = "";
}

bool RetrieveYoutubeUrl::isUrlSupported(const QString & url) {
	return (!getVideoID(url).isEmpty());
}

QString RetrieveYoutubeUrl::fullUrl(const QString & url) {
	QString r;
	QString ID = getVideoID(url);
	if (!ID.isEmpty()) {
		#ifdef YT_OBSOLETE
		QString scheme = use_https_main ? "https" : "http";
		#else
		QString scheme =  "https";
		#endif
		r = scheme + "://www.youtube.com/watch?v=" + ID;
	}
	return r;
}

QString RetrieveYoutubeUrl::getVideoID(QString video_url) {
	//qDebug() << "RetrieveYoutubeUrl::getVideoID: video_url:" << video_url;

	if (video_url.contains("m.youtube.com")) {
		video_url.replace("m.youtube.com", "www.youtube.com");
	}

	if (video_url.startsWith("youtube.com") || video_url.startsWith("www.youtube.com") ||
		video_url.startsWith("youtu.be") || video_url.startsWith("www.youtu.be") || 
		video_url.startsWith("y2u.be") || video_url.startsWith("www.y2u.be"))
	{
		video_url = "http://" + video_url;
	}

	//qDebug() << "RetrieveYoutubeUrl::getVideoID: fixed url:" << video_url;

	QUrl url(video_url);

	QString ID;

	#if QT_VERSION >= 0x050000
	QUrlQuery * q = new QUrlQuery(url);
	#else
	const QUrl * q = &url;
	#endif

	/*
	qDebug() << "host:" << url.host();
	qDebug() << "path:" << url.path();
	*/

	if (url.host() == "youtu.be" || url.host() == "y2u.be") {
		ID = url.path();
		if (ID.startsWith("/")) ID = ID.mid(1);
	}
	else
	if ((url.host().contains("youtube")) && (url.path().contains("watch_videos"))) {
		if (q->hasQueryItem("video_ids")) {
			int index = 0;
			if (q->hasQueryItem("index")) index = q->queryItemValue("index").toInt();
			QStringList list = q->queryItemValue("video_ids").split(",");
			if (index < list.count()) ID = list[index];
		}
	}
	else
	if ((url.host().contains("youtube")) && (url.path().contains("watch"))) {
		if (q->hasQueryItem("v")) {
			ID = q->queryItemValue("v");
		}
	}

	#if QT_VERSION >= 0x050000
	delete q;
	#endif

	qDebug() << "RetrieveYoutubeUrl::getVideoID: ID:" << ID;

	return ID;
}

void RetrieveYoutubeUrl::fetchPage(const QString & url) {
	runYtdl(url);
}

void RetrieveYoutubeUrl::runYtdl(const QString & url) {
	clearData();

	emit connecting("youtube.com");

	yt_url = url;

	QStringList args;
	QString format;

	QString resolution = resolutionToText(preferred_resolution);
	if (!resolution.isEmpty()) resolution = "[height<=?" + resolution + "]";

	if (!user_format.isEmpty()) {
		format = user_format;
	} else {
		if (use_dash) {
			format = "bestvideo" + resolution +"[protocol!=http_dash_segments]";
			if (!use_60fps) format += "[fps!=60]";
			if (!use_av1) format += "[vcodec!*=av01]";
			format += ",bestaudio[protocol!=http_dash_segments]";
			format += "/best" + resolution;
		} else {
			format = "best" + resolution;
			//format += ",bestaudio[protocol!=http_dash_segments]";
		}
	}

	#ifdef YT_USE_JSON
	args << "-j";
	#else
	args << "--get-title" << "--get-format" << "-g";
	#endif
	args << "-f" << format << "--no-playlist";
	if (!user_agent.isEmpty()) args << "--user-agent" << user_agent;
	args << url;

	QString app_bin = ytdlBin();
	QFileInfo fi(app_bin);
	#ifdef Q_OS_WIN
	app_bin = fi.absoluteFilePath();
	#else
	if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
		app_bin = fi.absoluteFilePath();
	}
	#endif

	QString command = app_bin + " " + args.join(" ");
	qDebug() << "RetrieveYoutubeUrl::runYtdl: command:" << command;

	//process->setWorkingDirectory(full_output_dir);
	process->start(app_bin, args);

	if (!process->waitForStarted()) {
		qDebug("RetrieveYoutubeUrl::runYtdl: error: the process didn't start");
		emit processFailedToStart();
	}
}

void RetrieveYoutubeUrl::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	qDebug() << "RetrieveYoutubeUrl::processFinished: exitCode:" << exitCode << "exitStatus:" << exitStatus;

	QByteArray data = process->readAll().replace("\r", "").trimmed();
#ifndef YT_USE_JSON
	qDebug() << "RetrieveYoutubeUrl::fetchPage: process output:" << data;
#endif
	QList<QByteArray> lines = data.split('\n');
	qDebug() << "RetrieveYoutubeUrl::fetchPage: lines:" << lines.count();

#ifdef YT_USE_JSON
	QJsonDocument doc;
	QJsonObject json;

	if (lines.count() >= 1) {
		doc = QJsonDocument::fromJson(lines[0]);
		QJsonObject json = doc.object();
		video_title = json["title"].toString();
		selected_video_url = json["url"].toString();
		selected_video_quality = (Quality) json["format_id"].toString().toInt();
	}

	if (lines.count() >= 2) {
		doc = QJsonDocument::fromJson(lines[1]);
		QJsonObject json = doc.object();
		selected_audio_url = json["url"].toString();
		selected_audio_quality = (Quality) json["format_id"].toString().toInt();
	}

	#ifdef DEBUG_OUTPUT_JSON
	QString output = QDir::tempPath() + "/smplayer_yt.json";
	QFile f(output);
	if (f.open(QIODevice::WriteOnly)) {
		QByteArray o = "{";
		if (lines.count() >= 1) {
			o += "\"video\": "+ lines[0];
		}
		if (lines.count() >= 2) {
			o += ", \"audio\": " + lines[1];
		}
		o += "}";
		f.write(o);
		f.close();
		qDebug() << "RetrieveYoutubeUrl::processFinished: json data saved to" << output;
	}
	#endif
#else
	#define LINE_TITLE 0
	#define LINE_VIDEO_URL 1
	#define LINE_VIDEO_ITAG 2
	#define LINE_AUDIO_URL 4
	#define LINE_AUDIO_ITAG 5

	if (lines.count() >= 3) {
		video_title = QString::fromUtf8(lines[LINE_TITLE]);
		if (lines[LINE_VIDEO_URL].startsWith("http")) selected_video_url = lines[LINE_VIDEO_URL];
		selected_video_quality = (Quality) getItagFromFormat(lines[LINE_VIDEO_ITAG]);
	}

	if (lines.count() >= 6) {
		if (lines[LINE_AUDIO_URL].startsWith("http")) selected_audio_url = lines[LINE_AUDIO_URL];
		selected_audio_quality = (Quality) getItagFromFormat(lines[LINE_AUDIO_ITAG]);
	}
#endif

	qDebug() << "RetrieveYoutubeUrl::fetchPage: title:" << video_title;
	qDebug() << "RetrieveYoutubeUrl::fetchPage: video url:" << selected_video_url;
	qDebug() << "RetrieveYoutubeUrl::fetchPage: video itag:" << selected_video_quality;

	qDebug() << "RetrieveYoutubeUrl::fetchPage: audio url:" << selected_audio_url;
	qDebug() << "RetrieveYoutubeUrl::fetchPage: audio itag:" << selected_audio_quality;

	if (selected_video_url.isEmpty()) {
		emit gotEmptyList();
	} else {
		emit gotPreferredUrl(selected_video_url, selected_video_quality);
	}
}

void RetrieveYoutubeUrl::processError(QProcess::ProcessError error) {
	qDebug() << "RetrieveYoutubeUrl::processError:" << error;
}

QString RetrieveYoutubeUrl::extensionForItag(int itag) {
	QString ext = ".mp4";
	switch (itag) {
		case RetrieveYoutubeUrl::FLV_240p:
		case RetrieveYoutubeUrl::FLV_360p:
		case RetrieveYoutubeUrl::FLV_480p:
			ext = ".flv";
			break;
		case RetrieveYoutubeUrl::WEBM_360p:
		case RetrieveYoutubeUrl::WEBM_480p:
		case RetrieveYoutubeUrl::WEBM_720p:
		case RetrieveYoutubeUrl::WEBM_1080p:
			ext = ".webm";
			break;
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_48:
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_128:
		case RetrieveYoutubeUrl::DASH_AUDIO_MP4_256:
			ext = ".m4a";
			break;
		case RetrieveYoutubeUrl::DASH_AUDIO_WEBM_128:
		case RetrieveYoutubeUrl::DASH_AUDIO_WEBM_192:
			ext = ".webm";
			break;
	}

	return ext;
}

QString RetrieveYoutubeUrl::resolutionToText(Resolution r) {
	QString s;
	switch (r) {
		case R240p: s = "240"; break;
		case R360p: s = "360"; break;
		case R480p: s = "480"; break;
		case R720p: s = "720"; break;
		case R1080p: s = "1080"; break;
		case R1440p: s = "1440"; break;
		case R2160p: s = "2160"; break;
	}
	return s;
}

int RetrieveYoutubeUrl::getItagFromFormat(const QByteArray & t) {
	int itag = -1;
	QList<QByteArray> l = t.split(' ');
	if (l.count() > 0) {
		itag = l[0].toInt();
	}
	return itag;
}

#include "moc_retrieveyoutubeurl.cpp"
