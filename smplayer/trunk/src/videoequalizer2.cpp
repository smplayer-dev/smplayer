/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "videoequalizer2.h"

VideoEqualizer2::VideoEqualizer2( QWidget* parent, Qt::WindowFlags f ) 
	: QWidget(parent, f)
{
	setupUi(this);

	contrast_label->setNum(0);
	brightness_label->setNum(0);
	hue_label->setNum(0);
	saturation_label->setNum(0);
	gamma_label->setNum(0);

	connect( contrast_slider, SIGNAL(valueChanged(int)),
             contrast_label, SLOT(setNum(int)) );

	connect( brightness_slider, SIGNAL(valueChanged(int)),
             brightness_label, SLOT(setNum(int)) );

	connect( hue_slider, SIGNAL(valueChanged(int)),
             hue_label, SLOT(setNum(int)) );

	connect( saturation_slider, SIGNAL(valueChanged(int)),
             saturation_label, SLOT(setNum(int)) );

	connect( gamma_slider, SIGNAL(valueChanged(int)),
             gamma_label, SLOT(setNum(int)) );

	// Reemit signals
	connect( contrast_slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(contrastChanged(int)) );
	connect( brightness_slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(brightnessChanged(int)) );
	connect( hue_slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(hueChanged(int)) );
	connect( saturation_slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(saturationChanged(int)) );
	connect( gamma_slider, SIGNAL(valueChanged(int)),
             this, SIGNAL(gammaChanged(int)) );
}

VideoEqualizer2::~VideoEqualizer2() {
}

void VideoEqualizer2::reset() {
	setContrast(0);
	setBrightness(0);
	setHue(0);
	setSaturation(0);
	setGamma(0);
}

void VideoEqualizer2::on_reset_button_clicked() {
	qDebug("VideoEqualizer2::on_reset_button_clicked");
	reset();
}

void VideoEqualizer2::on_makedefault_button_clicked() {
	qDebug("VideoEqualizer2::on_makedefault_button_clicked");
}

void VideoEqualizer2::hideEvent( QHideEvent * ) {
	emit visibilityChanged();
}

void VideoEqualizer2::showEvent( QShowEvent * ) {
	emit visibilityChanged();
}

#include "moc_videoequalizer2.cpp"
