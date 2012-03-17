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

#ifndef _TOOLBAR_EDITOR_H_
#define _TOOLBAR_EDITOR_H_

#include <QStringList>
#include <QWidget>
#include <QList>
#include <QAction>
#include "ui_toolbareditor.h"

class QListWidget;

class ToolbarEditor : public QDialog, public Ui::ToolbarEditor
{
	Q_OBJECT

public:

	ToolbarEditor( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~ToolbarEditor();

	void setAllActions(QList<QAction *> actions_list);
	void setActiveActions(QList<QAction *> actions_list);

	QStringList activeActionsToStringList();

	void setDefaultActions(QStringList action_names) { default_actions = action_names; }
	QStringList defaultActions() { return default_actions; }

	//! Save the widget's list of actions into a QStringList 
	static QStringList save(QWidget *w);

	//! Add to the widget the actions specified in l. actions_list is
	//! the list of all available actions
	static void load(QWidget *w, QStringList l, QList<QAction *> actions_list);

protected slots:
	void on_up_button_clicked();
	void on_down_button_clicked();
	void on_right_button_clicked();
	void on_left_button_clicked();
	void on_separator_button_clicked();
	void restoreDefaults();
	void checkRowsAllList(int currentRow);
	void checkRowsActiveList(int currentRow);

protected:
	static QAction * findAction(QString s, QList<QAction *> actions_list);

	static void populateList(QListWidget * w, QList<QAction *> actions_list, bool add_separators = false);
	static int findItem(const QString & action_name, QListWidget * w);

	static QString fixname(const QString & name, const QString & action_name);

	QList<QAction *> all_actions_copy;
	QStringList default_actions;
};

#endif

