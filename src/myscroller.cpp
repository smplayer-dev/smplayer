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

#include "myscroller.h"
#include <QObject>

#if QT_VERSION >= 0x050000
#include <QScroller>
#include <QScrollerProperties>
#endif

void MyScroller::setScroller(QObject * w, bool touch) {
#if QT_VERSION >= 0x050000
	QScroller::grabGesture(w, touch ? QScroller::TouchGesture : QScroller::LeftMouseButtonGesture);

	// Change properties
	QVariant overshoot = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
	QScrollerProperties sp = QScroller::scroller(w)->scrollerProperties();
	sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, overshoot);
	sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, overshoot);
	QScroller::scroller(w)->setScrollerProperties(sp);
#endif
}

