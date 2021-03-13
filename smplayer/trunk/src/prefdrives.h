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

#ifndef PREFDRIVES_H
#define PREFDRIVES_H

#include "ui_prefdrives.h"
#include "prefwidget.h"
#include "config.h"

class Preferences;

class PrefDrives : public PrefWidget, public Ui::PrefDrives
{
	Q_OBJECT

public:
	PrefDrives( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PrefDrives();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

    // Pass data to the dialog
    void setData(Preferences * pref);

    // Apply changes
    void getData(Preferences * pref);

protected:
	virtual void createHelp();

	void setDVDDevice( QString dir );
	QString dvdDevice();

#ifdef BLURAY_SUPPORT
	void setBlurayDevice( QString dir );
	QString blurayDevice();
#endif

	void setCDRomDevice( QString dir );
	QString cdromDevice();

#if DVDNAV_SUPPORT
	void setUseDVDNav(bool b);
	bool useDVDNav();
#endif

	void updateDriveCombos(bool detect_cd_devices = false);

protected slots:
	void on_check_drives_button_clicked();

protected:
	virtual void retranslateStrings();
};

#endif
