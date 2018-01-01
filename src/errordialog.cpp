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

#include "errordialog.h"
#include "images.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

ErrorDialog::ErrorDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(log->viewport());
#endif

	text->setOpenExternalLinks(true);

	icon->setText("");
	icon->setPixmap( Images::icon("crash") );

	intro_label->setText("<html><head/><body><p align=\"left\"><span style=\"font-size:14pt; font-weight:600;\">" + tr("Oops, something went wrong") +"</span></p></body></html>");

	text->setText("");
	toggleLog(false);

	connect( viewlog_button, SIGNAL(toggled(bool)),
             this, SLOT(toggleLog(bool)) );

	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

ErrorDialog::~ErrorDialog() {
}

void ErrorDialog::setText(QString error) {
	text->setText(error);
}

void ErrorDialog::setLog(QString log_text) {
	log->setPlainText("");
	log->append(log_text); // To move cursor to the end
}

void ErrorDialog::toggleLog(bool checked) {
	log->setVisible(checked);

	if (checked) 
		viewlog_button->setText(tr("Hide log"));
	else
		viewlog_button->setText(tr("Show log"));
}

#include "moc_errordialog.cpp"
