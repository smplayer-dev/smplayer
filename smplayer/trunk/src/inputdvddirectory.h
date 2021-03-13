/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef INPUTDVDDIRECTORY_H
#define INPUTDVDDIRECTORY_H

#include "ui_inputdvddirectory.h"

class InputDVDDirectory : public QDialog, public Ui::InputDVDDirectory
{
	Q_OBJECT

public:
	InputDVDDirectory( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~InputDVDDirectory();

	void setFolder(QString folder);
	QString folder();

protected slots:
    void on_searchButton_clicked();
};


#endif
