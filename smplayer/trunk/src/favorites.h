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

#ifndef _FAVORITES_H_
#define _FAVORITES_H_

#include <QMenu>
#include <QString>
#include <QList>

class QAction;
class QWidget;

class Favorite 
{
public:
	Favorite() { is_subentry = false; }
	Favorite(QString name, QString file, QString icon = QString::null, bool subentry = false) 
	{ 
		_name = name; _file = file; _icon = icon; is_subentry = subentry;
	};
	virtual ~Favorite() {};

	void setName(QString name) { _name = name; };
	void setFile(QString file) { _file = file; };
	void setIcon(QString file) { _icon = file; };
	void setSubentry(bool b) { is_subentry = b; }

	QString name() { return _name; };
	QString file() { return _file; }
	QString icon() { return _icon; };
	bool isSubentry() { return is_subentry; };

protected:
	QString _name, _file, _icon;
	bool is_subentry; // Not a favorite file, but a new favorite list
};

typedef QList<Favorite> FavoriteList;

class Favorites : public QMenu
{
	Q_OBJECT
public:
	Favorites(QString filename, QWidget * parent = 0);
	~Favorites();

	QAction * editAct() { return edit_act; };
	QAction * jumpAct() { return jump_act; };
	QAction * nextAct() { return next_act; };
	QAction * previousAct() { return previous_act; };
	QAction * addCurrentAct() { return add_current_act; };

public slots:
	void next();
	void previous();

	void getCurrentMedia(const QString & filename, const QString & title);

signals:
	void activated(QString filemane);
	//! Signal to resend the data to child
	void sendCurrentMedia(const QString & filename, const QString & title);

protected:
	virtual void save();
	virtual void load();
	virtual void updateMenu();
	virtual void populateMenu();
	virtual Favorites * createNewObject(QString filename, QWidget * parent);
	void delete_children();

	int findFile(QString filename);

	// Mark current action in the menu
	void markCurrent();

protected slots:
	void triggered_slot(QAction * action);
	virtual void edit();
	virtual void jump();
	virtual void addCurrentPlaying(); // Adds to menu current (or last played) file

protected:
    virtual void retranslateStrings();
    virtual void changeEvent(QEvent * event);

protected:
	FavoriteList f_list;
	QString _filename;
	QAction * edit_act;
	QAction * jump_act;
	QAction * next_act;
	QAction * previous_act;
	QAction * add_current_act;

	QWidget * parent_widget;

	// Current (or last) file clicked
	QString current_file;

	// Last item selected in the jump dialog
	int last_item;

	QString received_file_playing;
	QString received_title;

	QList<Favorites*> child;
};

#endif

