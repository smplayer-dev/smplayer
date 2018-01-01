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

#include "mplayerwindow.h"
#include "global.h"
#include "desktopinfo.h"
#include "colorutils.h"

#ifndef MINILIB
#include "images.h"
#endif

#include <QLabel>
#include <QTimer>
#include <QCursor>
#include <QEvent>
#include <QLayout>
#include <QPixmap>
#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QDebug>

#if LOGO_ANIMATION
#include <QPropertyAnimation>
#endif

//#define HANDLE_GESTURES

#ifdef HANDLE_GESTURES
#include <QGestureEvent>
#include <QTapGesture>
#endif

Screen::Screen(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f )
	, check_mouse_timer(0)
	, mouse_last_position(QPoint(0,0))
	, autohide_cursor(false)
	, autohide_interval(0)
{
	setMouseTracking(true);
	setFocusPolicy( Qt::NoFocus );
	setMinimumSize( QSize(0,0) );

	check_mouse_timer = new QTimer(this);
	connect( check_mouse_timer, SIGNAL(timeout()), this, SLOT(checkMousePos()) );

	setAutoHideInterval(1000);
	setAutoHideCursor(false);
}

Screen::~Screen() {
}

void Screen::setAutoHideCursor(bool b) {
	qDebug("Screen::setAutoHideCursor: %d", b);

	autohide_cursor = b;
	if (autohide_cursor) {
		check_mouse_timer->setInterval(autohide_interval);
		check_mouse_timer->start();
	} else {
		check_mouse_timer->stop();
	}
}

void Screen::checkMousePos() {
	//qDebug("Screen::checkMousePos");

	if (!autohide_cursor) {
		setCursor(QCursor(Qt::ArrowCursor));
		return;
	}

	QPoint pos = mapFromGlobal(QCursor::pos());

	//qDebug("Screen::checkMousePos: x: %d, y: %d", pos.x(), pos.y());

	if (mouse_last_position != pos) {
		setCursor(QCursor(Qt::ArrowCursor));
	} else {
		setCursor(QCursor(Qt::BlankCursor));
	}
	mouse_last_position = pos;
}

void Screen::mouseMoveEvent( QMouseEvent * e ) {
	//qDebug("Screen::mouseMoveEvent");
	emit mouseMoved(e->pos());

	if (cursor().shape() != Qt::ArrowCursor) {
		//qDebug(" showing mouse cursor" );
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void Screen::playingStarted() {
	qDebug("Screen::playingStarted");
	setAutoHideCursor(true);
}

void Screen::playingStopped() {
	qDebug("Screen::playingStopped");
	setAutoHideCursor(false);
}

/* ---------------------------------------------------------------------- */

MplayerLayer::MplayerLayer(QWidget* parent, Qt::WindowFlags f)
	: Screen(parent, f)
#if REPAINT_BACKGROUND_OPTION
	, repaint_background(false)
#endif
	, playing(false)
{
}

MplayerLayer::~MplayerLayer() {
}

#if REPAINT_BACKGROUND_OPTION
void MplayerLayer::setRepaintBackground(bool b) {
	qDebug("MplayerLayer::setRepaintBackground: %d", b);
	repaint_background = b;
}
#endif

void MplayerLayer::playingStarted() {
	qDebug("MplayerLayer::playingStarted");
//	repaint();
	playing = true;

#ifndef Q_OS_WIN
	if (!repaint_background) setUpdatesEnabled(false);
#endif

	Screen::playingStarted();
}

void MplayerLayer::playingStopped() {
	qDebug("MplayerLayer::playingStopped");
	playing = false;

#ifndef Q_OS_WIN
	setUpdatesEnabled(true);
#endif

//	repaint();
	Screen::playingStopped();
}

/* ---------------------------------------------------------------------- */

MplayerWindow::MplayerWindow(QWidget* parent, Qt::WindowFlags f)
	: Screen(parent, f)
	, video_width(0)
	, video_height(0)
	, aspect((double) 4/3)
	, monitoraspect(0)
	, mplayerlayer(0)
	, logo(0)
	, offset_x(0)
	, offset_y(0)
	, zoom_factor(1.0)
	, orig_x(0)
	, orig_y(0)
	, orig_width(0)
	, orig_height(0)
	, allow_video_movement(false)
#if DELAYED_RESIZE
	, resize_timer(0)
#endif
	, delay_left_click(false)
	, left_click_timer(0)
	, double_clicked(false)
#if LOGO_ANIMATION
	, animated_logo(false)
#endif
	, corner_widget(0)
    , drag_state(NOT_DRAGGING)
    , start_drag(QPoint(0,0))
    , mouse_drag_tracking(false)
{
	mplayerlayer = new MplayerLayer(this);
	mplayerlayer->setObjectName("mplayerlayer");

	logo = new QLabel( mplayerlayer );
	logo->setObjectName("mplayerwindowlogo");

	// Set colors
#ifdef CHANGE_WIDGET_COLOR
	setAutoFillBackground(true);
	ColorUtils::setBackgroundColor( this, QColor(0,0,0) );
	mplayerlayer->setAutoFillBackground(true);
	logo->setAutoFillBackground(true);
	ColorUtils::setBackgroundColor( logo, QColor(0,0,0) );
#else
	setStyleSheet("MplayerWindow { background-color: black;}");
	mplayerlayer->setStyleSheet("background-color: black;");
#endif

	QVBoxLayout * mplayerlayerLayout = new QVBoxLayout( mplayerlayer );
	mplayerlayerLayout->addWidget( logo, 0, Qt::AlignHCenter | Qt::AlignVCenter );

	setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding );
	setFocusPolicy( Qt::StrongFocus );
	
//#ifdef HANDLE_GESTURES
	grabGesture(Qt::TapGesture);
//#endif

	installEventFilter(this);
	mplayerlayer->installEventFilter(this);
	//logo->installEventFilter(this);

#if DELAYED_RESIZE
	resize_timer = new QTimer(this);
	resize_timer->setSingleShot(true);
	resize_timer->setInterval(50);
	connect( resize_timer, SIGNAL(timeout()), this, SLOT(resizeLater()) );
#endif

	left_click_timer = new QTimer(this);
	left_click_timer->setSingleShot(true);
	left_click_timer->setInterval(qApp->doubleClickInterval()+10);
	connect(left_click_timer, SIGNAL(timeout()), this, SIGNAL(leftClicked()));

	retranslateStrings();
}

MplayerWindow::~MplayerWindow() {
}

void MplayerWindow::setCornerWidget(QWidget * w) {
	corner_widget = w;

	QHBoxLayout * blayout = new QHBoxLayout;
	blayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
	blayout->addWidget(corner_widget);

	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
	layout->addLayout(blayout);
}

#if USE_COLORKEY
void MplayerWindow::setColorKey( QColor c ) {
	#ifdef CHANGE_WIDGET_COLOR
	ColorUtils::setBackgroundColor( mplayerlayer, c );
	#endif
}
#endif

void MplayerWindow::retranslateStrings() {
	//qDebug("MplayerWindow::retranslateStrings");
#ifndef MINILIB
	logo->setPixmap( Images::icon("background") );
#endif
}

void MplayerWindow::setLogoVisible( bool b) {
	qDebug() << "MplayerWindow::setLogoVisible:" << b;

#if REPAINT_BACKGROUND_OPTION
	if (b) mplayerlayer->setUpdatesEnabled(true);
#endif

	if (corner_widget) {
		corner_widget->setVisible(b);
	}

#if !LOGO_ANIMATION
	logo->setVisible(b);
#else
	if (!animated_logo) {
		logo->setVisible(b);
	} else {
		if (b) {
			logo->show();
			QPropertyAnimation * animation = new QPropertyAnimation(logo, "pos");
			animation->setDuration(200);
			animation->setEasingCurve(QEasingCurve::OutBounce);
			animation->setStartValue(QPoint(logo->x(), 0 - logo->y()));
			animation->setEndValue(logo->pos());
			animation->start();
		} else {
			QPropertyAnimation * animation = new QPropertyAnimation(logo, "pos");
			animation->setDuration(200);
			animation->setEasingCurve(QEasingCurve::OutBounce);
			animation->setEndValue(QPoint(width(), logo->y()));
			animation->setStartValue(logo->pos());
			animation->start();
			connect(animation, SIGNAL(finished()), logo, SLOT(hide()));
			//logo->hide();
		}
	}
#endif
}

/*
void MplayerWindow::changePolicy() {
	setSizePolicy( QSizePolicy::Preferred , QSizePolicy::Preferred  );
}
*/

void MplayerWindow::setResolution( int w, int h)
{
    video_width = w;
    video_height = h;
    
    //mplayerlayer->move(1,1);
    updateVideoWindow();
}


void MplayerWindow::resizeEvent( QResizeEvent * /* e */)
{
   /*qDebug("MplayerWindow::resizeEvent: %d, %d",
	   e->size().width(), e->size().height() );*/

#if !DELAYED_RESIZE
	offset_x = 0;
	offset_y = 0;

    updateVideoWindow();
	setZoom(zoom_factor);
#else
	resize_timer->start();
#endif
}

#if DELAYED_RESIZE
void MplayerWindow::resizeLater() {
	offset_x = 0;
	offset_y = 0;

    updateVideoWindow();
	setZoom(zoom_factor);
}
#endif

void MplayerWindow::setMonitorAspect(double asp) {
	monitoraspect = asp;
}

void MplayerWindow::setAspect( double asp) {
    aspect = asp;
	if (monitoraspect!=0) {
		aspect = aspect / monitoraspect * DesktopInfo::desktop_aspectRatio(this);
	}
	updateVideoWindow();
}


void MplayerWindow::updateVideoWindow()
{
	//qDebug("MplayerWindow::updateVideoWindow");

    //qDebug("aspect= %f", aspect);

    int w_width = size().width();
    int w_height = size().height();

	int w = w_width;
	int h = w_height;
	int x = 0;
	int y = 0;

	if (aspect != 0) {
	    int pos1_w = w_width;
	    int pos1_h = w_width / aspect + 0.5;
    
	    int pos2_h = w_height;
	    int pos2_w = w_height * aspect + 0.5;
    
	    //qDebug("pos1_w: %d, pos1_h: %d", pos1_w, pos1_h);
	    //qDebug("pos2_w: %d, pos2_h: %d", pos2_w, pos2_h);
    
	    if (pos1_h <= w_height) {
		//qDebug("Pos1!");
			w = pos1_w;
			h = pos1_h;
	
			y = (w_height - h) /2;
	    } else {
		//qDebug("Pos2!");
			w = pos2_w;
			h = pos2_h;
	
			x = (w_width - w) /2;
	    }
	}

    mplayerlayer->move(x,y);
    mplayerlayer->resize(w, h);

	orig_x = x;
	orig_y = y;
	orig_width = w;
	orig_height = h;
    
    //qDebug( "w_width: %d, w_height: %d", w_width, w_height);
    //qDebug("w: %d, h: %d", w,h);
}


void MplayerWindow::mouseReleaseEvent( QMouseEvent * e) {
	qDebug( "MplayerWindow::mouseReleaseEvent" );

	if (e->button() == Qt::LeftButton) {
		e->accept();
		if (delay_left_click) {
			if (!double_clicked) left_click_timer->start();
			double_clicked = false;
		} else {
			emit leftClicked();
		}
	}
	else
	if (e->button() == Qt::MidButton) {
		e->accept();
		emit middleClicked();
	}
	else
	if (e->button() == Qt::XButton1) {
		e->accept();
		emit xbutton1Clicked();
	}
	else
	if (e->button() == Qt::XButton2) {
		e->accept();
		emit xbutton2Clicked();
	}
	else
    if (e->button() == Qt::RightButton) {
		e->accept();
		//emit rightButtonReleased( e->globalPos() );
		emit rightClicked();
    } 
	else {
		e->ignore();
	}
}

void MplayerWindow::mouseDoubleClickEvent( QMouseEvent * e ) {
	if (e->button() == Qt::LeftButton) {
		e->accept();
		if (delay_left_click) {
			left_click_timer->stop();
			double_clicked = true;
		}
		emit doubleClicked();
	} else {
		e->ignore();
	}
}

void MplayerWindow::wheelEvent( QWheelEvent * e ) {
	qDebug("MplayerWindow::wheelEvent: delta: %d", e->delta());
	e->accept();

	if (e->orientation() == Qt::Vertical) {
	    if (e->delta() >= 0)
	        emit wheelUp();
	    else
	        emit wheelDown();
	} else {
		qDebug("MplayerWindow::wheelEvent: horizontal event received, doing nothing");
	}
}

bool MplayerWindow::eventFilter( QObject * object, QEvent * event ) {
	Q_UNUSED(object);

#ifdef HANDLE_GESTURES
	if (event->type() == QEvent::Gesture) {
		qDebug() << "MplayerWindow::eventFilter: event:" << event;
		QGestureEvent * ge = static_cast<QGestureEvent*>(event);
		qDebug() << "MplayerWindow::eventFilter: ge:" << ge;
		if (QGesture * tap = ge->gesture(Qt::TapGesture)) {
			QTapGesture * tg = static_cast<QTapGesture *>(tap);
			qDebug() << "MplayerWindow::eventFilter: tg:" << tg;
			event->setAccepted(true);
			return false;
		}
	}
#endif
	
    if (!mouse_drag_tracking)
        return false;

    QEvent::Type type = event->type();
    if (type != QEvent::MouseButtonPress
        && type != QEvent::MouseButtonRelease
        && type != QEvent::MouseMove)
        return false;

    QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
    if (!mouseEvent)
        return false;

    if (mouseEvent->modifiers() != Qt::NoModifier) {
        drag_state = NOT_DRAGGING;
        return false;
    }

    if (type == QEvent::MouseButtonPress) {
        if (mouseEvent->button() != Qt::LeftButton) {
            drag_state = NOT_DRAGGING;
            return false;
        }

        drag_state = START_DRAGGING;
        start_drag = mouseEvent->globalPos();
        // Don't filter, so others can have a look at it too
        return false;
    }

    if (type == QEvent::MouseButtonRelease) {
        if (drag_state != DRAGGING || mouseEvent->button() != Qt::LeftButton) {
            drag_state = NOT_DRAGGING;
            return false;
        }

        // Stop dragging and eat event
        drag_state = NOT_DRAGGING;
        event->accept();
        return true;
    }

    // type == QEvent::MouseMove
    if (drag_state == NOT_DRAGGING)
        return false;

    // buttons() note the s
    if (mouseEvent->buttons() != Qt::LeftButton) {
        drag_state = NOT_DRAGGING;
        return false;
    }

    QPoint pos = mouseEvent->globalPos();
    QPoint diff = pos - start_drag;
    if (drag_state == START_DRAGGING) {
        // Don't start dragging before moving at least DRAG_THRESHOLD pixels
        if (abs(diff.x()) < DRAG_THRESHOLD && abs(diff.y()) < DRAG_THRESHOLD)
            return false;

        drag_state = DRAGGING;
    }

    emit mouseMovedDiff(diff);
    start_drag = pos;
    event->accept();
    return true;
}

QSize MplayerWindow::sizeHint() const {
	//qDebug("MplayerWindow::sizeHint");
	return QSize( video_width, video_height );
}

QSize MplayerWindow::minimumSizeHint () const {
	return QSize(0,0);
}

void MplayerWindow::setOffsetX( int d) {
	offset_x = d;
	mplayerlayer->move( orig_x + offset_x, mplayerlayer->y() );
}

int MplayerWindow::offsetX() { return offset_x; }

void MplayerWindow::setOffsetY( int d) {
	offset_y = d;
	mplayerlayer->move( mplayerlayer->x(), orig_y + offset_y );
}

int MplayerWindow::offsetY() { return offset_y; }

void MplayerWindow::setZoom( double d) {
	zoom_factor = d;
	offset_x = 0;
	offset_y = 0;

	int x = orig_x;
	int y = orig_y;
	int w = orig_width;
	int h = orig_height;

	if (zoom_factor != 1.0) {
		w = w * zoom_factor;
		h = h * zoom_factor;

		// Center
		x = (width() - w) / 2;
		y = (height() -h) / 2;
	}

	mplayerlayer->move(x,y);
	mplayerlayer->resize(w,h);
}

double MplayerWindow::zoom() { return zoom_factor; }

void MplayerWindow::moveLayer( int offset_x, int offset_y ) {
	int x = mplayerlayer->x();
	int y = mplayerlayer->y();

	mplayerlayer->move( x + offset_x, y + offset_y );
}

void MplayerWindow::moveLeft() {
	if ((allow_video_movement) || (mplayerlayer->x()+mplayerlayer->width() > width() ))
		moveLayer( -16, 0 );
}

void MplayerWindow::moveRight() {
	if ((allow_video_movement) || ( mplayerlayer->x() < 0 ))
		moveLayer( +16, 0 );
}

void MplayerWindow::moveUp() {
	if ((allow_video_movement) || (mplayerlayer->y()+mplayerlayer->height() > height() ))
		moveLayer( 0, -16 );
}

void MplayerWindow::moveDown() {
	if ((allow_video_movement) || ( mplayerlayer->y() < 0 ))
		moveLayer( 0, +16 );
}

void MplayerWindow::incZoom() {
	setZoom( zoom_factor + ZOOM_STEP );
}

void MplayerWindow::decZoom() {
	double zoom = zoom_factor - ZOOM_STEP;
	if (zoom < ZOOM_MIN) zoom = ZOOM_MIN;
	setZoom( zoom );
}

// Language change stuff
void MplayerWindow::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		Screen::changeEvent(e);
	}
}

#include "moc_mplayerwindow.cpp"
