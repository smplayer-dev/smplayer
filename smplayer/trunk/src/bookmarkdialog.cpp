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

#include "bookmarkdialog.h"
#include "helper.h"

#include <QHeaderView>
#include <QTimeEdit>
#include <QItemDelegate>

#define COL_TIME 0
#define COL_NAME 1

class BEDelegate : public QItemDelegate
{
public:
	BEDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
	virtual void setModelData(QWidget * editor, QAbstractItemModel * model, 
                              const QModelIndex & index ) const;
};

BEDelegate::BEDelegate(QObject *parent) : QItemDelegate(parent) {
}

QWidget * BEDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	//qDebug("BEDelegate::createEditor");

	if (index.column() == COL_TIME) {
		QTimeEdit * e = new QTimeEdit(parent);
		e->setDisplayFormat("hh:mm:ss");
		int time = index.model()->data(index, Qt::UserRole).toInt();
		QTime t(0,0);
		e->setTime(t.addSecs(time));
		return e;
	}
	else {
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void BEDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	if (index.column() == COL_TIME) {
		QTimeEdit * e = static_cast<QTimeEdit*>(editor);
		QTime t(0,0);
		int time = t.secsTo(e->time());
		model->setData(index, Helper::formatTime(time));
		model->setData(index, time, Qt::UserRole);
	}
}


BookmarkDialog::BookmarkDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	table->setColumnCount(2);
	table->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Name") );

	table->setAlternatingRowColors(true);
#if QT_VERSION >= 0x050000
	table->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);
#else
	table->horizontalHeader()->setResizeMode(COL_NAME, QHeaderView::Stretch);
#endif

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::ExtendedSelection);

	table->setItemDelegateForColumn( COL_TIME, new BEDelegate(table) );
}

BookmarkDialog::~BookmarkDialog() {
}

void BookmarkDialog::setBookmarks(QMap<int, QString> b) {
	table->setRowCount(b.count());
	int n = 0;
	QMap<int, QString>::const_iterator i = b.constBegin();
	while (i != b.constEnd()) {
		QString name = i.value();
		int time = i.key();

		QTableWidgetItem * time_item = new QTableWidgetItem;
		time_item->setText(Helper::formatTime(time));
		time_item->setData(Qt::UserRole, time);

		QTableWidgetItem * name_item = new QTableWidgetItem;
		name_item->setText(name);

		table->setItem(n, COL_TIME, time_item);
		table->setItem(n, COL_NAME, name_item);

		i++;
		n++;
	}
}

QMap<int, QString> BookmarkDialog::bookmarks() {
	QMap<int, QString> b;

	for (int n = 0; n < table->rowCount(); n++) {
		int time = table->item(n, COL_TIME)->data(Qt::UserRole).toInt();
		QString name = table->item(n, COL_NAME)->text();
		b.insert(time, name);
	}

	return b;
}

void BookmarkDialog::on_delete_button_clicked() {
	int row = table->currentRow();
	qDebug("BookmarkDialog::on_delete_button_clicked: current_row: %d", row);

	if (row > -1) table->removeRow(row);

	if (row >= table->rowCount()) row--;
	table->setCurrentCell(row, table->currentColumn());
}

void BookmarkDialog::on_add_button_clicked() {
	int row = table->currentRow();
	qDebug("BookmarkDialog::on_add_button_clicked: current_row: %d", row);
	row++;
	table->insertRow(row);

	QTableWidgetItem * time_item = new QTableWidgetItem;
	int time = 0;
	time_item->setText(Helper::formatTime(time));
	time_item->setData(Qt::UserRole, time);

	table->setItem(row, COL_TIME, time_item);
	table->setItem(row, COL_NAME, new QTableWidgetItem);

	table->setCurrentCell(row, table->currentColumn());
}

#include "moc_bookmarkdialog.cpp"
