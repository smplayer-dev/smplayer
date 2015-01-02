/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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


#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H

#include <QObject>
#include <QDate>

class QSettings;
class QNetworkAccessManager;

class UpdateCheckerData {
public:
	UpdateCheckerData() { enabled = true; days_to_check = 7; };
	void save(QSettings * set);
	void load(QSettings * set);

	QDate last_checked;
	bool enabled;
	int days_to_check;
	QString last_known_version;
};

class UpdateChecker : public QObject {
	Q_OBJECT

public:
	UpdateChecker(QObject * parent, UpdateCheckerData * data);
	~UpdateChecker();

	void saveVersion(QString v);

protected slots:
	void gotReply();

signals:
	void newVersionFound(QString);

protected:
	QNetworkAccessManager * net_manager;
	UpdateCheckerData * d;
};

#endif

