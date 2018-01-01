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

#include "sharewidget.h"
#include "sharedata.h"
#include "images.h"

#include <QHBoxLayout>
#include <QDesktopServices>
#include <QSettings>
#include <QEvent>
#include <QTime>

//#define TEST_SHAREWIDGET

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
	, display(Random)
{
	QTime now = QTime::currentTime();
	qsrand(now.msec());

	donate_button = new ShareButton("paypal", "", this);
	fb_button = new ShareButton("social_facebook", "", this);
	twitter_button = new ShareButton("social_twitter", "", this);

	support_button = new QPushButton(this);
	support_button->setObjectName("support_button");
	connect(support_button, SIGNAL(clicked()), this, SIGNAL(supportClicked()));

	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->setSpacing(0);
	//hlayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));
	hlayout->addWidget(donate_button);
	hlayout->addWidget(fb_button);
	hlayout->addWidget(twitter_button);
	//hlayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));

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

	retranslateStrings();
	loadConfig();
	updateButtons();
}

ShareWidget::~ShareWidget() {
	saveConfig();
}

void ShareWidget::retranslateStrings() {
	donate_button->setToolTip(tr("Donate with PayPal"));
	fb_button->setToolTip(tr("Share SMPlayer in Facebook"));
	twitter_button->setToolTip(tr("Share SMPlayer in Twitter"));

	support_button->setText(tr("Support SMPlayer"));
	support_button->setToolTip(tr("Donate / Share SMPlayer with your friends"));
}

void ShareWidget::updateButtons() {
	qDebug("ShareWidget::updateButtons: actions_taken: %d", actions_taken);

	if ((actions_taken & ShareData::Donate) > 0) donate_button->hide();
	if ((actions_taken & ShareData::Facebook) > 0) fb_button->hide();
	if ((actions_taken & ShareData::Twitter) > 0) twitter_button->hide();

	if (actions_taken > 0) support_button->hide();
}


void ShareWidget::loadConfig() {
	if (set) {
		set->beginGroup("reminder");
		actions_taken = set->value("action", 0).toInt();
		count = set->value("count", 0).toInt();
		display = set->value("show", Random).toInt();
		set->endGroup();
	}
}

void ShareWidget::saveConfig() {
	if (set) {
		set->beginGroup("reminder");
		set->setValue("action", actions_taken);
		set->setValue("count", count);
		set->setValue("show", display);
		set->endGroup();
	}
}

void ShareWidget::setActions(int a) {
	actions_taken = a;
	updateButtons();
	saveConfig();
}

void ShareWidget::setActionPerformed(int action) {
	actions_taken |= action;
	updateButtons();
}

void ShareWidget::setVisible(bool visible) {
	//qDebug("ShareWidget::setVisible: %d", visible);

#if 1
	if (!visible) {
		QWidget::setVisible(false);
	} else {
		bool v = true;
		switch (display) {
			case Never: v = false; break;
			case Always: v = visible; break;
			case Random: {
				if (actions_taken == 7) {
					// User already clicked all buttons
					v = false;
				} else {
					int r = (qrand() % 10);
					//qDebug("ShareWidget::setVisible: r: %d", r);
					if (r != 1) v = false;
				}
			}
		}
		//qDebug("ShareWidget::setVisible: v: %d", v);
		QWidget::setVisible(v);
	}
#else
	QWidget::setVisible(visible);
#endif
}

void ShareWidget::donate() {
	qDebug("ShareWidget::donate");
	setActionPerformed(ShareData::Donate);
#ifndef TEST_SHAREWIDGET
	QDesktopServices::openUrl( ShareData::donateUrl() );
#endif
}

void ShareWidget::facebook() {
	qDebug("ShareWidget::facebook");
	setActionPerformed(ShareData::Facebook);
#ifndef TEST_SHAREWIDGET
	QDesktopServices::openUrl( ShareData::facebookUrl() );
#endif
}

void ShareWidget::twitter() {
	qDebug("ShareWidget::twitter");
	setActionPerformed(ShareData::Twitter);
#ifndef TEST_SHAREWIDGET
	QDesktopServices::openUrl( ShareData::twitterUrl() );
#endif
}

// Language change stuff
void ShareWidget::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_sharewidget.cpp"

