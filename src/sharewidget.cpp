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

ShareWidget::ShareWidget(QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent,f)
{
	QPushButton * donate_button = new QPushButton(QPixmap(":/icons-png/paypal.png"), "", this);
	QPushButton * fb_button = new QPushButton(QPixmap(":/icons-png/social_facebook.png"), "", this);
	QPushButton * twitter_button = new QPushButton(QPixmap(":/icons-png/social_twitter.png"), "", this);

	donate_button->setToolTip(tr("Donate with Paypal"));
	fb_button->setToolTip(tr("Share SMPlayer with your friends in Facebook"));
	twitter_button->setToolTip(tr("Share SMPlayer with your friends in Twitter"));

	QSize bsize = QSize(32,32);
	donate_button->setIconSize(bsize);
	fb_button->setIconSize(bsize);
	twitter_button->setIconSize(bsize);

	#if 1
	QString st = "margin: 0px; border: 0px;";
	donate_button->setStyleSheet(st);
	fb_button->setStyleSheet(st);
	twitter_button->setStyleSheet(st);
	#endif

	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->addWidget(donate_button);
	layout->addWidget(fb_button);
	layout->addWidget(twitter_button);

	
	//setBackgroundRole(QPalette::Window);
	//setAutoFillBackground(true);
	//setStyleSheet("background: black;");
	

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

