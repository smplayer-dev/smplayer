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

#include "findsubtitlesdialog.h"
#include "simplehttp.h"
#include "osparser.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#define COL_LANG 0
#define COL_NAME 1
#define COL_FORMAT 2
#define COL_FILES 3
#define COL_DATE 4
#define COL_USER 5

FindSubtitlesDialog::FindSubtitlesDialog( QWidget * parent, Qt::WindowFlags f )
	: QDialog(parent,f)
{
	setupUi(this);

	progress->hide();

	connect( file_chooser, SIGNAL(fileChanged(QString)),
             this, SLOT(setMovie(QString)) );
	connect( file_chooser->lineEdit(), SIGNAL(textChanged(const QString &)),
             this, SLOT(updateRefreshButton()) );

	connect( refresh_button, SIGNAL(clicked()),
             this, SLOT(refresh()) );

	connect( language_filter, SIGNAL(editTextChanged(const QString &)),
             this, SLOT(applyFilter(const QString &)) );

	table = new QStandardItemModel(this);
	table->setColumnCount(COL_USER + 1);

	proxy_model = new QSortFilterProxyModel(this);
	proxy_model->setSourceModel(table);
	proxy_model->setFilterKeyColumn(COL_LANG);
	proxy_model->setFilterRole(Qt::UserRole);

	view->setModel(proxy_model);
	view->setRootIsDecorated(false);
	view->setSortingEnabled(true);
	view->setAlternatingRowColors(true);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(view, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(itemActivated(const QModelIndex &)) );
	connect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex &,const QModelIndex &)),
            this, SLOT(currentItemChanged(const QModelIndex &,const QModelIndex &)) );

	downloader = new SimpleHttp(this);

	connect( downloader, SIGNAL(downloadFailed(QString)),
             this, SLOT(showError(QString)) );
	connect( downloader, SIGNAL(downloadFinished(QByteArray)), 
             this, SLOT(downloadFinished()) );
	connect( downloader, SIGNAL(downloadFinished(QByteArray)), 
             this, SLOT(parseInfo(QByteArray)) );
	connect( downloader, SIGNAL(stateChanged(int)),
             this, SLOT(updateRefreshButton()) );

	connect( downloader, SIGNAL(connecting(QString)),
             this, SLOT(connecting(QString)) );
	connect( downloader, SIGNAL(dataReadProgress(int, int)),
             this, SLOT(updateDataReadProgress(int, int)) );

	retranslateStrings();
}

FindSubtitlesDialog::~FindSubtitlesDialog() {
}

void FindSubtitlesDialog::retranslateStrings() {
	retranslateUi(this);

	QStringList labels;
	labels << tr("Language") << tr("Name") << tr("Format") 
           << tr("Files") << tr("Date") << tr("Uploaded by");

	table->setHorizontalHeaderLabels( labels );

#if QT_VERSION < 0x040300
	QPushButton * close_button = buttonBox->button(QDialogButtonBox::Close);
	close_button->setText( tr("Close") );
#endif
}

void FindSubtitlesDialog::setMovie(QString filename) {
	qDebug("FindSubtitlesDialog::setMovie: '%s'", filename.toLatin1().constData());

	file_chooser->setText(filename);
	table->setRowCount(0);

	QString hash = OSParser::calculateHash(filename);
	if (hash.isEmpty()) {
		qWarning("FindSubtitlesDialog::setMovie: hash invalid. Doing nothing.");
	} else {
		QString link = "http://www.opensubtitles.org/search/sublanguageid-all/moviehash-" + hash + "/simplexml";
		qDebug("FindSubtitlesDialog::setMovie: link: '%s'", link.toLatin1().constData());
		downloader->download(link);
	}
}

void FindSubtitlesDialog::refresh() {
	setMovie(file_chooser->text());
}

void FindSubtitlesDialog::updateRefreshButton() {
	QString file = file_chooser->lineEdit()->text();
	bool enabled = ( (!file.isEmpty()) && (QFile::exists(file)) && 
                     (downloader->state()==QHttp::Unconnected) );
	refresh_button->setEnabled(enabled);
}

void FindSubtitlesDialog::currentItemChanged(const QModelIndex & current, const QModelIndex & /*previous*/) {
	qDebug("FindSubtitlesDialog::currentItemChanged: row: %d, col: %d", current.row(), current.column());
	download_button->setEnabled(current.isValid());
}

void FindSubtitlesDialog::applyFilter(const QString & filter) {
	proxy_model->setFilterWildcard(filter);
}

void FindSubtitlesDialog::applyCurrentFilter() {
	proxy_model->setFilterWildcard(language_filter->currentText());
}

void FindSubtitlesDialog::showError(QString error) {
	QMessageBox::information(this, tr("Error"),
                             tr("Download failed: %1.")
                             .arg(error));
}

void FindSubtitlesDialog::connecting(QString host) {
	status->setText( tr("Connecting to %1...").arg(host) );
}

void FindSubtitlesDialog::updateDataReadProgress(int done, int total) {
	qDebug("FindSubtitlesDialog::updateDataReadProgress: %d, %d", done, total);

	status->setText( tr("Downloading...") );

	if (!progress->isVisible()) progress->show();
	progress->setMaximum(total);
	progress->setValue(done);
}

void FindSubtitlesDialog::downloadFinished() {
	status->setText( tr("Done.") );
	progress->setMaximum(1);
	progress->setValue(0);
	progress->hide();
}

void FindSubtitlesDialog::parseInfo(QByteArray xml_text) {
	OSParser osparser;
	bool ok = osparser.parseXml(xml_text);

	table->setRowCount(0);

	if (ok) {
		QList<OSSubtitle> l = osparser.subtitleList();
		for (int n=0; n < l.count(); n++) {

			QString title_name = l[n].movie;
			if (!l[n].releasename.isEmpty()) {
				title_name += " - " + l[n].releasename;
			}

			QStandardItem * i_name = new QStandardItem(title_name);
			i_name->setData( l[n].link );
			i_name->setToolTip( l[n].link );

			QStandardItem * i_lang = new QStandardItem(l[n].language);
			i_lang->setData(l[n].iso639, Qt::UserRole);
			i_lang->setToolTip(l[n].iso639);

			table->setItem(n, COL_LANG, i_lang);
			table->setItem(n, COL_NAME, i_name);
			table->setItem(n, COL_FORMAT, new QStandardItem(l[n].format));
			table->setItem(n, COL_FILES, new QStandardItem(l[n].files));
			table->setItem(n, COL_DATE, new QStandardItem(l[n].date));
			table->setItem(n, COL_USER, new QStandardItem(l[n].user));

		}
		status->setText( tr("%1 files available").arg(l.count()) );
		applyCurrentFilter();

		// For some reason sorting doesn't work
		/*
		qDebug("sort column: %d", view->header()->sortIndicatorSection());
		qDebug("sort indicator: %d", view->header()->sortIndicatorOrder());
		
		view->sortByColumn( view->header()->sortIndicatorSection(),
                            view->header()->sortIndicatorOrder() );
		*/
	} else {
		status->setText( tr("Failed to parse the received data.") );
	}

	view->resizeColumnToContents(COL_NAME);
}

void FindSubtitlesDialog::itemActivated(const QModelIndex & index ) {
	qDebug("FindSubtitlesDialog::itemActivated: row: %d, col %d", proxy_model->mapToSource(index).row(), proxy_model->mapToSource(index).column());

	QString download_link = table->item(proxy_model->mapToSource(index).row(), COL_NAME)->data().toString();

	qDebug("FindSubtitlesDialog::itemActivated: download link: '%s'", download_link.toLatin1().constData());

	QDesktopServices::openUrl( QUrl(download_link) );
}

void FindSubtitlesDialog::on_download_button_clicked() {
	qDebug("FindSubtitlesDialog::on_download_button_clicked");
	if (view->currentIndex().isValid()) {
		itemActivated(view->currentIndex());
	}
}

// Language change stuff
void FindSubtitlesDialog::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QDialog::changeEvent(e);
	}
}

#include "moc_findsubtitlesdialog.cpp"

