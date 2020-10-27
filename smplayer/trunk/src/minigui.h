/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef MINI_GUI_H
#define MINI_GUI_H

#include "baseguiplus.h"
#include "guiconfig.h"

#define USE_VOLUME_BAR 1

class TimeSliderAction;
class VolumeSliderAction;
class TimeLabelAction;
class AutohideWidget;
class EditableToolbar;

class MiniGui : public BaseGuiPlus
{
	Q_OBJECT

public:
	MiniGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	~MiniGui();

#if USE_MINIMUMSIZE
	virtual QSize minimumSizeHint () const;
#endif

protected slots:
	// Reimplemented:
#if AUTODISABLE_ACTIONS
	virtual void enableActionsOnPlaying();
	virtual void disableActionsOnStop();
#endif
	virtual void togglePlayAction(Core::State state);

	void adjustFloatingControlSize();

protected:
	virtual void retranslateStrings();
#if USE_CONFIGURABLE_TOOLBARS
	virtual QMenu * createPopupMenu();
#endif

	void createActions();
	void createControlWidget();
	void createFloatingControl();

	void loadConfig();
	void saveConfig();

	// Reimplemented
	virtual void aboutToEnterFullscreen();
	virtual void aboutToExitFullscreen();
	virtual void aboutToEnterCompactMode();
	virtual void aboutToExitCompactMode();

protected:
	EditableToolbar * controlwidget;

	AutohideWidget * floating_control;

	TimeSliderAction * timeslider_action;
#if USE_VOLUME_BAR
	VolumeSliderAction * volumeslider_action;
#endif
	TimeLabelAction * time_label_action;
	TimeLabelAction * current_time_label_action;
	TimeLabelAction * total_time_label_action;
	TimeLabelAction * remaining_time_label_action;

#if USE_CONFIGURABLE_TOOLBARS
	MyAction * editControlAct;
	MyAction * editFloatingControlAct;
#endif
};

#endif
