/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <ricardo@smplayer.info>

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


#ifndef MPLAYERWINDOW_H
#define MPLAYERWINDOW_H

#include <QWidget>
#include <QSize>
#include <QPoint>

#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>

#include "videolayer.h"
#include "config.h"

#if defined(USE_SHM) || defined(USE_COREVIDEO_BUFFER)
#define MULTIPLE_VIDEOLAYERS
#endif

class QWidget;
class QLabel;
class QKeyEvent;
class QTimer;
class ScreenHelper;
class VideoLayerShm;
class VideoLayerCV;

#define ZOOM_STEP 0.05
#define ZOOM_MIN 0.5

#define DELAYED_RESIZE 0

// Number of pixels the window has to be dragged at least before dragging starts
#define DRAG_THRESHOLD 4

enum TDragState {NOT_DRAGGING, START_DRAGGING, DRAGGING};

class MplayerWindow : public QWidget
{
	Q_OBJECT

public:
	MplayerWindow(QWidget* parent = 0, Qt::WindowFlags f = QFlag(0));
	~MplayerWindow();

	QWidget * videoLayer() { return videolayer; };

	void setResolution( int w, int h);
	void setAspect( double asp);
	void setMonitorAspect(double asp);
	void updateVideoWindow();

#if USE_COLORKEY
	void setColorKey(QColor c);
#endif

	void setOffsetX( int );
	int offsetX();

	void setOffsetY( int );
	int offsetY();

	void setZoom( double );
	double zoom();

	void allowVideoMovement(bool b) { allow_video_movement = b; };
	bool isVideoMovementAllowed() { return allow_video_movement; };

	void delayLeftClick(bool b) { delay_left_click = b; };
	bool isLeftClickDelayed() { return delay_left_click; };

	virtual QSize sizeHint () const;
	virtual QSize minimumSizeHint() const;

	virtual bool eventFilter(QObject *, QEvent *);

#if LOGO_ANIMATION
	bool animatedLogo() { return animated_logo; }
#endif

	void setCornerWidget(QWidget * w);
	QWidget * cornerWidget() { return corner_widget; };

#if REPAINT_BACKGROUND_OPTION
	void setRepaintBackground(bool b);
	bool repaintBackground() { return repaint_background; }
#endif

public slots:
	//! Should be called when a file has started.
	virtual void playingStarted();

	//! Should be called when a file has stopped.
	virtual void playingStopped();

	virtual void gotVO(QString);

	void setLogoVisible(bool b);
	void showLogo() { setLogoVisible(true); };
	void hideLogo() { setLogoVisible(false); };

#if LOGO_ANIMATION
	void setAnimatedLogo(bool b) { animated_logo = b; };
#endif

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void incZoom();
	void decZoom();

	void activateMouseDragTracking(bool active) { mouse_drag_tracking = active; }

#if DELAYED_RESIZE
protected slots:
	void resizeLater();
#endif

protected:
	virtual void retranslateStrings();
	virtual void changeEvent ( QEvent * event ) ;

	virtual void resizeEvent( QResizeEvent * e);
	virtual void mouseReleaseEvent( QMouseEvent * e);
	virtual void mouseDoubleClickEvent( QMouseEvent * e );
	virtual void wheelEvent( QWheelEvent * e );
	void moveLayer( int offset_x, int offset_y );

#ifdef MULTIPLE_VIDEOLAYERS
	void switchVideoLayer(VideoLayer * new_videolayer);
#endif

signals:
    //void rightButtonReleased( QPoint p );
	void doubleClicked();
	void leftClicked();
	void rightClicked();
	void middleClicked();
	void xbutton1Clicked(); // first X button
	void xbutton2Clicked(); // second X button
	void keyPressed(QKeyEvent * e);
	void wheelUp();
	void wheelDown();
	void mouseMovedDiff(QPoint);
	void mouseMoved(QPoint);

protected:
	int video_width, video_height;
	double aspect;
	double monitoraspect;

	ScreenHelper * helper;
	QLabel * logo;

	VideoLayer * videolayer;
#ifdef MULTIPLE_VIDEOLAYERS
	VideoLayer * videolayer_normal;
	#ifdef USE_SHM
	VideoLayerShm * videolayer_shm;
	#endif
	#ifdef USE_COREVIDEO_BUFFER
	VideoLayerCV * videolayer_cv;
	#endif
#endif

	// Zoom and moving
	int offset_x, offset_y;
	double zoom_factor;

	// Original pos and dimensions of the videolayer
	// before zooming or moving
	int orig_x, orig_y;
	int orig_width, orig_height;

	bool allow_video_movement;

#if DELAYED_RESIZE
	QTimer * resize_timer;
#endif

	// Delay left click event
	bool delay_left_click;
	QTimer * left_click_timer;
	bool double_clicked;

#if LOGO_ANIMATION
	bool animated_logo;
#endif

	QWidget * corner_widget;

#if REPAINT_BACKGROUND_OPTION
	bool repaint_background;
#endif

private:
	TDragState drag_state;
	QPoint start_drag;
	bool mouse_drag_tracking;
};

#endif

