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

#include "audiotracks.h"
#include <QRegExp>

AudioTracks::AudioTracks() { 
	clear();
}

AudioTracks::~AudioTracks() {
}

void AudioTracks::clear() {
	tm.clear();
}

void AudioTracks::addLang(int ID, QString lang) {
	tm[ID].setLang(lang);
	tm[ID].setID(ID);
}

void AudioTracks::addName(int ID, QString name) {
	tm[ID].setName(name);
	tm[ID].setID(ID);
}

void AudioTracks::addID(int ID) {
	tm[ID].setID(ID);
}


int AudioTracks::numItems() {
	return tm.count();
}

bool AudioTracks::existsItemAt(int n) {
	return ((n > 0) && (n < numItems()));
}

AudioData AudioTracks::itemAt(int n) {
	return tm.values()[n];
}

AudioData AudioTracks::item(int ID) {
	return tm[ID];
}

int AudioTracks::find(int ID) {
	for (int n=0; n < numItems(); n++) {
		if (itemAt(n).ID() == ID) return n;
	}
	return -1;
}

int AudioTracks::findLang(QString expr) {
	qDebug( "AudioTracks::findLang: '%s'", expr.toUtf8().data());
	QRegExp rx( expr );

	int res_id = -1;

	for (int n=0; n < numItems(); n++) {
		qDebug("AudioTracks::findLang: lang #%d '%s'", n, itemAt(n).lang().toUtf8().data());
		if (rx.indexIn( itemAt(n).lang() ) > -1) {
			qDebug("AudioTracks::findLang: found preferred lang!");
			res_id = itemAt(n).ID();
			break;	
		}
	}

	return res_id;
}

void AudioTracks::list() {
	QMapIterator<int, AudioData> i(tm);
	while (i.hasNext()) {
		i.next();
		AudioData d = i.value();
        qDebug("AudioTracks::list: item %d: ID: %d lang: '%s' name: '%s'",
               i.key(), d.ID(), d.lang().toUtf8().constData(), d.name().toUtf8().constData() );
	}
}

/*
#ifndef NO_USE_INI_FILES
void AudioTracks::save(QSettings & set) {
	qDebug("AudioTracks::save");

	set.setValue( "num_tracks", numItems() );
	for (int n=0; n < numItems(); n++) {
		set.beginGroup( "track_" + QString::number(n) );
		itemAt(n).save(set);
		set.endGroup();
	}
}

void AudioTracks::load(QSettings & set) {
	qDebug("AudioTracks::load");

	int num_tracks = set.value( "num_tracks", 0 ).toInt();

	int ID;
	for (int n=0; n < num_tracks; n++) {
		set.beginGroup( "track_" + QString::number(n) );
		ID = set.value("ID", -1).toInt();
		if (ID!=-1) {
			tm[ID].setID(ID);
			tm[ID].load(set);
		}
		set.endGroup();
	}

	//list();
}
#endif // NO_USE_INI_FILES
*/
