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

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QtGlobal>

#ifdef Q_OS_WIN
  //#define USE_WINEVENTFILTER
#endif

#if defined(USE_WINEVENTFILTER) && QT_VERSION >= 0x050000
#include <QAbstractNativeEventFilter>
#endif

#ifdef SINGLE_INSTANCE
#include "QtSingleApplication"

class MyApplication : public QtSingleApplication
#if defined(USE_WINEVENTFILTER) && QT_VERSION >= 0x050000
, QAbstractNativeEventFilter
#endif
{
	Q_OBJECT

public:
	MyApplication(const QString & appId, int & argc, char ** argv);

	#if QT_VERSION < 0x050000
	virtual void commitData(QSessionManager & manager);
	#endif

	inline static MyApplication * instance() {
		return qobject_cast<MyApplication*>(QApplication::instance());
	}
	
#ifdef USE_WINEVENTFILTER
	virtual bool winEventFilter(MSG * msg, long * result);
	#if QT_VERSION >= 0x050000
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
	#endif
#endif

#if QT_VERSION >= 0x050000
protected slots:
	void commitData(QSessionManager & manager);
#endif
};

#else
#include <QApplication>

class MyApplication : public QApplication
#if defined(USE_WINEVENTFILTER) && QT_VERSION >= 0x050000
, QAbstractNativeEventFilter
#endif
{
	Q_OBJECT

public:
	MyApplication ( const QString & appId, int & argc, char ** argv );

	#if QT_VERSION < 0x050000
	virtual void commitData(QSessionManager & manager);
	#endif

#ifdef USE_WINEVENTFILTER
	virtual bool winEventFilter(MSG * msg, long * result);
	#if QT_VERSION >= 0x050000
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
	#endif
#endif

#if QT_VERSION >= 0x050000
protected slots:
	void commitData(QSessionManager & manager);
#endif
};

#endif

#endif

