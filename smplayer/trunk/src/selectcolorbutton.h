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

#ifndef SELECTCOLORBUTTON_H
#define SELECTCOLORBUTTON_H

#include <QPushButton>

class SelectColorButton : public QPushButton
{
	Q_OBJECT

public:
	SelectColorButton ( QWidget * parent = 0 );
	~SelectColorButton();

	QColor color() { return _color;} ;

public slots:
	void setColor(QColor c);

private slots:
	void selectColor();

private:
	QColor _color;

	bool ignore_change_event;
	
protected:
	virtual void changeEvent ( QEvent * event ) ;
};

#endif

