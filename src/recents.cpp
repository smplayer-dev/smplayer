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

#include "recents.h"
#include "global.h"
#include <QSettings>

Recents::Recents(QObject* parent) : QObject(parent) 
{
	l.clear();
	max_items = 10;
}

Recents::~Recents() {
}

void Recents::clear() {
	l.clear();
}

void Recents::add(QString s) {
	qDebug("Recents::add: '%s'", s.toUtf8().data());

	int pos = l.indexOf(s);
	if (pos != -1) l.removeAt(pos);
	l.prepend(s);

	if (l.count() > max_items) l.removeLast();
}

int Recents::count() {
	return l.count();
}

QString Recents::item(int n) {
	return l[n];
}

void Recents::list() {
	qDebug("Recents::list");

	for (int n=0; n < count(); n++) {
		qDebug(" * item %d: '%s'", n, item(n).toUtf8().data() );
	}
}

void Recents::save(QSettings * set, QString section) {
	qDebug("Recents::save");

	if (section.isEmpty()) section = "recent_files";

	set->beginGroup(section);
	set->setValue( "files", l );
	set->endGroup();
}

void Recents::load(QSettings * set, QString section) {
	qDebug("Recents::load");

	l.clear();

	if (section.isEmpty()) section = "recent_files";

	set->beginGroup(section);
	l = set->value( "files" ).toStringList();
	set->endGroup();
}

#include "moc_recents.cpp"
