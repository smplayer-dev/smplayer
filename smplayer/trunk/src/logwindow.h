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

#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include "ui_logwindow.h"

class QTextEdit;

class LogWindow : public QWidget, public Ui::LogWindow
{
	Q_OBJECT

public:
	LogWindow( QWidget* parent = 0);
	~LogWindow();

	void setText(QString log);
	QString text();

	void setHtml(QString text);
	QString html();

	void clear();

	void appendText(QString text);
	void appendHtml(QString text);

	/* QTextEdit * editor(); */

protected:
	virtual void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;

protected slots:
	void on_copyButton_clicked();
    void on_saveButton_clicked();
};


#endif
