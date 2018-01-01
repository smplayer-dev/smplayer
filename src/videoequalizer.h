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

#ifndef VIDEOEQUALIZER_H
#define VIDEOEQUALIZER_H

#include "ui_videoequalizer.h"
#include <QWidget>

class VideoEqualizer : public QWidget, public Ui::VideoEqualizer
{
	Q_OBJECT

public:
	VideoEqualizer( QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog );
	~VideoEqualizer();

public slots:
	void setContrast(int v) { contrast_slider->setValue(v); }
	void setBrightness(int v) { brightness_slider->setValue(v); }
	void setHue(int v) { hue_slider->setValue(v); }
	void setSaturation(int v) { saturation_slider->setValue(v); }
	void setGamma(int v) { gamma_slider->setValue(v); }
	void setBySoftware(bool b) { bysoftware_check->setChecked(b); }

	void reset();

public:
	int contrast() { return contrast_slider->value(); }
	int brightness() { return brightness_slider->value(); }
	int hue() { return hue_slider->value(); }
	int saturation() { return saturation_slider->value(); }
	int gamma() { return gamma_slider->value(); }
	bool bySoftware() { return bysoftware_check->isChecked(); }

signals:
	void contrastChanged(int);
	void brightnessChanged(int);
	void hueChanged(int);
	void saturationChanged(int);
	void gammaChanged(int);

	void visibilityChanged();
	void requestToChangeDefaultValues();
	void bySoftwareChanged(bool);

protected slots:
	void on_reset_button_clicked();
	void on_bysoftware_check_stateChanged(int);

	virtual void hideEvent( QHideEvent * );
	virtual void showEvent( QShowEvent * );

protected:
	virtual void retranslateStrings();
	virtual void changeEvent( QEvent * event);
};

#endif
