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


#include "preferencesdialog.h"

#include "prefwidget.h"
#include "prefgeneral.h"
#include "prefdrives.h"
#include "prefinterface.h"
#include "prefperformance.h"
#include "prefinput.h"
#include "prefsubtitles.h"
#include "prefadvanced.h"
#include "prefplaylist.h"
#include "prefupdates.h"
#include "prefnetwork.h"
#include "infowindow.h"
#include "preferences.h"
#include "images.h"
#include <QVBoxLayout>
#include <QDebug>

#ifdef TV_SUPPORT
#include "preftv.h"
#endif

#if USE_ASSOCIATIONS
#include "prefassociations.h"
#endif

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

PreferencesDialog::PreferencesDialog(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f )
	, icon_mode(false)
{
	setupUi(this);
	/*
	sections->setUniformItemSizes(true);
	sections->setResizeMode(QListView::Adjust);
	*/
	sections->setMovement(QListView::Static);

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(sections->viewport());
#endif

	// Setup buttons
	okButton = buttonBox->button(QDialogButtonBox::Ok);
	cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
	applyButton = buttonBox->button(QDialogButtonBox::Apply);
	helpButton = buttonBox->button(QDialogButtonBox::Help);
	connect( applyButton, SIGNAL(clicked()), this, SLOT(apply()) );
	connect( helpButton, SIGNAL(clicked()), this, SLOT(showHelp()) );

	setWindowIcon( Images::icon("logo") );

	help_window = new InfoWindow(this);

	page_general = new PrefGeneral;
	addSection( page_general );

	page_drives = new PrefDrives;
	addSection( page_drives );

	page_performance = new PrefPerformance;
	addSection( page_performance );

	page_subtitles = new PrefSubtitles;
	addSection( page_subtitles );

	page_interface = new PrefInterface;
	addSection( page_interface );

	page_input = new PrefInput;
	addSection( page_input );

	page_playlist = new PrefPlaylist;
	addSection( page_playlist );

#ifdef TV_SUPPORT
	page_tv = new PrefTV;
	addSection( page_tv );
#endif

#if USE_ASSOCIATIONS
	page_associations = new PrefAssociations;
	addSection(page_associations);
#endif

	page_updates = new PrefUpdates;
	addSection( page_updates );

	page_network = new PrefNetwork;
	addSection( page_network );

	page_advanced = new PrefAdvanced;
	addSection( page_advanced );

	//sections->setIconSize( QSize(22,22) );
	sections->setCurrentRow(General);

	//adjustSize();
	retranslateStrings();

	/*
	qDebug() << "PreferencesDialog: movement:" << sections->movement();
	qDebug() << "PreferencesDialog: maximumWidth:" << sections->maximumWidth();
	qDebug() << "PreferencesDialog: spacing:" << sections->spacing();
	*/
}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::setIconMode(bool b) {
	qDebug() << "PreferencesDialog::setIconMode:" << b;

	if (b) {
		sections->setUniformItemSizes(true);
		sections->setViewMode(QListView::IconMode);
		sections->setMaximumWidth(128);
		sections->setSpacing(12);
	} else {
		sections->setUniformItemSizes(false);
		sections->setViewMode(QListView::ListMode);
		sections->setMaximumWidth(16777215);
		sections->setSpacing(0);
	}
}

void PreferencesDialog::showSection(Section s) {
	qDebug("PreferencesDialog::showSection: %d", s);

	sections->setCurrentRow(s);
}

void PreferencesDialog::retranslateStrings() {
	retranslateUi(this);

	for (int n=0; n < pages->count(); n++) {
		PrefWidget * w = (PrefWidget*) pages->widget(n);
		sections->item(n)->setText( w->sectionName() );
		sections->item(n)->setIcon( w->sectionIcon() );
		sections->item(n)->setToolTip( w->sectionName() );
	}

	if (help_window->isVisible()) {
		// Makes the help to retranslate
		showHelp();
	}

	help_window->setWindowTitle( tr("SMPlayer - Help") );
	help_window->setWindowIcon( Images::icon("logo") );

	// Some Qt versions don't provide translated strings
	// for these buttons
	okButton->setText( tr("&OK") );
	cancelButton->setText( tr("&Cancel") );
	applyButton->setText( tr("Apply") );
	helpButton->setText( tr("Help") );
}

void PreferencesDialog::accept() {
	hide();
	help_window->hide();
	setResult( QDialog::Accepted );
	emit applied();
}

void PreferencesDialog::apply() {
	setResult( QDialog::Accepted );
	emit applied();
}

void PreferencesDialog::reject() {
	hide();
	help_window->hide();
	setResult( QDialog::Rejected );

	setResult( QDialog::Accepted );
}

void PreferencesDialog::addSection(PrefWidget *w) {
	QListWidgetItem *i = new QListWidgetItem( w->sectionIcon(), w->sectionName() );
	sections->addItem( i );
	pages->addWidget(w);
}

void PreferencesDialog::setData(Preferences * pref) {
	page_general->setData(pref);
	page_drives->setData(pref);
	page_interface->setData(pref);
	page_performance->setData(pref);
	page_input->setData(pref);
	page_subtitles->setData(pref);
	page_advanced->setData(pref);
	page_playlist->setData(pref);
#ifdef TV_SUPPORT
	page_tv->setData(pref);
#endif
	page_updates->setData(pref);
	page_network->setData(pref);

#if USE_ASSOCIATIONS
	page_associations->setData(pref);
#endif
}

void PreferencesDialog::getData(Preferences * pref) {
	page_general->getData(pref);
	page_drives->getData(pref);
	page_interface->getData(pref);
	page_performance->getData(pref);
	page_input->getData(pref);
	page_subtitles->getData(pref);
	page_advanced->getData(pref);
	page_playlist->getData(pref);
#ifdef TV_SUPPORT
	page_tv->getData(pref);
#endif
	page_updates->getData(pref);
	page_network->getData(pref);

#if USE_ASSOCIATIONS
	page_associations->getData(pref);
#endif
}

bool PreferencesDialog::requiresRestart() {
	bool need_restart = page_general->requiresRestart();
	if (!need_restart) need_restart = page_drives->requiresRestart();
	if (!need_restart) need_restart = page_interface->requiresRestart();
	if (!need_restart) need_restart = page_performance->requiresRestart();
	if (!need_restart) need_restart = page_input->requiresRestart();
	if (!need_restart) need_restart = page_subtitles->requiresRestart();
	if (!need_restart) need_restart = page_advanced->requiresRestart();
	if (!need_restart) need_restart = page_playlist->requiresRestart();
#ifdef TV_SUPPORT
	if (!need_restart) need_restart = page_tv->requiresRestart();
#endif
	if (!need_restart) need_restart = page_updates->requiresRestart();
	if (!need_restart) need_restart = page_network->requiresRestart();

	return need_restart;
}

void PreferencesDialog::showHelp() {
	PrefWidget * w = (PrefWidget*) pages->currentWidget();
	help_window->setHtml( w->help() );
	help_window->show();
	help_window->raise();
}

// Language change stuff
void PreferencesDialog::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QDialog::changeEvent(e);
	}
}

#include "moc_preferencesdialog.cpp"
