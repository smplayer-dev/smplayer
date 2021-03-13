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

#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include "lineedit_with_icon.h"
#include <QFileDialog>

class QToolButton;

class FileChooser : public LineEditWithIcon
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QString caption READ caption WRITE setCaption)
	Q_PROPERTY(QString filter READ filter WRITE setFilter)
	Q_PROPERTY(DialogType dialogType READ dialogType WRITE setDialogType)
	Q_PROPERTY(QFileDialog::Options options READ options WRITE setOptions)

public:
	enum DialogType { GetFileName = 0, GetDirectory = 1 };

	FileChooser( QWidget* parent = 0 );
	~FileChooser();

	QString caption() const { return _caption; };
	QString filter() const { return _filter; };
	DialogType dialogType() const { return _type; };
	QFileDialog::Options options() const { return _options; };

public slots:
	void setCaption(const QString & caption) { _caption = caption; };
	void setFilter(const QString & filter) { _filter = filter; };
	void setDialogType( DialogType type) { _type = type; };
	void setOptions( QFileDialog::Options options) { _options = options; };

signals:
	void fileChanged(QString file);

protected:
	virtual void setupButton();

protected slots:
	virtual void openFileDialog();

protected:
	QString _caption;
	QString _filter;
	DialogType _type;
	QFileDialog::Options _options;

	static QString last_dir;
};

#endif
