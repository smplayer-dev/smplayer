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

FileChooser::FileChooser(QWidget * parent) : QLineEdit(parent) 
{
	setDialogType(GetFileName);
	setOptions(0);

    browse_button = new QToolButton(this);
#ifdef NO_SMPLAYER_SUPPORT
	QPixmap pixmap(":/find");
#else
    QPixmap pixmap = Images::icon("find");
#endif
    browse_button->setIcon(QIcon(pixmap));
    //browse_button->setIconSize(pixmap.size());
    browse_button->setCursor(Qt::ArrowCursor);
    browse_button->setStyleSheet("QToolButton { border: none; padding: 0px; }");

    connect(browse_button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(browse_button->sizeHint().width() + frameWidth + 1));
    /*
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), browse_button->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), browse_button->sizeHint().height() + frameWidth * 2 + 2));
    */
}

FileChooser::~FileChooser() {
}

void FileChooser::resizeEvent(QResizeEvent *) {
    QSize sz = browse_button->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    browse_button->move(rect().right() - frameWidth - sz.width(),
                      (rect().bottom() + 1 - sz.height())/2);
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

