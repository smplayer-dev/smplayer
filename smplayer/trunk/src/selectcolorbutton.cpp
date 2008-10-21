/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "selectcolorbutton.h"
#include "helper.h"
#include <QColorDialog>

SelectColorButton::SelectColorButton( QWidget * parent ) 
	: QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(selectColor()));
}

SelectColorButton::~SelectColorButton() {
}

void SelectColorButton::setColor(QColor c) {
	_color = c;

#ifdef Q_OS_WIN
	setStyleSheet( "border-width: 1px; border-style: solid; border-color: #000000; background: #" + Helper::colorToRRGGBB(_color.rgb()) + ";");
#else
	//setAutoFillBackground(true);
	Helper::setBackgroundColor( this, _color );
#endif
}

void SelectColorButton::selectColor() {
	QColor c = QColorDialog::getColor( _color, 0 );
	if (c.isValid()) {
		setColor( c );
	}
}

#include "moc_selectcolorbutton.cpp"
