/*  Mpcgui for SMPlayer.
    Copyright (C) 2008 matt_ <matt@endboss.org>

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

#ifndef MPC_GUI_H
#define MPC_GUI_H

#include "baseguiplus.h"
#include "guiconfig.h"

#define USE_VOLUME_BAR 1

class TimeSliderAction;
class VolumeSliderAction;
class AutohideWidget;
class QSpacerItem;
class QToolBar;

class MpcGui : public BaseGuiPlus
{
	Q_OBJECT

public:
	MpcGui( QWidget* parent = 0, Qt::WindowFlags flags = QFlag(0) );
	~MpcGui();

#if USE_MPCMUMSIZE
	virtual QSize mpcmumSizeHint () const;
#endif

protected slots:
    void muteIconChange(bool b);
    void iconChange(Core::State state);
    void updateAudioChannels();

    void displayFrame(int frame);
    void showFullscreenControls();
    void hideFullscreenControls();
    void setJumpTexts();
    void updateWidgets();

	// Reimplemented:
#if AUTODISABLE_ACTIONS
	virtual void enableActionsOnPlaying();
	virtual void disableActionsOnStop();
#endif

protected:
	virtual void retranslateStrings();

	void createActions();
	void createControlWidget();
	void createFloatingControl();
    void createStatusBar();

    void setupIcons();

	void loadConfig();
	void saveConfig();

	// Reimplemented
	virtual void aboutToEnterFullscreen();
	virtual void aboutToExitFullscreen();
	virtual void aboutToEnterCompactMode();
	virtual void aboutToExitCompactMode();

protected:
	QToolBar* controlwidget;
    QToolBar* timeslidewidget;

	QLabel * audiochannel_display;
	QLabel * time_display;
	QLabel * frame_display;

	QLabel * floating_control_time;

	AutohideWidget * floating_control;
	QSpacerItem * spacer;

	TimeSliderAction * timeslider_action;
#if USE_VOLUME_BAR
	VolumeSliderAction * volumeslider_action;
#endif
};

#endif
