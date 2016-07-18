/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QList>
#include <QStringList>
#include <QWidget>
#include <QModelIndex>
#include <QStandardItem>
#include <QProcess>
#include "mediadata.h"

#define PLAYLIST_DOWNLOAD

class PLItem : public QStandardItem {
public:
	 enum PLItem_Roles { Role_Played = Qt::UserRole + 2, Role_Current = Qt::UserRole + 3 };

	PLItem();
	PLItem(const QString filename, const QString name, double duration);
	~PLItem();

	void setFilename(const QString filename);
	void setName(const QString name);
	void setDuration(double duration);
	void setPlayed(bool played);
	void setPosition(int position);
	void setCurrent(bool b);

	QString filename();
	QString name();
	double duration();
	bool played();
	int position();
	bool isCurrent();

	QList<QStandardItem *> items();

protected:
	QStandardItem * col_num;
	QStandardItem * col_duration;
	QStandardItem * col_filename;
};


class QTableView;
class QStandardItemModel;
class QStandardItem;
class QSortFilterProxyModel;
class QToolBar;
class MyAction;
class MyLineEdit;
class LoadPage;
class QMenu;
class QSettings;
class QToolButton;
class QTimer;
class QMovie;
class URLHistory;

class Playlist : public QWidget
{
	Q_OBJECT

public:
	enum AutoGetInfo { NoGetInfo = 0, GetInfo = 1, UserDefined = 2 };

	Playlist(QWidget * parent = 0, Qt::WindowFlags f = Qt::Window );
	~Playlist();

	void setConfigPath(const QString & config_path);

	void clear();
	void list();

	int count();
	bool isEmpty();

	bool isModified() { return modified; };

	PLItem * itemData(int row);
	PLItem * itemFromProxy(int row);
	bool existsItem(int row);

	/*
	void changeItem(int row, const QString & filename, const QString name, double duration, bool played = false, int pos = -1);
	*/

public slots:
	void addItem(QString filename, QString name, double duration);

	// Start playing, from item 0 if shuffle is off, or from
	// a random item otherwise
	void startPlay();

	void playItem(int n);

	void playNext();
	void playPrev();

	void resumePlay();

	void removeSelected();
	void removeAll();

	void addCurrentFile();
	void addFiles();
	void addDirectory();
	void addUrls();

	void addFile(QString file, AutoGetInfo auto_get_info = UserDefined);
	void addFiles(QStringList files, AutoGetInfo auto_get_info = UserDefined);

	// Adds a directory, no recursive
	void addOneDirectory(QString dir);

	// Adds a directory, maybe with recursion (depends on user config)
	void addDirectory(QString dir);

	void deleteSelectedFileFromDisk();

	bool maybeSave();
	void load();
	bool save();

#ifdef PLAYLIST_DOWNLOAD
	void openUrl();
	void openUrl(const QString & url);
#endif

	void load_m3u(QString file);
	bool save_m3u(QString file);

	void load_pls(QString file);
	bool save_pls(QString file);

	void loadXSPF(const QString & filename);
	bool saveXSPF(const QString & filename);

	void setModified(bool);

	void setFilter(const QString & filter);

	// Slots to connect from basegui
	void getMediaInfo(const MediaData &);
	void playerFailed(QProcess::ProcessError);
	void playerFinishedWithError(int);

public:
	// Preferences
	void setDirectoryRecursion(bool b) { recursive_add_directory = b; };
	void setAutoGetInfo(bool b) { automatically_get_info = b; };
	void setSavePlaylistOnExit(bool b) { save_playlist_in_config = b; };
	void setPlayFilesFromStart(bool b) { play_files_from_start = b; };
	void setIgnorePlayerErrors(bool b) { ignore_player_errors = b; };
	void setAutomaticallyPlayNext(bool b) { automatically_play_next = b; };

	bool directoryRecursion() { return recursive_add_directory; };
	bool autoGetInfo() { return automatically_get_info; };
	bool savePlaylistOnExit() { return save_playlist_in_config; };
	bool playFilesFromStart() { return play_files_from_start; };
	bool ignorePlayerErrors() { return ignore_player_errors; };
	bool automaticallyPlayNext() { return automatically_play_next; };

/*
public:
	MyAction * playPrevAct() { return prevAct; };
	MyAction * playNextAct() { return nextAct; };
*/

signals:
	void requestToPlayFile(const QString & filename, int seek = -1);
	void requestToAddCurrentFile();
	void playlistEnded();
	void visibilityChanged(bool visible);
	void modifiedChanged(bool);

protected:
	void setCurrentItem(int current);
	int findCurrentItem();
	void clearPlayedTag();
	int chooseRandomItem();
	QString lastDir();

protected slots:
	void playCurrent();
	void itemActivated(const QModelIndex & index );
	void showPopup(const QPoint & pos);
	void upItem();
	void downItem();
	void editCurrentItem();
	void editItem(int row);

	void saveSettings();
	void loadSettings();

	void maybeSaveSettings();

	void filterEditChanged(const QString &);

#ifdef PLAYLIST_DOWNLOAD
	void playlistDownloaded(QByteArray);
	void errorOcurred(int error_number, QString error_str);
	void showLoadingAnimation(bool b);
#endif

protected:
	void createTable();
	void createActions();
	void createToolbar();

protected:
	void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;
	virtual void dragEnterEvent( QDragEnterEvent * ) ;
	virtual void dropEvent ( QDropEvent * );
	virtual void hideEvent ( QHideEvent * );
	virtual void showEvent ( QShowEvent * );
	virtual void closeEvent( QCloseEvent * e );

protected:
	QString playlist_path;
	QString latest_dir;

	QMenu * add_menu;
	QMenu * remove_menu;
	QMenu * popup;

	QTableView * listView;
	QStandardItemModel * table;
	QSortFilterProxyModel * proxy;

	QToolBar * toolbar;
	QToolButton * add_button;
	QToolButton * remove_button;

	MyLineEdit * filter_edit;

	MyAction * openAct;
#ifdef PLAYLIST_DOWNLOAD
	MyAction * openUrlAct;
#endif
	MyAction * saveAct;
	MyAction * playAct;
	MyAction * prevAct;
	MyAction * nextAct;
	MyAction * repeatAct;
	MyAction * shuffleAct;

	MyAction * moveUpAct;
	MyAction * moveDownAct;
	MyAction * editAct;

	MyAction * addCurrentAct;
	MyAction * addFilesAct;
	MyAction * addDirectoryAct;
	MyAction * addUrlsAct;

	MyAction * removeSelectedAct;
	MyAction * removeAllAct;

	MyAction * deleteSelectedFileFromDiskAct;

	QSettings * set;

#ifdef PLAYLIST_DOWNLOAD
	LoadPage * downloader;
	URLHistory * history_urls;
	QMovie * animation;
	QAction * loading_label_action;
#endif

private:
	bool modified;
	QTimer * save_timer;

	//Preferences
	bool recursive_add_directory;
	bool automatically_get_info;
	bool save_playlist_in_config;
	bool play_files_from_start;
	int row_spacing;

	bool automatically_play_next;
	bool ignore_player_errors;
	bool change_name;
	bool save_dirs;
};

#endif
