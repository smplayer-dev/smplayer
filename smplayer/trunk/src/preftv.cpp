/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "preftv.h"
#include "preferences.h"
#include "images.h"
#include "mediasettings.h"

PrefTV::PrefTV(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

#ifdef Q_OS_WIN
	rescan_check->hide();
#endif

	retranslateStrings();
}

PrefTV::~PrefTV()
{
}

QString PrefTV::sectionName() {
	return tr("TV and radio");
}

QPixmap PrefTV::sectionIcon() {
    return Images::icon("open_tv", 22);
}

void PrefTV::retranslateStrings() {
	retranslateUi(this);

	int deinterlace_item = deinterlace_combo->currentIndex();
	deinterlace_combo->clear();
	deinterlace_combo->addItem( tr("None"), MediaSettings::NoDeinterlace );
	deinterlace_combo->addItem( tr("Lowpass5"), MediaSettings::L5 );
	deinterlace_combo->addItem( tr("Yadif (normal)"), MediaSettings::Yadif );
	deinterlace_combo->addItem( tr("Yadif (double framerate)"), MediaSettings::Yadif_1 );
	deinterlace_combo->addItem( tr("Linear Blend"), MediaSettings::LB );
	deinterlace_combo->addItem( tr("Kerndeint"), MediaSettings::Kerndeint );
	deinterlace_combo->setCurrentIndex(deinterlace_item);

	createHelp();
}

void PrefTV::setData(Preferences * pref) {
	setInitialDeinterlace( pref->initial_tv_deinterlace );
	setRescan( pref->check_channels_conf_on_startup );
}

void PrefTV::getData(Preferences * pref) {
	requires_restart = false;

	pref->initial_tv_deinterlace = initialDeinterlace();
	pref->check_channels_conf_on_startup = rescan();
}

void PrefTV::setInitialDeinterlace(int ID) {
	int pos = deinterlace_combo->findData(ID);
	if (pos != -1) {
		deinterlace_combo->setCurrentIndex(pos);
	} else {
		qWarning("PrefTV::setInitialDeinterlace: ID: %d not found in combo", ID);
	}
}

int PrefTV::initialDeinterlace() {
	if (deinterlace_combo->currentIndex() != -1) {
		return deinterlace_combo->itemData( deinterlace_combo->currentIndex() ).toInt();
	} else {
		qWarning("PrefTV::initialDeinterlace: no item selected");
		return 0;
	}
}

void PrefTV::setRescan(bool b) {
	rescan_check->setChecked(b);
}

bool PrefTV::rescan() {
	return rescan_check->isChecked();
}

void PrefTV::createHelp() {
	clearHelp();

	setWhatsThis(deinterlace_combo, tr("Deinterlace by default for TV"),
        tr("Select the deinterlace filter that you want to be used for TV channels.") );

#ifndef Q_OS_WIN
	setWhatsThis(rescan_check, tr("Rescan ~/.mplayer/channels.conf on startup"),
		tr("If this option is enabled, SMPlayer will look for new TV and radio "
           "channels on ~/.mplayer/channels.conf.ter or ~/.mplayer/channels.conf.") );
#endif
}

#include "moc_preftv.cpp"
