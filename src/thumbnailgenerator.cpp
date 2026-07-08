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

#include "thumbnailgenerator.h"
#include "playerid.h"

#include <QProcess>
#include <QTemporaryDir>
#include <QDir>
#include <QFile>
#include <QDebug>

#define DEBUG 1

ThumbnailGenerator::ThumbnailGenerator(QObject * parent)
	: QObject(parent)
	, process(0)
	, tmp_dir(0)
	, cache(60) // keep up to 60 thumbnails in memory, plenty for one session
	, pending_time(-1)
	, extraction_running(false)
{
	process = new QProcess(this);

	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
	        this, SLOT(processFinished(int, QProcess::ExitStatus)));
	connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)),
	        this, SLOT(processError(QProcess::ProcessError)));

	tmp_dir = new QTemporaryDir();

	// For mplayer
	if (tmp_dir->isValid()) {
		process->setWorkingDirectory(tmp_dir->path());
	}
}

ThumbnailGenerator::~ThumbnailGenerator() {
	cancel();
	delete tmp_dir;
}

void ThumbnailGenerator::setPlayerBin(const QString & bin) {
	player_bin = bin;
}

void ThumbnailGenerator::setFilename(const QString & new_filename) {
	if (filename != new_filename) {
		cancel();
		cache.clear();
		filename = new_filename;
	}
}

QString ThumbnailGenerator::cacheKeyFor(double time) const {
	int bucket = qRound(time / CACHE_BUCKET_SECONDS) * CACHE_BUCKET_SECONDS;
	return filename + "|" + QString::number(bucket);
}

void ThumbnailGenerator::requestThumbnail(double time, const QPoint & pos) {
	if (filename.isEmpty() || player_bin.isEmpty()) return;
	QString key = cacheKeyFor(time);
	QPixmap * cached = cache.object(key);
	if (cached) {
		#if DEBUG
		qDebug("ThumbnailGenerator::requestThumbnail: cache hit for %s", key.toUtf8().constData());
		#endif
		emit thumbnailReady(*cached, time, pos);
		return;
	}

	// Coalesce: remember the latest request. If a process is already
	// running, let it finish (or get killed) before starting a new one;
	// processFinished() will pick up pending_time/pending_pos if they
	// changed meanwhile.
	pending_time = time;
	pending_pos = pos;

	if (extraction_running) {
		#if DEBUG
		qDebug("ThumbnailGenerator::requestThumbnail: extraction in progress, killing it");
		#endif
		process->kill();
		process->waitForFinished(500);
	}

	startExtraction(time, pos);
}

void ThumbnailGenerator::cancel() {
	pending_time = -1;
	if (extraction_running) {
		process->kill();
		process->waitForFinished(500);
	}
	extraction_running = false;
}

void ThumbnailGenerator::startExtraction(double time, const QPoint & pos) {
	Q_UNUSED(pos)

	// Clean up any leftover images from a previous extraction so
	// findExtractedImage() doesn't pick up a stale file.
	QDir dir(tmp_dir->path());
	foreach (const QString & f, dir.entryList(QDir::Files)) {
		dir.remove(f);
	}

	QStringList args;

	if (PlayerID::player(player_bin) == PlayerID::MPLAYER) {
		args << "-vo" << "png"
		     << "-ao" << "null"
		     << "-frames" << "3"
		     << "-ss" << QString::number(time, 'f', 2)
		     << "-really-quiet";
#ifdef Q_OS_WIN
		args << "-nofontconfig";
#endif
		args << filename;
	} else { // mpv
		args << ("--vo=image")
		     << ("--vo-image-outdir=" + tmp_dir->path())
		     << ("--vo-image-format=png")
		     << ("--vo-image-png-compression=0")
		     << "--frames=1"
		     << ("--start=" + QString::number(time, 'f', 2))
		     << "--no-audio"
		     << "--no-config"
		     << "--no-terminal"
		     << filename;
	}

	#if DEBUG
	qDebug() << "ThumbnailGenerator::startExtraction:" << player_bin << args;
	#endif

	extraction_running = true;
	process->start(player_bin, args);
}

QString ThumbnailGenerator::findExtractedImage() const {
	QDir dir(tmp_dir->path());
	QStringList images = dir.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
	if (images.isEmpty()) return QString();
	// With mplayer + "-frames 3" we may get several numbered files;
	// the last one (highest number) is the closest to the requested time.
	return dir.filePath(images.last());
}

void ThumbnailGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	Q_UNUSED(exitCode)
	extraction_running = false;

	if (exitStatus == QProcess::CrashExit) {
		#if DEBUG
		qDebug("ThumbnailGenerator::processFinished: process crashed or was killed");
		#endif
		return;
	}

	QString image_path = findExtractedImage();
	if (image_path.isEmpty()) {
		#if DEBUG
		qDebug("ThumbnailGenerator::processFinished: no image produced");
		#endif
		return;
	}

	QPixmap pixmap(image_path);
	QFile::remove(image_path);

	if (pixmap.isNull()) return;

	double time = pending_time;
	QPoint pos = pending_pos;

	cache.insert(cacheKeyFor(time), new QPixmap(pixmap));

	emit thumbnailReady(pixmap, time, pos);
}

void ThumbnailGenerator::processError(QProcess::ProcessError error) {
	#if DEBUG
	qDebug("ThumbnailGenerator::processError: %d", (int) error);
	#endif
	Q_UNUSED(error)
	extraction_running = false;
}

#include "moc_thumbnailgenerator.cpp"
