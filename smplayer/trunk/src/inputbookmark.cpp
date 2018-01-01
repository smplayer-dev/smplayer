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

#include "inputbookmark.h"

InputBookmark::InputBookmark( QWidget* parent, Qt::WindowFlags f ) 
	: QDialog(parent, f)
{
	setupUi(this);
	time_edit->setDisplayFormat("hh:mm:ss");
	name_edit->setFocus();
}

InputBookmark::~InputBookmark() {
}

void InputBookmark::setTime(int time) {
	QTime t(0,0);
	time_edit->setTime(t.addSecs(time));
}

int InputBookmark::time() {
	QTime t(0,0);
	return t.secsTo(time_edit->time());
}

void InputBookmark::setName(const QString & name) {
	name_edit->setText(name);
}

QString InputBookmark::name() {
	return name_edit->text();
}

#include "moc_inputbookmark.cpp"
