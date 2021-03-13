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

#include "tracks.h"
#include <QRegExp>
#include <QSettings>

Tracks::Tracks() { 
	clear();
}

Tracks::~Tracks() {
}

void Tracks::clear() {
	tm.clear();
}

void Tracks::addLang(int ID, QString lang) {
	tm[ID].setLang(lang);
	tm[ID].setID(ID);
}

void Tracks::addName(int ID, QString name) {
	tm[ID].setName(name);
	tm[ID].setID(ID);
}

void Tracks::addID(int ID) {
	tm[ID].setID(ID);
}


int Tracks::numItems() {
	return tm.count();
}

bool Tracks::existsItemAt(int n) {
	return ((n >= 0) && (n < numItems()));
}

int Tracks::IDAt(int n) {
	if (existsItemAt(n)) {
		return itemAt(n).ID();
	} else {
		return -1;
	}
}

TrackData Tracks::itemAt(int n) {
	return tm.values()[n];
}

TrackData Tracks::item(int ID) {
	return tm[ID];
}

int Tracks::find(int ID) {
	for (int n=0; n < numItems(); n++) {
		if (itemAt(n).ID() == ID) return n;
	}
	return -1;
}

#if !SIMPLE_TRACK_SELECTION
int Tracks::findLang(QString expr) {
	qDebug( "Tracks::findLang: '%s'", expr.toUtf8().data());
	QRegExp rx( expr );

	int res_id = -1;

	for (int n=0; n < numItems(); n++) {
		qDebug("Tracks::findLang: lang #%d '%s'", n, itemAt(n).lang().toUtf8().data());
		if (rx.indexIn( itemAt(n).lang() ) > -1) {
			qDebug("Tracks::findLang: found preferred lang!");
			res_id = itemAt(n).ID();
			break;
		}
	}

	return res_id;
}
#endif

void Tracks::list() {
	QMapIterator<int, TrackData> i(tm);
	while (i.hasNext()) {
		i.next();
		TrackData d = i.value();
        qDebug("Tracks::list: item %d: ID: %d lang: '%s' name: '%s'",
               i.key(), d.ID(), d.lang().toUtf8().constData(), d.name().toUtf8().constData() );
	}
}

void Tracks::save(QSettings * set, const QString & name) {
	set->beginWriteArray(name);
	for (int n = 0; n < numItems(); n++) {
		set->setArrayIndex(n);
		TrackData d = itemAt(n);
		set->setValue("id", d.ID());
		set->setValue("lang", d.lang());
		set->setValue("name", d.name());
	}
	set->endArray();
}

void Tracks::load(QSettings * set, const QString & name) {
	clear();
	int items = set->beginReadArray(name);
	for (int n = 0; n < items; n++) {
		set->setArrayIndex(n);
		int ID = set->value("id", -1).toInt();
		if (ID != -1) {
			addID(ID);
			addLang(ID, set->value("lang", "").toString());
			addName(ID, set->value("name", "").toString());
		}
	}
	set->endArray();
}
