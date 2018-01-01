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

#ifndef SKINGUI_H
#define SKINGUI_H

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
class MediaBarPanel;

#if MINI_ARROW_BUTTONS
class SeekingButton;
#endif

//#define SKIN_EDITABLE_CONTROL 1

class SkinGui : public BaseGuiPlus
{
	Q_OBJECT

public:
	SkinGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	~SkinGui();

public slots:
	//virtual void showPlaylist(bool b);

protected:
	virtual void retranslateStrings();
	virtual QMenu * createPopupMenu();

	void createMainToolBars();
	void createControlWidget();
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

protected slots:
	virtual void updateWidgets();
	virtual void displayState(Core::State state);
	virtual void displayMessage(QString message, int time);
	virtual void displayMessage(QString message);

	// Reimplemented:
#if AUTODISABLE_ACTIONS
	virtual void enableActionsOnPlaying();
	virtual void disableActionsOnStop();
#endif
	virtual void togglePlayAction(Core::State);

protected:
	MediaBarPanel* mediaBarPanel;
	QAction * mediaBarPanelAction;

	EditableToolbar * toolbar1;
	QToolBar * controlwidget;

	TimeSliderAction * timeslider_action;
	VolumeSliderAction * volumeslider_action;

#if MINI_ARROW_BUTTONS
	SeekingButton * rewindbutton_action;
	SeekingButton * forwardbutton_action;
#endif

	AutohideWidget * floating_control;
	TimeLabelAction * time_label_action;

#if USE_CONFIGURABLE_TOOLBARS
	MyAction * editToolbar1Act;
	#if defined(SKIN_EDITABLE_CONTROL)
	MyAction * editFloatingControlAct;
	#endif
#endif

	MyAction * viewVideoInfoAct;
	MyAction * scrollTitleAct;

	QMenu * toolbar_menu;
	QMenu * statusbar_menu;

	int last_second;

	bool fullscreen_toolbar1_was_visible;
	bool compact_toolbar1_was_visible;

	bool was_muted;
};

#endif
