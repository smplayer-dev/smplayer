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

#include "images.h"

ToolbarEditor::ToolbarEditor( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	up_button->setIcon(Images::icon("up"));
	down_button->setIcon(Images::icon("down"));
}

ToolbarEditor::~ToolbarEditor() {
}

void ToolbarEditor::populateList(QListWidget * w, QList<QAction *> actions_list) {
	w->clear();

	QAction * action;
	for (int n = 0; n < actions_list.count(); n++) {
		action = static_cast<QAction*> (actions_list[n]);
		if (!action->objectName().isEmpty()) {
			QListWidgetItem * i = new QListWidgetItem(w);
			QString text = action->text().replace("&", "");
			i->setText(text + " ("+ action->objectName() +")");
			i->setIcon(action->icon());
			i->setData(Qt::UserRole, action->objectName());
			w->addItem(i);
		}
	}
}

void ToolbarEditor::setAllActions(QList<QAction *> actions_list) {
	populateList(all_actions_list, actions_list);
}

void ToolbarEditor::setActiveActions(QList<QAction *> actions_list) {
	populateList(active_actions_list, actions_list);
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
