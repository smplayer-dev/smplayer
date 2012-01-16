/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@escomposlinux.org>

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
#include <QToolButton>
#include <QStyle>

//#define NO_SMPLAYER_SUPPORT

#ifndef NO_SMPLAYER_SUPPORT
#include "filedialog.h"
#include "images.h"
#else
#include <QFileDialog>
#endif

FileChooser::FileChooser(QWidget * parent) : LineEditWithIcon(parent) 
{
	setDialogType(GetFileName);
	setOptions(0);

	setupButton();

	connect(button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

FileChooser::~FileChooser() {
}

void FileChooser::setupButton() {
#ifdef NO_SMPLAYER_SUPPORT
	setIcon( QPixmap(":/find") );
#else
	setIcon( Images::icon("find") );
#endif
}

void FileChooser::openFileDialog() {
	qDebug("FileChooser::openFileDialog");

	QString result;
	QString f;

	if (dialogType() == GetFileName) {
		QFileDialog::Options opts = options();
		if (opts == 0) opts = QFileDialog::DontResolveSymlinks;

#ifndef NO_SMPLAYER_SUPPORT
		result = MyFileDialog::getOpenFileName( 
#else
		result = QFileDialog::getOpenFileName( 
#endif
                        this, caption(),
                        text(),
                        filter(), &f, opts );
	}
	else
	if (dialogType() == GetDirectory) {
		QFileDialog::Options opts = options();
		if (opts == 0) opts = QFileDialog::ShowDirsOnly;

#ifndef NO_SMPLAYER_SUPPORT
		result = MyFileDialog::getExistingDirectory(
#else
		result = QFileDialog::getExistingDirectory(
#endif
                    this, caption(),
                    text(), opts );
	}

	if (!result.isEmpty()) {
		QString old_file = text();
		setText(result);
		if (old_file != result) emit fileChanged(result);
	}
}

#include "moc_filechooser.cpp"
