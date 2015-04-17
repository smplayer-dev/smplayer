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

#include <QPushButton>
#include <QHBoxLayout>

#include "images.h"

#define SHAREBUTTON_MIN QSize(24,24)
#define SHAREBUTTON_MAX QSize(32,32)

ShareButton::ShareButton(const QString icon_name, const QString & text, QWidget * parent)
	: QPushButton(text, parent)
{
	setAttribute(Qt::WA_Hover, true);

	setIconSize(SHAREBUTTON_MIN);
	setIcon(Images::icon(icon_name));
	
}

QSize ShareButton::sizeHint() const {
	QSize s(SHAREBUTTON_MAX);
//	s += QSize(6,6);
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


ShareWidget::ShareWidget(QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent,f)
{
	ShareButton * donate_button = new ShareButton("paypal", "", this);
	ShareButton * fb_button = new ShareButton("social_facebook", "", this);
	ShareButton * twitter_button = new ShareButton("social_twitter", "", this);

	donate_button->setToolTip(tr("Donate with Paypal"));
	fb_button->setToolTip(tr("Share SMPlayer with your friends in Facebook"));
	twitter_button->setToolTip(tr("Share SMPlayer with your friends in Twitter"));

	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->addWidget(donate_button);
	layout->addWidget(fb_button);
	layout->addWidget(twitter_button);

	/*
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	setStyleSheet("background: yellow;");
	*/

	connect(donate_button, SIGNAL(clicked()), this, SLOT(donate()));
	connect(fb_button, SIGNAL(clicked()), this, SLOT(facebook()));
	connect(twitter_button, SIGNAL(clicked()), this, SLOT(twitter()));
}

ShareWidget::~ShareWidget() {
}

void ShareWidget::donate() {
	qDebug("ShareWidget::donate");
}

void ShareWidget::facebook() {
	qDebug("ShareWidget::facebook");
}

void ShareWidget::twitter() {
	qDebug("ShareWidget::twitter");
}

#include "moc_sharewidget.cpp"

