/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "lineedit_with_icon.h"
#include <QToolButton>
#include <QStyle>
#include <QEvent>

LineEditWithIcon::LineEditWithIcon(QWidget *parent) : QLineEdit(parent)
{
    button = new QToolButton(this);
	button->setCursor(Qt::ArrowCursor);
	setupButton();
}

void LineEditWithIcon::setupButton() {
}

void LineEditWithIcon::setIcon(const QPixmap & pixmap) {
	QPixmap p = pixmap;
	//qDebug("height: %d, icon height: %d", height(), p.height());
	int max_height = 16;
	if (max_height > height()) max_height = height() - 4;
	if (pixmap.height() > max_height) p = pixmap.scaledToHeight(max_height, Qt::SmoothTransformation);
	button->setIcon(p);
	button->setStyleSheet("QToolButton { border: none; padding: 0px; }");

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	//qDebug("frameWidth: %d", frameWidth);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(button->sizeHint().width() + frameWidth + 1));
    /*
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), button->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), button->sizeHint().height() + frameWidth * 2 + 2));
    */
}

void LineEditWithIcon::resizeEvent(QResizeEvent *)
{
    QSize sz = button->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    button->move(rect().right() - frameWidth - sz.width(),
                 (rect().bottom() + 1 - sz.height())/2);
}

// Language change stuff
void LineEditWithIcon::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		setupButton();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_lineedit_with_icon.cpp"
