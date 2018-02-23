/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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
//#define PLAYLIST_DOUBLE_TOOLBAR
#define PLAYLIST_DELETE_FROM_DISK

#define YT_PLAYLIST_SUPPORT

class PLItem : public QStandardItem {
public:
	 enum PLItem_Roles { Role_Played = Qt::UserRole + 2, Role_Current = Qt::UserRole + 3, Role_Params = Qt::UserRole + 4,
                         Role_Video_URL = Qt::UserRole + 5, Role_Icon_URL = Qt::UserRole + 6 };

	PLItem();
	PLItem(const QString filename, const QString name, double duration);
	~PLItem();

	void setFilename(const QString filename);
	void setName(const QString name);
	void setDuration(double duration);
	void setExtraParams(const QStringList & pars);
	void setVideoURL(const QString & url);
	void setIconURL(const QString & url);
	void setPlayed(bool played);
	void setPosition(int position);
	void setCurrent(bool b);

	QString filename();
	QString name();
	double duration();
	QStringList extraParams();
	QString videoURL();
	QString iconURL();
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
	enum M3UFormat { M3U = 0, M3U8 = 1, DetectFormat = 2 };

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
	void addItem(QString filename, QString name, double duration, QStringList params = QStringList(),
                 QString video_url = QString::null, QString icon_url = QString::null);

	// Start playing, from item 0 if shuffle is off, or from
	// a random item otherwise
	void startPlay();

	void playItem(int n);

	void playNext();
	void playPrev();

	void playNextAuto(); // Called from GUI when a file finished

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

#ifdef PLAYLIST_DELETE_FROM_DISK
	void deleteSelectedFileFromDisk();
#endif

	bool maybeSave();
	void load();

	bool saveCurrentPlaylist();
	bool save(const QString & filename = QString::null);

#ifdef PLAYLIST_DOWNLOAD
	void openUrl();
	void openUrl(const QString & url);
#endif

	void load_m3u(QString file, M3UFormat format = DetectFormat);
	bool save_m3u(QString file);

	void load_pls(QString file);
	bool save_pls(QString file);

	void loadXSPF(const QString & filename);
	bool saveXSPF(const QString & filename);

#ifdef YT_PLAYLIST_SUPPORT
	void loadYoutubeList(QByteArray & data);
#endif

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
	void setStartPlayOnLoad(bool b) { start_play_on_load = b; };
	void setAutomaticallyPlayNext(bool b) { automatically_play_next = b; };
	void setAutoSort(bool b);
	void setSortCaseSensitive(bool b);
	void setFilterCaseSensitive(bool b);

	bool directoryRecursion() { return recursive_add_directory; };
	bool autoGetInfo() { return automatically_get_info; };
	bool savePlaylistOnExit() { return save_playlist_in_config; };
	bool playFilesFromStart() { return play_files_from_start; };
	bool ignorePlayerErrors() { return ignore_player_errors; };
	bool startPlayOnLoad() { return start_play_on_load; };
	bool automaticallyPlayNext() { return automatically_play_next; };
	bool autoSort();
	bool sortCaseSensitive();
	bool filterCaseSensitive();

#ifdef PLAYLIST_DOWNLOAD
	void setMaxItemsUrlHistory(int max_items);
	int maxItemsUrlHistory();
#endif

#ifdef PLAYLIST_DELETE_FROM_DISK
	void allowDeleteFromDisk(bool enabled) { allow_delete_from_disk = enabled; };
	bool isDeleteFromDiskAllowed() { return allow_delete_from_disk; };
#endif

#ifdef YT_PLAYLIST_SUPPORT
	static bool isYTPlaylist(const QString & url);
#endif

/*
public:
	MyAction * playPrevAct() { return prevAct; };
	MyAction * playNextAct() { return nextAct; };
*/

signals:
	void requestToPlayFile(const QString & filename, int seek = -1);
	void requestToPlayStream(const QString & filename, QStringList params = QStringList());

	void requestToAddCurrentFile();
	void playlistEnded();
	void visibilityChanged(bool visible);
	void modifiedChanged(bool);
	void windowTitleChanged(const QString & title);

protected:
	void setCurrentItem(int current);
	int findCurrentItem();
	void clearPlayedTag();
	int chooseRandomItem();
	QString lastDir();

	void setPlaylistFilename(const QString &);
	QString playlistFilename() { return playlist_filename; };

	void updateWindowTitle();

protected slots:
	void playCurrent();
	void itemActivated(const QModelIndex & index );
	void showPopup(const QPoint & pos);
	void upItem();
	void downItem();
	void editCurrentItem();
	void editItem(int row);

	void copyURL();
	void openFolder();

#ifdef CHROMECAST_SUPPORT
	void playOnChromecast();
#else
	void openURLInWeb();
#endif

	void saveSettings();
	void loadSettings();

	void maybeSaveSettings();

	void filterEditChanged(const QString &);

#ifdef PLAYLIST_DOWNLOAD
	void playlistDownloaded(QByteArray);
	void errorOcurred(int error_number, QString error_str);
	void showLoadingAnimation(bool b);
#endif

	void setPositionColumnVisible(bool b);
	void setNameColumnVisible(bool b);
	void setDurationColumnVisible(bool b);
	void setFilenameColumnVisible(bool b);

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
	QString playlist_filename;
	QString latest_dir;

	QMenu * file_menu;
	QMenu * add_menu;
	QMenu * remove_menu;
	QMenu * popup;

	QTableView * listView;
	QStandardItemModel * table;
	QSortFilterProxyModel * proxy;

	QToolBar * toolbar;
#ifdef PLAYLIST_DOUBLE_TOOLBAR
	QToolBar * toolbar2;
#endif

	QToolButton * file_button;
	QToolButton * add_button;
	QToolButton * remove_button;

	MyLineEdit * filter_edit;

	MyAction * openAct;
#ifdef PLAYLIST_DOWNLOAD
	MyAction * openUrlAct;
#endif
	MyAction * saveAct;
	MyAction * saveAsAct;
	MyAction * playAct;
	MyAction * prevAct;
	MyAction * nextAct;
	MyAction * repeatAct;
	MyAction * shuffleAct;
	MyAction * showSearchAct;

	MyAction * moveUpAct;
	MyAction * moveDownAct;
	MyAction * editAct;

	MyAction * addCurrentAct;
	MyAction * addFilesAct;
	MyAction * addDirectoryAct;
	MyAction * addUrlsAct;

	MyAction * removeSelectedAct;
	MyAction * removeAllAct;

#ifdef PLAYLIST_DELETE_FROM_DISK
	MyAction * deleteSelectedFileFromDiskAct;
#endif

	MyAction * copyURLAct;
	MyAction * openFolderAct;

#ifdef CHROMECAST_SUPPORT
	MyAction * playOnChromecastAct;
#else
	MyAction * openURLInWebAct;
#endif

	MyAction * showPositionColumnAct;
	MyAction * showNameColumnAct;
	MyAction * showDurationColumnAct;
	MyAction * showFilenameColumnAct;

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

	bool start_play_on_load;
	bool automatically_play_next;
	bool ignore_player_errors;
	bool change_name;
	bool save_dirs;

#ifdef PLAYLIST_DELETE_FROM_DISK
	bool allow_delete_from_disk;
#endif
};

#endif
