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

#include "favorites.h"
#include "favoriteeditor.h"
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QFile>
#include <QTextStream>

Favorites::Favorites(QString filename, QWidget * parent) : QObject(parent)
{
	_filename = filename;
	_menu = 0;
	parent_widget = parent;

	editAct = new QAction( "Edit...", this);
	connect(editAct, SIGNAL(triggered()), this, SLOT(edit()));

	load();
}

Favorites::~Favorites() {
	save();
	if (_menu != 0) delete _menu;
}

QMenu * Favorites::menu() {
	if (_menu == 0) createMenu();
	return _menu;
}

void Favorites::createMenu() {
	_menu = new QMenu(parent_widget);
	connect( _menu, SIGNAL(triggered(QAction *)),
             this, SLOT(triggered_slot(QAction *)) );

	_menu->addAction(editAct);
	_menu->addSeparator();

	populateMenu();
}

void Favorites::populateMenu() {
	for (int n = 0; n < f_list.count(); n++) {
		QAction * a = _menu->addAction( f_list[n].name() );
		a->setData( f_list[n].file() );
		a->setIcon( QIcon( f_list[n].icon() ) );
	}
}

void Favorites::updateMenu() {
	// Remove all except the first 2 items
	while (_menu->actions().count() > 2) {
		QAction * a = _menu->actions()[2];
		_menu->removeAction( a );
		a->deleteLater();
	}

	populateMenu();
}

void Favorites::triggered_slot(QAction * action) {
	if (action->data().isValid()) {
		emit activated(action->data().toString());
	}
}

int Favorites::findFile(QString filename) {
	for (int n = 0; n < f_list.count(); n++) {
		if (f_list[n].file() == filename) return n;
	}
	return -1;
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
			stream << f_list[n].icon() << "\n";
			stream << f_list[n].file() << "\n";
		}
        f.close();
	}
}

void Favorites::load() {
	qDebug("Favorites::load");

	QRegExp m3u_id("^#EXTM3U|^#M3U");
	QRegExp info("^#EXTINF:(.*),(.*),(.*)");

    QFile f( _filename );
    if ( f.open( QIODevice::ReadOnly ) ) {

		f_list.clear();

		Favorite fav;

        QTextStream stream( &f );
		stream.setCodec("UTF-8");

        QString line;
        while ( !stream.atEnd() ) {
            line = stream.readLine(); // line of text excluding '\n'
            qDebug( " * line: '%s'", line.toUtf8().data() );
			if (m3u_id.indexIn(line)!=-1) {
				//#EXTM3U
				// Ignore line
			}
			else
			if (info.indexIn(line) != -1) {
				fav.setName( info.cap(2) );
				fav.setIcon( info.cap(3) );
			} 
			else
			if (line.startsWith("#")) {
				// Comment
				// Ignore
			} else {
				fav.setFile( line );
				if (fav.name().isEmpty()) fav.setName(line);
				f_list.append(fav);
			}
        }
        f.close();
	}
}

void Favorites::edit() {
	qDebug("Favorites::edit");

	FavoriteEditor e(parent_widget);
	e.setWindowTitle(tr("Favorite editor"));
	e.setCaption("<b>" + tr("Favorite list") + "</b>");

	e.setData(f_list);

	if (e.exec() == QDialog::Accepted) {
		f_list = e.data();
		updateMenu();
		/*
		for (int n = 0; n < f_list.count(); n++) {
			qDebug("item %d: name: '%s' file: '%s'", n, f_list[n].name().toUtf8().constData(), f_list[n].file().toUtf8().constData());
		}
		*/
	}
}

#include "moc_favorites.cpp"

