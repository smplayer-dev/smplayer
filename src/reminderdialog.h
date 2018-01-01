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

#ifndef REMINDERDIALOG_H
#define REMINDERDIALOG_H

#include "ui_reminderdialog.h"

class QPushButton;

class ReminderDialog : public QDialog, public Ui::ReminderDialog
{
	Q_OBJECT

public:
	enum Button { Donate = 1, Share = 2, Close = 0 };
	ReminderDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~ReminderDialog();

	bool isRemindChecked();

protected slots:
	void button_clicked(QAbstractButton *);

private:
	QPushButton * donate_button;
	QPushButton * facebook_button;
	QPushButton * close_button;
};

#endif
