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

#ifndef PREFNETWORK_H
#define PREFNETWORK_H

#include "ui_prefnetwork.h"
#include "prefwidget.h"
#include "config.h"

class Preferences;

class PrefNetwork : public PrefWidget, public Ui::PrefNetwork
{
	Q_OBJECT

public:
	PrefNetwork( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PrefNetwork();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

	// Pass data to the dialog
	void setData(Preferences * pref);

	// Apply changes
	void getData(Preferences * pref);

protected:
	virtual void createHelp();

	void setProxyType(int type);
	int proxyType();

#ifdef YOUTUBE_SUPPORT
	void setYTQuality(int q);
	int YTQuality();
#endif

	void setStreamingType(int);
	int streamingType();

#ifdef MPV_SUPPORT
	void setYTDLQuality(const QString & q);
	QString YTDLQuality();
#endif

#ifdef CHROMECAST_SUPPORT
	void setLocalIP(const QString & ip, const QStringList & values);
	QString localIP();
#endif

protected slots:
	void streaming_type_combo_changed(int);

protected:
	virtual void retranslateStrings();
};

#endif
