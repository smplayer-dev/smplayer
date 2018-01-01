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


#ifndef AUDIOEQUALIZER_H
#define AUDIOEQUALIZER_H

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
	enum Preset { User_defined = 0, Flat = 1, Pop = 2, Rock = 3, Classical = 4, Club = 5, Dance = 6, Fullbass = 7,
                  FullbassTreble = 8, Fulltreble = 9, Headphones = 10, LargeHall = 11, Live = 12,
                  Party = 13, Reggae = 14, Ska = 15, Soft = 16, SoftRock = 17, Techno = 18 };

	AudioEqualizer( QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog );
	~AudioEqualizer();

	EqSlider * eq[10];

	void setEqualizer(AudioEqualizerList l);

signals:
	void visibilityChanged();
	void applyClicked(AudioEqualizerList new_values);
	void valuesChanged(AudioEqualizerList values);

public slots:
	void reset();
	void setDefaults();

protected slots:
	void applyButtonClicked();
	void presetChanged(int index);
	void updatePresetCombo();

protected:
	virtual void hideEvent( QHideEvent * );
	virtual void showEvent( QShowEvent * );
	virtual void changeEvent( QEvent * event );
	virtual void retranslateStrings();

	void createPresets();
	void setValues(AudioEqualizerList l);
	int findPreset(AudioEqualizerList l);

protected:
	QLabel * presets_label;
	QComboBox * presets_combo;
	QPushButton * apply_button;
	QPushButton * reset_button;
	QPushButton * set_default_button;
	QPushButton * close_button;
	QMap<int,AudioEqualizerList> preset_list;
};


#endif
