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

#include "filepropertiesdialog.h"
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "images.h"
#include "infofile.h"
#include "playerid.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

FilePropertiesDialog::FilePropertiesDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	// Setup buttons
	okButton = buttonBox->button(QDialogButtonBox::Ok);
	cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
	applyButton = buttonBox->button(QDialogButtonBox::Apply);
	connect( applyButton, SIGNAL(clicked()), this, SLOT(apply()) );

#if ALLOW_DEMUXER_CODEC_CHANGE
	codecs_set = false;
#else
	// Hide unused tabs
	int i = tabWidget->indexOf(demuxer_page);
	if (i != -1) tabWidget->removeTab(i);
	i = tabWidget->indexOf(vc_page);
	if (i != -1) tabWidget->removeTab(i);
	i = tabWidget->indexOf(ac_page);
	if (i != -1) tabWidget->removeTab(i);
#endif

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(info_edit);
	MyScroller::setScroller(demuxer_listbox->viewport());
	MyScroller::setScroller(vc_listbox->viewport());
	MyScroller::setScroller(ac_listbox->viewport());
#endif

	retranslateStrings();
}

FilePropertiesDialog::~FilePropertiesDialog() {
}

void FilePropertiesDialog::setMediaData(MediaData md) {
	media_data = md;
	showInfo();
}

void FilePropertiesDialog::showInfo() {
	InfoFile info;
	info_edit->setText( info.getInfo(media_data) );
}

void FilePropertiesDialog::retranslateStrings() {
	retranslateUi(this);

	setWindowIcon( Images::icon("logo") );

	showInfo();

	okButton->setText( tr("&OK") );
	cancelButton->setText( tr("&Cancel") );
	applyButton->setText( tr("Apply") );

#if ALLOW_DEMUXER_CODEC_CHANGE
	int tab_idx = 4;
#else
	int tab_idx = 1;
#endif
	tabWidget->setTabText(tab_idx, tr("O&ptions for %1").arg(PLAYER_NAME) );
	groupBox->setTitle( tr("Additional Options for %1").arg(PLAYER_NAME) );
	options_info_label->setText( tr("Here you can pass extra options to %1.").arg(PLAYER_NAME) +"<br>"+
		tr("Write them separated by spaces.") + "<br>" + tr("Example:") + " -volume 50 -fps 25" );
}

void FilePropertiesDialog::accept() {
	qDebug("FilePropertiesDialog::accept");

	hide();
	setResult( QDialog::Accepted );
	emit applied();
}

void FilePropertiesDialog::apply() {
	qDebug("FilePropertiesDialog::apply");

	setResult( QDialog::Accepted );
	emit applied();
}

#if ALLOW_DEMUXER_CODEC_CHANGE
void FilePropertiesDialog::setCodecs(InfoList vc, InfoList ac, InfoList demuxer) 
{
	vclist = vc;
	aclist = ac;
	demuxerlist = demuxer;

	qSort(vclist);
	qSort(aclist);
	qSort(demuxerlist);

	vc_listbox->clear();
	ac_listbox->clear();
	demuxer_listbox->clear();

	InfoList::iterator it;

	for ( it = vclist.begin(); it != vclist.end(); ++it ) {
		vc_listbox->addItem( (*it).name() +" - "+ (*it).desc() );
	}

	for ( it = aclist.begin(); it != aclist.end(); ++it ) {
		ac_listbox->addItem( (*it).name() +" - "+ (*it).desc() );
	}

	for ( it = demuxerlist.begin(); it != demuxerlist.end(); ++it ) {
		demuxer_listbox->addItem( (*it).name() +" - "+ (*it).desc() );
	}

	codecs_set = true;
}

void FilePropertiesDialog::setDemuxer(QString demuxer, QString original_demuxer) {
	qDebug("FilePropertiesDialog::setDemuxer");
	if (!original_demuxer.isEmpty()) orig_demuxer = original_demuxer;
	int pos = find(demuxer, demuxerlist );
	if (pos != -1) demuxer_listbox->setCurrentRow(pos);

	qDebug(" * demuxer: '%s', pos: %d", demuxer.toUtf8().data(), pos );
}

QString FilePropertiesDialog::demuxer() {
	int pos = demuxer_listbox->currentRow();
	if ( pos < 0 )
		return "";
	else
		return demuxerlist[pos].name();
}

void FilePropertiesDialog::setVideoCodec(QString vc, QString original_vc) {
	qDebug("FilePropertiesDialog::setVideoCodec");
	if (!original_vc.isEmpty()) orig_vc = original_vc;
	int pos = find(vc, vclist );
	if (pos != -1) vc_listbox->setCurrentRow(pos);

	qDebug(" * vc: '%s', pos: %d", vc.toUtf8().data(), pos );
}

QString FilePropertiesDialog::videoCodec() {
	int pos = vc_listbox->currentRow();
	if ( pos < 0 )
		return "";
	else
		return vclist[pos].name();
}

void FilePropertiesDialog::setAudioCodec(QString ac, QString original_ac) {
	qDebug("FilePropertiesDialog::setAudioCodec");
	if (!original_ac.isEmpty()) orig_ac = original_ac;
	int pos = find(ac, aclist );
	if (pos != -1) ac_listbox->setCurrentRow(pos);

	qDebug(" * ac: '%s', pos: %d", ac.toUtf8().data(), pos );
}

QString FilePropertiesDialog::audioCodec() {
	int pos = ac_listbox->currentRow();
	if ( pos < 0 )
		return "";
	else
		return aclist[pos].name();
}

void FilePropertiesDialog::on_resetDemuxerButton_clicked() {
	setDemuxer( orig_demuxer );
}

void FilePropertiesDialog::on_resetACButton_clicked() {
	setAudioCodec( orig_ac );
}

void FilePropertiesDialog::on_resetVCButton_clicked() {
	setVideoCodec( orig_vc );
}

int FilePropertiesDialog::find(QString s, InfoList &list) {
	qDebug("FilePropertiesDialog::find");

	int n=0;
	InfoList::iterator it;

	for ( it = list.begin(); it != list.end(); ++it ) {
		//qDebug(" * item: '%s', s: '%s'", (*it).name().toUtf8().data(), s.toUtf8().data());
		if ((*it).name() == s) return n;
		n++;
	}
	return -1;
}
#endif

void FilePropertiesDialog::setMplayerAdditionalArguments(QString args) {
	mplayer_args_edit->setText(args);
}

QString FilePropertiesDialog::mplayerAdditionalArguments() {
	return mplayer_args_edit->text();
}

void FilePropertiesDialog::setMplayerAdditionalVideoFilters(QString s) {
	mplayer_vfilters_edit->setText(s);
}

QString FilePropertiesDialog::mplayerAdditionalVideoFilters() {
	return mplayer_vfilters_edit->text();
}

void FilePropertiesDialog::setMplayerAdditionalAudioFilters(QString s) {
	mplayer_afilters_edit->setText(s);
}

QString FilePropertiesDialog::mplayerAdditionalAudioFilters() {
	return mplayer_afilters_edit->text();
}

// Language change stuff
void FilePropertiesDialog::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QDialog::changeEvent(e);
	}
}

#include "moc_filepropertiesdialog.cpp"
