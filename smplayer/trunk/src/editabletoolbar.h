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

#ifndef EDITABLETOOLBAR_H
#define EDITABLETOOLBAR_H

#include <QToolBar>
#include <QList>
#include <QStringList>

class QAction;
class QWidget;

class EditableToolbar : public QToolBar
{
	Q_OBJECT

public:
	EditableToolbar( QWidget * parent = 0 );
	~EditableToolbar();

	void setActionsFromStringList(QStringList action_names);
	QStringList actionsToStringList();

	void setAvailableActions(QList<QAction *> available_actions) { all_actions = available_actions; }

	void takeAvailableActionsFrom(QWidget * w) { widget = w; }

	void setDefaultActions(QStringList action_names) { default_actions = action_names; }
	QStringList defaultActions() { return default_actions; }

public slots:
	void edit();

protected:
	QList<QAction *> allActions();

	QList<QAction *> all_actions;
	QWidget * widget;

	QStringList default_actions;
};

#endif
