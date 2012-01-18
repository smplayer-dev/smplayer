/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "favorites.h"
#include "favoriteeditor.h"

#include <QAction>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QFileInfo>

//#define FIRST_MENU_ENTRY 4
#define FIRST_MENU_ENTRY 3

Favorites::Favorites(QString filename, QWidget * parent) : QMenu(parent)
{
	_filename = filename;

	parent_widget = parent;

	current_file = QString::null;
	last_item = 1;

	edit_act = new QAction( "Edit...", this);
	connect(edit_act, SIGNAL(triggered()), this, SLOT(edit()));

	jump_act = new QAction( "Jump...", this);
	connect(jump_act, SIGNAL(triggered()), this, SLOT(jump()));

	next_act = new QAction(this);
	connect(next_act, SIGNAL(triggered()), this, SLOT(next()));

	previous_act = new QAction(this);
	connect(previous_act, SIGNAL(triggered()), this, SLOT(previous()));

	add_current_act = new QAction( "Add current media", this);
	add_current_act->setEnabled(false);
	connect(add_current_act, SIGNAL(triggered()), SLOT(addCurrentPlaying()));

	retranslateStrings();

	load();

	connect( this, SIGNAL(triggered(QAction *)),
             this, SLOT(triggered_slot(QAction *)) );

	addAction(edit_act);
	addAction(add_current_act);
	//addAction(jump_act);
	addSeparator();

	populateMenu();

}

Favorites::~Favorites() {
	qDebug("Favorites::~Favorites");

	save();

	delete_children();
}

void Favorites::delete_children() {
	for (int n=0; n < child.count(); n++) {
		if (child[n]) delete child[n];
		child[n] = 0;
	}
	child.clear();
}

void Favorites::retranslateStrings() {
	edit_act->setText( tr("&Edit...") );
	jump_act->setText( tr("&Jump...") );
	next_act->setText( tr("&Next") );
	previous_act->setText( tr("&Previous") );
	add_current_act->setText( tr("&Add current media") );
}

Favorites * Favorites::createNewObject(QString filename, QWidget * parent) {
	return new Favorites(filename, parent);
}

void Favorites::populateMenu() {
	for (int n = 0; n < f_list.count(); n++) {
		QString i = QString::number(n+1);
		QString name = QString("%1 - " + f_list[n].name() ).arg( i.insert( i.size()-1, '&' ), 3, ' ' );
		if (f_list[n].isSubentry()) {
			Favorites * new_fav = createNewObject(f_list[n].file(), parent_widget);
			new_fav->getCurrentMedia(received_file_playing, received_title);
			connect(this, SIGNAL(sendCurrentMedia(const QString &, const QString &)), 
                    new_fav, SLOT(getCurrentMedia(const QString &, const QString &)));
			/*
			new_fav->editAct()->setText( editAct()->text() );
			new_fav->jumpAct()->setText( jumpAct()->text() );
			new_fav->nextAct()->setText( nextAct()->text() );
			new_fav->previousAct()->setText( previousAct()->text() );
			new_fav->addCurrentAct()->setText( addCurrentAct()->text() );
			*/

			child.push_back(new_fav);
			
			QAction * a = addMenu( new_fav );
			a->setText( name );
			a->setIcon( QIcon( f_list[n].icon() ) );
		} else {
			QAction * a = addAction( name );
			a->setData( f_list[n].file() );
			a->setIcon( QIcon( f_list[n].icon() ) );
			a->setStatusTip( f_list[n].file() );
		}
	}
}

void Favorites::updateMenu() {
	// Remove all except the first 2 items
	while (actions().count() > FIRST_MENU_ENTRY) {
		QAction * a = actions()[FIRST_MENU_ENTRY];
		removeAction( a );
		a->deleteLater();
	}

	delete_children();

	populateMenu();
	markCurrent();
}

void Favorites::triggered_slot(QAction * action) {
	if (action->data().isValid()) {
		QString file = action->data().toString();
		emit activated( file );
		current_file = file;;
		markCurrent();
	}
}

void Favorites::markCurrent() {
	for (int n = FIRST_MENU_ENTRY; n < actions().count(); n++) {
		QAction * a = actions()[n];
		QString file = a->data().toString();
		QFont f = a->font();

		if ((!file.isEmpty()) && (file == current_file)) {
			f.setBold(true);
			a->setFont( f );
		} else {
			f.setBold(false);
			a->setFont( f );
		}
	}
}

int Favorites::findFile(QString filename) {
	for (int n = 0; n < f_list.count(); n++) {
		if (f_list[n].file() == filename) return n;
	}
	return -1;
}

void Favorites::next() {
	qDebug("Favorites::next");

	int current = findFile(current_file);
	//if (current < 0) current = 0;

	current++;
	if (current >= f_list.count()) current = 0;

	QAction * a = actions()[current+FIRST_MENU_ENTRY]; // Skip "edit" and separator
	if (a != 0) {
		a->trigger();
	}
}

void Favorites::previous() {
	qDebug("Favorites::previous");

	int current = findFile(current_file);
	//if (current < 0) current = 0;

	current--;
	if (current < 0) current = f_list.count()-1;

	QAction * a = actions()[current+FIRST_MENU_ENTRY]; // Skip "edit" and separator
	if (a != 0) {
		a->trigger();
	}
}

void Favorites::getCurrentMedia(const QString & filename, const QString & title) {
	qDebug("Favorites::getCurrentMedia: '%s', '%s'", filename.toUtf8().constData(), title.toUtf8().constData());

	if (!filename.isEmpty()) {
		received_file_playing = filename;
		received_title = title;

		emit sendCurrentMedia(filename, title);

		add_current_act->setEnabled(true);
	}
}

void Favorites::addCurrentPlaying() {
	if (received_file_playing.isEmpty()) {
		qDebug("Favorites::addCurrentPlaying: received file is empty, doing nothing");
	} else {
		Favorite fav;
		fav.setName(received_title);
		fav.setFile(received_file_playing);
		f_list.append(fav);
		save();
		updateMenu();
	}
}

void Favorites::save() {
	qDebug("Favorites::save");

	QFile f( _filename );
    if ( f.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &f );
		stream.setCodec("UTF-8");

		stream << "#EXTM3U" << "\n";
		for (int n = 0; n < f_list.count(); n++) {
			stream << "#EXTINF:0,";
			stream << f_list[n].name() << ",";
			stream << f_list[n].icon() << ",";
			stream << f_list[n].isSubentry() << "\n";
			stream << f_list[n].file() << "\n";
		}
        f.close();
	}
}

void Favorites::load() {
	qDebug("Favorites::load");

	QRegExp m3u_id("^#EXTM3U|^#M3U");
	QRegExp info1("^#EXTINF:(.*),(.*),(.*)");
	QRegExp info2("^#EXTINF:(.*),(.*),(.*),(.*)");

    QFile f( _filename );
    if ( f.open( QIODevice::ReadOnly ) ) {

		f_list.clear();

		Favorite fav;

        QTextStream stream( &f );
		stream.setCodec("UTF-8");

        QString line;
        while ( !stream.atEnd() ) {
            line = stream.readLine(); // line of text excluding '\n'
			//qDebug("info2.indexIn: %d", info2.indexIn(line));
			//qDebug("info1.indexIn: %d", info1.indexIn(line));
            //qDebug( " * line: '%s'", line.toUtf8().data() );
			if (m3u_id.indexIn(line)!=-1) {
				//#EXTM3U
				// Ignore line
			}
			else
			if (info2.indexIn(line) != -1) {
				fav.setName( info2.cap(2) );
				fav.setIcon( info2.cap(3) );
				fav.setSubentry( info2.cap(4).toInt() == 1 );
			} 
			else
			// Compatibility with old files
			if (info1.indexIn(line) != -1) {
				fav.setName( info1.cap(2) );
				fav.setIcon( info1.cap(3) );
				fav.setSubentry( false );
			} 
			else
			if (line.startsWith("#")) {
				// Comment
				// Ignore
			} else {
				fav.setFile( line );
				if (fav.name().isEmpty()) fav.setName(line);
				//qDebug("Favorites::load: adding '%s' '%s'", fav.name().toUtf8().constData(), fav.file().toUtf8().constData());
				f_list.append(fav);

				// Clear data
				fav.setName("");
				fav.setFile("");
				fav.setIcon("");
				fav.setSubentry(false);
			}
        }
        f.close();
	}
}

void Favorites::edit() {
	qDebug("Favorites::edit");

	FavoriteEditor e(parent_widget);

	e.setData(f_list);
	e.setStorePath( QFileInfo(_filename).absolutePath() );

	if (e.exec() == QDialog::Accepted) {
		f_list = e.data();
		save();
		updateMenu();
		/*
		for (int n = 0; n < f_list.count(); n++) {
			qDebug("item %d: name: '%s' file: '%s'", n, f_list[n].name().toUtf8().constData(), f_list[n].file().toUtf8().constData());
		}
		*/
	}
}

void Favorites::jump() {
	qDebug("Favorites::jump");

	bool ok;
	int item = QInputDialog::getInteger(parent_widget, tr("Jump to item"),
                          tr("Enter the number of the item in the list to jump:"), 
                          last_item, 1, f_list.count(), 1, &ok);
	if (ok) {
		last_item = item;
		item--;
		actions()[item+FIRST_MENU_ENTRY]->trigger();
	}
}

// Language change stuff
void Favorites::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_favorites.cpp"

