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

#include <QToolBar>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
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

#include "mytablewidget.h"
#include "myaction.h"
#include "filedialog.h"
#include "helper.h"
#include "images.h"
#include "preferences.h"
#include "multilineinputdialog.h"
#include "version.h"
#include "global.h"
#include "core.h"
#include "extensions.h"
#include "guiconfig.h"


#if USE_INFOPROVIDER
#include "infoprovider.h"
#endif

#define DRAG_ITEMS 0
#define PL_ALLOW_DUPLICATES 1

#define COL_PLAY 0
#define COL_NAME 1
#define COL_TIME 2

using namespace Global;


Playlist::Playlist( Core *c, QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent,f)
	, modified(false)
	, recursive_add_directory(false)
	, automatically_get_info(false)
	, save_playlist_in_config(true)
	, play_files_from_start(true)
	, row_spacing(-1) // Default height
	, automatically_play_next(true)
	, ignore_player_errors(false)
	, change_title(false)
{
	core = c;
	playlist_path = "";
	latest_dir = "";

	createTable();
	createActions();
	createToolbar();

	connect( core, SIGNAL(mediaFinished()), this, SLOT(playNext()), Qt::QueuedConnection );
	connect( core, SIGNAL(mplayerFailed(QProcess::ProcessError)), this, SLOT(playerFailed(QProcess::ProcessError)) );
	connect( core, SIGNAL(mplayerFinishedWithError(int)), this, SLOT(playerFinishedWithError(int)) );
	connect( core, SIGNAL(mediaLoaded()), this, SLOT(getMediaInfo()) );

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

	loadSettings();

	// Ugly hack to avoid to play next item automatically
	if (!automatically_play_next) {
		disconnect( core, SIGNAL(mediaFinished()), this, SLOT(playNext()) );
	}

	// Save config every 5 minutes.
	save_timer = new QTimer(this);
	connect( save_timer, SIGNAL(timeout()), this, SLOT(maybeSaveSettings()) );
	save_timer->start( 5 * 60000 ); 
}

Playlist::~Playlist() {
	saveSettings();
}

void Playlist::setModified(bool mod) {
	qDebug("Playlist::setModified: %d", mod);

	modified = mod;
	emit modifiedChanged(modified);
}

void Playlist::createTable() {
	listView = new MyTableWidget( 0, COL_TIME + 1, this);
	listView->setObjectName("playlist_table");
	listView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	listView->setContextMenuPolicy( Qt::CustomContextMenu );
	listView->setShowGrid(false);
	listView->setSortingEnabled(false);
	//listView->setAlternatingRowColors(true);

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(listView->viewport());

	listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	listView->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);
#else
	listView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	listView->horizontalHeader()->setResizeMode(COL_NAME, QHeaderView::Stretch);
#endif
	/*
	listView->horizontalHeader()->setResizeMode(COL_TIME, QHeaderView::ResizeToContents);
	listView->horizontalHeader()->setResizeMode(COL_PLAY, QHeaderView::ResizeToContents);
	*/
	listView->setIconSize( Images::icon("ok").size() );

#if DRAG_ITEMS
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setDragEnabled(true);
	listView->setAcceptDrops(true);
	listView->setDropIndicatorShown(true);
	listView->setDragDropMode(QAbstractItemView::InternalMove);
#endif

	connect( listView, SIGNAL(cellActivated(int,int)),
             this, SLOT(itemDoubleClicked(int)) );

	// EDIT BY NEO -->
	connect( listView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortBy(int)));
	// <--
}

void Playlist::createActions() {
	openAct = new MyAction(this, "pl_open", false);
	connect( openAct, SIGNAL(triggered()), this, SLOT(load()) );

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

	toolbar->addAction(openAct);
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
	listView->setHorizontalHeaderLabels( QStringList() << "   " <<
        tr("Name") << tr("Length") );

	openAct->change( Images::icon("open"), tr("&Load") );
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

	// Icon
	setWindowIcon( Images::icon("logo", 64) );
	setWindowTitle( tr( "SMPlayer - Playlist" ) );
}

void Playlist::list() {
	qDebug("Playlist::list");

	PlaylistItemList::iterator it;
	for ( it = pl.begin(); it != pl.end(); ++it ) {
		qDebug( "filename: '%s', name: '%s' duration: %f",
               (*it).filename().toUtf8().data(), (*it).name().toUtf8().data(),
               (*it).duration() );
	}
}


QString Playlist::print(QString seperator){
	qDebug("Playlist::print");
	QString output = "";

	PlaylistItemList::iterator it;
	for ( it = pl.begin(); it != pl.end(); ++it ) {
		output += it->filename() + seperator + it->name() + seperator + QString::number(it->duration()) + "\r\n";
	}

	return output;
}

void Playlist::updateView() {
	qDebug("Playlist::updateView");

	listView->setRowCount( pl.count() );

	//QString number;
	QString name;
	QString time;

	for (int n=0; n < pl.count(); n++) {
		name = pl[n].name();
		if (name.isEmpty()) name = pl[n].filename();
		time = Helper::formatTime( (int) pl[n].duration() );
		
		//listView->setText(n, COL_POS, number);
		qDebug("Playlist::updateView: name: '%s'", name.toUtf8().data());
		listView->setText(n, COL_NAME, name);
		listView->setText(n, COL_TIME, time);

		if (pl[n].played()) {
			listView->setIcon(n, COL_PLAY, Images::icon("ok") );
		} else {
			listView->setIcon(n, COL_PLAY, QPixmap() );
		}

		if (row_spacing > -1) listView->setRowHeight(n, listView->font().pointSize() + row_spacing);
	}
	//listView->resizeColumnsToContents();
	listView->resizeColumnToContents(COL_PLAY);
	listView->resizeColumnToContents(COL_TIME);

	setCurrentItem(current_item);

	//adjustSize();
}

void Playlist::setCurrentItem(int current) {
	QIcon play_icon;
	play_icon = Images::icon("play");

	int old_current = current_item;
	current_item = current;

	if ((current_item > -1) && (current_item < pl.count())) {
		pl[current_item].setPlayed(true);
	}

	if ( (old_current >= 0) && (old_current < listView->rowCount()) ) {
		listView->setIcon(old_current, COL_PLAY, QPixmap() );
	}

	if ( (current_item >= 0) && (current_item < listView->rowCount()) ) {
		listView->setIcon(current_item, COL_PLAY, play_icon );
	}
	//if (current_item >= 0) listView->selectRow(current_item);
	if (current_item >= 0) {
		listView->clearSelection();
		listView->setCurrentCell( current_item, 0);
	}
}

void Playlist::clear() {
	pl.clear();

	listView->clearContents();
	listView->setRowCount(0);

	setCurrentItem(0);

	setModified( false );
}

void Playlist::remove(int i){
	if(i > -1 && i < pl.count()){
		pl.removeAt(i);
		if(current_item == i && i == (pl.count() - 1))
			setCurrentItem(i - 1);
		setModified(false);
		updateView();
	} //end if
}

int Playlist::count() {
	return pl.count();
}

bool Playlist::isEmpty() {
	return pl.isEmpty();
}

void Playlist::addItem(QString filename, QString name, double duration) {
	qDebug("Playlist::addItem: '%s'", filename.toUtf8().data());

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	filename = Helper::changeSlashes(filename);
	#endif

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
}

// EDIT BY NEO -->
void Playlist::sortBy(int section) {
	qDebug("Playlist::sortBy");

	sortBy(section, false, 0);
}

void Playlist::sortBy(int section, bool revert, int count) {
    // Bubble sort
    bool swaped = false;

    for ( int n = 0; n < (pl.count() - count); n++) {

      int last = n - 1;
      int current = n;

      // Revert the sort
      if (revert) {
          last = n;
          current = n - 1;
      }

      if (n > 0) {
          int compare = 0;

          if (section == 0) {
              // Sort by played
              bool lastItem = pl[last].played();
              bool currentItem = pl[current].played();

              if (!lastItem && currentItem) {
                  compare = 1;
              } else if (lastItem && currentItem) {
                  if (last == current_item) {
                     compare = 1;
                 } else {
                     compare = -1;
                 }
              } else {
                  compare = -1;
              }
          }
          else if (section == 1) {
              // Sort alphabetically
              QString lastItem = pl[last].name();
              QString currentItem = pl[current].name();
              compare = lastItem.compare(currentItem);
          } else if (section == 2) {
              // Sort by duration
              double lastItem = pl[last].duration();
              double currentItem = pl[current].duration();

              if (lastItem == currentItem) {
                  compare = 0;
              } else if (lastItem > currentItem) {
                  compare = 1;
              } else {
                  compare = -1;
              }
          }

          // Swap items
          if(compare > 0) {
              swapItems(n, n - 1);

              if (current_item == (n - 1)) {
                  current_item = n;
              } else if (current_item == n) {
                  current_item = n - 1;
              }

              listView->clearSelection();
              listView->setCurrentCell(n - 1, 0);

              swaped = true;
          }
      }
    }

    if ((count == 0) && !swaped && !revert) {
        // Revert sort
        sortBy(section, true, 0);
    }else if(swaped) {
        // Sort until there is nothing to sort
        sortBy(section, revert, ++count);
    } else {
        updateView();
    }
}
// <--

void Playlist::load_m3u(QString file) {
	qDebug("Playlist::load_m3u");

	bool utf8 = (QFileInfo(file).suffix().toLower() == "m3u8");

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
		list();
		updateView();

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

	list();
	updateView();

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

		list();
		updateView();
		setModified( false );
		startPlay();
	}
}

bool Playlist::save_m3u(QString file) {
	qDebug("Playlist::save_m3u: '%s'", file.toUtf8().data());

	QString dir_path = QFileInfo(file).path();
	if (!dir_path.endsWith("/")) dir_path += "/";

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	dir_path = Helper::changeSlashes(dir_path);
	#endif

	qDebug(" * dirPath: '%s'", dir_path.toUtf8().data());

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

		PlaylistItemList::iterator it;
		for ( it = pl.begin(); it != pl.end(); ++it ) {
			filename = (*it).filename();
			#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
			filename = Helper::changeSlashes(filename);
			#endif
			stream << "#EXTINF:";
			stream << (*it).duration() << ",";
			stream << (*it).name() << "\n";
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
	qDebug("Playlist::save_pls: '%s'", file.toUtf8().data());

	QString dir_path = QFileInfo(file).path();
	if (!dir_path.endsWith("/")) dir_path += "/";

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	dir_path = Helper::changeSlashes(dir_path);
	#endif

	qDebug(" * dirPath: '%s'", dir_path.toUtf8().data());

	QSettings set(file, QSettings::IniFormat);
	set.beginGroup( "playlist");
	
	QString filename;

	PlaylistItemList::iterator it;
	for ( int n=0; n < pl.count(); n++ ) {
		filename = pl[n].filename();
		#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
		filename = Helper::changeSlashes(filename);
		#endif

		// Try to save the filename as relative instead of absolute
		if (filename.startsWith( dir_path )) {
			filename = filename.mid( dir_path.length() );
		}

		set.setValue("File"+QString::number(n+1), filename);
		set.setValue("Title"+QString::number(n+1), pl[n].name());
		set.setValue("Length"+QString::number(n+1), (int) pl[n].duration());
	}

	set.setValue("NumberOfEntries", pl.count());
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

		for ( int n=0; n < pl.count(); n++ ) {
			QString location = pl[n].filename();
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

			QString title = pl[n].name();
			int duration = pl[n].duration() * 1000;

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
	int current = listView->currentRow();
	if (current > -1) {
		playItem(current);
	}
}

void Playlist::itemDoubleClicked(int row) {
	qDebug("Playlist::itemDoubleClicked: row: %d", row );
	playItem(row);
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
	qDebug("Playlist::playItem: %d (count:%d)", n, pl.count());

	if ( (n >= pl.count()) || (n < 0) ) {
		qDebug("Playlist::playItem: out of range");
		emit playlistEnded();
		return;
	}

	qDebug(" playlist_path: '%s'", playlist_path.toUtf8().data() );

	QString filename = pl[n].filename();
	QString filename_with_path = playlist_path + "/" + filename;

	if (!filename.isEmpty()) {
		//pl[n].setPlayed(true);
		setCurrentItem(n);
		if (play_files_from_start) 
			core->open(filename, 0);
		else
			core->open(filename);
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
		playItem( chosen_item );
	} else {
		bool finished_list = (current_item+1 >= pl.count());
		if (finished_list) clearPlayedTag();

		if ( (repeatAct->isChecked()) && (finished_list) ) {
			playItem(0);
		} else {
			playItem( current_item+1 );
		}
	}
}

void Playlist::playPrev() {
	qDebug("Playlist::playPrev");
	if (current_item > 0) {
		playItem( current_item-1 );
	} else {
		if (pl.count() > 1) playItem( pl.count() -1 );
	}
}


void Playlist::resumePlay() {
	if (pl.count() > 0) {
		if (current_item < 0) current_item = 0;
		playItem(current_item);
	}
}

void Playlist::getMediaInfo() {
	qDebug("Playlist::getMediaInfo");

	QString filename = core->mdat.filename;
	double duration = core->mdat.duration;
	QString artist = core->mdat.clip_artist;

	#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	filename = Helper::changeSlashes(filename);
	#endif

	QString name;
	if (change_title) {
		name = core->mdat.clip_name;
		if (name.isEmpty()) name = core->mdat.stream_title;

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

	for (int n = 0; n < pl.count(); n++) {
		if (pl[n].filename() == filename) {
			// Found item
			if (pl[n].duration() < 1) {
				if (!name.isEmpty()) {
					pl[n].setName(name);
				}
				pl[n].setDuration(duration);
			}
			else
			// Edited name (sets duration to 1)
			if (pl[n].duration() == 1) {
				pl[n].setDuration(duration);
			}
		}
	}

	updateView();
}

// Add current file to playlist
void Playlist::addCurrentFile() {
	qDebug("Playlist::addCurrentFile");
	if (!core->mdat.filename.isEmpty()) {
		addItem( core->mdat.filename, "", 0 );
		getMediaInfo();
	}
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
	if (pl.count() == 1) initial_file = pl[0].filename();
	int new_current_item = -1;

	for (int n = 0; n < files.count(); n++) {
		QString name = "";
		double duration = 0;
#if USE_INFOPROVIDER
		if ( (get_info) && (QFile::exists(files[n])) ) {
			data = InfoProvider::getInfo(files[n]);
			name = data.displayName();
			duration = data.duration;
			//updateView();
			//qApp->processEvents();
		}
#endif

		//qDebug() << "Playlist::addFiles: comparing:" << initial_file << "with" << files[n];

		if (!initial_file.isEmpty() && files[n] == initial_file) {
			PlaylistItem first_item = pl.takeFirst();
			name = first_item.name();
			duration = first_item.duration();
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
	updateView();

	qDebug( "Playlist::addFiles: latest_dir: '%s'", latest_dir.toUtf8().constData() );
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
		updateView();
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

	int first_selected = -1;
	int number_previous_item = 0;

	for (int n=0; n < listView->rowCount(); n++) {
		if (listView->isSelected(n, 0)) {
			qDebug(" row %d selected", n);
			pl[n].setMarkForDeletion(true);
			number_previous_item++;
			if (first_selected == -1) first_selected = n;
		}
	}

	PlaylistItemList::iterator it;
	for ( it = pl.begin(); it != pl.end(); ++it ) {
		if ( (*it).markedForDeletion() ) {
			qDebug("Remove '%s'", (*it).filename().toUtf8().data());
			it = pl.erase(it);
			it--;
			setModified( true );
		}
	}


    if (first_selected < current_item) {
        current_item -= number_previous_item;
    }

	if (isEmpty()) setModified(false);
	updateView();

	if (first_selected >= listView->rowCount()) 
		first_selected = listView->rowCount() - 1;

	if ( ( first_selected > -1) && ( first_selected < listView->rowCount() ) ) {
		listView->clearSelection();
		listView->setCurrentCell( first_selected, 0);
		//listView->selectRow( first_selected );
	}
}

void Playlist::removeAll() {
	/*
	pl.clear();
	updateView();
	setModified( false );
	*/
	clear();
}

void Playlist::clearPlayedTag() {
	for (int n = 0; n < pl.count(); n++) {
		pl[n].setPlayed(false);
	}
	updateView();
}

int Playlist::chooseRandomItem() {
	qDebug( "Playlist::chooseRandomItem");

	QList <int> fi; //List of not played items (free items)
	for (int n = 0; n < pl.count(); n++) {
		if (!pl[n].played()) fi.append(n);
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

void Playlist::swapItems(int item1, int item2 ) {
	PlaylistItem it1 = pl[item1];
	pl[item1] = pl[item2];
	pl[item2] = it1;
	setModified( true );
}


void Playlist::upItem() {
	qDebug("Playlist::upItem");

	int current = listView->currentRow();
	qDebug(" currentRow: %d", current );

	moveItemUp(current);

}

void Playlist::downItem() {
	qDebug("Playlist::downItem");

	int current = listView->currentRow();
	qDebug(" currentRow: %d", current );

	moveItemDown(current);
}

void Playlist::moveItemUp(int current){
	qDebug("Playlist::moveItemUp");

	if (current >= 1) {
		swapItems( current, current-1 );
		if (current_item == (current-1)) current_item = current;
		else
		if (current_item == current) current_item = current-1;
		updateView();
		listView->clearSelection();
		listView->setCurrentCell( current-1, 0);
	}
}
void Playlist::moveItemDown(int current	){
	qDebug("Playlist::moveItemDown");

	if ( (current > -1) && (current < (pl.count()-1)) ) {
		swapItems( current, current+1 );
		if (current_item == (current+1)) current_item = current;
		else
		if (current_item == current) current_item = current+1;
		updateView();
		listView->clearSelection();
		listView->setCurrentCell( current+1, 0);
	}
}

void Playlist::editCurrentItem() {
	int current = listView->currentRow();
	if (current > -1) editItem(current);
}

void Playlist::editItem(int item) {
	QString current_name = pl[item].name();
	if (current_name.isEmpty()) current_name = pl[item].filename();

	bool ok;
	QString text = QInputDialog::getText( this,
            tr("Edit name"), 
            tr("Type the name that will be displayed in the playlist for this file:"), 
            QLineEdit::Normal,
            current_name, &ok );
    if ( ok && !text.isEmpty() ) {
        // user entered something and pressed OK
		pl[item].setName(text);

		// If duration == 0 the name will be overwritten!
		if (pl[item].duration()<1) pl[item].setDuration(1); 
		updateView();

		setModified( true );
    } 
}

void Playlist::deleteSelectedFileFromDisk() {
	qDebug("Playlist::deleteSelectedFileFromDisk");

	int current = listView->currentRow();
	if (current > -1) {
		// If more that one row is selected, select only the current one
		listView->clearSelection();
		listView->setCurrentCell(current, 0);

		QString filename = pl[current].filename();
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
				bool success = QFile::remove(filename);
				//bool success = false;

				if (success) {
					// Remove item from the playlist
					removeSelected();
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

	QSettings * set = settings;

	set->beginGroup( "directories");
	bool save_dirs = set->value("save_dirs", false).toBool();
	set->endGroup();

	set->beginGroup( "playlist");

	set->setValue( "repeat", repeatAct->isChecked() );
	set->setValue( "shuffle", shuffleAct->isChecked() );

	set->setValue( "auto_get_info", automatically_get_info );
	set->setValue( "recursive_add_directory", recursive_add_directory );
	set->setValue( "save_playlist_in_config", save_playlist_in_config );
	set->setValue( "play_files_from_start", play_files_from_start );
	set->setValue( "automatically_play_next", automatically_play_next );
	set->setValue( "ignore_player_errors", ignore_player_errors );
	set->setValue( "change_title", change_title );

	set->setValue( "row_spacing", row_spacing );

#if !DOCK_PLAYLIST
	set->setValue( "size", size() );
#endif
	if (save_dirs) {
		set->setValue( "latest_dir", latest_dir );
	} else {
		set->setValue( "latest_dir", "" );
	}

	set->endGroup();

	if (save_playlist_in_config) {
		//Save current list
		set->beginGroup( "playlist_contents");

		set->setValue( "count", (int) pl.count() );
		for ( int n=0; n < pl.count(); n++ ) {
			set->setValue( QString("item_%1_filename").arg(n), pl[n].filename() );
			set->setValue( QString("item_%1_duration").arg(n), pl[n].duration() );
			set->setValue( QString("item_%1_name").arg(n), pl[n].name() );
		}
		set->setValue( "current_item", current_item );
		set->setValue( "modified", modified );

		set->endGroup();
	}
}

void Playlist::loadSettings() {
	qDebug("Playlist::loadSettings");

	QSettings * set = settings;

	set->beginGroup( "playlist");

	repeatAct->setChecked( set->value( "repeat", repeatAct->isChecked() ).toBool() );
	shuffleAct->setChecked( set->value( "shuffle", shuffleAct->isChecked() ).toBool() );

	automatically_get_info = set->value( "auto_get_info", automatically_get_info ).toBool();
	recursive_add_directory = set->value( "recursive_add_directory", recursive_add_directory ).toBool();
	save_playlist_in_config = set->value( "save_playlist_in_config", save_playlist_in_config ).toBool();
	play_files_from_start = set->value( "play_files_from_start", play_files_from_start ).toBool();
	automatically_play_next = set->value( "automatically_play_next", automatically_play_next ).toBool();
	ignore_player_errors = set->value( "ignore_player_errors", ignore_player_errors ).toBool();
	change_title = set->value( "change_title", change_title ).toBool();

	row_spacing = set->value( "row_spacing", row_spacing ).toInt();

#if !DOCK_PLAYLIST
	resize( set->value("size", size()).toSize() );
#endif

	latest_dir = set->value( "latest_dir", latest_dir ).toString();

	set->endGroup();

	if (save_playlist_in_config) {
		//Load latest list
		set->beginGroup( "playlist_contents");

		int count = set->value( "count", 0 ).toInt();
		QString filename, name;
		double duration;
		for ( int n=0; n < count; n++ ) {
			filename = set->value( QString("item_%1_filename").arg(n), "" ).toString();
			duration = set->value( QString("item_%1_duration").arg(n), -1 ).toDouble();
			name = set->value( QString("item_%1_name").arg(n), "" ).toString();
			addItem( filename, name, duration );
		}
		setCurrentItem( set->value( "current_item", -1 ).toInt() );
		setModified( set->value( "modified", false ).toBool() );
		updateView();

		set->endGroup();
	}
}

QString Playlist::lastDir() {
	QString last_dir = latest_dir;
	if (last_dir.isEmpty()) last_dir = pref->latest_dir;
	return last_dir;
}

// Language change stuff
void Playlist::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_playlist.cpp"