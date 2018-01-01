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

#ifndef DEFAULTGUI_H
#define DEFAULTGUI_H

#define BUFFERING_ANIMATION
//#define LANGUAGE_TOOLBAR

#ifdef IDOPT_BUILD
#define ADD_QUICK_ACCESS
#endif

#include "guiconfig.h"
#include "baseguiplus.h"
#include <QPoint>

class QToolBar;
class EditableToolbar;
class QPushButton;
class QResizeEvent;
class MyAction;
class QMenu;
class TimeSliderAction;
class VolumeSliderAction;
class AutohideWidget;
class TimeLabelAction;
class MyAction;

#if MINI_ARROW_BUTTONS
class SeekingButton;
#endif

#ifdef BUFFERING_ANIMATION
class StateWidget;
#endif

class DefaultGui : public BaseGuiPlus
{
	Q_OBJECT

public:
	DefaultGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	~DefaultGui();

#if USE_MINIMUMSIZE
	virtual QSize minimumSizeHint () const;
#endif

public slots:
	//virtual void showPlaylist(bool b);

protected:
	virtual void retranslateStrings();
	virtual QMenu * createPopupMenu();

	void createStatusBar();
	void createMainToolBars();
	void createControlWidget();
	void createControlWidgetMini();
	void createFloatingControl();
	void createActions();
	void createMenus();
	virtual void populateMainMenu();

	void loadConfig();
	void saveConfig();

    virtual void aboutToEnterFullscreen();
    virtual void aboutToExitFullscreen();
    virtual void aboutToEnterCompactMode();
    virtual void aboutToExitCompactMode();

	virtual void resizeEvent( QResizeEvent * );
	/* virtual void closeEvent( QCloseEvent * ); */

protected slots:
	virtual void updateWidgets();
	virtual void displayTime(double sec);
	virtual void displayFrame(int frame);
	virtual void displayABSection(int secs_a, int secs_b);
	virtual void displayVideoInfo(int width, int height, double fps);
	virtual void displayBitrateInfo(int vbitrate, int abitrate);
	void checkCompactMode();
#ifdef ADD_QUICK_ACCESS
	void adaptForTabletMode();
#endif

	// Reimplemented:
#if AUTODISABLE_ACTIONS
	virtual void enableActionsOnPlaying();
	virtual void disableActionsOnStop();
#endif
	virtual void togglePlayAction(Core::State state);

	void adjustFloatingControlSize();

protected:
	void reconfigureFloatingControl();

protected:
	QLabel * time_display;
	QLabel * frame_display;
	QLabel * ab_section_display;
	QLabel * video_info_display;
	QLabel * format_info_display;
	QLabel * bitrate_info_display;
#ifdef BUFFERING_ANIMATION
	StateWidget * state_widget;
#endif

	EditableToolbar * controlwidget;
	EditableToolbar * controlwidget_mini;

	EditableToolbar * toolbar1;

#ifdef LANGUAGE_TOOLBAR
	QToolBar * toolbar2;
	QPushButton * select_audio;
	QPushButton * select_subtitle;
#endif

	TimeSliderAction * timeslider_action;
	VolumeSliderAction * volumeslider_action;

#if MINI_ARROW_BUTTONS
	SeekingButton * rewindbutton_action;
	SeekingButton * forwardbutton_action;
#endif

	AutohideWidget * floating_control;
	TimeLabelAction * time_label_action;
	TimeLabelAction * current_time_label_action;
	TimeLabelAction * total_time_label_action;
	TimeLabelAction * remaining_time_label_action;

	MyAction * viewFrameCounterAct;
	MyAction * viewVideoInfoAct;
	MyAction * viewFormatInfoAct;
	MyAction * viewBitrateInfoAct;
	MyAction * useMillisecondsAct;

#if USE_CONFIGURABLE_TOOLBARS
	MyAction * editToolbar1Act;
	MyAction * editControl1Act;
	MyAction * editControl2Act;
	MyAction * editFloatingControlAct;
#endif

	QMenu * toolbar_menu;
	QMenu * statusbar_menu;

	bool fullscreen_toolbar1_was_visible;
	bool compact_toolbar1_was_visible;
#ifdef LANGUAGE_TOOLBAR
	bool fullscreen_toolbar2_was_visible;
	bool compact_toolbar2_was_visible;
#endif
};

#endif
