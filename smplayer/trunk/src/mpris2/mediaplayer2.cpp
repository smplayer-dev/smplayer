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

#include "mediaplayer2.h"
#include "mpris2.h"
#include "basegui.h"
#include <QApplication>


MediaPlayer2::MediaPlayer2(BaseGui* gui, QObject* parent)
    : QDBusAbstractAdaptor(parent),
      m_gui(gui)
{
//	connect(m_gui, SIGNAL(fullScreen(bool)), this, SLOT(emitFullscreenChange(bool)));
}

MediaPlayer2::~MediaPlayer2() {
}

bool MediaPlayer2::CanQuit() const {
    return true;
}

void MediaPlayer2::Quit() const {
	qDebug("MediaPlayer2::Quit");
	m_gui->runActions("close");
}

bool MediaPlayer2::CanRaise() const {
    return true;
}

void MediaPlayer2::Raise() const {
	qDebug("MediaPlayer2::Raise");
	m_gui->raise();
	m_gui->activateWindow();
}

bool MediaPlayer2::Fullscreen() const {
    return m_gui->isFullScreen();
}

void MediaPlayer2::setFullscreen(bool fullscreen) const {
	qDebug() << "MediaPlayer2::setFullscreen:" << fullscreen;
	m_gui->toggleFullscreen(fullscreen);
}

void MediaPlayer2::emitFullscreenChange(bool fullscreen) const {
	QVariantMap properties;
	properties["Fullscreen"] = fullscreen;
	Mpris2::signalPropertiesChange(this, properties);
}

bool MediaPlayer2::CanSetFullscreen() const {
	return true;
}

bool MediaPlayer2::HasTrackList() const {
	return false;
}

QString MediaPlayer2::Identity() const {
	return QString("SMPlayer");
}

QString MediaPlayer2::DesktopEntry() const {
	return QString("smplayer");
}

QStringList MediaPlayer2::SupportedUriSchemes() const {
	//TODO: Implement me
	return QStringList();
}

QStringList MediaPlayer2::SupportedMimeTypes() const {
	//TODO: Implement me
	return QStringList();
}

#include "moc_mediaplayer2.cpp"
