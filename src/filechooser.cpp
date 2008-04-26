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

#include "filechooser.h"
#include "filedialog.h"
#include "images.h"

FileChooser::FileChooser(QWidget * parent) : QWidget(parent) 
{
	setupUi(this);

	button->setIcon(Images::icon("find"));

	setDialogType(GetFileName);
	setOptions(0);
}

FileChooser::~FileChooser() {
}

QLineEdit * FileChooser::lineEdit() {
	return line_edit;
}

QToolButton * FileChooser::toolButton() {
	return button;
}

QString FileChooser::text() const {
	return line_edit->text();
}

void FileChooser::setText(const QString & text) {
	line_edit->setText(text);
}

void FileChooser::on_button_clicked() {
	QString result;
	QString f;

	if (dialogType() == GetFileName) {
		QFileDialog::Options opts = options();
		if (opts == 0) opts = QFileDialog::DontResolveSymlinks;

		result = MyFileDialog::getOpenFileName( 
                        this, caption(),
                        line_edit->text(),
                        filter(), &f, opts );
	}
	else
	if (dialogType() == GetDirectory) {
		QFileDialog::Options opts = options();
		if (opts == 0) opts = QFileDialog::ShowDirsOnly;

		result = MyFileDialog::getExistingDirectory(
                    this, caption(),
                    line_edit->text(), opts );
	}

	if (!result.isEmpty()) {
		line_edit->setText(result);
	}
}

#include "moc_filechooser.cpp"

