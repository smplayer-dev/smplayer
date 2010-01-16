/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "filters.h"
#include <QSettings>

Filters::Filters(QObject * parent) : QObject(parent) 
{
	init();
}

void Filters::init() {
	list.clear();

	// Video
	list["noise"] = Filter(tr("add noise"), "noise", "9ah:5ah");
	list["deblock"] = Filter(tr("deblock"), "pp", "vb/hb");
	list["denoise_normal"] = Filter(tr("normal denoise"), "hqdn3d");
	list["denoise_soft"] = Filter(tr("soft denoise"), "hqdn3d", "2:1:2");

	// Audio
	list["volnorm"] = Filter(tr("volume normalization"), "volnorm", "1");
}

Filter Filters::item(const QString & key) {
	return list[key];
}

void Filters::save(QSettings *set) {
	set->beginGroup("filter_options");

	QMap<QString, Filter>::iterator i;
	for (i = list.begin(); i != list.end(); ++i) {
		set->setValue(i.key(), i.value().options());
	}

	set->endGroup();
}

void Filters::load(QSettings *set) {
	set->beginGroup("filter_options");

	QMap<QString, Filter>::iterator i;
	for (i = list.begin(); i != list.end(); ++i) {
		i.value().setOptions( set->value(i.key(), i.value().options()).toString() );
	}

	set->endGroup();
}

#include "moc_filters.cpp"
