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

#include "audioequalizer.h"
#include "eqslider.h"
#include "images.h"
#include "preferences.h"
#include "global.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>

using namespace Global;

AudioEqualizer::AudioEqualizer( QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QBoxLayout *bl = new QHBoxLayout; //(0, 4, 2);

	for (int n = 0; n < 10; n++) {
		eq[n] = new EqSlider(this);
		eq[n]->setIcon( QPixmap() );
		eq[n]->sliderWidget()->setRange(-120, 120);
		bl->addWidget(eq[n]);
	}

	presets_combo = new QComboBox(this);
	connect(presets_combo, SIGNAL(activated(int)), this, SLOT(presetChanged(int)));

	presets_label = new QLabel("&Preset", this);
	presets_label->setBuddy(presets_combo);

	reset_button = new QPushButton( "&Reset", this);
	connect( reset_button, SIGNAL(clicked()), this, SLOT(reset()) );

	set_default_button = new QPushButton( "&Set as default values", this );
	connect( set_default_button, SIGNAL(clicked()), this, SLOT(setDefaults()) );

	apply_button = new QPushButton( "&Apply", this );
	connect( apply_button, SIGNAL(clicked()), this, SLOT(applyButtonClicked()) );

	QBoxLayout *button_layout = new QHBoxLayout; //(0, 4, 2);
	button_layout->addWidget(presets_label);
	button_layout->addWidget(presets_combo);
	button_layout->addStretch();
	button_layout->addWidget(apply_button);
	button_layout->addWidget(reset_button);
	button_layout->addWidget(set_default_button);

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

	eq[0]->setLabel( tr("31.25 Hz") );
	eq[1]->setLabel( tr("62.50 Hz") );
	eq[2]->setLabel( tr("125 Hz") );
	eq[3]->setLabel( tr("250 Hz") );
	eq[4]->setLabel( tr("500 Hz") );
	eq[5]->setLabel( tr("1 kHz") );
	eq[6]->setLabel( tr("2 kHz") );
	eq[7]->setLabel( tr("4 KHz") );
	eq[8]->setLabel( tr("8 kHz") );
	eq[9]->setLabel( tr("16 kHz") );

	presets_label->setText( tr("&Preset") );
	apply_button->setText( tr("&Apply") );
	reset_button->setText( tr("&Reset") );
	set_default_button->setText( tr("&Set as default values") );

	int presets_combo_index = presets_combo->currentIndex();
	if (presets_combo_index < 0) presets_combo_index = 0;
	presets_combo->clear();
	presets_combo->addItem( tr("Flat"), Flat);
	presets_combo->addItem( tr("Pop"), Pop);
	presets_combo->addItem( tr("Rock"), Rock);
	presets_combo->setCurrentIndex(presets_combo_index);

	// What's this help:
	set_default_button->setWhatsThis(
			tr("Use the current values as default values for new videos.") );

	reset_button->setWhatsThis( tr("Set all controls to zero.") );

}

void AudioEqualizer::reset() {
	for (int n = 0; n < 10; n++) {
		eq[n]->setValue(0);
	}
}

void AudioEqualizer::setDefaults() {
	AudioEqualizerList l;
	for (int n = 0; n < 10; n++) {
		l << eq[n]->value();
	}
	pref->initial_audio_equalizer = l;

	QMessageBox::information(this, tr("Information"), 
                             tr("The current values have been stored to be "
                                "used as default.") );
}

void AudioEqualizer::setValues(int e0, int e1, int e2, int e3, int e4, int e5, int e6, int e7, int e8, int e9) {
	eq[0]->setValue(e0);
	eq[1]->setValue(e1);
	eq[2]->setValue(e2);
	eq[3]->setValue(e3);
	eq[4]->setValue(e4);
	eq[5]->setValue(e5);
	eq[6]->setValue(e6);
	eq[7]->setValue(e7);
	eq[8]->setValue(e8);
	eq[9]->setValue(e9);
}

void AudioEqualizer::presetChanged(int index) {
	qDebug("AudioEqualizer::presetChanged: %d", index);
	int p = presets_combo->itemData(index).toInt();
	switch (p) {
		case Flat:	reset(); break;
		case Pop:	setValues(0, 0, 35, 72, 81, 60, 26, 13, 0, 0); break;
		case Rock:	setValues(51, 37, 24, -14, -54, -14, 19, 52, 78, 78); break;
	}
}

void AudioEqualizer::applyButtonClicked() {
	AudioEqualizerList l;
	for (int n = 0; n < 10; n++) {
		l << eq[n]->value();
	}
	emit applyClicked( l );
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
