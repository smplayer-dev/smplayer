/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "sharedialog.h"
#include <QDesktopServices>
#include <QUrl>

ShareDialog::ShareDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
	, actions_taken(0)
{
	setupUi(this);

	donate_button->setIcon(QPixmap(":/icons-png/paypal.png"));
	donate_button->setIconSize(QSize(64,64));
	donate_button->setText(tr("Donate with Paypal"));
	donate_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	facebook_button->setIcon(QPixmap(":/icons-png/social_facebook.png"));
	facebook_button->setIconSize(QSize(64,64));
	facebook_button->setText("Facebook");
	facebook_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	twitter_button->setIcon(QPixmap(":/icons-png/social_twitter.png"));
	twitter_button->setIconSize(QSize(64,64));
	twitter_button->setText("Twitter");
	twitter_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	intro_label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">" +
		tr("You can support SMPlayer by sending a donation or sharing it with your friends.") + "</span></p></body></html>");

	setMinimumSize(QSize(435, 226));

	adjustSize();
	//layout()->setSizeConstraint(QLayout::SetFixedSize);

	share_url = "http://smplayer.sourceforge.net";
}

ShareDialog::~ShareDialog() {
}

bool ShareDialog::isRemindChecked() {
	return remind_check->isChecked();
}

void ShareDialog::showRemindCheck(bool b) {
	check_widget->setVisible(b);
	adjustSize();
}

void ShareDialog::on_donate_button_clicked() {
	qDebug("ShareDialog::on_donate_button_clicked");
	actions_taken |= Donate;
	QDesktopServices::openUrl(QUrl("http://sourceforge.net/donate/index.php?group_id=185512"));
}

void ShareDialog::on_facebook_button_clicked() {
	qDebug("ShareDialog::on_facebook_button_clicked");
	actions_taken |= Facebook;
	QDesktopServices::openUrl(QUrl("http://www.facebook.com/sharer.php?u=" + share_url /* + "&t=" + share_text */ ));
}

void ShareDialog::on_twitter_button_clicked() {
	qDebug("ShareDialog::on_twitter_button_clicked");
	actions_taken |= Twitter;

	QString text = tr("SMPlayer is a free media player for PC. It plays all formats and can even download Youtube videos.",
					  "This text is to be published on twitter and the translation should not be more than 99 characters long");

	qDebug("ShareDialog::on_twitter_button_clicked: length: %d", text.length());
	if (text.length() > 99) {
		qDebug("ShareDialog::on_twitter_button_clicked: the translation text is too long (%d), it shouldn't be longer than 99 characters. Using the English text.", text.length());
		text = "SMPlayer is a free media player for PC. It plays all formats and can even download Youtube videos.";
	}
	//text = text.replace("SMPlayer", "#SMPlayer");
	text = QUrl::toPercentEncoding(text);
	QString url = "http://twitter.com/intent/tweet?text=" + text + "&url=" + QUrl::toPercentEncoding(share_url) + "/&via=smplayer_dev"; 
	QDesktopServices::openUrl(QUrl::fromEncoded(url.toLatin1()));
}

#include "moc_sharedialog.cpp"
