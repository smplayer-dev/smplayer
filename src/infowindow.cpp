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

#include "infowindow.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

InfoWindow::InfoWindow( QWidget* parent )
	: QWidget(parent, Qt::Window ) 
{
	setupUi(this);
	browser->setOpenExternalLinks(true);

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(browser);
#endif
}

InfoWindow::~InfoWindow() {
}

void InfoWindow::setHtml(QString text) {
	browser->setHtml(text);
}

QString InfoWindow::html() {
	return browser->toHtml();
}

void InfoWindow::clear() {
	browser->clear();
}

#include "moc_infowindow.cpp"
