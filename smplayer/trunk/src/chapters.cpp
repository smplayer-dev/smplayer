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

#include "chapters.h"

Chapters::Chapters() {
	clear();
}

Chapters::~Chapters() {
}

void Chapters::clear() {
	cm.clear();
}

void Chapters::addName(int ID, QString name) {
	cm[ID].setName(name);
	cm[ID].setID(ID);
}

void Chapters::addStart(int ID, double start) {
	cm[ID].setStart(start);
	cm[ID].setID(ID);
}

void Chapters::addEnd(int ID, double end) {
	cm[ID].setEnd(end);
	cm[ID].setID(ID);
}

void Chapters::addID(int ID) {
	cm[ID].setID(ID);
}

int Chapters::numItems() {
	return cm.count();
}

bool Chapters::existsItemAt(int n) {
	return ((n > 0) && (n < numItems()));
}

ChapterData Chapters::itemAt(int n) {
	return cm.values()[n];
}

ChapterData Chapters::item(int ID) {
	return cm[ID];
}

ChapterData Chapters::itemFromTime(double sec)
{
	QMapIterator<int, ChapterData> i(cm);
	while(i.hasNext()) {
		i.next();
		ChapterData d = i.value();
		if(d.start() <= sec && d.end() >= sec)
			return d;
	}
	return cm[-1];
}

ChapterData Chapters::itemAfterTime(double sec)
{
	QMapIterator<int, ChapterData> i(cm);
	int ID = -1;
	double min_time = 1.7976931348623158e+308;
	while(i.hasNext()) {
		i.next();
		ChapterData d = i.value();
		if(d.start() > sec && d.start() < min_time)
		{
			ID = d.ID();
			min_time = d.start();
		}
	}
	return cm[ID];
}

ChapterData Chapters::itemBeforeTime(double sec)
{
	QMapIterator<int, ChapterData> i(cm);
	int ID = -1;
	double max_time = 0;
	while(i.hasNext()) {
		i.next();
		ChapterData d = i.value();
		if(d.end() < sec && d.end() > max_time)
		{
			ID = d.ID();
			max_time = d.end();
		}
	}
	return cm[ID];
}

int Chapters::find(int ID) {
	for (int n=0; n < numItems(); n++) {
		if (itemAt(n).ID() == ID) return n;
	}
	return -1;
}

void Chapters::list() {
	QMapIterator<int, ChapterData> i(cm);
	while (i.hasNext()) {
		i.next();
		ChapterData d = i.value();
        qDebug("Chapters::list: item %d: ID: %d name: '%s' start: %g end: %g",
               i.key(), d.ID(), d.name().toUtf8().constData(), d.start(), d.end() );
	}
}
