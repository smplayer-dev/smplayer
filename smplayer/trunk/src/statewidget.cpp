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

#include "statewidget.h"
#include <QMovie>
#include <QDebug>

StateWidget::StateWidget(QWidget * parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
	qDebug() << "StateWidget::StateWidget: supported formats for QMovie:" << QMovie::supportedFormats();

	movie = new QMovie();
	//movie->setScaledSize(QSize(16, 16));

	// Buffering icon from: http://preloaders.net/
	setAnimation(":/default-theme/buffering.gif");
}

StateWidget::~StateWidget() {
}

void StateWidget::setAnimation(const QString & filename) {
	movie->setFileName(filename);

	if (movie->isValid()) {
		setMovie(movie);
	} else {
		qWarning() << "StateWidget::setAnimation: movie is not valid";
	}
}

void StateWidget::watchState(Core::State state) {
	qDebug() << "StateWidget::watchState:" << state;

	if (movie->isValid()) {
		if (state == Core::Buffering) {
			show();
			movie->start();
		} else {
			movie->stop();
			hide();
		}
	}
#if 0
	else {
		switch (state) {
			case Core::Playing:		setText("Play"); break;
			case Core::Paused:		setText("Pause"); break;
			case Core::Stopped:		setText("Stop"); break;
			case Core::Buffering:	setText("Buffering"); break;
		}
	}
#endif
}

#include "moc_statewidget.cpp"


