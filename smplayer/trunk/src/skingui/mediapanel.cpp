/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>
    umplayer, Copyright (C) 2010 Ori Rejwan

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

#include "mediapanel.h"
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QTimerEvent>
#include <QGridLayout>
#include <QLabel>
#include <QHelpEvent>
#include <QToolTip>
#include <QDebug>
#include "qpropertysetter.h"
#include "widgetactions.h"
#include "core.h"
#include "config.h"
#include "actiontools.h"


MediaPanel::MediaPanel(QWidget *parent)
    : QWidget(parent), duration(0)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_StyledBackground, true);
	setMinimumWidth(270);

	if (fontInfo().pixelSize() > 12) {
		QFont f = font();
		f.setPixelSize(12);
		setFont(f);
	}

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mediaLabel = new ScrollingLabel(this);
	resolutionLabel = new QLabel(this);
	resolutionLabel->setObjectName("panel-resolution");
	resolutionLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	repeatButton = new MyButton(this);
	shuffleButton = new MyButton(this);
	seeker = new PanelTimeSeeker;
	seeker->setObjectName("panel-seeker");
	seeker->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
#ifdef SEEKBAR_RESOLUTION
	seeker->setRange(0, SEEKBAR_RESOLUTION);
#endif
	seeker->installEventFilter(this);
	mediaLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mediaLabel->setObjectName("panel-main-label");
	layout = new QGridLayout;
	elapsedLabel = new QLabel(this);
	elapsedLabel->setObjectName("panel-elapsed-label");
	elapsedLabel->setMargin(0);
	elapsedLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	elapsedLabel->setIndent(3);
	elapsedLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	totalLabel = new QLabel(this);
	totalLabel->setObjectName("panel-total-label");
	totalLabel->setMargin(0);
	totalLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	totalLabel->setIndent(3);
	/*
	layout->addWidget( mediaLabel, 0, 0, 1, 2 );
	layout->addWidget( resolutionLabel, 0, 2, 1, 1 );
	layout->addWidget( repeatButton, 0, 3  );
	layout->addWidget( shuffleButton, 0, 4  );
	layout->addWidget(elapsedLabel, 1, 0, 1, 1);
	layout->addWidget(seeker, 1, 1, 1, 2);
	layout->addWidget(totalLabel, 1, 3, 1, 2);
	*/
	rearrangeWidgets(false);
	layout->setSpacing(0);
	layout->setContentsMargins(8,3,8, 3);
	elapsedLabel->setText("00:00:00");
	totalLabel->setText("00:00:00");
	//resolutionLabel->setText("1920x1024");
	//resolutionLabel->hide();
	setLayout(layout);
	timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(2000);
	connect(timer, SIGNAL(timeout()), this, SLOT(reverseStatus()));
	connect(seeker, SIGNAL(valueChanged(int)), this, SIGNAL(seekerChanged(int)));
	connect(seeker, SIGNAL(wheelUp()), this, SIGNAL(seekerWheelUp()));
	connect(seeker, SIGNAL(wheelDown()), this, SIGNAL(seekerWheelDown()));
}

MediaPanel::~MediaPanel() {
}

void MediaPanel::rearrangeWidgets(bool resolution_visible) {
	if (resolution_visible) {
		layout->addWidget( mediaLabel, 0, 0, 1, 2 );
		layout->addWidget( resolutionLabel, 0, 2, 1, 1 );
		layout->addWidget( repeatButton, 0, 3  );
		layout->addWidget( shuffleButton, 0, 4  );
		layout->addWidget(elapsedLabel, 1, 0, 1, 1);
		layout->addWidget(seeker, 1, 1, 1, 2);
		layout->addWidget(totalLabel, 1, 3, 1, 2);
		resolutionLabel->setVisible(true);
	} else {
		layout->addWidget( mediaLabel, 0, 0, 1, 2 );
		layout->addWidget( repeatButton, 0, 2  );
		layout->addWidget( shuffleButton, 0, 3  );
		layout->addWidget(elapsedLabel, 1, 0, 1, 1);
		layout->addWidget(seeker, 1, 1, 1, 1);
		layout->addWidget(totalLabel, 1, 2, 1, 2);
		resolutionLabel->setVisible(false);
	}
}

void MediaPanel::setResolutionVisible(bool b) {
	rearrangeWidgets(b);
}

void MediaPanel::setScrollingEnabled(bool b) {
	mediaLabel->setScrollingEnabled(b);
}

void MediaPanel::paintEvent(QPaintEvent * e) {
	Q_UNUSED(e);

	QPainter p(this);
	p.drawPixmap(0,0,leftBackground.width(), 53, leftBackground);
	p.drawPixmap(width() - rightBackground.width(), 0, rightBackground.width(), 53, rightBackground );
	p.drawTiledPixmap(leftBackground.width(), 0, width() - leftBackground.width() - rightBackground.width(), 53, centerBackground  );    
}

void MediaPanel::setShuffleIcon( MyIcon icon ) {
	shuffleButton->setMyIcon(icon);
	shuffleButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));
}

void MediaPanel::setRepeatIcon(MyIcon icon) {
	repeatButton->setMyIcon(icon);
	repeatButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));
}

void MediaPanel::setActionCollection(QList<QAction *>actions) {
	//ActionTools::findAction("aaa", actions);
	SETACTIONTOBUTTON(shuffleButton, "pl_shuffle");
	SETACTIONTOBUTTON(repeatButton, "pl_repeat");

	retranslateStrings();
}

void MediaPanel::setMplayerState(int state) {
	Core::State s = static_cast<Core::State>(state);
	if (s == Core::Stopped) {
		seeker->setEnabled(false);
	}
	else
	if (s == Core::Paused || s == Core::Playing) {
		seeker->setEnabled(true);
	}
}

void MediaPanel::setDuration(int duration) {
	this->duration = duration;
	if (duration == 0) {
		seeker->setState(PanelSeeker::Stopped, true);
	}
	else {
		seeker->setState(PanelSeeker::Stopped, false);
	}
	setTotalText(Helper::formatTime(duration));
	setElapsedText(Helper::formatTime(0));
}

void MediaPanel::setMediaLabelText(QString text) {
	mediaLabel->setText(text);
	mediaLabel->update();
	originalTitle = text;
}

void MediaPanel::setResolutionLabelText(QString text) {
	resolutionLabel->setText(text);
}

void MediaPanel::setStatusText(QString text, int time) {
	mediaLabel->setText(text);
	mediaLabel->update();
	if (time > 0)
		timer->start(time);
	else
		timer->stop();
}

void MediaPanel::reverseStatus() {
	setMediaLabelText(originalTitle);
}

void MediaPanel::setBuffering(bool enable) {
	if (enable) {
		seeker->setState(PanelSeeker::Buffering, true);
	}
	else
	{
		seeker->setState(PanelSeeker::Buffering, false);
	}
}

void MediaPanel::setSeeker(int v) {
	seeker->setSliderValue(v);
}

bool MediaPanel::eventFilter(QObject *o, QEvent *e) {
	if (o == seeker && e->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
		qreal value = seeker->valueForPos(helpEvent->pos().x())* duration/seeker->maximum();
		if (value >=0 && value <= duration) {
			QToolTip::showText(helpEvent->globalPos(),Helper::formatTime(value), seeker);
		} else {
			QToolTip::hideText();
		}
	}
	return false;
}

// Language change stuff
void MediaPanel::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

void MediaPanel::retranslateStrings() {
	if (shuffleButton) shuffleButton->setToolTip(tr("Shuffle playlist"));
	if (repeatButton) repeatButton->setToolTip(tr("Repeat playlist"));
}

void ScrollingLabel::paintEvent(QPaintEvent * e) {
	Q_UNUSED(e);

	QPainter p(this);
	p.setFont(font());
	p.setPen(palette().color(foregroundRole()));
	p.setRenderHint(QPainter::TextAntialiasing, true);
	QRect widgetRect = rect();
	if (textRect.width() <= width()) {
		p.drawText(widgetRect, Qt::AlignVCenter | Qt::AlignLeading, mText  );
	} else {
		p.translate(-scrollPos, 0);
		p.drawText(widgetRect.adjusted(0,0,scrollPos, 0), Qt::AlignVCenter | Qt::AlignLeading, mText);
		p.translate(textRect.width() + gap, 0);
		p.drawText(widgetRect.adjusted(0, 0, scrollPos - gap - textRect.width(), 0) , Qt::AlignVCenter | Qt::AlignLeading, mText);
	}
	p.end();
}

void ScrollingLabel::setText(QString text) {
	mText = text;
	updateLabel();
	repaint();
}

void ScrollingLabel::changeEvent(QEvent * e) {
	if (e->type() == QEvent::FontChange) {
		updateLabel();
	}
}

void ScrollingLabel::updateLabel() {
	QFontMetrics fm(font());
	QRect rect = fm.boundingRect(mText);
	textRect = rect;

	if (timerId > 0) {
		killTimer(timerId);
		timerId = -1;
		scrollPos = 0;
	}

	if (scrolling_enabled) {
		if (rect.width() > width()) {
			timerId = startTimer(20);
		}
	}
}

void ScrollingLabel::timerEvent(QTimerEvent * t) {
	Q_UNUSED(t);

	scrollPos += 1;
	scrollPos = scrollPos % (textRect.width() + gap);
	update();
}

ScrollingLabel::ScrollingLabel(QWidget* parent ) {
	Q_UNUSED(parent);

	scrollPos =0;
	timerId = -1;
	scrolling_enabled = false;
	textRect = QRect();
	setAttribute(Qt::WA_StyledBackground, true);
	setText("SMPlayer");
}

void ScrollingLabel::setScrollingEnabled(bool b) {
	scrolling_enabled = b;
	updateLabel();
	repaint();
}

void ScrollingLabel::resizeEvent(QResizeEvent *) {
	updateLabel();
}

QSize ScrollingLabel::sizeHint() const {
	QFontMetrics fm(font());
	return QSize(0, fm.height());
}

#include "moc_mediapanel.cpp"
