/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "sharewidget.h"
#include "sharedata.h"
#include "images.h"

#include <QHBoxLayout>
#include <QDesktopServices>
#include <QSettings>

#define SHAREBUTTON_MIN QSize(24,24)
#define SHAREBUTTON_MAX QSize(32,32)

ShareButton::ShareButton(const QString icon_name, const QString & tooltip, QWidget * parent)
	: QPushButton("", parent)
{
	setAttribute(Qt::WA_Hover, true);

	setIconSize(SHAREBUTTON_MIN);
	setIcon(Images::icon(icon_name));
	setToolTip(tooltip);
}

QSize ShareButton::sizeHint() const {
	QSize s(SHAREBUTTON_MAX);
	s += QSize(4,4);
	return s;
}

void ShareButton::enterEvent(QEvent *) {
	//qDebug("ShareButton::enterEvent");
	setIconSize(SHAREBUTTON_MAX);
}

void ShareButton::leaveEvent(QEvent *) {
	//qDebug("ShareButton::leaveEvent");
	setIconSize(SHAREBUTTON_MIN);
}



ShareWidget::ShareWidget(QSettings * settings, QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent,f)
	, set(settings)
	, actions_taken(0)
	, count(0)
{
	donate_button = new ShareButton("paypal", tr("Donate with PayPal"), this);
	fb_button = new ShareButton("social_facebook", tr("Share SMPlayer in Facebook"), this);
	twitter_button = new ShareButton("social_twitter", tr("Share SMPlayer in Twitter"), this);

	QPushButton * support_button = new QPushButton(tr("Support SMPlayer"), this);
	support_button->setObjectName("support_button");
	support_button->setToolTip(tr("Donate / Share SMPlayer with your friends"));
	connect(support_button, SIGNAL(clicked()), this, SIGNAL(supportClicked()));

	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->setSpacing(0);
	hlayout->addWidget(donate_button);
	hlayout->addWidget(fb_button);
	hlayout->addWidget(twitter_button);

	QVBoxLayout * vlayout = new QVBoxLayout(this);
	vlayout->setSpacing(0);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(support_button);

	/*
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	setStyleSheet("background: yellow;");
	*/

	connect(donate_button, SIGNAL(clicked()), this, SLOT(donate()));
	connect(fb_button, SIGNAL(clicked()), this, SLOT(facebook()));
	connect(twitter_button, SIGNAL(clicked()), this, SLOT(twitter()));

	loadConfig();
}

ShareWidget::~ShareWidget() {
	saveConfig();
}

void ShareWidget::loadConfig() {
	if (set) {
		set->beginGroup("reminder");
		actions_taken = set->value("action", 0).toInt();
		count = set->value("count", 0).toInt();
		set->endGroup();
	}
}

void ShareWidget::saveConfig() {
	if (set) {
		set->beginGroup("reminder");
		set->setValue("action", actions_taken);
		set->setValue("count", count);
		set->endGroup();
	}
}

void ShareWidget::setActionPerformed(int action) {
	actions_taken |= action;
}

void ShareWidget::setVisible(bool visible) {
	//qDebug("ShareWidget::setVisible: %d", visible);

#if 1
	if (!visible) {
		QWidget::setVisible(false);
	} else {
		bool v = true;

		if (actions_taken == 7) {
			// User already clicked all buttons
			v = false;
		} else {
			if ((actions_taken & ShareData::Donate) > 0) donate_button->hide();
			if ((actions_taken & ShareData::Facebook) > 0) fb_button->hide();
			if ((actions_taken & ShareData::Twitter) > 0) twitter_button->hide();

			count++;

			// Display the buttons from time to time
			if ((count % 5) != 1) v = false;
		}

		QWidget::setVisible(v);
	}
#else
	QWidget::setVisible(visible);
#endif
}

void ShareWidget::donate() {
	qDebug("ShareWidget::donate");
	setActionPerformed(ShareData::Donate);
	QDesktopServices::openUrl( ShareData::donateUrl() );
}

void ShareWidget::facebook() {
	qDebug("ShareWidget::facebook");
	setActionPerformed(ShareData::Facebook);
	QDesktopServices::openUrl( ShareData::facebookUrl() );
}

void ShareWidget::twitter() {
	qDebug("ShareWidget::twitter");
	setActionPerformed(ShareData::Twitter);
	QDesktopServices::openUrl( ShareData::twitterUrl() );
}

#include "moc_sharewidget.cpp"

