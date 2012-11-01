/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>
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
        if(fontInfo().pixelSize() > 12)
        {
            QFont f = font();
            f.setPixelSize(12);
            setFont(f);
        }
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        mediaLabel = new ScrollingLabel(this);
        repeatButton = new MyButton(this);
        shuffleButton = new MyButton(this);        
        seeker = new PanelSeeker;
        seeker->setObjectName("panel-seeker");
        seeker->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
#ifdef SEEKBAR_RESOLUTION
        seeker->setRange(0, SEEKBAR_RESOLUTION);
#endif
        seeker->installEventFilter(this);
        mediaLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        mediaLabel->setObjectName("panel-main-label");
        QGridLayout* layout = new QGridLayout;
        elapsedLabel = new QLabel(this);
        elapsedLabel->setObjectName("panel-elapsed-label");
        elapsedLabel->setMargin(0);
        elapsedLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        elapsedLabel->setIndent(3);
        totalLabel = new QLabel(this);
        totalLabel->setObjectName("panel-total-label");
        totalLabel->setMargin(0);
        totalLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        totalLabel->setIndent(3);
        layout->addWidget( mediaLabel, 0, 0, 1, 2 );
        layout->addWidget( repeatButton, 0, 2  );
        layout->addWidget( shuffleButton, 0, 3  );
        layout->addWidget(elapsedLabel, 1, 0, 1, 1);
        layout->addWidget(seeker, 1, 1, 1, 1);
        layout->addWidget(totalLabel, 1, 2, 1, 2);
        layout->setSpacing(0);
        layout->setContentsMargins(8,3,8, 3);
        elapsedLabel->setText("00:00:00");
        totalLabel->setText("00:00:00");
        setLayout(layout);
        timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(2000);
        connect(timer, SIGNAL(timeout()), this, SLOT(reverseStatus()));
        //mediaLabel->setText("Blue Blue my love is blue. Blue is my love, since I am without you.");

	connect(seeker, SIGNAL(valueChanged(int)), this, SIGNAL(seekerChanged(int)));
}

MediaPanel::~MediaPanel()
{

}



void MediaPanel::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.drawPixmap(0,0,leftBackground.width(), 53, leftBackground);
    p.drawPixmap(width() - rightBackground.width(), 0, rightBackground.width(), 53, rightBackground );
    p.drawTiledPixmap(leftBackground.width(), 0, width() - leftBackground.width() - rightBackground.width(), 53, centerBackground  );    
}

void MediaPanel::setShuffleIcon( MyIcon icon )
{
    shuffleButton->setMyIcon(icon);
    shuffleButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));
}

void MediaPanel::setRepeatIcon(MyIcon icon)
{
    repeatButton->setMyIcon(icon);
    repeatButton->setFixedSize(icon.size(MyIcon::Normal, MyIcon::Off));
}

void MediaPanel::setActionCollection(QList<QAction *>actions)
{
	//ActionTools::findAction("aaa", actions);
	SETACTIONTOBUTTON(shuffleButton, "pl_shuffle");
	SETACTIONTOBUTTON(repeatButton, "pl_repeat");

	if (shuffleButton) shuffleButton->setToolTip(tr("Shuffle playlist"));
	if (repeatButton) repeatButton->setToolTip(tr("Repeat playlist"));
}

void MediaPanel::setMplayerState(int state)
{
    Core::State s = static_cast<Core::State>(state);
    if(s == Core::Stopped)
    {
        seeker->setEnabled(false);
    }
    else if(s == Core::Paused || s == Core::Playing)
    {
        seeker->setEnabled(true);
    }
}

void MediaPanel::setDuration(int duration)
{
    this->duration = duration;
    if(duration == 0)
    {
        seeker->setState(PanelSeeker::Stopped, true);
    }
    else
    {
        seeker->setState(PanelSeeker::Stopped, false);
    }
    setTotalText(Helper::formatTime(duration));
    setElapsedText(Helper::formatTime(0));
}

void MediaPanel::setMediaLabelText(QString text)
{
    mediaLabel->setText(text);
    mediaLabel->update();
    originalTitle = text;
}

void MediaPanel::setStatusText(QString text, int time)
{
    mediaLabel->setText(text);
    mediaLabel->update();
    if(time > 0)
        timer->start(time);
    else
        timer->stop();
}

void MediaPanel::reverseStatus()
{
    setMediaLabelText(originalTitle);
}

void MediaPanel::setBuffering(bool enable)
{
    if(enable)
    {
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

bool MediaPanel::eventFilter(QObject *o, QEvent *e)
{
    if (o == seeker && e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        qreal value = seeker->valueForPos(helpEvent->pos().x())* duration/seeker->maximum();
        if (value >=0 && value <= duration)
        {
            QToolTip::showText(helpEvent->globalPos(),Helper::formatTime(value), seeker);
        } else {
            QToolTip::hideText();
        }
    }
    return false;
}

void ScrollingLabel::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.setFont(font());
    p.setPen(palette().color(foregroundRole()));    
    p.setRenderHint(QPainter::TextAntialiasing, true);    
    QRect widgetRect = rect();
    if(textRect.width() <= width()) // No scrolling
    {
        p.drawText(widgetRect, Qt::AlignVCenter | Qt::AlignLeading, mText  );
    }
    else
    {
        p.translate(-scrollPos, 0);
        p.drawText(widgetRect.adjusted(0,0,scrollPos, 0), Qt::AlignVCenter | Qt::AlignLeading, mText);
        p.translate(textRect.width() + gap, 0);
        p.drawText(widgetRect.adjusted(0, 0, scrollPos - gap - textRect.width(), 0) , Qt::AlignVCenter | Qt::AlignLeading, mText);
    }        
    p.end();
}

void ScrollingLabel::setText(QString text)
{
    mText = text;    
    updateLabel();
    repaint();
}

void ScrollingLabel::changeEvent(QEvent * e)
{
    if(e->type() == QEvent::FontChange)
    {
        updateLabel();
    }    
}

void ScrollingLabel::updateLabel()
{
    QFontMetrics fm(font());
    QRect rect = fm.boundingRect(mText);
    textRect = rect;
    if(timerId > 0)
    {
        killTimer(timerId);
        timerId = -1;
        scrollPos = 0;
    }
    if(rect.width() > width()) // scrolling
    {
        timerId = startTimer(20);
    }
}

void ScrollingLabel::timerEvent(QTimerEvent * t)
{
    scrollPos += 1;
    scrollPos = scrollPos % (textRect.width() + gap);
    update();
}


ScrollingLabel::ScrollingLabel(QWidget* parent )
{
    scrollPos =0;
    timerId = -1;
    textRect = QRect();
    setAttribute(Qt::WA_StyledBackground, true);
    setText("SMPlayer");
}

void ScrollingLabel::resizeEvent(QResizeEvent *)
{
    updateLabel();
}

QSize  ScrollingLabel::sizeHint() const
{
    QFontMetrics fm(font());
    return QSize(0, fm.height());
}

#include "moc_mediapanel.cpp"
