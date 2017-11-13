/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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


/***********************************************************************
 * Copyright 2012  Eike Hein <hein@kde.org>
 * Copyright 2012  Bernd Buschinski <b.buschinski@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#include "mediaplayer2player.h"
#include "mpris2.h"
#include "basegui.h"
#include "core.h"
#include "playlist.h"

#include <QCryptographicHash>

static QByteArray makeTrackId(const QString& source) {
    return QByteArray("/org/smplayer/") + "smplayer" + "/tid_" +
        QCryptographicHash::hash(source.toLocal8Bit(), QCryptographicHash::Sha1)
            .toHex();
}

MediaPlayer2Player::MediaPlayer2Player(BaseGui * gui, QObject* parent)
    : QDBusAbstractAdaptor(parent),
      m_core(gui->getCore()),
      m_playlist(gui->getPlaylist())
{
	//connect(m_core, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
	//connect(m_core, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChanged(bool)));
	connect(m_core, SIGNAL(mediaPlaying(QString,QString)), this, SLOT(currentSourceChanged()));
	connect(m_core, SIGNAL(stateChanged(Core::State)), this, SLOT(stateUpdated()));
	connect(m_core, SIGNAL(mediaInfoChanged()), this, SLOT(emitMetadataChange()));
	connect(m_core, SIGNAL(volumeChanged(int)), this, SLOT(volumeChanged()));
}

MediaPlayer2Player::~MediaPlayer2Player() {
}

bool MediaPlayer2Player::CanGoNext() const {
	return true;
}

void MediaPlayer2Player::Next() const {
	qDebug("MediaPlayer2Player::Next");
	m_playlist->playNext();
}

bool MediaPlayer2Player::CanGoPrevious() const {
	return true;
}

void MediaPlayer2Player::Previous() const {
	qDebug("MediaPlayer2Player::Previous");
    m_playlist->playPrev();
}

bool MediaPlayer2Player::CanPause() const {
	return true;
}

void MediaPlayer2Player::Pause() const {
	qDebug("MediaPlayer2Player::Pause");
	m_core->pause();
}

void MediaPlayer2Player::PlayPause() const {
	qDebug("MediaPlayer2Player::PlayPause");
	m_core->play_or_pause();
}

void MediaPlayer2Player::Stop() const {
	qDebug("MediaPlayer2Player::Stop");
	m_core->stop();
}

bool MediaPlayer2Player::CanPlay() const {
	return true;
}

void MediaPlayer2Player::Play() const {
	qDebug("MediaPlayer2Player::Play");
	m_core->play();
}

void MediaPlayer2Player::SetPosition(const QDBusObjectPath& TrackId, qlonglong Position) const {
	qDebug() << "MediaPlayer2Player::SetPosition: TrackId:" << TrackId.path() << "Position:" << Position;
	if (TrackId.path().toLocal8Bit() == makeTrackId(m_core->mdat.filename)) {
		m_core->seek(static_cast<int>(Position / 1000000));
	}
}

void MediaPlayer2Player::OpenUri(QString uri) const {
	qDebug() << "MediaPlayer2Player::OpenUri:" << uri;
	m_core->open(uri);
}

QString MediaPlayer2Player::PlaybackStatus() const {
	return m_core->stateToString();
}

QString MediaPlayer2Player::LoopStatus() const {
	return "None";
}

void MediaPlayer2Player::setLoopStatus(const QString& loopStatus) const {
	Q_UNUSED(loopStatus)
}

double MediaPlayer2Player::Rate() const {
	return m_core->mset.speed;
}

void MediaPlayer2Player::setRate(double rate) const {
	qDebug() << "MediaPlayer2Player::setRate:" << rate;
	m_core->setSpeed(rate);
}

bool MediaPlayer2Player::Shuffle() const {
	return false;
}

void MediaPlayer2Player::setShuffle(bool shuffle) const {
	Q_UNUSED(shuffle)
}

QVariantMap MediaPlayer2Player::Metadata() const {
	QVariantMap metaData;

	if (!m_core->mdat.initialized) {
		return metaData;
	}

	metaData["mpris:trackid"] = QVariant::fromValue<QDBusObjectPath>(QDBusObjectPath(makeTrackId(m_core->mdat.filename).constData()));
	metaData["mpris:length"] = m_core->mdat.duration * 1000000;

	if (m_core->mdat.type == TYPE_STREAM)
		metaData["xesam:url"] = m_core->mdat.stream_url;
	else
		metaData["xesam:url"] = m_core->mdat.filename;

	if (!m_core->mdat.clip_album.isEmpty())
		metaData["xesam:album"] = m_core->mdat.clip_album;
	if (!m_core->mdat.clip_name.isEmpty())
		metaData["xesam:title"] = m_core->mdat.clip_name;
	else if (!m_core->mdat.filename.isEmpty()) {
		QFileInfo fileInfo(m_core->mdat.filename);
		metaData["xesam:title"] = fileInfo.fileName();
	}
	if (!m_core->mdat.clip_artist.isEmpty())
		metaData["xesam:artist"] = m_core->mdat.clip_artist;
	if (!m_core->mdat.clip_genre.isEmpty())
		metaData["xesam:genre"] = m_core->mdat.clip_genre;

	return metaData;
}

double MediaPlayer2Player::Volume() const {
	return static_cast<double>(m_core->mset.volume / 100.0);
}

void MediaPlayer2Player::setVolume(double volume) const {
	qDebug() << "MediaPlayer2Player::setVolume:" << volume;
	m_core->setVolume(static_cast<int>(volume*100));
}

qlonglong MediaPlayer2Player::Position() const {
	return static_cast<qlonglong>(m_core->mset.current_sec * 1000000);
}

double MediaPlayer2Player::MinimumRate() const {
	return 0.01;
}

double MediaPlayer2Player::MaximumRate() const {
	return 100.0;
}

bool MediaPlayer2Player::CanSeek() const {
	return true;
}

void MediaPlayer2Player::Seek(qlonglong Offset) const {
	qDebug() << "MediaPlayer2Player::Seek:" << Offset;
	m_core->seek(static_cast<int>(Offset/1000000));
}

bool MediaPlayer2Player::CanControl() const {
	return true;
}

void MediaPlayer2Player::tick(qint64 newPos) {
	/*
	if (newPos - oldPos > tickInterval + 250 || newPos < oldPos) {
		emit Seeked(newPos * 1000);
	}
	*/

	oldPos = newPos;
}

void MediaPlayer2Player::emitMetadataChange() const {
	QVariantMap properties;
	properties["Metadata"] = Metadata();
	Mpris2::signalPropertiesChange(this, properties);
}

void MediaPlayer2Player::currentSourceChanged() const {
	QVariantMap properties;
	properties["Metadata"] = Metadata();
	properties["CanSeek"] = CanSeek();
	Mpris2::signalPropertiesChange(this, properties);
}

void MediaPlayer2Player::stateUpdated() const {
	QVariantMap properties;
	properties["PlaybackStatus"] = PlaybackStatus();
	properties["CanPause"] = CanPause();
	Mpris2::signalPropertiesChange(this, properties);
}

void MediaPlayer2Player::totalTimeChanged() const {
	QVariantMap properties;
	properties["Metadata"] = Metadata();
	Mpris2::signalPropertiesChange(this, properties);
}

void MediaPlayer2Player::seekableChanged(bool seekable) const {
	QVariantMap properties;
	properties["CanSeek"] = seekable;
	Mpris2::signalPropertiesChange(this, properties);
}

void MediaPlayer2Player::volumeChanged() const {
	QVariantMap properties;
	properties["Volume"] = Volume();
	Mpris2::signalPropertiesChange(this, properties);
}

#include "moc_mediaplayer2player.cpp"
