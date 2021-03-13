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

/* This is based on qq14-actioneditor-code.zip from Qt */

#ifndef ACTIONSEDITOR_H
#define ACTIONSEDITOR_H

#include <QWidget>
#include <QList>
#include <QStringList>
#include "guiconfig.h"

class QTableView;
class QStandardItemModel;
class QStandardItem;
class QModelIndex;
class QSortFilterProxyModel;

class QAction;
class QSettings;
class QPushButton;
class QLineEdit;

class ActionsEditor : public QWidget
{
	Q_OBJECT

public:
	ActionsEditor( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~ActionsEditor();

	// Clear the actionlist
	void clear();

	// There are no actions yet?
	bool isEmpty();

	void addActions(QWidget * widget);

	// Static functions
	static QAction * findAction(QObject *o, const QString & name);
	static QStringList actionsNames(QObject *o);

	static void saveToConfig(QObject *o, QSettings *set);
	static void loadFromConfig(QObject *o, QSettings *set);

	#if USE_MULTIPLE_SHORTCUTS
	static QString shortcutsToString(QList <QKeySequence> shortcuts_list);
	static QList <QKeySequence> stringToShortcuts(QString shortcuts);
	#endif

public slots:
	void applyChanges();
	void saveActionsTable();
	bool saveActionsTable(const QString & filename);
	void loadActionsTable();
	bool loadActionsTable(const QString & filename);

	void updateView();

protected:
	virtual void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;

	// Find in table, not in actionslist
	int findActionName(const QString & name);
	int findActionAccel(const QString & accel, int ignoreRow = -1);
	bool hasConflicts();
	static bool containsShortcut(const QString & accel, const QString & shortcut);

protected slots:
	#if !USE_SHORTCUTGETTER
	void recordAction(const QModelIndex &);
	void validateAction(QStandardItem * i);
	#else
	void editShortcut();
	#endif
	void filterEditChanged(const QString &);

private:
	QTableView *actionsTable;
	QStandardItemModel * table;
	QSortFilterProxyModel * proxy;

	QList<QAction*> actionsList;
	QPushButton *saveButton;
	QPushButton *loadButton;
	QLineEdit * filterEdit;
	QString latest_dir;

	#if USE_SHORTCUTGETTER
	QPushButton *editButton;
	#else
	QString oldAccelText;
	bool dont_validate;
	#endif
};

#endif
