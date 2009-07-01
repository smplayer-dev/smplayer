/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "favoriteeditor.h"
#include "images.h"

#include <QHeaderView>
#include <QFileDialog>

#define COL_ICON 0
#define COL_NAME 1
#define COL_FILE 2

FavoriteEditor::FavoriteEditor( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	table->setColumnCount(3);
	table->setHorizontalHeaderLabels(QStringList() << tr("Icon") << tr("Name") << tr("Media") );

	table->setAlternatingRowColors(true);
	table->horizontalHeader()->setResizeMode(COL_FILE, QHeaderView::Stretch);

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::ExtendedSelection);

	connect(table, SIGNAL(cellActivated(int,int)), this, SLOT(edit_icon(int,int)));

	setWindowTitle( tr("Favorite editor") );

	setCaption( tr("Favorite list") );
	setIntro( tr("You can edit, delete, sort or add new items. Double click on "
                 "a cell to edit its contents.") );

	setDialogIcon( Images::icon("favorite") );
}

FavoriteEditor::~FavoriteEditor() {
}

void FavoriteEditor::setCaption(const QString & caption) {
	caption_text = caption;
	updateTitleLabel();
}

QString FavoriteEditor::caption() {
	return caption_text;
}

void FavoriteEditor::setIntro(const QString & intro) {
	intro_text = intro;
	updateTitleLabel();
}

QString FavoriteEditor::intro() {
	return intro_text;
}

void FavoriteEditor::updateTitleLabel() {
	title_label->setText( "<h1>" + caption_text + "</h1>" + intro_text );
}

void FavoriteEditor::setDialogIcon( const QPixmap & icon ) {
	dialog_icon->setPixmap(icon);
}

const QPixmap * FavoriteEditor::dialogIcon() const {
	return dialog_icon->pixmap();
}

void FavoriteEditor::setData( FavoriteList list ) {
	table->setRowCount(list.count());

	for (int n = 0; n < list.count(); n++) {
		QTableWidgetItem * icon_item = new QTableWidgetItem;
		icon_item->setIcon( QIcon(list[n].icon()) );
		icon_item->setData( Qt::UserRole, list[n].icon() );
		icon_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		QTableWidgetItem * name_item = new QTableWidgetItem;
		name_item->setText( list[n].name() );

		QTableWidgetItem * file_item = new QTableWidgetItem;
		file_item->setText( list[n].file() );

		table->setItem(n, COL_ICON, icon_item);
		table->setItem(n, COL_NAME, name_item);
		table->setItem(n, COL_FILE, file_item);
	}

	table->setCurrentCell(0, 0);
}

FavoriteList FavoriteEditor::data() {
	FavoriteList list;

	for (int n = 0; n < table->rowCount(); n++) {
		Favorite f;
		f.setName( table->item(n, COL_NAME)->text() );
		f.setFile( table->item(n, COL_FILE)->text() );
		f.setIcon( table->item(n, COL_ICON)->data(Qt::UserRole).toString() );

		list.append(f);
	}

	return list;
}

void FavoriteEditor::on_delete_button_clicked() {
	int row = table->currentRow();
	qDebug("FavoriteEditor::on_delete_button_clicked: current_row: %d", row);

	if (row > -1) table->removeRow(row);

	if (row >= table->rowCount()) row--;
	table->setCurrentCell(row, table->currentColumn());
}

void FavoriteEditor::on_delete_all_button_clicked() {
	qDebug("FavoriteEditor::on_delete_all_button_clicked");
	table->setRowCount(0);
}

void FavoriteEditor::on_add_button_clicked() {
	int row = table->currentRow();
	qDebug("FavoriteEditor::on_add_button_clicked: current_row: %d", row);
	row++;
	table->insertRow(row);

	QTableWidgetItem * icon_item = new QTableWidgetItem;
	icon_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	table->setItem(row, COL_ICON, icon_item);
	table->setItem(row, COL_NAME, new QTableWidgetItem);
	table->setItem(row, COL_FILE, new QTableWidgetItem);

	table->setCurrentCell(row, table->currentColumn());
}

void FavoriteEditor::on_up_button_clicked() {
	int row = table->currentRow();
	qDebug("FavoriteEditor::on_up_button_clicked: current_row: %d", row);

	if (row == 0) return;

	// take whole rows
	QList<QTableWidgetItem*> source_items = takeRow(row);
	QList<QTableWidgetItem*> dest_items = takeRow(row-1);
 
	// set back in reverse order
	setRow(row, dest_items);
	setRow(row-1, source_items);

	table->setCurrentCell(row-1, table->currentColumn());
}

void FavoriteEditor::on_down_button_clicked() {
	int row = table->currentRow();
	qDebug("FavoriteEditor::on_down_button_clicked: current_row: %d", row);

	if ((row+1) >= table->rowCount()) return;

	// take whole rows
	QList<QTableWidgetItem*> source_items = takeRow(row);
	QList<QTableWidgetItem*> dest_items = takeRow(row+1);
 
	// set back in reverse order
	setRow(row, dest_items);
	setRow(row+1, source_items);

	table->setCurrentCell(row+1, table->currentColumn());
}
 
// takes and returns the whole row
QList<QTableWidgetItem*> FavoriteEditor::takeRow(int row) {
	QList<QTableWidgetItem*> rowItems;
	for (int col = 0; col < table->columnCount(); ++col)
	{
		rowItems << table->takeItem(row, col);
	}
	return rowItems;
}
 
// sets the whole row
void FavoriteEditor::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
	for (int col = 0; col < table->columnCount(); ++col)
	{
		table->setItem(row, col, rowItems.at(col));
	}
}

void FavoriteEditor::edit_icon(int row, int column ) {
	qDebug("FavoriteEditor::edit_icon: %d, %d", row, column);

	if (column != COL_ICON) return;

	QTableWidgetItem * i = table->item(row, column);
	QString icon_filename = i->data(Qt::UserRole).toString();

	qDebug("FavoriteEditor::edit_icon: icon file: '%s'", icon_filename.toUtf8().constData());

	QString dir = icon_filename;
	if (dir.isEmpty()) dir = last_dir;

	QString res = QFileDialog::getOpenFileName(this, tr("Select an icon file"),
                                               dir,
                                               tr("Images") + " (*.png *.xpm *.jpg)");
	if (!res.isEmpty()) {
		i->setIcon( QIcon(res) );
		i->setData( Qt::UserRole, res );

		last_dir = QFileInfo(res).absolutePath();
	}
}

#include "moc_favoriteeditor.cpp"
