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

#include "toolbareditor.h"

#include <QToolBar>
#include <QToolButton>
#include <QMatrix>

#include "images.h"

ToolbarEditor::ToolbarEditor( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	up_button->setIcon(Images::icon("up"));
	down_button->setIcon(Images::icon("down"));

	QMatrix matrix;
	matrix.rotate(90);

	right_button->setIcon( Images::icon("up").transformed(matrix) );
	left_button->setIcon( Images::icon("down").transformed(matrix) );

	QPushButton * restore = buttonBox->button(QDialogButtonBox::RestoreDefaults);
	connect(restore, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

	connect(all_actions_list, SIGNAL(currentRowChanged(int)),
            this, SLOT(checkRowsAllList(int)));
	connect(active_actions_list, SIGNAL(currentRowChanged(int)),
            this, SLOT(checkRowsActiveList(int)));

#if QT_VERSION >= 0x040600
	all_actions_list->setSelectionMode(QAbstractItemView::SingleSelection);
	all_actions_list->setDragEnabled(true);
	all_actions_list->viewport()->setAcceptDrops(true);
	all_actions_list->setDropIndicatorShown(true);
	all_actions_list->setDefaultDropAction(Qt::MoveAction); // Qt 4.6
	//all_actions_list->setDragDropMode(QAbstractItemView::InternalMove);

	active_actions_list->setSelectionMode(QAbstractItemView::SingleSelection);
	active_actions_list->setDragEnabled(true);
	active_actions_list->viewport()->setAcceptDrops(true);
	active_actions_list->setDropIndicatorShown(true);
	active_actions_list->setDefaultDropAction(Qt::MoveAction); // Qt 4.6
	//active_actions_list->setDragDropMode(QAbstractItemView::InternalMove);
#endif
}

ToolbarEditor::~ToolbarEditor() {
}

void ToolbarEditor::populateList(QListWidget * w, QList<QAction *> actions_list, bool add_separators) {
	w->clear();

	QAction * action;
	for (int n = 0; n < actions_list.count(); n++) {
		action = static_cast<QAction*> (actions_list[n]);
		if (action) {
			if (!action->objectName().isEmpty()) {
				QListWidgetItem * i = new QListWidgetItem;
				QString text = fixname(action->text(), action->objectName());
				i->setText(text + " ("+ action->objectName() +")");
				i->setIcon(action->icon());
				i->setData(Qt::UserRole, action->objectName());
				w->addItem(i);
			}
			else
			if ((action->isSeparator()) && (add_separators)) {
				QListWidgetItem * i = new QListWidgetItem;
				i->setText(tr("(separator)"));
				i->setData(Qt::UserRole, "separator");
				w->addItem(i);
			}
		}
	}
}

void ToolbarEditor::setAllActions(QList<QAction *> actions_list) {
	populateList(all_actions_list, actions_list, false);
	all_actions_copy = actions_list;
}

void ToolbarEditor::setActiveActions(QList<QAction *> actions_list) {
	populateList(active_actions_list, actions_list, true);

	// Delete actions from the "all list" which are in the active list
	for (int n = 0; n < active_actions_list->count(); n++) {
		int row = findItem( active_actions_list->item(n)->data(Qt::UserRole).toString(), all_actions_list );
		if (row > -1) {
			qDebug("found: %s", active_actions_list->item(n)->data(Qt::UserRole).toString().toUtf8().constData());
			all_actions_list->takeItem(row);
		}
	}
}

int ToolbarEditor::findItem(const QString & action_name, QListWidget * w) {
	for (int n = 0; n < w->count(); n++) {
		if (w->item(n)->data(Qt::UserRole).toString() == action_name) {
			return n;
		}
	}
	return -1;
}

void ToolbarEditor::on_up_button_clicked() {
	int row = active_actions_list->currentRow();
	qDebug("ToolbarEditor::on_up_button_clicked: current_row: %d", row);

	if (row == 0) return;

	QListWidgetItem * current = active_actions_list->takeItem(row);
	active_actions_list->insertItem(row-1, current);
	active_actions_list->setCurrentRow(row-1);
}

void ToolbarEditor::on_down_button_clicked() {
	int row = active_actions_list->currentRow();
	qDebug("ToolbarEditor::on_down_button_clicked: current_row: %d", row);

	if ((row+1) >= active_actions_list->count()) return;

	QListWidgetItem * current = active_actions_list->takeItem(row);
	active_actions_list->insertItem(row+1, current);
	active_actions_list->setCurrentRow(row+1);
}

void ToolbarEditor::on_right_button_clicked() {
	int row = all_actions_list->currentRow();
	qDebug("ToolbarEditor::on_right_button_clicked: current_row: %d", row);

	if (row > -1) {
		QListWidgetItem * current = all_actions_list->takeItem(row);
		int dest_row = active_actions_list->currentRow();
		if (dest_row > -1) {
			active_actions_list->insertItem(dest_row+1, current);
		} else {
			active_actions_list->addItem(current);
		}
	}
}

void ToolbarEditor::on_left_button_clicked() {
	int row = active_actions_list->currentRow();
	qDebug("ToolbarEditor::on_left_button_clicked: current_row: %d", row);

	if (row > -1) {
		QListWidgetItem * current = active_actions_list->takeItem(row);
		if (current->data(Qt::UserRole).toString() != "separator") {
			int dest_row = all_actions_list->currentRow();
			if (dest_row > -1) {
				all_actions_list->insertItem(dest_row+1, current);
			} else {
				all_actions_list->addItem(current);
			}
		}
	}
}

void ToolbarEditor::on_separator_button_clicked() {
	qDebug("ToolbarEditor::on_separator_button_clicked");

	QListWidgetItem * i = new QListWidgetItem;
	i->setText(tr("(separator)"));
	i->setData(Qt::UserRole, "separator");

	int row = active_actions_list->currentRow();
	if (row > -1) {
		active_actions_list->insertItem(row+1, i);
	} else {
		active_actions_list->addItem(i);
	}
}

void ToolbarEditor::restoreDefaults() {
	qDebug("ToolbarEditor::restoreDefaults");
	populateList(all_actions_list, all_actions_copy, false);

	// Create list of actions
	QList<QAction *> actions;
	QAction * a = 0;
	for (int n = 0; n < default_actions.count(); n++) {
		if (default_actions[n] == "separator") {
			QAction * sep = new QAction(this);
			sep->setSeparator(true);
			actions.push_back(sep);
		} else {
			a = findAction(default_actions[n], all_actions_copy);
			if (a) actions.push_back(a);
		}
	}
	setActiveActions(actions);
}

QStringList ToolbarEditor::activeActionsToStringList() {
	QStringList o;
	for (int n = 0; n < active_actions_list->count(); n++) {
		o << active_actions_list->item(n)->data(Qt::UserRole).toString();
	}
	return o;
}

void ToolbarEditor::checkRowsAllList(int currentRow) {
	qDebug("ToolbarEditor::checkRowsAllList: current row: %d", currentRow);
	right_button->setEnabled(currentRow > -1);
}

void ToolbarEditor::checkRowsActiveList(int currentRow) {
	qDebug("ToolbarEditor::checkRowsActiveList: current row: %d", currentRow);
	left_button->setEnabled(currentRow > -1);
	if (currentRow == -1) {
		up_button->setEnabled(false);
		down_button->setEnabled(false);
	} else {
		up_button->setEnabled((currentRow > 0));
		down_button->setEnabled((currentRow < active_actions_list->count()-1));
	}
}

QString ToolbarEditor::fixname(const QString & name, const QString & action_name) {
	QString s = name;
	s = s.replace("&", "");
	if (action_name == "timeslider_action") s = tr("Time slider");
	else
	if (action_name == "volumeslider_action") s = tr("Volume slider");
	else
	if (action_name == "timelabel_action") s = tr("Display time");
	else
	if (action_name == "rewindbutton_action") s = tr("3 in 1 rewind");
	else
	if (action_name == "forwardbutton_action") s = tr("3 in 1 forward");
	return s;
}

QStringList ToolbarEditor::save(QWidget * w) {
	qDebug("ToolbarEditor::save: '%s'", w->objectName().toUtf8().data());

	QList<QAction *> list = w->actions();
	QStringList o;
	QAction * action;

	for (int n = 0; n < list.count(); n++) {
		action = static_cast<QAction*> (list[n]);
		if (action->isSeparator()) {
			o << "separator";
		}
		else
		if (!action->objectName().isEmpty()) {
			o << action->objectName();
		}
		else
		qWarning("ToolbarEditor::save: unknown action at pos %d", n);
	}

	return o;
}

void ToolbarEditor::load(QWidget *w, QStringList l, QList<QAction *> actions_list)
{
	qDebug("ToolbarEditor::load: '%s'", w->objectName().toUtf8().data());

	QAction * action;

	for (int n = 0; n < l.count(); n++) {
		qDebug("ToolbarEditor::load: loading action %s", l[n].toUtf8().data());

		if (l[n] == "separator") {
			qDebug("ToolbarEditor::load: adding separator");
			QAction * sep = new QAction(w);
			sep->setSeparator(true);
			w->addAction(sep);
		} else {
			action = findAction(l[n], actions_list);
			if (action) {
				w->addAction(action);
				if (action->objectName().endsWith("_menu")) {
					// If the action is a menu and is in a toolbar, as a toolbutton, change some of its properties
					QToolBar * toolbar = qobject_cast<QToolBar *>(w);
					if (toolbar) {
						QToolButton * button = qobject_cast<QToolButton *>(toolbar->widgetForAction(action));
						if (button) {
							//qDebug("ToolbarEditor::load: action %s is a toolbutton", action->objectName().toUtf8().constData());
							button->setPopupMode(QToolButton::InstantPopup);
							//button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
						}
					}
				}
			} else {
				qWarning("ToolbarEditor::load: action %s not found", l[n].toUtf8().data());
			}
		}
	}
}

QAction * ToolbarEditor::findAction(QString s, QList<QAction *> actions_list) {
	QAction * action;

	for (int n = 0; n < actions_list.count(); n++) {
		action = static_cast<QAction*> (actions_list[n]);
		if (action->objectName() == s) return action;
	}

	return 0;
}

#include "moc_toolbareditor.cpp"
