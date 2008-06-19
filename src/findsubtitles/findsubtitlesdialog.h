/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#ifndef _FINDSUBTITLESDIALOG_H_
#define _FINDSUBTITLESDIALOG_H_

#include "ui_findsubtitlesdialog.h"
#include <QByteArray>

class SimpleHttp;
class QStandardItemModel;
class QSortFilterProxyModel;
class QModelIndex;

class FindSubtitlesDialog : public QDialog, public Ui::FindSubtitlesDialog
{
	Q_OBJECT

public:
	FindSubtitlesDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~FindSubtitlesDialog();

public slots:
	void setMovie(QString filename);
	void refresh();
	void applyFilter(const QString & filter);
	void applyCurrentFilter();

protected slots:
	void showError(QString error);
	void connecting(QString host);
	void updateDataReadProgress(int done, int total);
	void downloadFinished();

	void updateRefreshButton();

	void parseInfo(QByteArray xml_text);

	void itemActivated(const QModelIndex & index );
	void currentItemChanged(const QModelIndex & current, const QModelIndex & previous);

	void on_download_button_clicked();

protected:
	virtual void retranslateStrings();
	virtual void changeEvent(QEvent * event);

private:
	SimpleHttp * downloader;
	QStandardItemModel * table;
	QSortFilterProxyModel * proxy_model;
	QString last_file;
};

#endif

