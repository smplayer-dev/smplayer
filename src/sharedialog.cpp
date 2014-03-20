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

	setMinimumSize(QSize(435, 226));

	/*
	setMinimumSize(QSize(500, 100));

	icon_label->setPixmap(Images::icon("donate_big"));

	text_label->setText(
		tr("If you like SMPlayer and want to support its development, you can send a donation. "
           "Even the smallest one is highly appreciated.") + "<br><br>"+
		tr("Or you maybe you want to share SMPlayer with your friends in Facebook.") + "<br><br>" +
		tr("What would you like to do?") );

	donate_button = buttonBox->addButton(tr("&Donate"), QDialogButtonBox::ActionRole);
	facebook_button = buttonBox->addButton(tr("&Share with my friends"), QDialogButtonBox::ActionRole);
	close_button = buttonBox->button(QDialogButtonBox::Close);

	donate_button->setDefault(true);
	donate_button->setFocus();
	*/

	adjustSize();
	//layout()->setSizeConstraint(QLayout::SetFixedSize);

	//connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(button_clicked(QAbstractButton *)));

	share_text = QString("SMPlayer - Free Media Player with built-in codecs that can play and download Youtube videos").replace(" ","+");
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
	QDesktopServices::openUrl(QUrl("http://www.facebook.com/sharer.php?u=" + share_url + "&t=" + share_text));
}

void ShareDialog::on_twitter_button_clicked() {
	qDebug("ShareDialog::on_twitter_button_clicked");
	actions_taken |= Twitter;
	QDesktopServices::openUrl(QUrl("http://twitter.com/intent/tweet?text=" + share_text + "&url=" + share_url + "/&via=smplayer_dev"));
}

#include "moc_sharedialog.cpp"
