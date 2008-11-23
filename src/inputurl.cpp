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

#include "inputurl.h"
#include "images.h"
#include "constants.h"
#include "recents.h"

class URLHistory : public Recents
{
public:
	URLHistory() : Recents() {};
	virtual ~URLHistory() {};

	virtual void addItem(QString url, bool is_playlist) {
		qDebug("Recents::addItem: '%s'", url.toUtf8().data());

		// Delete duplicates
		QStringList::iterator iterator = l.begin();
		while (iterator != l.end()) {
			QString s = (*iterator);
			if (s.endsWith(IS_PLAYLIST_TAG)) {
				s = s.remove( QRegExp(IS_PLAYLIST_TAG_RX) );
			}
			if (s == url) 
				iterator = l.erase(iterator);
			else
				iterator++;
		}

		// Add new item to list
		if (is_playlist) url = url + IS_PLAYLIST_TAG;
		l.prepend(url);

		if (l.count() > max_items) l.removeLast();
	}
};


InputURL::InputURL( QWidget* parent, Qt::WindowFlags f ) 
	: QDialog(parent, f)
{
	setupUi(this);

	url_list = new URLHistory;

	url_icon->setPixmap( Images::icon("url_big") );
	url_edit->setFocus();

	playlist_check->setWhatsThis( 
		tr("If this option is checked, the URL will be treated as a playlist: "
        "it will be opened as text and will play the URLs in it.") );

	connect(url_edit, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged()));
	connect(playlist_check, SIGNAL(stateChanged(int)), this, SLOT(playlistChanged(int)));
}

InputURL::~InputURL() {
	delete url_list;
}

void InputURL::setURL(QString url) {
	url_edit->insertItem(url_edit->count(), url);
}

QString InputURL::url() {
	return url_edit->currentText();
}

void InputURL::setPlaylist(bool b) {
	playlist_check->setChecked(b);

	int pos = url_edit->currentIndex();
	url_edit->setItemData(pos, b);
}

bool InputURL::isPlaylist() {
	return playlist_check->isChecked();
}

void InputURL::indexChanged(void) {
	int pos = url_edit->currentIndex();
	if (url_edit->itemText(pos) == url_edit->currentText()) {
		playlist_check->setChecked( url_edit->itemData(pos).toBool() );
	}
}

void InputURL::playlistChanged(int state) {
	/*
	int pos = url_edit->currentIndex();
	if (url_edit->itemText(pos) == url_edit->currentText()) {
		bool is_playlist = (state == Qt::Checked);
		url_edit->setItemIcon( pos, is_playlist ? Images::icon("playlist") : QIcon() );
	}
	*/
}

void InputURL::accept() {
	qDebug("InputURL::accept");

	QString url = url_edit->currentText();
	url_list->addItem(url, playlist_check->isChecked());

	QDialog::accept();
}

void InputURL::setURLs(QStringList urls) {
	url_list->fromStringList(urls);

	url_edit->clear();

	QString url;
	bool is_playlist;
	for (int n = 0; n < url_list->count(); n++) {
		url = url_list->item(n);
		is_playlist = false;
		if (url.endsWith(IS_PLAYLIST_TAG)) {
			url = url.remove( QRegExp(IS_PLAYLIST_TAG_RX) );
			is_playlist = true;
		}
		url_edit->addItem( /* is_playlist ? Images::icon("playlist") : QIcon(),*/ url, is_playlist);
	}
}

QStringList InputURL::urls() {
	return url_list->toStringList();
}

#include "moc_inputurl.cpp"
