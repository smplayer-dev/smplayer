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

#include "playlist.h"

#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QToolBar>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QMenu>
#include <QDateTime>
#include <QSettings>
#include <QInputDialog>
#include <QToolButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHeaderView>
#include <QTextCodec>
#include <QApplication>
#include <QMimeData>
#include <QDomDocument>
#include <QDebug>

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

#include "myaction.h"
#include "mylineedit.h"
#include "filedialog.h"
#include "helper.h"
#include "images.h"
#include "preferences.h"
#include "multilineinputdialog.h"
#include "version.h"
#include "extensions.h"
#include "guiconfig.h"

#ifdef PLAYLIST_DOWNLOAD
#include "inputurl.h"
#include "youtube/loadpage.h"
#include "urlhistory.h"
#include <QNetworkAccessManager>
#include <QTemporaryFile>
#include <QClipboard>
#include <QMovie>
#endif

#if USE_INFOPROVIDER
#include "infoprovider.h"
#endif

#define DRAG_ITEMS 0
#define PL_ALLOW_DUPLICATES 1
#define SIMULATE_FILE_DELETION 0
#define USE_ITEM_DELEGATE 0

#define COL_NUM 0
#define COL_NAME 1
#define COL_TIME 2
#define COL_FILENAME 3

#if USE_ITEM_DELEGATE
class PlaylistDelegate : public QStyledItemDelegate {
public:
	PlaylistDelegate(QObject * parent = 0) : QStyledItemDelegate(parent) {};
	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
		QStyleOptionViewItem opt = option;
		initStyleOption(&opt, index);
		if (index.column() == COL_NAME) {
			bool played = index.data(PLItem::Role_Played).toBool();
			bool current = index.data(PLItem::Role_Current).toBool();
			if (current) opt.font.setBold(true);
			else
			if (played) opt.font.setItalic(true);
		}
		else
		if (index.column() == COL_FILENAME) {
			opt.textElideMode = Qt::ElideMiddle;
		}
		QStyledItemDelegate::paint(painter, opt, index);
	}
};
#endif

/* ----------------------------------------------------------- */


PLItem::PLItem() : QStandardItem() {
	col_num = new QStandardItem();
	col_duration = new QStandardItem();
	col_filename = new QStandardItem();

	setDuration(0);
	setPlayed(false);
	setCurrent(false);

	col_num->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

PLItem::PLItem(const QString filename, const QString name, double duration) : QStandardItem() {
	col_num = new QStandardItem();
	col_duration = new QStandardItem();
	col_filename = new QStandardItem();

	setFilename(filename);
	setName(name);
	setDuration(duration);
	setPlayed(false);
	setCurrent(false);

	col_num->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

PLItem::~PLItem() {
}

void PLItem::setFilename(const QString filename) {
	col_filename->setText(filename);
	col_filename->setToolTip(filename);
	col_filename->setData(filename);

	if (!filename.contains("://") && filename.count() > 50) {
		QStringList parts = filename.split(QDir::separator());
		//if (!parts.isEmpty() && parts[0].isEmpty()) parts[0] = "/";
		//qDebug() << "PLItem::setFilename: parts count:" << parts.count() << "parts:" << parts;
		if (parts.count() >= 2) {
			QString s = parts[parts.count()-2] + QDir::separator() + parts[parts.count()-1];
			if (parts.count() > 2) s = QString("...") + QDir::separator() + s;
			col_filename->setText(s);
		}
	}
}

void PLItem::setName(const QString name) {
	setText(name);
	setData(name);
	setToolTip(name);
}

void PLItem::setDuration(double duration) {
	col_duration->setData(duration);
	col_duration->setText(Helper::formatTime(duration));
}

void PLItem::setPlayed(bool played) {
	setData(played, Role_Played);
#if !USE_ITEM_DELEGATE
	QFont f = font();
	f.setItalic(played);
	setFont(f);
#endif
}

void PLItem::setPosition(int position) {
	//col_num->setText(QString("%1").arg(position, 4, 10, QChar('0')));
	col_num->setText(QString::number(position));
	col_num->setData(position);
}

void PLItem::setCurrent(bool b) {
	setData(b, Role_Current);
#if !USE_ITEM_DELEGATE
	QFont f = font();
	f.setBold(b);
	f.setItalic(b ? false : played());
	setFont(f);
#endif
}

QString PLItem::filename() {
	return col_filename->data().toString();
}

QString PLItem::name() {
	return text();
}

double PLItem::duration() {
	return col_duration->data().toDouble();
}

bool PLItem::played() {
	return data(Role_Played).toBool();
}

int PLItem::position() {
	return col_num->data().toInt();
}

bool PLItem::isCurrent() {
	return data(Role_Current).toBool();
}

QList<QStandardItem *> PLItem::items() {
	QList<QStandardItem *> l;
	l << col_num << this << col_duration << col_filename;
	return l;
}


/* ----------------------------------------------------------- */


Playlist::Playlist(QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent,f)
	, set(0)
	, modified(false)
	, recursive_add_directory(false)
	, automatically_get_info(false)
	, save_playlist_in_config(true)
	, play_files_from_start(true)
	, row_spacing(-1) // Default height
	, automatically_play_next(true)
	, ignore_player_errors(false)
	, change_name(true)
	, save_dirs(true)
{
	playlist_path = "";
	latest_dir = "";

	createTable();
	createActions();
	createToolbar();

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget( listView );
	layout->addWidget( toolbar );
	setLayout(layout);

	clear();

	retranslateStrings();

#if !DOCK_PLAYLIST
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
	adjustSize();
#else
	//setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
	//setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
	setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
#endif

	setAcceptDrops(true);
	setAttribute(Qt::WA_NoMousePropagation);

	// Random seed
	QTime now = QTime::currentTime();
	qsrand(now.msec());

	//loadSettings();

	// Save config every 5 minutes.
	save_timer = new QTimer(this);
	connect( save_timer, SIGNAL(timeout()), this, SLOT(maybeSaveSettings()) );
	save_timer->start( 5 * 60000 );

#ifdef PLAYLIST_DOWNLOAD
	downloader = new LoadPage(new QNetworkAccessManager(this), this);
	downloader->setUserAgent("SMPlayer");
	connect(downloader, SIGNAL(pageLoaded(QByteArray)), this, SLOT(playlistDownloaded(QByteArray)));
	connect(downloader, SIGNAL(errorOcurred(int, QString)), this, SLOT(errorOcurred(int, QString)));

	history_urls = new URLHistory;
	history_urls->addUrl("http://smplayer.info/onlinetv.php");
#endif
}

Playlist::~Playlist() {
	saveSettings();
	if (set) delete set;

#ifdef PLAYLIST_DOWNLOAD
	delete history_urls;
#endif
}

void Playlist::setConfigPath(const QString & config_path) {
	qDebug() << "Playlist::setConfigPath:" << config_path;

	if (set) {
		delete set;
		set = 0;
	}

	if (!config_path.isEmpty()) {
		QString inifile = config_path + "/playlist.ini";
		qDebug() << "Playlist::setConfigPath: ini file:" << inifile;
		set = new QSettings(inifile, QSettings::IniFormat);
		loadSettings();
	}
}


void Playlist::setModified(bool mod) {
	qDebug("Playlist::setModified: %d", mod);

	modified = mod;
	emit modifiedChanged(modified);
}

void Playlist::createTable() {
	table = new QStandardItemModel(this);
	table->setColumnCount(COL_FILENAME + 1);
	//table->setSortRole(Qt::UserRole + 1);

	proxy = new QSortFilterProxyModel(this);
	proxy->setSourceModel(table);
	proxy->setSortRole(Qt::UserRole + 1);
	proxy->setFilterRole(Qt::UserRole + 1);
	proxy->setFilterKeyColumn(-1); // All columns

#if USE_ITEM_DELEGATE
	PlaylistDelegate * pl_delegate = new PlaylistDelegate(this);
#endif

	listView = new QTableView(this);
	listView->setModel(proxy);

#if USE_ITEM_DELEGATE
	listView->setItemDelegateForColumn(COL_NAME, pl_delegate);
	//listView->setItemDelegateForColumn(COL_FILENAME, pl_delegate);
#endif

	listView->setObjectName("playlist_table");
	listView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	listView->setContextMenuPolicy( Qt::CustomContextMenu );
	listView->setShowGrid(false);
	listView->setSortingEnabled(true);
#if !USE_ITEM_DELEGATE
	listView->setAlternatingRowColors(true);
#endif
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	listView->verticalHeader()->hide();

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(listView->viewport());

	listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
//	listView->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);
#else
	listView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
//	listView->horizontalHeader()->setResizeMode(COL_NAME, QHeaderView::Stretch);
//	listView->horizontalHeader()->setResizeMode(COL_FILENAME, QHeaderView::Interactive);
#endif
	listView->horizontalHeader()->setStretchLastSection(true);

	/*
	listView->horizontalHeader()->setResizeMode(COL_TIME, QHeaderView::ResizeToContents);
	listView->horizontalHeader()->setResizeMode(COL_PLAY, QHeaderView::ResizeToContents);
	*/
	//listView->setIconSize( Images::icon("ok").size() );

#if DRAG_ITEMS
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setDragEnabled(true);
	listView->setAcceptDrops(true);
	listView->setDropIndicatorShown(true);
	listView->setDragDropMode(QAbstractItemView::InternalMove);
#endif

	connect(listView, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(itemActivated(const QModelIndex &)) );
}

void Playlist::createActions() {
	openAct = new MyAction(this, "pl_open", false);
	connect( openAct, SIGNAL(triggered()), this, SLOT(load()) );

#ifdef PLAYLIST_DOWNLOAD
	openUrlAct = new MyAction(this, "pl_open_url", false);
	connect( openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()) );
#endif

	saveAct = new MyAction(this, "pl_save", false);
	connect( saveAct, SIGNAL(triggered()), this, SLOT(save()) );

	playAct = new MyAction(this, "pl_play", false);
	connect( playAct, SIGNAL(triggered()), this, SLOT(playCurrent()) );

	nextAct = new MyAction(Qt::Key_N /*Qt::Key_Greater*/, this, "pl_next", false);
	connect( nextAct, SIGNAL(triggered()), this, SLOT(playNext()) );

	prevAct = new MyAction(Qt::Key_P /*Qt::Key_Less*/, this, "pl_prev", false);
	connect( prevAct, SIGNAL(triggered()), this, SLOT(playPrev()) );

	moveUpAct = new MyAction(this, "pl_move_up", false);
	connect( moveUpAct, SIGNAL(triggered()), this, SLOT(upItem()) );

	moveDownAct = new MyAction(this, "pl_move_down", false);
	connect( moveDownAct, SIGNAL(triggered()), this, SLOT(downItem()) );

	repeatAct = new MyAction(this, "pl_repeat", false);
	repeatAct->setCheckable(true);

	shuffleAct = new MyAction(this, "pl_shuffle", false);
	shuffleAct->setCheckable(true);

	// Add actions
	addCurrentAct = new MyAction(this, "pl_add_current", false);
	connect( addCurrentAct, SIGNAL(triggered()), this, SLOT(addCurrentFile()) );

	addFilesAct = new MyAction(this, "pl_add_files", false);
	connect( addFilesAct, SIGNAL(triggered()), this, SLOT(addFiles()) );

	addDirectoryAct = new MyAction(this, "pl_add_directory", false);
	connect( addDirectoryAct, SIGNAL(triggered()), this, SLOT(addDirectory()) );

	addUrlsAct = new MyAction(this, "pl_add_urls", false);
	connect( addUrlsAct, SIGNAL(triggered()), this, SLOT(addUrls()) );

	// Remove actions
	removeSelectedAct = new MyAction(this, "pl_remove_selected", false);
	connect( removeSelectedAct, SIGNAL(triggered()), this, SLOT(removeSelected()) );

	removeAllAct = new MyAction(this, "pl_remove_all", false);
	connect( removeAllAct, SIGNAL(triggered()), this, SLOT(removeAll()) );

	// Edit
	editAct = new MyAction(this, "pl_edit", false);
	connect( editAct, SIGNAL(triggered()), this, SLOT(editCurrentItem()) );

	deleteSelectedFileFromDiskAct = new MyAction(this, "pl_delete_from_disk");
	connect( deleteSelectedFileFromDiskAct, SIGNAL(triggered()), this, SLOT(deleteSelectedFileFromDisk()));
}

void Playlist::createToolbar() {
	toolbar = new QToolBar(this);
	toolbar->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	//toolbar->setIconSize(QSize(48,48));

	toolbar->addAction(openAct);
#ifdef PLAYLIST_DOWNLOAD
	toolbar->addAction(openUrlAct);
#endif
	toolbar->addAction(saveAct);;
	toolbar->addSeparator();

	add_menu = new QMenu( this );
	add_menu->addAction(addCurrentAct);
	add_menu->addAction(addFilesAct );
	add_menu->addAction(addDirectoryAct);
	add_menu->addAction(addUrlsAct);

	add_button = new QToolButton( this );
	add_button->setMenu( add_menu );
	add_button->setPopupMode(QToolButton::InstantPopup);

	remove_menu = new QMenu( this );
	remove_menu->addAction(removeSelectedAct);
	remove_menu->addAction(removeAllAct);

	remove_button = new QToolButton( this );
	remove_button->setMenu( remove_menu );
	remove_button->setPopupMode(QToolButton::InstantPopup);

	filter_edit = new MyLineEdit(this);
	connect(filter_edit, SIGNAL(textChanged(const QString &)), this, SLOT(filterEditChanged(const QString &)));

#ifdef PLAYLIST_DOWNLOAD
	QLabel * loading_label = new QLabel(this);
	animation = new QMovie();
	animation->setFileName(Images::file("pl_loading.gif"));
	loading_label->setMovie(animation);
#endif

	toolbar->addWidget(add_button);
	toolbar->addWidget(remove_button);

	toolbar->addSeparator();
	toolbar->addAction(playAct);
	toolbar->addSeparator();
	toolbar->addAction(prevAct);
	toolbar->addAction(nextAct);
	toolbar->addSeparator();
	toolbar->addAction(repeatAct);
	toolbar->addAction(shuffleAct);
	toolbar->addSeparator();
	toolbar->addAction(moveUpAct);
	toolbar->addAction(moveDownAct);
	toolbar->addSeparator();
	toolbar->addWidget(filter_edit);

#ifdef PLAYLIST_DOWNLOAD
	loading_label_action = toolbar->addWidget(loading_label);
	loading_label_action->setVisible(false);
#endif

	// Popup menu
	popup = new QMenu(this);
	popup->addAction(playAct);
	popup->addAction(removeSelectedAct);
	popup->addAction(editAct);
	popup->addAction(deleteSelectedFileFromDiskAct);

	connect( listView, SIGNAL(customContextMenuRequested(const QPoint &)),
             this, SLOT(showPopup(const QPoint &)) );
}

void Playlist::retranslateStrings() {
	table->setHorizontalHeaderLabels(QStringList() << " " << tr("Name") << tr("Length") << tr("Filename / URL") );

	openAct->change( Images::icon("open"), tr("&Load") );
#ifdef PLAYLIST_DOWNLOAD
	openUrlAct->change( Images::icon("url"), tr("&Open URL") );
	openUrlAct->setToolTip(tr("Download playlist from URL"));
#endif
	saveAct->change( Images::icon("save"), tr("&Save") );

	playAct->change( tr("&Play") );

	nextAct->change( tr("&Next") );
	prevAct->change( tr("Pre&vious") );

	playAct->setIcon( Images::icon("play") );
	nextAct->setIcon( Images::icon("next") );
	prevAct->setIcon( Images::icon("previous") );

	moveUpAct->change( Images::icon("up"), tr("Move &up") );
	moveDownAct->change( Images::icon("down"), tr("Move &down") );

	repeatAct->change( Images::icon("repeat"), tr("&Repeat") );
	shuffleAct->change( Images::icon("shuffle"), tr("S&huffle") );

	// Add actions
	addCurrentAct->change( tr("Add &current file") );
	addFilesAct->change( tr("Add &file(s)") );
	addDirectoryAct->change( tr("Add &directory") );
	addUrlsAct->change( tr("Add &URL(s)") );

	// Remove actions
	removeSelectedAct->change( tr("Remove &selected") );
	removeAllAct->change( tr("Remove &all") );

	deleteSelectedFileFromDiskAct->change( tr("&Delete file from disk") );

	// Edit
	editAct->change( tr("&Edit") );

	// Tool buttons
	add_button->setIcon( Images::icon("plus") );
	add_button->setToolTip( tr("Add...") );
	remove_button->setIcon( Images::icon("minus") );
	remove_button->setToolTip( tr("Remove...") );

	// Filter edit
#if QT_VERSION >= 0x040700
	filter_edit->setPlaceholderText(tr("Search"));
#endif

	// Icon
	setWindowIcon( Images::icon("logo", 64) );
	setWindowTitle( tr( "SMPlayer - Playlist" ) );
}

void Playlist::list() {
	qDebug("Playlist::list");

	for (int n = 0; n < count(); n++) {
		PLItem * i = itemData(n);
		qDebug() << "Playlist::list: filename:" << i->filename() << "name:" << i->name() << "duration:" << i->duration();
	}
}

void Playlist::setFilter(const QString & filter) {
	proxy->setFilterWildcard(filter);
}

void Playlist::filterEditChanged(const QString & text) {
	qDebug() << "Playlist::filterEditChanged:" << text;
	setFilter(text);
}

void Playlist::setCurrentItem(int current) {
	QModelIndex index = proxy->index(current, 0);
	QModelIndex s_index = proxy->mapToSource(index);

	//qDebug() << "Playlist::setCurrentItem: index:" << index.row() << "s_index:" << s_index.row();

	int s_current = s_index.row();

	PLItem * item = 0;
	for (int n = 0; n < count(); n++) {
		item = itemData(n);
		if (n == s_current) {
			item->setPlayed(true);
		}
		item->setCurrent( (n == s_current) );
	}

	listView->clearSelection();
	listView->selectionModel()->setCurrentIndex(listView->model()->index(current, 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

int Playlist::findCurrentItem() {
	//qDebug("Playlist::findCurrentItem");

	static int last_current = -1;

	// Check if the last found current is still the current item to save time
	PLItem * i = itemFromProxy(last_current);
	if (i && i->isCurrent()) {
		//qDebug() << "Playlist::findCurrentItem: return last_current:" << last_current;
		return last_current;
	}

	for (int n = 0; n < proxy->rowCount(); n++) {
		if (itemFromProxy(n)->isCurrent()) {
			last_current = n;
			return n;
		}
	}

	return -1;
}

void Playlist::clear() {
	table->setRowCount(0);
	setCurrentItem(0);
	setModified(false);
}

int Playlist::count() {
	return table->rowCount();
}

bool Playlist::isEmpty() {
	return (table->rowCount() > 0);
}

bool Playlist::existsItem(int row) {
	return (row > -1 && row < table->rowCount());
}

PLItem * Playlist::itemData(int row) {
	QStandardItem * i = table->item(row, COL_NAME);
	return static_cast<PLItem*>(i);
}

PLItem * Playlist::itemFromProxy(int row) {
	QModelIndex index = proxy->index(row, 0);
	QModelIndex s_index = proxy->mapToSource(index);
	//qDebug() << "Playlist::itemFromProxy: index is valid:" << index.isValid() << "s_index is valid:" << s_index.isValid();
	if (index.isValid() && s_index.isValid()) {
		return itemData(s_index.row());
	} else {
		return 0;
	}
}

/*
void Playlist::changeItem(int row, const QString & filename, const QString name, double duration, bool played, int pos) {
	PLItem * i = itemData(row);

	int position = row + 1;
	if (pos != -1) position = pos;
	i->setPosition(position);

	i->setFilename(filename);
	i->setName(name);
	i->setDuration(duration);
	i->setPlayed(played);
}
*/

void Playlist::addItem(QString filename, QString name, double duration) {
	//qDebug() << "Playlist::addItem:" << filename;

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	filename = Helper::changeSlashes(filename);
	#endif

		if (name.isEmpty()) {
			QFileInfo fi(filename);
			// Let's see if it looks like a file (no dvd://1 or something)
			if (filename.indexOf(QRegExp("^.*://.*")) == -1) {
				// Local file
				name = fi.fileName();
			} else {
				// Stream
				name = filename;
			}
		}

	PLItem * i = new PLItem(filename, name, duration);
	i->setPosition(count()+1);
	table->appendRow(i->items());

	if (findCurrentItem() == -1) setCurrentItem(0);

/*
#if !PL_ALLOW_DUPLICATES
	// Test if already is in the list
	bool exists = false;
	for ( int n = 0; n < pl.count(); n++) {
		if ( pl[n].filename() == filename ) {
			exists = true;
			int last_item =  pl.count()-1;
			pl.move(n, last_item);
			qDebug("Playlist::addItem: item already in list (%d), moved to %d", n, last_item);
			if (current_item > -1) {
				if (current_item > n) current_item--;
				else
				if (current_item == n) current_item = last_item;
			}
			break;
		}
	}

	if (!exists) {
#endif
		if (name.isEmpty()) {
			QFileInfo fi(filename);
			// Let's see if it looks like a file (no dvd://1 or something)
			if (filename.indexOf(QRegExp("^.*://.*")) == -1) {
				// Local file
				name = fi.fileName(); //fi.baseName(true);
			} else {
				// Stream
				name = filename;
			}
		}
		pl.append( PlaylistItem(filename, name, duration) );
		//setModified( true ); // Better set the modified on a higher level
#if !PL_ALLOW_DUPLICATES
	} else {
		qDebug("Playlist::addItem: item not added, already in the list");
	}
#endif
*/
}


void Playlist::load_m3u(QString file, M3UFormat format) {
	bool utf8 = false;
	if (format == DetectFormat) {
		utf8 = (QFileInfo(file).suffix().toLower() == "m3u8");
	} else {
		utf8 = (format == M3U8);
	}

	qDebug() << "Playlist::load_m3u: utf8:" << utf8;

	QRegExp m3u_id("^#EXTM3U|^#M3U");
	QRegExp info("^#EXTINF:(.*),(.*)");

	QFile f( file );
	if ( f.open( QIODevice::ReadOnly ) ) {
		playlist_path = QFileInfo(file).path();

		clear();
		QString filename="";
		QString name="";
		double duration=0;

		QTextStream stream( &f );

		if (utf8)
			stream.setCodec("UTF-8");
		else
			stream.setCodec(QTextCodec::codecForLocale());

		QString line;
		while ( !stream.atEnd() ) {
			line = stream.readLine().trimmed();
			if (line.isEmpty()) continue; // Ignore empty lines

			qDebug( "Playlist::load_m3u: line: '%s'", line.toUtf8().data() );
			if (m3u_id.indexIn(line)!=-1) {
				//#EXTM3U
				// Ignore line
			}
			else
			/*
			if (info.indexIn(line)!=-1) {
				duration = info.cap(1).toDouble();
				name = info.cap(2);
				qDebug("Playlist::load_m3u: name: '%s', duration: %f", name.toUtf8().data(), duration );
			} 
			*/
			if (line.startsWith("#EXTINF:")) {
				QStringList fields = line.mid(8).split(",");
				//qDebug() << "Playlist::load_m3u: fields:" << fields;
				if (fields.count() >= 1) duration = fields[0].toDouble();
				if (fields.count() >= 2) name = fields[1];
			}
			else
			if (line.startsWith("#")) {
				// Comment
				// Ignore
			} else {
				filename = line;
				QFileInfo fi(filename);
				if (fi.exists()) {
					filename = fi.absoluteFilePath();
				}
				if (!fi.exists()) {
					if (QFileInfo( playlist_path + "/" + filename).exists() ) {
						filename = playlist_path + "/" + filename;
					}
				}
				addItem( filename, name, duration );
				name=""; 
				duration = 0;
			}
		}
		f.close();
		//list();

		setModified( false );

		startPlay();
	}
}

void Playlist::load_pls(QString file) {
	qDebug("Playlist::load_pls");

	if (!QFile::exists(file)) {
		qDebug("Playlist::load_pls: '%s' doesn't exist, doing nothing", file.toUtf8().constData());
		return;
	}

	playlist_path = QFileInfo(file).path();

	QSettings set(file, QSettings::IniFormat);
	set.beginGroup("playlist");

	if (set.status() == QSettings::NoError) {
		clear();
		QString filename;
		QString name;
		double duration;

		int num_items = set.value("NumberOfEntries", 0).toInt();

		for (int n=0; n < num_items; n++) {
			filename = set.value("File"+QString::number(n+1), "").toString();
			name = set.value("Title"+QString::number(n+1), "").toString();
			duration = (double) set.value("Length"+QString::number(n+1), 0).toInt();

			QFileInfo fi(filename);
			if (fi.exists()) {
				filename = fi.absoluteFilePath();
			}
			if (!fi.exists()) {
				if (QFileInfo( playlist_path + "/" + filename).exists() ) {
					filename = playlist_path + "/" + filename;
				}
			}
			addItem( filename, name, duration );
		}
	}

	set.endGroup();

	//list();

	setModified( false );

	if (set.status() == QSettings::NoError) startPlay();
}

void Playlist::loadXSPF(const QString & filename) {
	qDebug() << "Playlist::loadXSPF:" << filename;

	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly)) {
		return;
	}

	QDomDocument dom_document;
	bool ok = dom_document.setContent(f.readAll());
	qDebug() << "Playlist::loadXSPF: success:" << ok;
	if (!ok) return;

	QDomNode root = dom_document.documentElement();
	qDebug() << "Playlist::loadXSPF: tagname:" << root.toElement().tagName();

	QDomNode child = root.firstChildElement("trackList");
	if (!child.isNull()) {
		clear();

		qDebug() << "Playlist::loadXSPF: child:" << child.nodeName();
		QDomNode track = child.firstChildElement("track");
		while (!track.isNull()) {
			QString location = QUrl::fromPercentEncoding(track.firstChildElement("location").text().toLatin1());
			QString title = track.firstChildElement("title").text();
			int duration = track.firstChildElement("duration").text().toInt();

			qDebug() << "Playlist::loadXSPF: location:" << location;
			qDebug() << "Playlist::loadXSPF: title:" << title;
			qDebug() << "Playlist::loadXSPF: duration:" << duration;

			addItem( location, title, (double) duration / 1000 );

			track = track.nextSiblingElement("track");
		}

		//list();
		setModified( false );
		startPlay();
	}
}

bool Playlist::save_m3u(QString file) {
	qDebug() << "Playlist::save_m3u:" << file;

	QString dir_path = QFileInfo(file).path();
	if (!dir_path.endsWith("/")) dir_path += "/";

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	dir_path = Helper::changeSlashes(dir_path);
	#endif

	qDebug() << "Playlist::save_m3u: dir_path:" << dir_path;

	bool utf8 = (QFileInfo(file).suffix().toLower() == "m3u8");

	QFile f( file );
	if ( f.open( QIODevice::WriteOnly ) ) {
		QTextStream stream( &f );

		if (utf8)
			stream.setCodec("UTF-8");
		else
			stream.setCodec(QTextCodec::codecForLocale());

		QString filename;

		stream << "#EXTM3U" << "\n";
		stream << "# Playlist created by SMPlayer " << Version::printable() << " \n";

		for (int n = 0; n < count(); n++) {
			PLItem * i = itemData(n);
			filename = i->filename();
			#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
			filename = Helper::changeSlashes(filename);
			#endif
			stream << "#EXTINF:";
			stream << i->duration() << ",";
			stream << i->name() << "\n";
			// Try to save the filename as relative instead of absolute
			if (filename.startsWith( dir_path )) {
				filename = filename.mid( dir_path.length() );
			}
			stream << filename << "\n";
		}
		f.close();

		setModified( false );
		return true;
	} else {
		return false;
	}
}


bool Playlist::save_pls(QString file) {
	qDebug() << "Playlist::save_pls:" << file;

	QString dir_path = QFileInfo(file).path();
	if (!dir_path.endsWith("/")) dir_path += "/";

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	dir_path = Helper::changeSlashes(dir_path);
	#endif

	qDebug() << "Playlist::save_pls: dir_path:" << dir_path;

	QSettings set(file, QSettings::IniFormat);
	set.beginGroup( "playlist");

	QString filename;

	for (int n = 0; n < count(); n++) {
		PLItem * i = itemData(n);
		filename = i->filename();
		#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
		filename = Helper::changeSlashes(filename);
		#endif

		// Try to save the filename as relative instead of absolute
		if (filename.startsWith( dir_path )) {
			filename = filename.mid( dir_path.length() );
		}

		set.setValue("File"+QString::number(n+1), filename);
		set.setValue("Title"+QString::number(n+1), i->name());
		set.setValue("Length"+QString::number(n+1), (int) i->duration());
	}

	set.setValue("NumberOfEntries", count());
	set.setValue("Version", 2);

	set.endGroup();

	set.sync();

	bool ok = (set.status() == QSettings::NoError);
	if (ok) setModified( false );

	return ok;
}

bool Playlist::saveXSPF(const QString & filename) {
	qDebug() << "Playlist::saveXSPF:" << filename;

	QFile f(filename);
	if (f.open( QIODevice::WriteOnly)) {
		QTextStream stream(&f);
		stream.setCodec("UTF-8");

		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		stream << "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n";
		stream << "\t<trackList>\n";

		for (int n = 0; n < count(); n++) {
			PLItem * i = itemData(n);
			QString location = i->filename();
			qDebug() << "Playlist::saveXSPF:" << location;

			bool is_local = QFile::exists(location);
			
			#ifdef Q_OS_WIN
			if (is_local) {
				location.replace("\\", "/");
			}
			#endif
			//qDebug() << "Playlist::saveXSPF:" << location;
			
			QUrl url(location);
			location = url.toEncoded();
			//qDebug() << "Playlist::saveXSPF:" << location;
			
			if (!location.startsWith("file:") && is_local) {
				#ifdef Q_OS_WIN
				location = "file:///" + location;
				#else
				location = "file://" + location;
				#endif
			}

			QString title = i->name();
			int duration = i->duration() * 1000;

			#if QT_VERSION >= 0x050000
			location = location.toHtmlEscaped();
			title = title.toHtmlEscaped();
			#else
			location = Qt::escape(location);
			title = Qt::escape(title);
			#endif

			stream << "\t\t<track>\n";
			stream << "\t\t\t<location>" << location << "</location>\n";
			stream << "\t\t\t<title>" << title << "</title>\n";
			stream << "\t\t\t<duration>" << duration << "</duration>\n";
			stream << "\t\t</track>\n";
		}

		stream << "\t</trackList>\n";
		stream << "</playlist>\n";

		setModified(false);
		return true;
	} else {
		return false;
	}
}


void Playlist::load() {
	if (maybeSave()) {
		Extensions e;
		QString s = MyFileDialog::getOpenFileName(
                    this, tr("Choose a file"), 
                    lastDir(),
                    tr("Playlists") + e.playlist().forFilter() + ";;" + tr("All files") +" (*)");

		if (!s.isEmpty()) {
			latest_dir = QFileInfo(s).absolutePath();

			QString suffix = QFileInfo(s).suffix().toLower();
			if (suffix == "pls") {
				load_pls(s);
			}
			else
			if (suffix == "xspf") {
				loadXSPF(s);
			}
			else {
				load_m3u(s);
			}
			//listView->resizeColumnsToContents();
		}
	}
}

bool Playlist::save() {
	Extensions e;
	QString s = MyFileDialog::getSaveFileName(
                    this, tr("Choose a filename"), 
                    lastDir(),
                    tr("Playlists") + e.playlist().forFilter() + ";;" + tr("All files") +" (*)");

	if (!s.isEmpty()) {
		// If filename has no extension, add it
		if (QFileInfo(s).suffix().isEmpty()) {
			s = s + ".m3u";
		}
		if (QFileInfo(s).exists()) {
			int res = QMessageBox::question( this,
					tr("Confirm overwrite?"),
                    tr("The file %1 already exists.\n"
                       "Do you want to overwrite?").arg(s),
                    QMessageBox::Yes,
                    QMessageBox::No,
                    QMessageBox::NoButton);
			if (res == QMessageBox::No ) {
            	return false;
			}
		}
		latest_dir = QFileInfo(s).absolutePath();

		QString suffix = QFileInfo(s).suffix().toLower();
		if (suffix  == "pls") {
			return save_pls(s);
		}
		else
		if (suffix  == "xspf") {
			return saveXSPF(s);
		}
		else {
			return save_m3u(s);
		}

	} else {
		return false;
	}
}

bool Playlist::maybeSave() {
	if (!isModified()) return true;

	int res = QMessageBox::question( this,
				tr("Playlist modified"),
                tr("There are unsaved changes, do you want to save the playlist?"),
                QMessageBox::Yes,
                QMessageBox::No,
                QMessageBox::Cancel);

	switch (res) {
		case QMessageBox::No : return true; // Discard changes
		case QMessageBox::Cancel : return false; // Cancel operation
		default : return save();
	}
}

void Playlist::playCurrent() {
	int current = listView->currentIndex().row();
	if (current > -1) {
		playItem(current);
	}
}

void Playlist::itemActivated(const QModelIndex & index ) {
	qDebug() << "Playlist::itemActivated: row:" << index.row();
	playItem(index.row());
}

void Playlist::showPopup(const QPoint & pos) {
	qDebug("Playlist::showPopup: x: %d y: %d", pos.x(), pos.y() );

	if (!popup->isVisible()) {
		popup->move( listView->viewport()->mapToGlobal(pos) );
		popup->show();
	}
}

void Playlist::startPlay() {
	// Start to play
	if ( shuffleAct->isChecked() ) 
		playItem( chooseRandomItem() );
	else
		playItem(0);
}

void Playlist::playItem( int n ) {
	qDebug("Playlist::playItem: %d (count: %d)", n, proxy->rowCount());

	if ( (n >= proxy->rowCount()) || (n < 0) ) {
		qDebug("Playlist::playItem: out of range");
		emit playlistEnded();
		return;
	}

	QString filename = itemFromProxy(n)->filename();
	if (!filename.isEmpty()) {
		setCurrentItem(n);
		if (play_files_from_start) {
			emit requestToPlayFile(filename, 0);
		} else {
			emit requestToPlayFile(filename);
		}
	}
}

void Playlist::playNext() {
	qDebug("Playlist::playNext");

	if (shuffleAct->isChecked()) {
		// Shuffle
		int chosen_item = chooseRandomItem();
		qDebug("Playlist::playNext: chosen_item: %d", chosen_item);
		if (chosen_item == -1) {
			clearPlayedTag();
			if (repeatAct->isChecked()) {
				chosen_item = chooseRandomItem();
				if (chosen_item == -1) chosen_item = 0;
			}
		}
		playItem(chosen_item);
	} else {
		int current = findCurrentItem();
		bool finished_list = (current + 1 >= proxy->rowCount());
		if (finished_list) clearPlayedTag();

		if (repeatAct->isChecked() && finished_list) {
			playItem(0);
		} else {
			playItem(current + 1);
		}
	}
}

void Playlist::playPrev() {
	qDebug("Playlist::playPrev");
	int current = findCurrentItem() - 1;
	if (current >= 0) {
		playItem(current);
	} else {
		if (proxy->rowCount() > 1) playItem(proxy->rowCount() - 1);
	}
}


void Playlist::resumePlay() {
	qDebug("Playlist::resumePlay");

	if (count() > 0) {
		int current = findCurrentItem();
		if (current < 0) current = 0;
		playItem(current);
	}
}

void Playlist::getMediaInfo(const MediaData & mdat) {
	qDebug("Playlist::getMediaInfo");

	QString filename = mdat.filename;
	double duration = mdat.duration;
	QString artist = mdat.clip_artist;

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	filename = Helper::changeSlashes(filename);
	#endif

	QString name;
	if (change_name) {
		name = mdat.clip_name;
		if (name.isEmpty()) name = mdat.stream_title;

		if (name.isEmpty()) {
			QFileInfo fi(filename);
			if (fi.exists()) {
				// Local file
				name = fi.fileName();
			} else {
				// Stream
				name = filename;
			}
		}
		if (!artist.isEmpty()) name = artist + " - " + name;
	}

	for (int n = 0; n < count(); n++) {
		PLItem * i = itemData(n);
		if (i->filename() == filename) {
			// Found item
			bool modified_name = !(i->filename().endsWith(i->name()));
			if (i->duration() < 1) {
				if (!modified_name && !name.isEmpty()) {
					i->setName(name);
				}
				i->setDuration(duration);
			}
			else
			// Edited name (sets duration to 1)
			if (i->duration() == 1) {
				i->setDuration(duration);
			}
		}
	}
}

// Add current file to playlist
void Playlist::addCurrentFile() {
	qDebug("Playlist::addCurrentFile");
	emit requestToAddCurrentFile();
}

void Playlist::addFiles() {
	Extensions e;
	QStringList files = MyFileDialog::getOpenFileNames(
                            this, tr("Select one or more files to open"), 
                            lastDir(),
                            tr("Multimedia") + e.multimedia().forFilter() + ";;" +
                            tr("All files") +" (*.*)" );

	if (files.count()!=0) addFiles(files);  
}

void Playlist::addFiles(QStringList files, AutoGetInfo auto_get_info) {
	qDebug("Playlist::addFiles");

	#if USE_INFOPROVIDER
	bool get_info = (auto_get_info == GetInfo);
	if (auto_get_info == UserDefined) {
		get_info = automatically_get_info;
	}

	MediaData data;
	setCursor(Qt::WaitCursor);
	#endif

	QString initial_file;
	if (count() == 1) initial_file = itemData(0)->filename();
	int new_current_item = -1;

	for (int n = 0; n < files.count(); n++) {
		QString name = "";
		double duration = 0;
		#if USE_INFOPROVIDER
		if ( (get_info) && (QFile::exists(files[n])) ) {
			data = InfoProvider::getInfo(files[n]);
			name = data.displayName();
			duration = data.duration;
			//qApp->processEvents();
		}
		#endif

		//qDebug() << "Playlist::addFiles: comparing:" << initial_file << "with" << files[n];

		if (!initial_file.isEmpty() && files[n] == initial_file) {
			PLItem * first_item = itemData(0);
			name = first_item->name();
			duration = first_item->duration();
			table->removeRow(0);
			new_current_item = n;
		}
		addItem(files[n], name, duration);

		if (QFile::exists(files[n])) {
			latest_dir = QFileInfo(files[n]).absolutePath();
		}
	}
	#if USE_INFOPROVIDER
	unsetCursor();
	#endif

	if (new_current_item != -1) setCurrentItem(new_current_item);

	qDebug() << "Playlist::addFiles: latest_dir:" << latest_dir;
}

void Playlist::addFile(QString file, AutoGetInfo auto_get_info) {
	addFiles( QStringList() << file, auto_get_info );
}

void Playlist::addDirectory() {
	QString s = MyFileDialog::getExistingDirectory(
                    this, tr("Choose a directory"),
                    lastDir() );

	if (!s.isEmpty()) {
		addDirectory(s);
		latest_dir = s;
	}
}

void Playlist::addUrls() {
	MultilineInputDialog d(this);
	if (d.exec() == QDialog::Accepted) {
		QStringList urls = d.lines();
		foreach(QString u, urls) {
			if (!u.isEmpty()) addItem( u, "", 0 );
		}
	}
}

void Playlist::addOneDirectory(QString dir) {
	QStringList filelist;

	Extensions e;
	QRegExp rx_ext(e.multimedia().forRegExp());
	rx_ext.setCaseSensitivity(Qt::CaseInsensitive);

	QStringList dir_list = QDir(dir).entryList();

	QString filename;
	QStringList::Iterator it = dir_list.begin();
	while( it != dir_list.end() ) {
		filename = dir;
		if (filename.right(1)!="/") filename += "/";
		filename += (*it);
		QFileInfo fi(filename);
		if (!fi.isDir()) {
			if (rx_ext.indexIn(fi.suffix()) > -1) {
				filelist << filename;
			}
		}
		++it;
	}
	addFiles(filelist);
}

void Playlist::addDirectory(QString dir) {
	addOneDirectory(dir);

	if (recursive_add_directory) {
		QFileInfoList dir_list = QDir(dir).entryInfoList(QStringList() << "*", QDir::AllDirs | QDir::NoDotAndDotDot);
		for (int n=0; n < dir_list.count(); n++) {
			if (dir_list[n].isDir()) {
				qDebug("Playlist::addDirectory: adding directory: %s", dir_list[n].filePath().toUtf8().data());
				addDirectory(dir_list[n].filePath());
			}
		}
	}
}

// Remove selected items
void Playlist::removeSelected() {
	qDebug("Playlist::removeSelected");

	QModelIndexList indexes = listView->selectionModel()->selectedRows();
	int count = indexes.count();

	for (int n = count; n > 0; n--) {
		QModelIndex s_index = proxy->mapToSource(indexes.at(n-1));
		table->removeRow(s_index.row());
		setModified(true);
	}

	if (isEmpty()) setModified(false);

	if (findCurrentItem() == -1) {
		int current = indexes.at(0).row() - 1;
		if (current < 0) current = 0;
		setCurrentItem(current);
	}
}

void Playlist::removeAll() {
	clear();
}

void Playlist::clearPlayedTag() {
	for (int n = 0; n < count(); n++) {
		itemData(n)->setPlayed(false);
	}
}

int Playlist::chooseRandomItem() {
	qDebug( "Playlist::chooseRandomItem");

	QList<int> fi; //List of not played items (free items)
	for (int n = 0; n < proxy->rowCount(); n++) {
		if (!itemFromProxy(n)->played()) fi.append(n);
	}

	qDebug("Playlist::chooseRandomItem: free items: %d", fi.count() );

	if (fi.count() == 0) return -1; // none free

	qDebug("Playlist::chooseRandomItem: items: ");
	for (int i = 0; i < fi.count(); i++) {
		qDebug("Playlist::chooseRandomItem: * item: %d", fi[i]);
	}

	int selected = (qrand() % fi.count());
	qDebug("Playlist::chooseRandomItem: selected item: %d (%d)", selected, fi[selected]);
	return fi[selected];
}

void Playlist::upItem() {
	QModelIndex index = listView->currentIndex();
	QModelIndex s_index = proxy->mapToSource(index);

	QModelIndex prev = listView->model()->index(index.row()-1, 0);
	QModelIndex s_prev = proxy->mapToSource(prev);

	qDebug() << "Playlist::upItem: row:" << index.row() << "source row:" << s_index.row();
	qDebug() << "Playlist::upItem: previous row:" << prev.row() << "previous source row:" << s_prev.row();

	if (s_index.isValid() && s_prev.isValid()) {
		int row = s_index.row();
		QList<QStandardItem*> cells = table->takeRow(row);
		table->insertRow(s_prev.row(), cells);
		listView->selectionModel()->setCurrentIndex(listView->model()->index(index.row()-1, 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
	}
}

void Playlist::downItem() {
	qDebug("Playlist::downItem");

	QModelIndex index = listView->currentIndex();
	QModelIndex s_index = proxy->mapToSource(index);

	QModelIndex next = listView->model()->index(index.row()+1, 0);
	QModelIndex s_next = proxy->mapToSource(next);

	qDebug() << "Playlist::downItem: row:" << index.row() << "source row:" << s_index.row();
	qDebug() << "Playlist::downItem: next row:" << next.row() << "next source row:" << s_next.row();

	if (s_index.isValid() && s_next.isValid()) {
		int row = s_index.row();
		QList<QStandardItem*> cells = table->takeRow(row);
		table->insertRow(s_next.row(), cells);
		listView->selectionModel()->setCurrentIndex(listView->model()->index(index.row()+1, 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
	}
}

void Playlist::editCurrentItem() {
	QModelIndex v_index = listView->currentIndex();
	QModelIndex s_index = proxy->mapToSource(v_index);
	qDebug() << "Playlist::editCurrentItem: row:" << v_index.row() << "source row:" << s_index.row();
	int current = s_index.row();
	if (current > -1) editItem(current);
}

void Playlist::editItem(int row) {
	qDebug() << "Playlist::editItem:" << row;

	PLItem * i = itemData(row);
	QString current_name = i->name();
	if (current_name.isEmpty()) current_name = i->filename();

	bool ok;
	QString text = QInputDialog::getText( this,
            tr("Edit name"),
            tr("Type the name that will be displayed in the playlist for this file:"),
            QLineEdit::Normal,
            current_name, &ok );
    if ( ok && !text.isEmpty() ) {
		// user entered something and pressed OK
		i->setName(text);

		// If duration == 0 the name will be overwritten!
		if (i->duration() < 1) i->setDuration(1);

		setModified( true );
	}
}

void Playlist::deleteSelectedFileFromDisk() {
	qDebug("Playlist::deleteSelectedFileFromDisk");

	QModelIndex index = listView->currentIndex();
	if (!index.isValid()) return;

	QModelIndex s_index = proxy->mapToSource(index);

	qDebug() << "Playlist::deleteSelectedFileFromDisk: row:" << index.row() << "source row:" << s_index.row();
	int current = s_index.row();

	// Select only the current row
	listView->selectionModel()->setCurrentIndex(listView->model()->index(index.row(), 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

	QString filename = itemData(current)->filename();
	qDebug() << "Playlist::deleteSelectedFileFromDisk: current file:" << filename;

	QFileInfo fi(filename);
	if (fi.exists() && fi.isFile() && fi.isWritable()) {
		// Ask the user for confirmation
		int res = QMessageBox::question(this, tr("Confirm deletion"),
					tr("You're about to DELETE the file '%1' from your drive.").arg(filename) + "<br>"+
					tr("This action cannot be undone. Are you sure you want to proceed?"),
					QMessageBox::Yes, QMessageBox::No);

		if (res == QMessageBox::Yes) {
			// Delete file
			#if SIMULATE_FILE_DELETION
			bool success = true;
			#else
			bool success = QFile::remove(filename);
			#endif

			if (success) {
				// Remove item from the playlist
				table->removeRow(current);
				if (findCurrentItem() == -1) {
					if (current > 0) setCurrentItem(current-1); else setCurrentItem(0);
				}
			} else {
				QMessageBox::warning(this, tr("Deletion failed"),
					tr("It wasn't possible to delete '%1'").arg(filename));
			}
		}
	} else {
		qDebug("Playlist::deleteSelectedFileFromDisk: file doesn't exists, it's not a file or it's not writable");
		QMessageBox::information(this, tr("Error deleting the file"),
			tr("It's not possible to delete '%1' from the filesystem.").arg(filename));
	}
}

// Drag&drop
void Playlist::dragEnterEvent( QDragEnterEvent *e ) {
	qDebug("Playlist::dragEnterEvent");

	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}

void Playlist::dropEvent( QDropEvent *e ) {
	qDebug("Playlist::dropEvent");

	QStringList files;

	if (e->mimeData()->hasUrls()) {
		QList <QUrl> l = e->mimeData()->urls();
		QString s;
		for (int n=0; n < l.count(); n++) {
			if (l[n].isValid()) {
				qDebug("Playlist::dropEvent: scheme: '%s'", l[n].scheme().toUtf8().data());
				if (l[n].scheme() == "file") 
					s = l[n].toLocalFile();
				else
					s = l[n].toString();
				/*
				qDebug(" * '%s'", l[n].toString().toUtf8().data());
				qDebug(" * '%s'", l[n].toLocalFile().toUtf8().data());
				*/
				qDebug("Playlist::dropEvent: file: '%s'", s.toUtf8().data());
				files.append(s);
			}
		}
	}

	#ifdef Q_OS_WIN
	files = Helper::resolveSymlinks(files); // Check for Windows shortcuts
	#endif
	files.sort();

	QStringList only_files;
	for (int n = 0; n < files.count(); n++) {
		if ( QFileInfo( files[n] ).isDir() ) {
			addDirectory( files[n] );
		} else {
			only_files.append( files[n] );
		}
	}
	addFiles( only_files );
}


void Playlist::hideEvent( QHideEvent * ) {
	emit visibilityChanged(false);
}

void Playlist::showEvent( QShowEvent * ) {
	emit visibilityChanged(true);
}

void Playlist::closeEvent( QCloseEvent * e )  {
	saveSettings();
	e->accept();
}

void Playlist::playerFailed(QProcess::ProcessError e) {
	qDebug("Playlist::playerFailed");
	if (ignore_player_errors) {
		if (e != QProcess::FailedToStart) {
			playNext();
		}
	}
}

void Playlist::playerFinishedWithError(int e) {
	qDebug("Playlist::playerFinishedWithError: %d", e);
	if (ignore_player_errors) {
		playNext();
	}
}

void Playlist::maybeSaveSettings() {
	qDebug("Playlist::maybeSaveSettings");
	if (isModified()) saveSettings();
}

void Playlist::saveSettings() {
	qDebug("Playlist::saveSettings");

	if (!set) return;

	set->beginGroup( "playlist");

	set->setValue( "repeat", repeatAct->isChecked() );
	set->setValue( "shuffle", shuffleAct->isChecked() );

	set->setValue( "auto_get_info", automatically_get_info );
	set->setValue( "recursive_add_directory", recursive_add_directory );
	set->setValue( "save_playlist_in_config", save_playlist_in_config );
	set->setValue( "play_files_from_start", play_files_from_start );
	set->setValue( "automatically_play_next", automatically_play_next );
	set->setValue( "ignore_player_errors", ignore_player_errors );
	set->setValue( "change_name", change_name );

	set->setValue( "row_spacing", row_spacing );

#if !DOCK_PLAYLIST
	set->setValue( "size", size() );
#endif
	set->setValue(QString("header_state/%1").arg(Helper::qtVersion()), listView->horizontalHeader()->saveState());

	set->setValue( "sort_column", proxy->sortColumn() );
	set->setValue( "sort_order", proxy->sortOrder() );
	set->setValue( "filter_case_sensivity", proxy->filterCaseSensitivity() );
	set->setValue( "filter", filter_edit->text() );
	set->setValue( "sort_case_sensivity", proxy->sortCaseSensitivity() );

	set->endGroup();

	set->beginGroup( "directories");
	set->setValue("save_dirs", save_dirs);
	set->setValue("latest_dir", save_dirs ? latest_dir : "" );
	set->endGroup();

	if (save_playlist_in_config) {
		//Save current list
		set->beginGroup("playlist_contents");
		set->beginWriteArray("items");
		//set->setValue( "count", count() );
		for (int n = 0; n < count(); n++ ) {
			set->setArrayIndex(n);
			PLItem * i = itemData(n);
			set->setValue( QString("item_%1_filename").arg(n), i->filename() );
			set->setValue( QString("item_%1_duration").arg(n), i->duration() );
			set->setValue( QString("item_%1_name").arg(n), i->name() );
		}
		set->endArray();
		set->setValue( "current_item", findCurrentItem() );
		set->setValue( "modified", modified );

		set->endGroup();
	}

#ifdef PLAYLIST_DOWNLOAD
	set->beginGroup("history");
	set->setValue("max_items", history_urls->maxItems());
	set->setValue("urls", history_urls->toStringList());
	set->endGroup();
#endif

	if (set->contains("playlist/change_title")) set->remove("playlist/change_title");
}

void Playlist::loadSettings() {
	qDebug("Playlist::loadSettings");

	if (!set) return;

	set->beginGroup( "playlist");

	repeatAct->setChecked( set->value( "repeat", repeatAct->isChecked() ).toBool() );
	shuffleAct->setChecked( set->value( "shuffle", shuffleAct->isChecked() ).toBool() );

	automatically_get_info = set->value( "auto_get_info", automatically_get_info ).toBool();
	recursive_add_directory = set->value( "recursive_add_directory", recursive_add_directory ).toBool();
	save_playlist_in_config = set->value( "save_playlist_in_config", save_playlist_in_config ).toBool();
	play_files_from_start = set->value( "play_files_from_start", play_files_from_start ).toBool();
	automatically_play_next = set->value( "automatically_play_next", automatically_play_next ).toBool();
	ignore_player_errors = set->value( "ignore_player_errors", ignore_player_errors ).toBool();
	change_name = set->value( "change_name", change_name ).toBool();

	row_spacing = set->value( "row_spacing", row_spacing ).toInt();

#if !DOCK_PLAYLIST
	resize( set->value("size", size()).toSize() );
#endif
	listView->horizontalHeader()->restoreState(set->value(QString("header_state/%1").arg(Helper::qtVersion()), QByteArray()).toByteArray());

	int sort_column = set->value("sort_column", COL_NUM).toInt();
	int sort_order = set->value("sort_order", Qt::AscendingOrder).toInt();
	int filter_case_sensivity = set->value("filter_case_sensivity", Qt::CaseInsensitive).toInt();
	QString filter = set->value( "filter").toString();
	int sort_case_sensivity = set->value("sort_case_sensivity", Qt::CaseInsensitive).toInt();

	set->endGroup();

	set->beginGroup( "directories");
	save_dirs = set->value("save_dirs", save_dirs).toBool();
	if (save_dirs) {
		latest_dir = set->value("latest_dir", latest_dir).toString();
	}
	set->endGroup();

	if (save_playlist_in_config) {
		//Load latest list
		set->beginGroup("playlist_contents");
		int count = set->beginReadArray("items");

		QString filename, name;
		double duration;
		for (int n = 0; n < count; n++) {
			set->setArrayIndex(n);
			filename = set->value( QString("item_%1_filename").arg(n), "" ).toString();
			duration = set->value( QString("item_%1_duration").arg(n), -1 ).toDouble();
			name = set->value( QString("item_%1_name").arg(n), "" ).toString();
			addItem( filename, name, duration );
		}
		set->endArray();
		setCurrentItem( set->value( "current_item", -1 ).toInt() );
		setModified( set->value( "modified", false ).toBool() );

		set->endGroup();
		//listView->resizeColumnsToContents();
	}

#ifdef PLAYLIST_DOWNLOAD
	set->beginGroup("history");
	history_urls->setMaxItems(set->value("max_items", 50).toInt());
	history_urls->fromStringList( set->value("urls", history_urls->toStringList()).toStringList() );
	set->endGroup();
#endif

	proxy->setFilterCaseSensitivity( (Qt::CaseSensitivity) filter_case_sensivity);
	proxy->setSortCaseSensitivity( (Qt::CaseSensitivity) sort_case_sensivity);
	proxy->sort(sort_column, (Qt::SortOrder) sort_order);
	filter_edit->setText(filter);
}

QString Playlist::lastDir() {
	QString last_dir = latest_dir;
	return last_dir;
}

#ifdef PLAYLIST_DOWNLOAD
void Playlist::openUrl() {
	qDebug("Playlist::openUrl");

	InputURL d(this);

	// Get url from clipboard
	QString clipboard_text = QApplication::clipboard()->text();
	if (!clipboard_text.isEmpty() && clipboard_text.contains("://")) {
		d.setURL(clipboard_text);
	}

	for (int n = 0; n < history_urls->count(); n++) {
		d.setURL(history_urls->url(n));
	}

	if (d.exec() == QDialog::Accepted ) {
		QString url = d.url();
		if (!url.isEmpty()) {
			history_urls->addUrl(url);
			openUrl(url);
		}
	}
}

void Playlist::openUrl(const QString & url) {
	qDebug() << "Playlist::openUrl:" << url;
	downloader->fetchPage(url);

	showLoadingAnimation(true);
}

void Playlist::playlistDownloaded(QByteArray data) {
	qDebug("Playlist::playlistDownloaded");
	// Save to a temporary file
	QTemporaryFile tf;
	tf.open();
	tf.write(data);
	tf.close();
	QString tfile = tf.fileName();
	qDebug() << "Playlist::playlistDownloaded: tfile:" << tfile;

	if (data.contains("#EXTM3U")) {
		load_m3u(tfile, M3U8);
	}
	else
	if (data.contains("[playlist]")) {
		load_pls(tfile);
	}
	else
	if (data.contains("xspf.org")) {
		loadXSPF(tfile);
	}
	else {
		QMessageBox::warning(this, "SMPlayer", tr("It's not possible to load this playlist") +": "+ tr("Unrecognized format."));
	}

	showLoadingAnimation(false);
}

void Playlist::errorOcurred(int error_number, QString error_str) {
	showLoadingAnimation(false);

	qDebug() << "Playlist::errorOcurred:" << error_number << ":" << error_str;
	QMessageBox::warning(this, "SMPlayer", error_str);
}

void Playlist::showLoadingAnimation(bool b) {
	if (b) animation->start(); else animation->stop();
	loading_label_action->setVisible(b);
}

void Playlist::setMaxItemsUrlHistory(int max_items) {
	history_urls->setMaxItems(max_items);
}

int Playlist::maxItemsUrlHistory() {
	return history_urls->maxItems();
}
#endif

// Language change stuff
void Playlist::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_playlist.cpp"