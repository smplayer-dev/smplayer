/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#include "findsubtitleswindow.h"
#include "findsubtitlesconfigdialog.h"

#include "osclient.h"
#include "filehash.h"
#include "languages.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QSettings>

#ifdef DOWNLOAD_SUBS
#include <QBuffer>
#include "filedownloader.h"
#include "subchooserdialog.h"
#include "fixsubs.h"
#include <zlib.h>
#endif

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

//#define NO_SMPLAYER_SUPPORT

#ifndef NO_SMPLAYER_SUPPORT
#include "images.h"
#include "filedialog.h"
#endif

#define COL_LANG 0
#define COL_NAME 1
#define COL_DATE 2
#define COL_USER 3

#define DATA_LINK    Qt::UserRole + 1
#define DATA_FILE_ID DATA_LINK + 1

FindSubtitlesWindow::FindSubtitlesWindow( QWidget * parent, Qt::WindowFlags f )
	: QWidget(parent,f)
{
	setupUi(this);

	set = 0; // settings

	subtitles_for_label->setBuddy(file_chooser);

	progress->hide();

	connect( file_chooser, SIGNAL(fileChanged(QString)),
             this, SLOT(setMovie(QString)) );
	connect( file_chooser, SIGNAL(textChanged(const QString &)),
             this, SLOT(updateRefreshButton()) );

	connect( search_edit, SIGNAL(returnPressed()), this, SLOT(searchTitle()) );
	connect( search_title_button, SIGNAL(clicked()), this, SLOT(searchTitle()) );

	connect( refresh_button, SIGNAL(clicked()),
             this, SLOT(refresh()) );

	connect( download_button, SIGNAL(clicked()),
             this, SLOT(download()) );

	/*
	connect( language_filter, SIGNAL(editTextChanged(const QString &)),
             this, SLOT(applyFilter(const QString &)) );
	*/
	connect( language_filter, SIGNAL(activated(int)),
             this, SLOT(applyCurrentFilter()) );

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
	view->header()->setSortIndicator(COL_LANG, Qt::AscendingOrder);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	view->setContextMenuPolicy( Qt::CustomContextMenu );

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(view->viewport());
#endif

	connect(view, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(itemActivated(const QModelIndex &)) );
	connect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex &,const QModelIndex &)),
            this, SLOT(currentItemChanged(const QModelIndex &,const QModelIndex &)) );

	connect(view, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)) );

	/*
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
             this, SLOT(showConnecting(QString)) );
	connect( downloader, SIGNAL(dataReadProgress(int, int)),
             this, SLOT(updateDataReadProgress(int, int)) );
	*/

	osclient = new OSClient();
	connect( osclient, SIGNAL(searchFinished()), this, SLOT(downloadFinished()) );
	connect( osclient, SIGNAL(searchFinished()), this, SLOT(parseInfo()) );
	connect( osclient, SIGNAL(loginFailed()), this, SLOT(showLoginFailed()) );
	connect( osclient, SIGNAL(searchFailed()), this, SLOT(showSearchFailed()) );
	connect( osclient, SIGNAL(getDownloadLinkFailed()), this, SLOT(showDownloadFailed()) );
	connect( osclient, SIGNAL(errorFound(int, const QString &)), this, SLOT(showErrorOS(int, const QString &)) );
	connect( osclient, SIGNAL(connecting()), this, SLOT(showConnecting()) );

#ifdef DOWNLOAD_SUBS
	include_lang_on_filename = false;

	file_downloader = new FileDownloader(this);
	file_downloader->setModal(false);
	file_downloader->hide();

	connect( file_downloader, SIGNAL(downloadFailed(QString)),
             this, SLOT(showError(QString)), Qt::QueuedConnection );
	connect( file_downloader, SIGNAL(downloadFinished(const QByteArray &)),
             this, SLOT(archiveDownloaded(const QByteArray &)), Qt::QueuedConnection );
	connect( this, SIGNAL(subtitleDownloaded(const QString &)),
             this, SLOT(fixSubtitles(const QString &)) );
#endif

	// Actions
	downloadAct = new QAction(this);
	downloadAct->setEnabled(false);
	connect( downloadAct, SIGNAL(triggered()), this, SLOT(download()) );

	copyLinkAct = new QAction(this);
	copyLinkAct->setEnabled(false);
	connect( copyLinkAct, SIGNAL(triggered()), this, SLOT(copyLink()) );

	context_menu = new QMenu(this);
	context_menu->addAction(downloadAct);
	context_menu->addAction(copyLinkAct);

	retranslateStrings();

	language_filter->setCurrentIndex(0);

#ifdef FS_USE_PROXY
	// Proxy
	use_proxy = false;
	proxy_type = QNetworkProxy::HttpProxy;
	proxy_host = "";
	proxy_port = 0;
	proxy_username = "";
	proxy_password = "";

	setupProxy();
#endif
}

FindSubtitlesWindow::~FindSubtitlesWindow() {
	if (set) saveSettings();
}

void FindSubtitlesWindow::setSettings(QSettings * settings) {
	set = settings;
	loadSettings();
#ifdef FS_USE_PROXY
	setupProxy();
#endif
}

#ifdef FS_USE_PROXY
void FindSubtitlesWindow::setProxy(QNetworkProxy proxy) {
	/*
	downloader->abort();
	downloader->setProxy(proxy);
	*/
	osclient->setProxy(proxy);

#ifdef DOWNLOAD_SUBS
	file_downloader->setProxy(proxy);
#endif

	qDebug("FindSubtitlesWindow::setProxy: host: '%s' port: %d type: %d",
           proxy.hostName().toUtf8().constData(), proxy.port(), proxy.type());
}
#endif

void FindSubtitlesWindow::retranslateStrings() {
	retranslateUi(this);

	QStringList labels;
	labels << tr("Language") << tr("Name") << tr("Date") << tr("Uploaded by");

	table->setHorizontalHeaderLabels( labels );

	// Language combobox
	//int language_index = language_filter->currentIndex();
	QString current_language = language_filter->itemData(language_filter->currentIndex()).toString();
	language_filter->clear();

	QMap<QString,QString> l1 = Languages::most_used_list();
	QMapIterator<QString, QString> i1(l1);
	while (i1.hasNext()) {
		i1.next();
		if (i1.key() == "es") {
			language_filter->addItem( tr("Spanish") + " (es,sp,ea)", "ea,es,sp");
		}
		else
		if (i1.key() == "pt") {
			language_filter->addItem( tr("Portuguese") + " (pt-pt,pt-br,pm)", "pm,pt-pb,pt-pt");
		}
		else {
			language_filter->addItem( i1.value() + " (" + i1.key() + ")", i1.key() );
		}
	}

	language_filter->model()->sort(0);
	#if QT_VERSION >= 0x040400
	language_filter->insertSeparator(language_filter->count());
	#endif

	QMap<QString,QString> l2 = Languages::os_languages();
	foreach (QString key, l2.keys()) {
		QString lang = l2.value(key);
		language_filter->addItem( lang + " (" + key + ")", key );
	}

	//language_filter->model()->sort(0);
	language_filter->insertItem( 0, tr("All"), "" );
	#if QT_VERSION >= 0x040400
	language_filter->insertSeparator(1);
	#endif
	//language_filter->setCurrentIndex(language_index);
	language_filter->setCurrentIndex(language_filter->findData(current_language));

#if QT_VERSION < 0x040300
	QPushButton * close_button = buttonBox->button(QDialogButtonBox::Close);
	close_button->setText( tr("Close") );
#endif

	// Actions
	downloadAct->setText( tr("&Download") );
	copyLinkAct->setText( tr("&Copy link to clipboard") );

	// Icons
#ifdef NO_SMPLAYER_SUPPORT
	download_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
	refresh_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
	configure_button->setIcon(QIcon::fromTheme("preferences-other"));
	downloadAct->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
	copyLinkAct->setIcon(QIcon::fromTheme("edit-copy"));
#else
	download_button->setIcon( Images::icon("download") );
	configure_button->setIcon( Images::icon("prefs") );
	refresh_button->setIcon( Images::icon("refresh") );

	downloadAct->setIcon( Images::icon("download") );
	copyLinkAct->setIcon( Images::icon("copy") );
#endif

	credits_label->setText("<i>"+ tr("Subtitles service powered by %1")
                           .arg("<a href=\"http://www.opensubtitles.com\">www.OpenSubtitles.com</a>") + "</i>");
}

void FindSubtitlesWindow::setMovie(QString filename) {
	qDebug() << "FindSubtitlesWindow::setMovie:" << filename;

	if (filename == last_file) {
		return;
	}

	file_chooser->setText(filename);
	table->setRowCount(0);

	QString hash = FileHash::calculateHash(filename);
	if (hash.isEmpty()) {
		qWarning("FindSubtitlesWindow::setMovie: hash invalid. Doing nothing.");
	} else {
		QString lang = language_filter->itemData(language_filter->currentIndex()).toString();
		QFileInfo fi(filename);
		qint64 file_size = fi.size();
		QString basename;
		basename = fi.completeBaseName(); // Filename without extension
		search_edit->setText(basename);
		osclient->search(hash, file_size, basename, lang);
		last_file = filename;
	}
}

void FindSubtitlesWindow::searchTitle() {
	QString t = search_edit->text();
	qDebug() << "FindSubtitlesWindow::searchTitle:" << t;

	if (osclient->searchMethod() != OSClient::Hash) {
		QString lang = language_filter->itemData(language_filter->currentIndex()).toString();
		osclient->search("", 0, t, lang);
	}
}

void FindSubtitlesWindow::updateSearchTitleWidget() {
	bool title_search_allowed = (osclient->searchMethod() != OSClient::Hash);
	search_for_label->setEnabled(title_search_allowed);
	search_edit->setEnabled(title_search_allowed);
	search_title_button->setEnabled(title_search_allowed);
}

void FindSubtitlesWindow::refresh() {
	last_file = "";
	setMovie(file_chooser->text());
}

void FindSubtitlesWindow::updateRefreshButton() {
	qDebug("FindSubtitlesWindow::updateRefreshButton:");
	refresh_button->setEnabled(true);
}

void FindSubtitlesWindow::currentItemChanged(const QModelIndex & current, const QModelIndex & /*previous*/) {
	qDebug("FindSubtitlesWindow::currentItemChanged: row: %d, col: %d", current.row(), current.column());
	download_button->setEnabled(current.isValid());
	downloadAct->setEnabled(current.isValid());
	copyLinkAct->setEnabled(current.isValid());
}

void FindSubtitlesWindow::applyFilter(const QString & filter) {
#if QT_VERSION_MAJOR < 6
	proxy_model->setFilterRegExp(filter);
#else
    proxy_model->setFilterRegularExpression(filter);
#endif
}

void FindSubtitlesWindow::applyCurrentFilter() {
	#if 0
	//proxy_model->setFilterWildcard(language_filter->currentText());
	QString filter = language_filter->itemData( language_filter->currentIndex() ).toString();
	applyFilter(filter);
	#endif
}

void FindSubtitlesWindow::setLanguage(const QString & lang) {
	int idx = language_filter->findData(lang);
	if (idx < 0) idx = 0;
	language_filter->setCurrentIndex(idx);
}

QString FindSubtitlesWindow::language() {
	int idx = language_filter->currentIndex();
	return language_filter->itemData(idx).toString();
}

void FindSubtitlesWindow::showError(QString error) {
	status->setText( tr("Download failed") );

	QMessageBox::information(this, tr("Error"),
                             tr("Download failed: %1.")
                             .arg(error));
}

void FindSubtitlesWindow::showConnecting() {
	status->setText(tr("Connecting..."));
}

/*
void FindSubtitlesWindow::showConnecting(QString host) {
	status->setText( tr("Connecting to %1...").arg(host) );
}
*/

void FindSubtitlesWindow::showLoginFailed() {
	status->setText( tr("Login to opensubtitles.org has failed") );
}

void FindSubtitlesWindow::showSearchFailed() {
	status->setText( tr("Search has failed") );
}

void FindSubtitlesWindow::showDownloadFailed() {
	status->setText( tr("File URL not found") );
}

void FindSubtitlesWindow::showErrorOS(int, const QString & error) {
	status->setText(error);
	if (error.contains("299:")) {
		status->setText(tr("Error: daily quota exceeded"));
	}
}

void FindSubtitlesWindow::updateDataReadProgress(int done, int total) {
	qDebug("FindSubtitlesWindow::updateDataReadProgress: %d, %d", done, total);

	status->setText( tr("Downloading...") );

	if (!progress->isVisible()) progress->show();
	progress->setMaximum(total);
	progress->setValue(done);
}

void FindSubtitlesWindow::downloadFinished() {
	status->setText( tr("Done.") );
	progress->setMaximum(1);
	progress->setValue(0);
	progress->hide();
}

void FindSubtitlesWindow::parseInfo() {
	bool ok = true;

	table->setRowCount(0);

	QMap <QString,QString> language_list = Languages::list();

	if (ok) {
		QList<OSSubtitle> l = osclient->subtitleList();
		for (int n=0; n < l.count(); n++) {

			QString title_name = l[n].movie;
			if (!l[n].releasename.isEmpty()) {
				title_name += " - " + l[n].releasename;
			}

			QStandardItem * i_name = new QStandardItem(title_name);
			i_name->setData( l[n].link );
			i_name->setData( l[n].file_id, DATA_FILE_ID );
			if (!l[n].comments.isEmpty()) i_name->setToolTip( l[n].comments );
			#if QT_VERSION < 0x040400
			i_name->setToolTip( l[n].link );
			#endif

			QStandardItem * i_lang = new QStandardItem(l[n].language);
			i_lang->setData(l[n].iso639, Qt::UserRole);
			#if QT_VERSION < 0x040400
			i_lang->setToolTip(l[n].iso639);
			#endif
			if (language_list.contains(l[n].iso639)) {
				i_lang->setText( language_list[ l[n].iso639 ] );
			}

			table->setItem(n, COL_LANG, i_lang);
			table->setItem(n, COL_NAME, i_name);
			QString l_date = l[n].date.replace("T", " ").replace("Z", "");
			table->setItem(n, COL_DATE, new QStandardItem(l_date));
			table->setItem(n, COL_USER, new QStandardItem(l[n].user));

		}
		status->setText( tr("%1 files available").arg(l.count()) );
		applyCurrentFilter();

		qDebug("FindSubtitlesWindow::parseInfo: sort column: %d", view->header()->sortIndicatorSection());
		qDebug("FindSubtitlesWindow::parseInfo: sort indicator: %d", view->header()->sortIndicatorOrder());

		table->sort( view->header()->sortIndicatorSection(),
                     view->header()->sortIndicatorOrder() );
	} else {
		status->setText( tr("Failed to parse the received data.") );
	}

	view->resizeColumnToContents(COL_NAME);
}

void FindSubtitlesWindow::itemActivated(const QModelIndex & index ) {
	qDebug("FindSubtitlesWindow::itemActivated: row: %d, col %d", proxy_model->mapToSource(index).row(), proxy_model->mapToSource(index).column());

	QStandardItem * item = table->item(proxy_model->mapToSource(index).row(), COL_NAME);
	QString download_link = item->data().toString();
	QString file_id = item->data(DATA_FILE_ID).toString();
	qDebug() << "FindSubtitlesWindow::itemActivated: file_id:" << file_id;

	int remaining_downloads;
	if (download_link.isEmpty()) {
		download_link = osclient->getDownloadLink(file_id, &remaining_downloads);
		item->setData(download_link);
	}
	qDebug() << "FindSubtitlesWindow::itemActivated: download link:" << download_link;

	if (download_link.isEmpty()) {
		// Failed to get the download link
		return;
	}

#ifdef DOWNLOAD_SUBS
	file_downloader->download( QUrl(download_link) );
	file_downloader->show();
#else
	QDesktopServices::openUrl( QUrl(download_link) );
#endif
	status->setText(tr("Remaining downloads: %1").arg(remaining_downloads));
}

void FindSubtitlesWindow::download() {
	qDebug("FindSubtitlesWindow::download");
	if (view->currentIndex().isValid()) {
		itemActivated(view->currentIndex());
	}
}

void FindSubtitlesWindow::copyLink() {
	qDebug("FindSubtitlesWindow::copyLink");
	if (view->currentIndex().isValid()) {
		const QModelIndex & index = view->currentIndex();
		QStandardItem * item = table->item(proxy_model->mapToSource(index).row(), COL_NAME);
		QString download_link = item->data().toString();
		QString file_id = item->data(DATA_FILE_ID).toString();
		qDebug() << "FindSubtitlesWindow::copyLink: file_id:" << file_id;

		int remaining_downloads;
		if (download_link.isEmpty()) {
			download_link = osclient->getDownloadLink(file_id, &remaining_downloads);
			item->setData(download_link);
		}
		qDebug() << "FindSubtitlesWindow::copyLink: link:" << download_link;
		if (!download_link.isEmpty()) {
			qApp->clipboard()->setText(download_link);
			status->setText(tr("Remaining downloads: %1").arg(remaining_downloads));
		}
	}
}

void FindSubtitlesWindow::showContextMenu(const QPoint & pos) {
	qDebug("FindSubtitlesWindow::showContextMenu");

	context_menu->move( view->viewport()->mapToGlobal(pos) );
	context_menu->show();
}

// Language change stuff
void FindSubtitlesWindow::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#ifdef DOWNLOAD_SUBS

void FindSubtitlesWindow::archiveDownloaded(const QByteArray & buffer) {
	qDebug("FindSubtitlesWindow::archiveDownloaded");

	#if 0
	QByteArray uncompress_data = gUncompress(buffer);
	//qDebug("uncompress_data: %s", uncompress_data.constData());

	if (uncompress_data.isEmpty()) {
		status->setText(tr("Download failed"));
		return;
	}
	#endif

	QString lang = "unknown";
	QString extension = "unknown";
	if (view->currentIndex().isValid()) {
		const QModelIndex & index = view->currentIndex();
		lang = table->item(proxy_model->mapToSource(index).row(), COL_LANG)->data(Qt::UserRole).toString();
		extension = "srt";
	}

	QString output_file;

	if (!file_chooser->text().isEmpty() && QFile::exists(file_chooser->text())) {
		QFileInfo fi(file_chooser->text());
		QString output_name = fi.completeBaseName();
		if (include_lang_on_filename) output_name += "."+ lang;
		output_name += "." + extension;

		output_file = fi.absolutePath() + "/" + output_name;
		qDebug() << "FindSubtitlesWindow::archiveDownloaded: save subtitle as" << output_file;
	}

	if (output_file.isEmpty()) {
		QString output_name = "subtitle";
		if (!search_edit->text().isEmpty()) output_name = search_edit->text();
		if (include_lang_on_filename) output_name += "_"+ lang;
		output_name += "." + extension;
		#ifndef NO_SMPLAYER_SUPPORT
		output_file = MyFileDialog::getSaveFileName(this, tr("Save File"), output_name);
		#else
		output_file = QFileDialog::getSaveFileName(this, tr("Save File"), output_name);
		#endif
	}

	if (!output_file.isEmpty()) {
		QFile file(output_file);
		file.open(QIODevice::WriteOnly);
		#if 0
		bool error = (file.write(uncompress_data) == -1);
		#else
		bool error = (file.write(buffer) == -1);
		#endif
		file.close();

		if (error) {
			qWarning("FindSubtitlesWindow::archiveDownloaded: can't write subtitle file");
			QMessageBox::warning(this, tr("Error saving file"),
                             tr("It wasn't possible to save the downloaded\n"
                                "file in folder %1\n"
                                "Please check the permissions of that folder.").arg(QFileInfo(output_file).absolutePath()));
		} else {
			status->setText(tr("Subtitle saved as %1").arg(output_file));
			emit subtitleDownloaded( output_file );
		}
	}
}

QByteArray FindSubtitlesWindow::gUncompress(const QByteArray &data)
{
    if (data.size() <= 4) {
        qWarning("gUncompress: Input data is truncated");
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
            /* FALLTHROUGH */
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

void FindSubtitlesWindow::fixSubtitles(const QString & filename) {
	qDebug("FindSubtitlesWindow::fixSubtitles: %s", filename.toUtf8().constData());

	QFileInfo fi(filename);
	if (fi.suffix().toLower() == "sub") {
		qDebug("FindSubtitlesWindow::fixSubtitles: fixing end of lines");
		if (FixSubtitles::fix(filename) != FixSubtitles::NoError) {
			status->setText( tr("Error fixing the subtitle lines") );
			qDebug("FindSubtitlesWindow::fixSubtitles: error fixing the subtitles");
		}
	}
}

#endif // DOWNLOAD_SUBS

void FindSubtitlesWindow::on_configure_button_clicked() {
	qDebug("FindSubtitlesWindow::on_configure_button_clicked");

	FindSubtitlesConfigDialog d(this);

	#ifdef FS_USE_SERVER_CONFIG
	d.setServer( os_server );
	#endif
	d.setSearchMethod(osclient->searchMethod());
	d.setUsername(osclient->username());
	d.setPassword(osclient->password());
	#ifdef OS_SEARCH_WORKAROUND
	d.setRetries(osclient->retries());
	#endif
	#ifdef FS_USE_PROXY
	d.setUseProxy( use_proxy );
	d.setProxyHostname( proxy_host );
	d.setProxyPort( proxy_port );
	d.setProxyUsername( proxy_username );
	d.setProxyPassword( proxy_password );
	d.setProxyType( proxy_type );
	#endif

	#ifdef DOWNLOAD_SUBS
	d.setAppendLang(include_lang_on_filename);
	#endif

	if (d.exec() == QDialog::Accepted) {
		#ifdef FS_USE_SERVER_CONFIG
		os_server = d.server();
		#endif
		osclient->setSearchMethod( (OSClient::SearchMethod) d.searchMethod() );
		osclient->setUsername(d.username());
		osclient->setPassword(d.password());
		#ifdef OS_SEARCH_WORKAROUND
		osclient->setRetries( d.retries() );
		#endif
		#ifdef FS_USE_PROXY
		use_proxy = d.useProxy();
		proxy_host = d.proxyHostname();
		proxy_port = d.proxyPort();
		proxy_username = d.proxyUsername();
		proxy_password = d.proxyPassword();
		proxy_type = d.proxyType();
		#endif

		#ifdef FS_USE_SERVER_CONFIG
		osclient->setServer(os_server);
		#endif

		#ifdef FS_USE_PROXY
		setupProxy();
		#endif

		#ifdef DOWNLOAD_SUBS
		include_lang_on_filename = d.appendLang();
		#endif
	}

	updateSearchTitleWidget();
}

#ifdef FS_USE_PROXY
void FindSubtitlesWindow::setupProxy() {
	QNetworkProxy proxy;

	if ( (use_proxy) && (!proxy_host.isEmpty()) ) {
		proxy.setType((QNetworkProxy::ProxyType) proxy_type);
		proxy.setHostName(proxy_host);
		proxy.setPort(proxy_port);
		if ( (!proxy_username.isEmpty()) && (!proxy_password.isEmpty()) ) {
			proxy.setUser(proxy_username);
			proxy.setPassword(proxy_password);
		}
		qDebug("FindSubtitlesWindow::userProxy: using proxy: host: %s, port: %d, type: %d", 
               proxy_host.toUtf8().constData(), proxy_port, proxy_type);
	} else {
		// No proxy
		proxy.setType(QNetworkProxy::NoProxy);
		qDebug("FindSubtitlesDialog::userProxy: no proxy");
	}

	setProxy(proxy);
}
#endif

void FindSubtitlesWindow::saveSettings() {
	qDebug("FindSubtitlesWindow::saveSettings");

	set->beginGroup("findsubtitles");

	set->setValue("xmlrpc_server", os_server);
#ifdef OS_SEARCH_WORKAROUND
	set->setValue("retries", osclient->retries());
#endif

	set->setValue("language", language());
#ifdef DOWNLOAD_SUBS
	set->setValue("include_lang_on_filename", includeLangOnFilename());
#endif

#ifdef FS_USE_PROXY
	set->setValue("proxy/use_proxy", use_proxy);
	set->setValue("proxy/type", proxy_type);
	set->setValue("proxy/host", proxy_host);
	set->setValue("proxy/port", proxy_port);
	set->setValue("proxy/username", proxy_username);
	set->setValue("proxy/password", proxy_password);
#endif

	set->setValue("search_method", osclient->searchMethod());

	set->setValue("credentials/username", osclient->username().toUtf8().toBase64());
	set->setValue("credentials/password", osclient->password().toUtf8().toBase64());

	set->endGroup();
}

void FindSubtitlesWindow::loadSettings() {
	qDebug("FindSubtitlesWindow::loadSettings");

	set->beginGroup("findsubtitles");

	os_server = set->value("xmlrpc_server", os_server).toString();
#ifdef OS_SEARCH_WORKAROUND
	osclient->setRetries( set->value("retries", osclient->retries()).toInt() );
#endif

	setLanguage( set->value("language", language()).toString() );
#ifdef DOWNLOAD_SUBS
	setIncludeLangOnFilename( set->value("include_lang_on_filename", includeLangOnFilename()).toBool() );
#endif

#ifdef FS_USE_PROXY
	use_proxy = set->value("proxy/use_proxy", use_proxy).toBool();
	proxy_type = set->value("proxy/type", proxy_type).toInt();
	proxy_host = set->value("proxy/host", proxy_host).toString();
	proxy_port = set->value("proxy/port", proxy_port).toInt();
	proxy_username = set->value("proxy/username", proxy_username).toString();
	proxy_password = set->value("proxy/password", proxy_password).toString();
#endif

	osclient->setSearchMethod( (OSClient::SearchMethod) set->value("search_method", osclient->searchMethod()).toInt() );

	osclient->setUsername(QString::fromUtf8(QByteArray::fromBase64(set->value("credentials/username", "").toByteArray())));
	osclient->setPassword(QString::fromUtf8(QByteArray::fromBase64(set->value("credentials/password", "").toByteArray())));

	set->endGroup();

	updateSearchTitleWidget();
}

#include "moc_findsubtitleswindow.cpp"

