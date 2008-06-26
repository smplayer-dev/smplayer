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

#include "audioequalizer.h"
#include "eqslider.h"
#include "images.h"
#include "preferences.h"
#include "global.h"
#include <QLayout>
#include <QPushButton>
/*
#include <QVBoxLayout>
#include <QHBoxLayout>
*/

using namespace Global;

AudioEqualizer::AudioEqualizer( QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	eq0 = new EqSlider(this);
	eq1 = new EqSlider(this);
	eq2 = new EqSlider(this);
	eq3 = new EqSlider(this);
	eq4 = new EqSlider(this);
	eq5 = new EqSlider(this);
	eq6 = new EqSlider(this);
	eq7 = new EqSlider(this);
	eq8 = new EqSlider(this);
	eq9 = new EqSlider(this);

	eq0->setRange(-120, 120);
	eq1->setRange(-120, 120);
	eq2->setRange(-120, 120);
	eq3->setRange(-120, 120);
	eq4->setRange(-120, 120);
	eq5->setRange(-120, 120);
	eq6->setRange(-120, 120);
	eq7->setRange(-120, 120);
	eq8->setRange(-120, 120);
	eq9->setRange(-120, 120);

	QBoxLayout *bl = new QHBoxLayout; //(0, 4, 2);
	bl->addWidget(eq0);
	bl->addWidget(eq1);
	bl->addWidget(eq2);
	bl->addWidget(eq3);
	bl->addWidget(eq4);
	bl->addWidget(eq5);
	bl->addWidget(eq6);
	bl->addWidget(eq7);
	bl->addWidget(eq8);
	bl->addWidget(eq9);

	reset_button = new QPushButton( "&Reset", this);
	connect( reset_button, SIGNAL(clicked()), this, SLOT(reset()) );
	set_default_button = new QPushButton( "&Set as default values", this );
	connect( set_default_button, SIGNAL(clicked()), this, SLOT(setDefaults()) );

	QBoxLayout *button_layout = new QVBoxLayout; //(0, 4, 2);
	button_layout->addWidget(set_default_button);
	button_layout->addWidget(reset_button);

	QBoxLayout *layout = new QVBoxLayout(this); //, 4, 2);
	layout->addLayout(bl);
	layout->addLayout(button_layout);

	retranslateStrings();

	adjustSize();
	//setFixedSize( sizeHint() );
}

AudioEqualizer::~AudioEqualizer() {
}

void AudioEqualizer::retranslateStrings() {
	setWindowTitle( tr("Audio Equalizer") );
	setWindowIcon( Images::icon("logo") );

	eq0->setLabel( tr("31.25 Hz") );
	eq1->setLabel( tr("62.50 Hz") );
	eq2->setLabel( tr("125.0 Hz") );
	eq3->setLabel( tr("250.0 Hz") );
	eq4->setLabel( tr("500.0 Hz") );
	eq5->setLabel( tr("1.000 kHz") );
	eq6->setLabel( tr("2.000 kHz") );
	eq7->setLabel( tr("4.000 kHz") );
	eq8->setLabel( tr("8.000 kHz") );
	eq9->setLabel( tr("16.00 kHz") );

/*
	contrast->setLabel( tr("Contrast") );
	contrast->setToolTip( tr("Contrast") );
	contrast->setIcon( Images::icon("contrast") );

	brightness->setLabel( tr("Brightness") );
	brightness->setToolTip( tr("Brightness") );
	brightness->setIcon( Images::icon("brightness") );

	hue->setLabel( tr("Hue") );
	hue->setToolTip( tr("Hue") );
	hue->setIcon( Images::icon("hue") );

	saturation->setLabel( tr("Saturation") );
	saturation->setToolTip( tr("Saturation") );
	saturation->setIcon( Images::icon("saturation") );

	gamma->setLabel( tr("Gamma") );
	gamma->setToolTip( tr("Gamma") );
	gamma->setIcon( Images::icon("gamma") );
*/

	reset_button->setText( tr("&Reset") );
	set_default_button->setText( tr("&Set as default values") );

	// What's this help:
	set_default_button->setWhatsThis(
			tr("Use the current values as default values for new videos.") );

	reset_button->setWhatsThis( tr("Set all controls to zero.") );

}

void AudioEqualizer::reset() {
	eq0->setValue(0);
	eq1->setValue(0);
	eq2->setValue(0);
	eq3->setValue(0);
	eq4->setValue(0);
	eq5->setValue(0);
	eq6->setValue(0);
	eq7->setValue(0);
	eq8->setValue(0);
	eq9->setValue(0);
}

void AudioEqualizer::setDefaults() {
	qDebug("Not implemented yet");
}

void AudioEqualizer::hideEvent( QHideEvent * ) {
	emit visibilityChanged();
}

void AudioEqualizer::showEvent( QShowEvent * ) {
	emit visibilityChanged();
}

// Language change stuff
void AudioEqualizer::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_audioequalizer.cpp"
