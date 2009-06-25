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

#ifndef _FAVORITES_H_
#define _FAVORITES_H_

#include <QObject>
#include <QString>
#include <QList>

class QMenu;
class QAction;

class Favorite 
{
public:
	Favorite() {}
	Favorite(QString name, QString file, QString icon_file = QString::null) { _name = name; _file = file; _icon = icon_file; };
	virtual ~Favorite() {};

	void setName(QString name) { _name = name; };
	void setFile(QString file) { _file = file; };
	void setIconFile(QString file) { _icon = file; };

	QString name() { return _name; };
	QString file() { return _file; }
	QString iconFile() { return _icon; };

protected:
	QString _name, _file, _icon;
};

class Favorites : public QObject
{
	Q_OBJECT
public:
	Favorites(QString filename, QObject * parent = 0);
	~Favorites();

	QMenu * menu();

signals:
	void activated(QString filemane);

protected:
	void save();
	void load();
	void createMenu();

	int findFile(QString filename);

protected slots:
	void triggered_slot(QAction * action);
	void edit();

protected:
	QList<Favorite> f_list;
	QString _filename;
	QMenu * _menu;
	QAction * editAct;
};

#endif

