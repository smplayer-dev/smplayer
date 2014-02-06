/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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


#ifndef _AUDIOOEQUALIZER_H_
#define _AUDIOOEQUALIZER_H_

#include <QWidget>
#include <QHideEvent>
#include <QShowEvent>
#include "audioequalizerlist.h"

class QLabel;
class QComboBox;
class QPushButton;
class EqSlider;

class AudioEqualizer : public QWidget
{
	Q_OBJECT

public:
	enum Preset { Flat = 0, Pop = 1, Rock = 2, Classical = 3, Club = 4, Dance = 5, Fullbass = 6,
                  FullbassTreble = 7, Fulltreble = 8, Headphones = 9, LargeHall = 10, Live = 11,
                  Party = 12, Reggae = 13, Ska = 14, Soft = 15, SoftRock = 16, Techno = 17 };

	AudioEqualizer( QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog );
	~AudioEqualizer();

	EqSlider * eq[10];

signals:
	void visibilityChanged();
	void applyClicked(AudioEqualizerList new_values);

public slots:
	void reset();
	void setDefaults();

protected slots:
	void applyButtonClicked();
	void presetChanged(int index);

protected:
	virtual void hideEvent( QHideEvent * );
	virtual void showEvent( QShowEvent * );
	virtual void changeEvent( QEvent * event );
	virtual void retranslateStrings();

	void createPresets();
	void setValues(AudioEqualizerList l);

protected:
	QLabel * presets_label;
	QComboBox * presets_combo;
	QPushButton * apply_button;
	QPushButton * reset_button;
	QPushButton * set_default_button;
	QMap<int,AudioEqualizerList> preset_list;
};


#endif
