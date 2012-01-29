/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _FAVORITEEDITOR_H_
#define _FAVORITEEDITOR_H_

#include "ui_favoriteeditor.h"
#include "favorites.h"

class QPixmap;

class FavoriteEditor : public QDialog, public Ui::FavoriteEditor
{
	Q_OBJECT

public:
	FavoriteEditor( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~FavoriteEditor();

	void setCaption(const QString & caption);
	QString caption();

	void setIntro(const QString & intro);
	QString intro();

	void setData( FavoriteList list );
	FavoriteList data();

	void setDialogIcon( const QPixmap & icon );
	const QPixmap * dialogIcon() const;

	//! The editor will give a filename to subentries (submenus), using this path.
	void setStorePath(const QString & path) { store_path = path; }
	QString storePath() { return store_path; }

protected slots:
	void on_up_button_clicked();
	void on_down_button_clicked();
	void on_delete_button_clicked();
	void on_delete_all_button_clicked();
	void on_add_button_clicked();
	void on_add_submenu_button_clicked();

	void edit_icon( int row, int column ); 

protected:
	QList<QTableWidgetItem*> takeRow(int row);
	void setRow(int row, const QList<QTableWidgetItem*>& rowItems);

	void updateTitleLabel();

	QString caption_text, intro_text;
	static QString last_dir;
	QString store_path;
};

#endif
