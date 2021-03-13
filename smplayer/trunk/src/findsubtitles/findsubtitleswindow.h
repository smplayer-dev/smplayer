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

#ifndef FINDSUBTITLESWINDOW_H
#define FINDSUBTITLESWINDOW_H

#include "ui_findsubtitleswindow.h"

#ifdef FS_USE_PROXY
#include <QNetworkProxy>
#endif

class OSClient;
class QStandardItemModel;
class QSortFilterProxyModel;
class QModelIndex;
class QMenu;
class QAction;
class QSettings;

#ifdef DOWNLOAD_SUBS
class FileDownloader;
class QBuffer;
class QuaZip;
#endif

class FindSubtitlesWindow : public QWidget, public Ui::FindSubtitlesWindow
{
	Q_OBJECT

public:
	FindSubtitlesWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~FindSubtitlesWindow();

	QString language();
#ifdef DOWNLOAD_SUBS
	bool includeLangOnFilename() { return include_lang_on_filename; };
#endif

	void setSettings(QSettings * settings);
	QSettings * settings() { return set; };

public slots:
	void setMovie(QString filename);
	void setLanguage(const QString & lang);
	void searchTitle();
	void refresh();
	void download();
	void copyLink();
#ifdef DOWNLOAD_SUBS
	void setIncludeLangOnFilename(bool b) { include_lang_on_filename = b; };
#endif

protected slots:
	void applyFilter(const QString & filter);
	void applyCurrentFilter();

	void showError(QString error);
	void showConnecting();
	//void showConnecting(QString host);
	void showLoginFailed();
	void showSearchFailed();
	void showErrorOS(int, const QString &);
	void updateDataReadProgress(int done, int total);
	void downloadFinished();

	void updateRefreshButton();

	void parseInfo();

	void itemActivated(const QModelIndex & index );
	void currentItemChanged(const QModelIndex & current, const QModelIndex & previous);

	void showContextMenu(const QPoint & pos);

#ifdef DOWNLOAD_SUBS
	void archiveDownloaded(const QByteArray & buffer);
#endif

	void on_configure_button_clicked();

protected:
	virtual void retranslateStrings();
	virtual void changeEvent(QEvent * event);

#ifdef FS_USE_PROXY
	void setProxy(QNetworkProxy proxy);
	void setupProxy();
#endif

	void saveSettings();
	void loadSettings();

	void updateSearchTitleWidget();

#ifdef DOWNLOAD_SUBS
signals:
	void subtitleDownloaded(const QString & filename);

protected:
	#ifdef USE_QUAZIP
	bool uncompressZip(const QString & filename, const QString & output_path, const QString & preferred_output_name);
	bool extractFile(QuaZip & zip, const QString & filename, const QString & output_name);
	#else
	QByteArray gUncompress(const QByteArray &data);
	#endif

protected slots:
	void fixSubtitles(const QString & filename);
#endif

protected:
	OSClient * osclient;
	QStandardItemModel * table;
	QSortFilterProxyModel * proxy_model;
	QString last_file;

	QMenu * context_menu;
	QAction * downloadAct;
	QAction * copyLinkAct;

#ifdef DOWNLOAD_SUBS
	FileDownloader * file_downloader;
	bool include_lang_on_filename;
#endif

	// Opensubtitles server
	QString os_server;

#ifdef FS_USE_PROXY
	// Proxy
	bool use_proxy;
	int proxy_type;
	QString proxy_host;
	int proxy_port;
	QString proxy_username;
	QString proxy_password;
#endif

	QSettings * set;
};

#endif

