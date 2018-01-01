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

#include "desktopinfo.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

QSize DesktopInfo::desktop_size(QWidget *w) {
	QDesktopWidget * dw = QApplication::desktop();
	qDebug("DesktopInfo::desktop_size: primary screen: %d", dw->primaryScreen());

	QSize s = dw->screen( dw->primaryScreen() )->size();

	qDebug("DesktopInfo::desktop_size: size of primary screen: %d x %d", s.width(), s.height() );
	//return dw->screen( dw->primaryScreen() )->size();

	QRect r = dw->screenGeometry(w);
	qDebug("DesktopInfo::desktop_size: size of screen: %d x %d", r.width(), r.height() );

	return QSize(r.width(), r.height() );
}

double DesktopInfo::desktop_aspectRatio(QWidget *w) {
    QSize s = DesktopInfo::desktop_size(w);
    return  (double) s.width() / s.height() ;
}

bool DesktopInfo::isInsideScreen(QWidget *w) {
	QDesktopWidget * dw = QApplication::desktop();
	QRect r = dw->screenGeometry(w);

	qDebug() << "DesktopInfo::isInsideScreen: geometry of screen:" << r;

	QPoint p = w->pos();

	p.setX(p.x() + w->size().width() / 2);
	p.setY(p.y() + w->size().height() / 2);

	qDebug() << "DesktopInfo::isInsideScreen: center point of window:" << p;

	return r.contains(p);
}

QPoint DesktopInfo::topLeftPrimaryScreen() {
	int screen = QApplication::desktop()->primaryScreen();
	QRect r = QApplication::desktop()->screenGeometry(screen);
	return r.topLeft();
}

