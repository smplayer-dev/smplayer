/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2026 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef QTCOMPAT_H
#define QTCOMPAT_H

#include <QtGlobal>

#include <QSettings>
#include <QProcess>

// QSettings (IniFormat): Qt 5 reads/writes the ini file as Latin-1 and escapes
// every non-ASCII character as \xNNNN, while Qt 6 always reads/writes UTF-8.
// Forcing UTF-8 in Qt 5 makes both versions write and read the same file.
inline void QTC_setIniUtf8(QSettings * s) {
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	s->setIniCodec("UTF-8");
#else
	Q_UNUSED(s);
#endif
}

// Type of the "result" parameter of nativeEvent() / QAbstractNativeEventFilter::nativeEventFilter():
// it's a long* in Qt 5 and a qintptr* in Qt 6 (they are different types on 64-bit Windows,
// so a nativeEvent() declared with long* silently stops overriding the virtual in Qt 6).
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
typedef qintptr QTC_NativeEventResult;
#else
typedef long QTC_NativeEventResult;
#endif

// QProcess::error(QProcess::ProcessError) signal doesn't exist in Qt 6,
// it was replaced by errorOccurred() (available since Qt 5.6).
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#define QTC_PROCESS_ERROR_SIGNAL SIGNAL(errorOccurred(QProcess::ProcessError))
#else
#define QTC_PROCESS_ERROR_SIGNAL SIGNAL(error(QProcess::ProcessError))
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#define QTC_SkipEmptyParts Qt::SkipEmptyParts
#define QTC_endl Qt::endl
#else
#define QTC_SkipEmptyParts QString::SkipEmptyParts
#define QTC_endl endl
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
#define QTC_SPRINTF asprintf
#else
#define QTC_SPRINTF sprintf
#endif

#endif
