/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#include "desktopinfo.h"
#include <QApplication>

#if QT_VERSION >= 0x060000
#include <QWidget>
#else
#include <QDesktopWidget>
#endif
#include <QDebug>

#if QT_VERSION >= 0x050000
#include <QScreen>
#include <QWindow>

QScreen * DesktopInfo::find_screen(QWidget *w) {
	QScreen * screen = 0;

	while (w) {
		QWindow * window = w->windowHandle();
		if (window) {
			screen = window->screen();
			break;
		}
		w = w->parentWidget();
	}

	if (screen == 0) screen = QApplication::primaryScreen();
	return screen;
}
#endif

QSize DesktopInfo::desktop_size(QWidget *w) {
#if QT_VERSION >= 0x050000
	QScreen * screen = find_screen(w);
	qDebug() << "DesktopInfo::desktop_size: size of screen:" << screen->size();
	return screen->size();
#else
	QDesktopWidget * dw = QApplication::desktop();
	qDebug() << "DesktopInfo::desktop_size: primary screen:" << dw->primaryScreen();

	QSize s = dw->screen( dw->primaryScreen() )->size();

	qDebug() << "DesktopInfo::desktop_size: size of primary screen:" <<  s;
	//return dw->screen( dw->primaryScreen() )->size();

	QRect r = dw->screenGeometry(w);
	qDebug() << "DesktopInfo::desktop_size: size of screen:" << r;

	return QSize(r.width(), r.height() );
#endif
}

double DesktopInfo::desktop_aspectRatio(QWidget *w) {
	QSize s = DesktopInfo::desktop_size(w);
	return  (double) s.width() / s.height() ;
}

bool DesktopInfo::isInsideScreen(QWidget *w) {
	QPoint p = w->pos();

	p.setX(p.x() + w->size().width() / 2);
	p.setY(p.y() + w->size().height() / 2);

	qDebug() << "DesktopInfo::isInsideScreen: center point of window:" << p;

#if QT_VERSION >= 0x050000
	QRect r = QApplication::primaryScreen()->availableVirtualGeometry();
#else
	QRect r = QApplication::desktop()->geometry();
#endif

	qDebug() << "DesktopInfo::isInsideScreen: virtual geometry:" << r;
	return r.contains(p);
}

QPoint DesktopInfo::topLeftPrimaryScreen() {
#if QT_VERSION >= 0x050000
	QRect r = QApplication::primaryScreen()->geometry();
#else
	int screen = QApplication::desktop()->primaryScreen();
	QRect r = QApplication::desktop()->screenGeometry(screen);
#endif
	return r.topLeft();
}

