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

#include "mycombobox.h"
#include <QDir>
#include <QStringListModel>
#include <QDebug>

MyComboBox::MyComboBox( QWidget * parent ) : QComboBox(parent)
{
}

MyComboBox::~MyComboBox() 
{
}

void MyComboBox::setCurrentText( const QString & text ) {
	int i = findText(text);
	if (i != -1)
		setCurrentIndex(i);
	else if (isEditable())
		setEditText(text);
	else
		setItemText(currentIndex(), text);
}

void MyComboBox::insertStringList( const QStringList & list, int index ) {
	insertItems((index < 0 ? count() : index), list);
}



MyFontComboBox::MyFontComboBox( QWidget * parent ) : QFontComboBox(parent)
{
}

MyFontComboBox::~MyFontComboBox() 
{
}

void MyFontComboBox::setCurrentText( const QString & text ) {
	int i = findText(text);
	if (i != -1)
		setCurrentIndex(i);
	else if (isEditable())
		setEditText(text);
	else
		setItemText(currentIndex(), text);
}

void MyFontComboBox::setFontsFromDir(const QString & fontdir) {
	QString current_text = currentText();

	if (fontdir.isEmpty()) {
		QFontDatabase::removeAllApplicationFonts();
		clear();
		setWritingSystem(QFontDatabase::Any);
	} else {
		QFontDatabase fdb;
		QStringList fontnames;
		QStringList fontfiles = QDir(fontdir).entryList(QStringList() << "*.ttf" << "*.otf", QDir::Files);
		for (int n=0; n < fontfiles.count(); n++) {
			qDebug() << "MyFontComboBox::setFontsFromDir: adding font:" << fontfiles[n];
			int id = fdb.addApplicationFont(fontdir +"/"+ fontfiles[n]);
			fontnames << fdb.applicationFontFamilies(id);
		}
		//fdb.removeAllApplicationFonts();
		fontnames.removeDuplicates();
		qDebug() << "MyFontComboBox::setFontsFromDir: fontnames:" << fontnames;
		clear();
		QStringListModel *m = qobject_cast<QStringListModel *>(model());
		if (m) m->setStringList(fontnames);
	}

	setCurrentText(current_text);
}

