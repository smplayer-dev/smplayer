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

#include "reminderdialog.h"
#include "images.h"
#include <QPushButton>

ReminderDialog::ReminderDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

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

	adjustSize();
	//layout()->setSizeConstraint(QLayout::SetFixedSize);

	connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(button_clicked(QAbstractButton *)));
}

ReminderDialog::~ReminderDialog() {
}

void ReminderDialog::button_clicked(QAbstractButton * button) {
	int res = Close;

	if (button == donate_button) res = Donate;
	else
	if (button == facebook_button) res = Share;

	qDebug("ReminderDialog::button_clicked: res: %d", res);

	done(res);
}

bool ReminderDialog::isRemindChecked() {
	return remind_check->isChecked();
}

#include "moc_reminderdialog.cpp"
