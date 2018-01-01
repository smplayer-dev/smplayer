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

#include "fontcache.h"
#include <QProcess>
#include <QApplication>
#include <QDir>

FontCacheDialog::FontCacheDialog(QWidget * parent, Qt::WindowFlags f)
	: QProgressDialog(parent, f)
{
	setWindowTitle(tr("SMPlayer is initializing"));
	setLabelText(tr("Creating a font cache..."));

	process = new QProcess(this);
	process->setProcessChannelMode( QProcess::MergedChannels );
	//connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));

	connect(this, SIGNAL(canceled()), process, SLOT(kill()));
}

FontCacheDialog::~FontCacheDialog() 
{
}

void FontCacheDialog::run(QString mplayer_bin, QString file) {
	qDebug("FontCacheDialog::run: mplayer_bin: '%s', file: '%s'", mplayer_bin.toUtf8().constData(), file.toUtf8().constData());

	QRegExp rx_scanning_font("Scanning file");

	int max = 100;
	QDir fon_dir("c:/windows/fonts");
	QStringList fon_files = fon_dir.entryList(QStringList() << "*.*", QDir::Files);
	qDebug("FontCacheDialog::run: number of fonts: %d", fon_files.count());
	if (fon_files.count() > 2) max = fon_files.count();
	setMaximum(max);

	QStringList arg;
	arg << "-fontconfig" << "-ass" << "-vo" << "null" << "-ao" << "null";
	arg << file;

	process->start(mplayer_bin, arg);
	if (!process->waitForStarted()) {
		qDebug("FontCacheDialog::run: failed to start process");
		return;
	}

	int fonts = 0;
	int v = 0;
	QByteArray line;
	while (process->state() == QProcess::Running) {
		qApp->processEvents();
		if (process->waitForReadyRead(100)) {
			line = process->readLine().trimmed();
			qDebug("FontCacheDialog::run: line: %s", line.constData());
			if (rx_scanning_font.indexIn(line) > -1) {
				fonts++;
				v++;
				if (v >= (max-1)) v = 0;
				setValue(v);
			}
		}
	}
	qDebug("FontCacheDialog::run: %d fonts found", fonts);
}

/*
void FontCacheDialog::readOutput() {
	int v = 0;
	QByteArray line;
	while (process->canReadLine()) {
		line = process->readLine().trimmed();
		qDebug("line: %s", line.constData());
		v++;
		setValue(v);
	}
}
*/

#include "moc_fontcache.cpp"

