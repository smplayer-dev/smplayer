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

#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QCache>
#include <QPoint>
#include <QProcess>

class QTemporaryDir;

/*
   Generates a single-frame thumbnail from a local video file at a given
   time, using either mplayer or mpv (whichever is the currently selected
   player), by running it as a one-shot headless process that dumps one
   frame to disk as a PNG.

   Only meant to be used with local files (see BaseGuiPlus::showThumbnail
   for the check on MediaData::type). Streams are explicitly out of scope:
   reopening a network source on every request would be slow and, for
   live sources, doesn't even make sense.

   Requests are debounced/coalesced: if a new request for the same slider
   comes in while a previous extraction is still running, the previous
   process is killed and only the latest position is honored. Results are
   cached in memory (keyed by a rounded timestamp) so hovering back over
   an already-seen position is instantaneous.
*/
class ThumbnailGenerator : public QObject
{
	Q_OBJECT

public:
	ThumbnailGenerator(QObject * parent = 0);
	~ThumbnailGenerator();

	// Path to the player binary (mplayer or mpv) as selected in preferences.
	void setPlayerBin(const QString & bin);

	// File currently being previewed. Clears the cache if it changed.
	void setFilename(const QString & filename);

	// Rounds "time" and either serves it from cache immediately (emitting
	// thumbnailReady synchronously) or launches an extraction process.
	// "pos" is the screen point the resulting popup should be shown at.
	void requestThumbnail(double time, const QPoint & pos);

	// Aborts any in-flight extraction and hides/cancels pending results.
	void cancel();

signals:
	void thumbnailReady(QPixmap pixmap, double time, QPoint pos);

private slots:
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void processError(QProcess::ProcessError error);

private:
	enum { CACHE_BUCKET_SECONDS = 2 };

	QString cacheKeyFor(double time) const;
	void startExtraction(double time, const QPoint & pos);
	QString findExtractedImage() const;

	QProcess * process;
	QTemporaryDir * tmp_dir;
	QCache<QString, QPixmap> cache;

	QString player_bin;
	QString filename;

	double pending_time;
	QPoint pending_pos;
	bool extraction_running;
};

#endif
