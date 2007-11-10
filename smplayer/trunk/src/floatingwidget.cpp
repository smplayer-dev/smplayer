/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2007 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "floatingwidget.h"
#include <QToolBar>
#include <QHBoxLayout>

FloatingWidget::FloatingWidget( QWidget * parent )
	: QWidget( parent, Qt::Window | Qt::FramelessWindowHint |
                       Qt::WindowStaysOnTopHint |
                       Qt::X11BypassWindowManagerHint )
{
	setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );

	tb = new QToolBar;
	tb->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(2);
	layout->setMargin(2);
	layout->addWidget(tb);

	setLayout(layout);
}

FloatingWidget::~FloatingWidget() {
}

