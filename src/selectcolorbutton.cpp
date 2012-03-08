/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "colorutils.h"
#include <QColorDialog>
#include <QApplication>
#include <QStyle>

SelectColorButton::SelectColorButton( QWidget * parent ) 
	: QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(selectColor()));

	ignore_change_event = false;
}

SelectColorButton::~SelectColorButton() {
}

void SelectColorButton::setColor(QColor c) {
	_color = c;

	QString current_style = qApp->style()->objectName();
	qDebug("SelectColorButton::setColor: current style name: %s", current_style.toUtf8().constData());

	ignore_change_event = true;

	setStyleSheet(
		QString("QPushButton { background-color: #%1; border-style: outset; "
                "border-width: 2px; border-radius: 5px; "
                "border-color: grey; padding: 3px; min-width: 4ex; min-height: 1.2ex; } "
                "QPushButton:pressed { border-style: inset; }"
                ).arg(ColorUtils::colorToRRGGBB(_color.rgb())) );

	ignore_change_event = false;
}

void SelectColorButton::selectColor() {
	QColor c = QColorDialog::getColor( _color, 0 );
	if (c.isValid()) {
		setColor( c );
	}
}

void SelectColorButton::changeEvent(QEvent *e) {

	QPushButton::changeEvent(e);
	
	if ((e->type() == QEvent::StyleChange) && (!ignore_change_event)) {
		setColor( color() );
	}

}

#include "moc_selectcolorbutton.cpp"
