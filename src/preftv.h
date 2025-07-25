/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef PREFTV_H
#define PREFTV_H

#include "ui_preftv.h"
#include "prefwidget.h"

class Preferences;

class PrefTV : public PrefWidget, public Ui::PrefTV
{
	Q_OBJECT

public:
	PrefTV( QWidget * parent = 0, Qt::WindowFlags f = QFlag(0) );
	~PrefTV();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

    // Pass data to the dialog
    void setData(Preferences * pref);

    // Apply changes
    void getData(Preferences * pref);

protected:
	virtual void createHelp();

	void setInitialDeinterlace(int ID);
	int initialDeinterlace();

	void setRescan(bool b);
	bool rescan();


protected:
	virtual void retranslateStrings();
};

#endif
