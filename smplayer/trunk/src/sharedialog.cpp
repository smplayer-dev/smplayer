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

#include "sharedialog.h"
#include "sharedata.h"
#include "images.h"
#include "links.h"
#include <QDesktopServices>

ShareDialog::ShareDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
	, actions_taken(0)
{
	setupUi(this);

	donate_button->setIcon(Images::icon("paypal"));
	donate_button->setIconSize(QSize(64,64));
	donate_button->setText(tr("Donate with PayPal"));
	donate_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	facebook_button->setIcon(Images::icon("social_facebook"));
	facebook_button->setIconSize(QSize(64,64));
	facebook_button->setText("Facebook");
	facebook_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	twitter_button->setIcon(Images::icon("social_twitter"));
	twitter_button->setIconSize(QSize(64,64));
	twitter_button->setText("Twitter");
	twitter_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	intro_label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">" +
		tr("You can support SMPlayer by sending a donation or sharing it with your friends.") + "</span></p></body></html>");

	setMinimumSize(QSize(435, 226));

	adjustSize();
	//layout()->setSizeConstraint(QLayout::SetFixedSize);

	share_url = URL_HOMEPAGE;
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
	actions_taken |= ShareData::Donate;
	QDesktopServices::openUrl(ShareData::donateUrl());
}

void ShareDialog::on_facebook_button_clicked() {
	qDebug("ShareDialog::on_facebook_button_clicked");
	actions_taken |= ShareData::Facebook;
	QDesktopServices::openUrl(ShareData::facebookUrl());
}

void ShareDialog::on_twitter_button_clicked() {
	qDebug("ShareDialog::on_twitter_button_clicked");
	actions_taken |= ShareData::Twitter;
	QDesktopServices::openUrl(ShareData::twitterUrl());
}

#include "moc_sharedialog.cpp"
