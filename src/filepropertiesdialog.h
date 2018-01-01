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

#ifndef FILEPROPERTIESDIALOG_H
#define FILEPROPERTIESDIALOG_H

#include "ui_filepropertiesdialog.h"
#include "inforeader.h"
#include "mediadata.h"
#include "config.h"

class QPushButton;

class FilePropertiesDialog : public QDialog, public Ui::FilePropertiesDialog
{
	Q_OBJECT

public:
    FilePropertiesDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~FilePropertiesDialog();

	void setMediaData(MediaData md);

#if ALLOW_DEMUXER_CODEC_CHANGE
	// Call it as soon as possible
	void setCodecs(InfoList vc, InfoList ac, InfoList demuxer);

	void setDemuxer(QString demuxer, QString original_demuxer="");
	QString demuxer();

	void setVideoCodec(QString vc, QString original_vc="");
	QString videoCodec();

	void setAudioCodec(QString ac, QString original_ac="");
	QString audioCodec();
#endif

	void setMplayerAdditionalArguments(QString args);
	QString mplayerAdditionalArguments();

	void setMplayerAdditionalVideoFilters(QString s);
	QString mplayerAdditionalVideoFilters();

	void setMplayerAdditionalAudioFilters(QString s);
	QString mplayerAdditionalAudioFilters();

public slots:
	void accept(); // Reimplemented to send a signal
	void apply();

signals:
	void applied();

#if ALLOW_DEMUXER_CODEC_CHANGE
protected slots:
	virtual void on_resetDemuxerButton_clicked();
	virtual void on_resetACButton_clicked();
	virtual void on_resetVCButton_clicked();
#endif

protected:
#if ALLOW_DEMUXER_CODEC_CHANGE
	bool hasCodecsList() { return codecs_set; };
	int find(QString s, InfoList &list);
#endif
	void showInfo();

protected:
	virtual void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;

private:
#if ALLOW_DEMUXER_CODEC_CHANGE
	bool codecs_set;
	InfoList vclist, aclist, demuxerlist;
	QString orig_demuxer, orig_ac, orig_vc;
#endif
	MediaData media_data;

	QPushButton * okButton;
	QPushButton * cancelButton;
	QPushButton * applyButton;
};

#endif
