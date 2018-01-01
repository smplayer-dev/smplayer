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

#include "defaultgui.h"
#include "helper.h"
#include "colorutils.h"
#include "core.h"
#include "global.h"
#include "widgetactions.h"
#include "playlist.h"
#include "mplayerwindow.h"
#include "myaction.h"
#include "images.h"
#include "autohidewidget.h"
#include "desktopinfo.h"
#include "editabletoolbar.h"

#if !USE_CONFIGURABLE_TOOLBARS
#include "favorites.h"
#endif

#if DOCK_PLAYLIST
#include "playlistdock.h"
#endif

#ifdef BUFFERING_ANIMATION
#include "statewidget.h"
#endif

#include <QMenu>
#include <QSettings>
#include <QLabel>
#include <QStatusBar>
#include <QPushButton>
#include <QToolButton>
#include <QMenuBar>
#include <QMovie>
#include <QtCore/qmath.h>

#define TOOLBAR_VERSION "2"
#define CONTROLWIDGET_VERSION "1"
#define CONTROLWIDGETMINI_VERSION "1"
#define FLOATING_CONTROL_VERSION "1"

using namespace Global;

DefaultGui::DefaultGui( QWidget * parent, Qt::WindowFlags flags )
	: BaseGuiPlus( parent, flags )
{
	createStatusBar();

	connect( core, SIGNAL(showTime(double)), this, SLOT(displayTime(double)));
	connect( this, SIGNAL(frameChanged(int)),
             this, SLOT(displayFrame(int)) );
	connect( this, SIGNAL(ABMarkersChanged(int,int)),
             this, SLOT(displayABSection(int,int)) );
	connect( this, SIGNAL(videoInfoChanged(int,int,double)),
             this, SLOT(displayVideoInfo(int,int,double)) );
	connect( core, SIGNAL(bitrateChanged(int,int)), this, SLOT(displayBitrateInfo(int,int)) );

	createActions();
	createMainToolBars();
	createControlWidget();
	createControlWidgetMini();
	createFloatingControl();
	createMenus();

#if USE_CONFIGURABLE_TOOLBARS
	connect( editToolbar1Act, SIGNAL(triggered()),
             toolbar1, SLOT(edit()) );
	connect( editControl1Act, SIGNAL(triggered()),
             controlwidget, SLOT(edit()) );
	connect( editControl2Act, SIGNAL(triggered()),
             controlwidget_mini, SLOT(edit()) );
	EditableToolbar * iw = static_cast<EditableToolbar *>(floating_control->internalWidget());
	iw->takeAvailableActionsFrom(this);
	connect( editFloatingControlAct, SIGNAL(triggered()),
             iw, SLOT(edit()) );
#endif

	connect(this, SIGNAL(preferencesChanged()), this, SLOT(checkCompactMode()));

	menuBar()->setObjectName("menubar");

	retranslateStrings();

	loadConfig();

	//if (playlist_visible) showPlaylist(true);

	if (pref->compact_mode) {
		controlwidget->hide();
		toolbar1->hide();
		#ifdef LANGUAGE_TOOLBAR
		toolbar2->hide();
		#endif

		if (pref->floating_display_in_compact_mode) {
			reconfigureFloatingControl();
			floating_control->activate();
		}
	}

	applyStyles();

#ifdef ADD_QUICK_ACCESS
	connect(this, SIGNAL(tabletModeChanged(bool)), this, SLOT(adaptForTabletMode()));
	adaptForTabletMode();
#endif
}

DefaultGui::~DefaultGui() {
	qDebug("DefaultGui::~DefaultGui");
	saveConfig();
}

/*
void DefaultGui::closeEvent( QCloseEvent * ) {
	qDebug("DefaultGui::closeEvent");

	//BaseGuiPlus::closeEvent(e);
	qDebug("w: %d h: %d", width(), height() );
}
*/

void DefaultGui::createActions() {
	qDebug("DefaultGui::createActions");

	timeslider_action = createTimeSliderAction(this);
	volumeslider_action = createVolumeSliderAction(this);

#if AUTODISABLE_ACTIONS
	timeslider_action->disable();
	volumeslider_action->disable();
#endif

	// Create the time label
	time_label_action = createTimeLabelAction(TimeLabelAction::CurrentAndTotalTime, this);
	time_label_action->setObjectName("timelabel_action");

	current_time_label_action = createTimeLabelAction(TimeLabelAction::CurrentTime, this);
	current_time_label_action->setObjectName("current_timelabel_action");

	total_time_label_action = createTimeLabelAction(TimeLabelAction::TotalTime, this);
	total_time_label_action->setObjectName("total_timelabel_action");

	remaining_time_label_action = createTimeLabelAction(TimeLabelAction::RemainingTime, this);
	remaining_time_label_action->setObjectName("remaining_timelabel_action");

#if MINI_ARROW_BUTTONS
	QList<QAction*> rewind_actions;
	rewind_actions << rewind1Act << rewind2Act << rewind3Act;
	rewindbutton_action = new SeekingButton(rewind_actions, this);
	rewindbutton_action->setObjectName("rewindbutton_action");

	QList<QAction*> forward_actions;
	forward_actions << forward1Act << forward2Act << forward3Act;
	forwardbutton_action = new SeekingButton(forward_actions, this);
	forwardbutton_action->setObjectName("forwardbutton_action");
#endif

	// Statusbar
	viewVideoInfoAct = new MyAction(this, "toggle_video_info" );
	viewVideoInfoAct->setCheckable(true);
	connect( viewVideoInfoAct, SIGNAL(toggled(bool)),
             video_info_display, SLOT(setVisible(bool)) );

	viewFrameCounterAct = new MyAction( this, "toggle_frame_counter" );
	viewFrameCounterAct->setCheckable( true );
	connect( viewFrameCounterAct, SIGNAL(toggled(bool)),
             frame_display, SLOT(setVisible(bool)) );

	viewFormatInfoAct = new MyAction( this, "toggle_format_info" );
	viewFormatInfoAct->setCheckable( true );
	connect( viewFormatInfoAct, SIGNAL(toggled(bool)),
             format_info_display, SLOT(setVisible(bool)) );

	viewBitrateInfoAct = new MyAction( this, "toggle_bitrate_info" );
	viewBitrateInfoAct->setCheckable( true );
	connect( viewBitrateInfoAct, SIGNAL(toggled(bool)),
             bitrate_info_display, SLOT(setVisible(bool)) );

	useMillisecondsAct = new MyAction( this, "use_milliseconds" );
	useMillisecondsAct->setCheckable( true );

#if USE_CONFIGURABLE_TOOLBARS
	editToolbar1Act = new MyAction( this, "edit_main_toolbar" );
	editControl1Act = new MyAction( this, "edit_control1" );
	editControl2Act = new MyAction( this, "edit_control2" );
	editFloatingControlAct = new MyAction( this, "edit_floating_control" );
#endif
}

#if AUTODISABLE_ACTIONS
void DefaultGui::enableActionsOnPlaying() {
	qDebug("DefaultGui::enableActionsOnPlaying");
	BaseGuiPlus::enableActionsOnPlaying();

	timeslider_action->enable();
	volumeslider_action->enable();
}

void DefaultGui::disableActionsOnStop() {
	qDebug("DefaultGui::disableActionsOnStop");
	BaseGuiPlus::disableActionsOnStop();

	timeslider_action->disable();
	volumeslider_action->disable();
}
#endif // AUTODISABLE_ACTIONS

void DefaultGui::togglePlayAction(Core::State state) {
	qDebug("DefaultGui::togglePlayAction");
	BaseGui::togglePlayAction(state);

	if (state == Core::Playing) {
		playOrPauseAct->setIcon(Images::icon("pause"));
	} else {
		playOrPauseAct->setIcon(Images::icon("play"));
	}
}

void DefaultGui::createMenus() {
	toolbar_menu = new QMenu(this);
	toolbar_menu->addAction(toolbar1->toggleViewAction());
#ifdef LANGUAGE_TOOLBAR
	toolbar_menu->addAction(toolbar2->toggleViewAction());
#endif

#if USE_CONFIGURABLE_TOOLBARS
	toolbar_menu->addSeparator();
	toolbar_menu->addAction(editToolbar1Act);
	toolbar_menu->addAction(editControl1Act);
	toolbar_menu->addAction(editControl2Act);
	toolbar_menu->addAction(editFloatingControlAct);
#endif

	statusbar_menu = new QMenu(this);
	statusbar_menu->addAction(viewVideoInfoAct);
	statusbar_menu->addAction(viewFormatInfoAct);
	statusbar_menu->addAction(viewBitrateInfoAct);
	statusbar_menu->addAction(viewFrameCounterAct);
	statusbar_menu->addAction(useMillisecondsAct);

	populateMainMenu();
}

void DefaultGui::populateMainMenu() {
	qDebug("DefaultGui::populateMainMenu");

	BaseGuiPlus::populateMainMenu();

	optionsMenu->addSeparator();
	optionsMenu->addMenu(toolbar_menu);
	optionsMenu->addMenu(statusbar_menu);
}

QMenu * DefaultGui::createPopupMenu() {
	QMenu * m = new QMenu(this);
#if USE_CONFIGURABLE_TOOLBARS
	m->addAction(editToolbar1Act);
	m->addAction(editControl1Act);
	m->addAction(editControl2Act);
	m->addAction(editFloatingControlAct);
#else
	m->addAction(toolbar1->toggleViewAction());
	#ifdef LANGUAGE_TOOLBAR
	m->addAction(toolbar2->toggleViewAction());
	#endif
#endif
	return m;
}

void DefaultGui::createMainToolBars() {
	toolbar1 = new EditableToolbar( this );
	toolbar1->setObjectName("toolbar1");
	//toolbar1->setMovable(false);
	addToolBar(Qt::TopToolBarArea, toolbar1);
#if USE_CONFIGURABLE_TOOLBARS
	QStringList toolbar1_actions;
	toolbar1_actions << "open_file" << "open_url" << "favorites_menu" << "separator"
                     << "screenshot" << "separator" << "show_file_properties" << "show_playlist"
                     #ifndef IDOPT_BUILD
                     << "show_tube_browser"
                     #endif
                     << "separator" << "show_preferences"
                     #ifndef IDOPT_BUILD
                     << "separator" << "play_prev" << "play_next"
                     #endif
                     << "separator" << "audiotrack_menu" << "subtitlestrack_menu"
                     #ifdef IDOPT_BUILD
                     << "separator" << "send_to_screen_menu" << "tablet_mode"
                     #endif
                     ;

	toolbar1->setDefaultActions(toolbar1_actions);
#else
	toolbar1->addAction(openFileAct);
	/* toolbar1->addAction(openDVDAct); */
	toolbar1->addAction(openURLAct);
	toolbar1->addAction(favorites->menuAction());
	toolbar1->addSeparator();
	/*
	toolbar1->addAction(compactAct);
	toolbar1->addAction(fullscreenAct);
	toolbar1->addSeparator();
	*/
	toolbar1->addAction(screenshotAct);
	toolbar1->addSeparator();
	toolbar1->addAction(showPropertiesAct);
	toolbar1->addAction(showPlaylistAct);
	toolbar1->addAction(showPreferencesAct);
	toolbar1->addSeparator();
	toolbar1->addAction(playPrevAct);
	toolbar1->addAction(playNextAct);
	toolbar1->addSeparator();
	toolbar1->addAction(audiotrack_menu->menuAction());
	toolbar1->addAction(subtitles_track_menu->menuAction());

	// Test:
	//toolbar1->addSeparator();
	//toolbar1->addAction(timeslider_action);
	//toolbar1->addAction(volumeslider_action);

	QToolButton * button = qobject_cast<QToolButton *>(toolbar1->widgetForAction(favorites->menuAction()));
	button->setPopupMode(QToolButton::InstantPopup);

	button = qobject_cast<QToolButton *>(toolbar1->widgetForAction(audiotrack_menu->menuAction()));
	button->setPopupMode(QToolButton::InstantPopup);

	button = qobject_cast<QToolButton *>(toolbar1->widgetForAction(subtitles_track_menu->menuAction()));
	button->setPopupMode(QToolButton::InstantPopup);

	#ifdef IDOPT_BUILD
	toolbar1->addSeparator();
	toolbar1->addAction(sendToScreen_menu->menuAction());
	toolbar1->addAction(tabletModeAct);

	button = qobject_cast<QToolButton *>(toolbar1->widgetForAction(sendToScreen_menu->menuAction()));
	button->setPopupMode(QToolButton::InstantPopup);
	#endif
#endif

#ifdef LANGUAGE_TOOLBAR
	toolbar2 = new QToolBar( this );
	toolbar2->setObjectName("toolbar2");
	//toolbar2->setMovable(false);
	addToolBar(Qt::TopToolBarArea, toolbar2);

	select_audio = new QPushButton( this );
	select_audio->setMenu( audiotrack_menu );
	toolbar2->addWidget(select_audio);

	select_subtitle = new QPushButton( this );
	select_subtitle->setMenu( subtitles_track_menu );
	toolbar2->addWidget(select_subtitle);
#endif

	/*
	toolbar1->show();
	toolbar2->show();
	*/

	// Modify toolbars' actions
	QAction *tba;
	tba = toolbar1->toggleViewAction();
	tba->setObjectName("show_main_toolbar");
	tba->setShortcut(Qt::Key_F5);

#ifdef LANGUAGE_TOOLBAR
	tba = toolbar2->toggleViewAction();
	tba->setObjectName("show_language_toolbar");
	tba->setShortcut(Qt::Key_F6);
#endif

	toolbar1->setIconSize(QSize(32,32));
}


void DefaultGui::createControlWidgetMini() {
	qDebug("DefaultGui::createControlWidgetMini");

	controlwidget_mini = new EditableToolbar( this );
	controlwidget_mini->setObjectName("controlwidget_mini");
	controlwidget_mini->setLayoutDirection(Qt::LeftToRight);
	//controlwidget_mini->setResizeEnabled(false);
	controlwidget_mini->setMovable(false);
	//addDockWindow(controlwidget_mini, Qt::DockBottom );
	addToolBar(Qt::BottomToolBarArea, controlwidget_mini);

#if USE_CONFIGURABLE_TOOLBARS
	QStringList controlwidget_mini_actions;
	controlwidget_mini_actions << "play_or_pause" << "stop" << "separator" << "rewind1" << "timeslider_action" 
                               << "forward1" << "separator" << "mute" << "volumeslider_action";
	controlwidget_mini->setDefaultActions(controlwidget_mini_actions);
#else
	controlwidget_mini->addAction(playOrPauseAct);
	controlwidget_mini->addAction(stopAct);
	controlwidget_mini->addSeparator();

	controlwidget_mini->addAction(rewind1Act);

	controlwidget_mini->addAction(timeslider_action);

	controlwidget_mini->addAction(forward1Act);

	controlwidget_mini->addSeparator();

	controlwidget_mini->addAction(muteAct );

	controlwidget_mini->addAction(volumeslider_action);
#endif // USE_CONFIGURABLE_TOOLBARS

	controlwidget_mini->hide();
}

void DefaultGui::createControlWidget() {
	qDebug("DefaultGui::createControlWidget");

	controlwidget = new EditableToolbar( this );
	controlwidget->setObjectName("controlwidget");
	controlwidget->setLayoutDirection(Qt::LeftToRight);
	//controlwidget->setResizeEnabled(false);
	controlwidget->setMovable(false);
	//addDockWindow(controlwidget, Qt::DockBottom );
	addToolBar(Qt::BottomToolBarArea, controlwidget);

#if USE_CONFIGURABLE_TOOLBARS
	QStringList controlwidget_actions;
	controlwidget_actions << "play_or_pause" << "stop" << "separator";
	#if MINI_ARROW_BUTTONS
	controlwidget_actions << "rewindbutton_action";
	#else
	controlwidget_actions << "rewind3" << "rewind2" << "rewind1";
	#endif
	controlwidget_actions << "timeslider_action";
	#if MINI_ARROW_BUTTONS
	controlwidget_actions << "forwardbutton_action";
	#else
	controlwidget_actions << "forward1" << "forward2" << "forward3";
	#endif
	controlwidget_actions << "separator" << "fullscreen" << "mute" << "volumeslider_action";

	#ifdef ADD_QUICK_ACCESS
	controlwidget_actions << "quick_access_menu";
	#endif

	controlwidget->setDefaultActions(controlwidget_actions);
#else
	/*
	controlwidget->addAction(playAct);
	controlwidget->addAction(pauseAndStepAct);
	*/
	controlwidget->addAction(playOrPauseAct);
	controlwidget->addAction(stopAct);

	controlwidget->addSeparator();

	#if MINI_ARROW_BUTTONS
	controlwidget->addAction( rewindbutton_action );
	#else
	controlwidget->addAction(rewind3Act);
	controlwidget->addAction(rewind2Act);
	controlwidget->addAction(rewind1Act);
	#endif

	controlwidget->addAction(timeslider_action);

	#if MINI_ARROW_BUTTONS
	controlwidget->addAction( forwardbutton_action );
	#else
	controlwidget->addAction(forward1Act);
	controlwidget->addAction(forward2Act);
	controlwidget->addAction(forward3Act);
	#endif

	controlwidget->addSeparator();

	controlwidget->addAction(fullscreenAct);
	controlwidget->addAction(muteAct);

	controlwidget->addAction(volumeslider_action);

	#ifdef ADD_QUICK_ACCESS
	controlwidget->addAction(access_menu->menuAction());
	QToolButton * button = qobject_cast<QToolButton *>(controlwidget->widgetForAction(access_menu->menuAction()));
	button->setPopupMode(QToolButton::InstantPopup);
	#endif
#endif // USE_CONFIGURABLE_TOOLBARS

	/*
	controlwidget->show();
	*/

	controlwidget->setIconSize(QSize(40,40));
}

void DefaultGui::createFloatingControl() {
	// Floating control
	floating_control = new AutohideWidget(mplayerwindow);
	floating_control->setAutoHide(true);

	EditableToolbar * iw = new EditableToolbar(floating_control);
	iw->setObjectName("floating_control");
	connect(iw, SIGNAL(iconSizeChanged(const QSize &)), this, SLOT(adjustFloatingControlSize()));

#if USE_CONFIGURABLE_TOOLBARS
	QStringList floatingcontrol_actions;
	floatingcontrol_actions << "play_or_pause" << "stop" << "separator";
	/*
	#if MINI_ARROW_BUTTONS
	floatingcontrol_actions << "rewindbutton_action";
	#else
	floatingcontrol_actions << "rewind3" << "rewind2" << "rewind1";
	#endif
	*/
	floatingcontrol_actions << "rewind1";
	floatingcontrol_actions << "current_timelabel_action" << "timeslider_action" << "total_timelabel_action";
	/*
	#if MINI_ARROW_BUTTONS
	floatingcontrol_actions << "forwardbutton_action";
	#else
	floatingcontrol_actions << "forward1" << "forward2" << "forward3";
	#endif
	*/
	floatingcontrol_actions << "forward1";
	floatingcontrol_actions << "separator" << "fullscreen" << "mute" << "volumeslider_action";
	iw->setDefaultActions(floatingcontrol_actions);
#else
	/*
	iw->addAction(playAct);
	iw->addAction(pauseAct);
	*/
	iw->addAction(playOrPauseAct);
	iw->addAction(stopAct);
	iw->addSeparator();

	#if MINI_ARROW_BUTTONS
	iw->addAction( rewindbutton_action );
	#else
	iw->addAction(rewind3Act);
	iw->addAction(rewind2Act);
	iw->addAction(rewind1Act);
	#endif

	iw->addAction(current_time_label_action);
	iw->addAction(timeslider_action);
	iw->addAction(total_time_label_action);

	#if MINI_ARROW_BUTTONS
	iw->addAction( forwardbutton_action );
	#else
	iw->addAction(forward1Act);
	iw->addAction(forward2Act);
	iw->addAction(forward3Act);
	#endif

	iw->addSeparator();
	iw->addAction(fullscreenAct);
	iw->addAction(muteAct);
	iw->addAction(volumeslider_action);
	/*
	iw->addSeparator();
	iw->addAction(time_label_action);
	*/
#endif // USE_CONFIGURABLE_TOOLBARS

	floating_control->setInternalWidget(iw);
	iw->setIconSize(QSize(48,48));

#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	// To make work the ESC key (exit fullscreen) and Ctrl-X (close) in Windows and OS2
	/*
	floating_control->addAction(exitFullscreenAct);
	floating_control->addAction(exitAct);
	*/
	//floating_control->addActions(actions());
#endif

#if !USE_CONFIGURABLE_TOOLBARS
	floating_control->adjustSize();
#endif

	floating_control->hide();
}

void DefaultGui::createStatusBar() {
	qDebug("DefaultGui::createStatusBar");

	time_display = new QLabel( statusBar() );
	time_display->setObjectName("time_display");
	time_display->setAlignment(Qt::AlignRight);
	time_display->setFrameShape(QFrame::NoFrame);
	time_display->setText(" 88:88:88 / 88:88:88 ");
	time_display->setMinimumSize(time_display->sizeHint());
	//connect(this, SIGNAL(timeChanged(QString)), time_display, SLOT(setText(QString)));

	frame_display = new QLabel( statusBar() );
	frame_display->setObjectName("frame_display");
	frame_display->setAlignment(Qt::AlignRight);
	frame_display->setFrameShape(QFrame::NoFrame);
	frame_display->setText("88888888");
	frame_display->setMinimumSize(frame_display->sizeHint());

	ab_section_display = new QLabel( statusBar() );
	ab_section_display->setObjectName("ab_section_display");
	ab_section_display->setAlignment(Qt::AlignRight);
	ab_section_display->setFrameShape(QFrame::NoFrame);
//	ab_section_display->setText("A:0:00:00 B:0:00:00");
//	ab_section_display->setMinimumSize(ab_section_display->sizeHint());

	video_info_display = new QLabel( statusBar() );
	video_info_display->setObjectName("video_info_display");
	video_info_display->setAlignment(Qt::AlignRight);
	video_info_display->setFrameShape(QFrame::NoFrame);

	format_info_display = new QLabel( statusBar() );
	format_info_display->setObjectName("format_info_display");
	format_info_display->setAlignment(Qt::AlignRight);
	format_info_display->setFrameShape(QFrame::NoFrame);

	bitrate_info_display = new QLabel( statusBar() );
	bitrate_info_display->setObjectName("bitrate_info_display");
	bitrate_info_display->setAlignment(Qt::AlignRight);
	bitrate_info_display->setFrameShape(QFrame::NoFrame);

#ifdef BUFFERING_ANIMATION
	state_widget = new StateWidget(statusBar());
	connect(core, SIGNAL(stateChanged(Core::State)), state_widget, SLOT(watchState(Core::State)));
	statusBar()->addPermanentWidget(state_widget);
#endif

	statusBar()->setAutoFillBackground(true);

	/*
	ColorUtils::setBackgroundColor( statusBar(), QColor(0,0,0) );
	ColorUtils::setForegroundColor( statusBar(), QColor(255,255,255) );
	ColorUtils::setBackgroundColor( time_display, QColor(0,0,0) );
	ColorUtils::setForegroundColor( time_display, QColor(255,255,255) );
	ColorUtils::setBackgroundColor( frame_display, QColor(0,0,0) );
	ColorUtils::setForegroundColor( frame_display, QColor(255,255,255) );
	ColorUtils::setBackgroundColor( ab_section_display, QColor(0,0,0) );
	ColorUtils::setForegroundColor( ab_section_display, QColor(255,255,255) );
	ColorUtils::setBackgroundColor( video_info_display, QColor(0,0,0) );
	ColorUtils::setForegroundColor( video_info_display, QColor(255,255,255) );
	*/
	statusBar()->setSizeGripEnabled(false);

	statusBar()->addPermanentWidget( format_info_display );
	statusBar()->addPermanentWidget( bitrate_info_display );
	statusBar()->addPermanentWidget( ab_section_display );
	statusBar()->addPermanentWidget( video_info_display );

	statusBar()->showMessage( tr("Ready") );
	statusBar()->addPermanentWidget( frame_display, 0 );
	frame_display->setText( "0" );

	statusBar()->addPermanentWidget( time_display, 0 );
	time_display->setText(" 00:00:00 / 00:00:00 ");

	time_display->show();
	frame_display->hide();
	ab_section_display->show();
	video_info_display->hide();
	format_info_display->hide();
	bitrate_info_display->hide();
}

void DefaultGui::retranslateStrings() {
	BaseGuiPlus::retranslateStrings();

	// Change the icon of the play/pause action
	playOrPauseAct->setIcon(Images::icon("play"));

	toolbar_menu->menuAction()->setText( tr("&Toolbars") );
	toolbar_menu->menuAction()->setIcon( Images::icon("toolbars") );

	statusbar_menu->menuAction()->setText( tr("Status&bar") );
	statusbar_menu->menuAction()->setIcon( Images::icon("statusbar") );

	toolbar1->setWindowTitle( tr("&Main toolbar") );
	toolbar1->toggleViewAction()->setIcon(Images::icon("main_toolbar"));

#ifdef LANGUAGE_TOOLBAR
	toolbar2->setWindowTitle( tr("&Language toolbar") );
	toolbar2->toggleViewAction()->setIcon(Images::icon("lang_toolbar"));

	select_audio->setText( tr("Audio") );
	select_subtitle->setText( tr("Subtitle") );
#endif

	viewVideoInfoAct->change(Images::icon("view_video_info"), tr("&Video info") );
	viewFrameCounterAct->change( Images::icon("frame_counter"), tr("&Frame counter") );
	viewFormatInfoAct->change( Images::icon("view_format_info"), tr("F&ormat info") );
	viewBitrateInfoAct->change( Images::icon("view_bitrate_info"), tr("&Bitrate info") );
	useMillisecondsAct->change( Images::icon("use_milliseconds"), tr("&Show the current time with milliseconds") );

#if USE_CONFIGURABLE_TOOLBARS
	editToolbar1Act->change( tr("Edit main &toolbar") );
	editControl1Act->change( tr("Edit &control bar") );
	editControl2Act->change( tr("Edit m&ini control bar") );
	editFloatingControlAct->change( tr("Edit &floating control") );
#endif

#ifdef BUFFERING_ANIMATION
	state_widget->setAnimation(Images::file("buffering.gif"));
#endif
}


void DefaultGui::displayTime(double sec) {
	//qDebug() << "DefaultGui::displayTime:" << sec;

	static int last_second = 0;
	QString time;

	if (useMillisecondsAct->isChecked()) {
		time = Helper::formatTime2(sec) + " / " + Helper::formatTime( (int) core->mdat.duration );
	} else {
		if (qFloor(sec) == last_second) return; // Update only once per second
		last_second = qFloor(sec);
		time = Helper::formatTime( (int) sec ) + " / " + Helper::formatTime( (int) core->mdat.duration );
	}
	time_display->setText(time);
}

void DefaultGui::displayFrame(int frame) {
	if (frame_display->isVisible()) {
		frame_display->setNum( frame );
	}
}

void DefaultGui::displayABSection(int secs_a, int secs_b) {
	QString s;
	if (secs_a > -1) s = tr("A:%1").arg(Helper::formatTime(secs_a));

	if (secs_b > -1) {
		if (!s.isEmpty()) s += " ";
		s += tr("B:%1").arg(Helper::formatTime(secs_b));
	}

	ab_section_display->setText( s );

	ab_section_display->setVisible( !s.isEmpty() );
}

void DefaultGui::displayVideoInfo(int width, int height, double fps) {
	if ((width != 0) && (height != 0)) {
		video_info_display->setText(tr("%1x%2 %3 fps", "width + height + fps").arg(width).arg(height).arg(fps));
	} else {
		video_info_display->setText(" ");
	}

	QString format = core->mdat.video_format;
	if (!format.isEmpty() && !core->mdat.audio_format.isEmpty()) format += " / ";
	format += core->mdat.audio_format;
	format_info_display->setText(format.toUpper());
}

void DefaultGui::displayBitrateInfo(int vbitrate, int abitrate) {
	bitrate_info_display->setText(tr("V: %1 kbps A: %2 kbps").arg(vbitrate/1000).arg(abitrate/1000));
}

void DefaultGui::updateWidgets() {
	qDebug("DefaultGui::updateWidgets");

	BaseGuiPlus::updateWidgets();

	viewFrameCounterAct->setEnabled((PlayerID::player(pref->mplayer_bin) == PlayerID::MPLAYER));
	viewBitrateInfoAct->setEnabled((PlayerID::player(pref->mplayer_bin) == PlayerID::MPV));

	panel->setFocus();
}

void DefaultGui::checkCompactMode() {
	qDebug("DefaultGui::checkCompactMode");

	if ((pref->compact_mode) && (pref->floating_display_in_compact_mode)) {
		reconfigureFloatingControl();
		floating_control->activate();
	} else {
		floating_control->deactivate();
	}
}

#ifdef ADD_QUICK_ACCESS
void DefaultGui::adaptForTabletMode() {
	qDebug("DefaultGui::adaptForTabletMode");

	bool b = pref->tablet_mode;

	if (!pref->compact_mode) {
		menuBar()->setVisible(!b);
		toolbar1->setVisible(!b);
	}
	access_menu->menuAction()->setVisible(b);
}
#endif

void DefaultGui::reconfigureFloatingControl() {
	floating_control->setMargin(pref->floating_control_margin);
	floating_control->setPercWidth(pref->floating_control_width);
	floating_control->setAnimated(pref->floating_control_animated);
	floating_control->setActivationArea( (AutohideWidget::Activation) pref->floating_activation_area);
	floating_control->setHideDelay(pref->floating_hide_delay);
}

void DefaultGui::aboutToEnterFullscreen() {
	qDebug("DefaultGui::aboutToEnterFullscreen");

	BaseGuiPlus::aboutToEnterFullscreen();

	// Show floating_control
	reconfigureFloatingControl();
	floating_control->deactivate(); // Hide the control in case it was running from compact mode
	QTimer::singleShot(100, floating_control, SLOT(activate()));


	// Save visibility of toolbars
	fullscreen_toolbar1_was_visible = toolbar1->isVisible();
#ifdef LANGUAGE_TOOLBAR
	fullscreen_toolbar2_was_visible = toolbar2->isVisible();
#endif

	if (!pref->compact_mode) {
		//menuBar()->hide();
		//statusBar()->hide();
		controlwidget->hide();
		controlwidget_mini->hide();
		toolbar1->hide();
		#ifdef LANGUAGE_TOOLBAR
		toolbar2->hide();
		#endif
	}
}

void DefaultGui::aboutToExitFullscreen() {
	qDebug("DefaultGui::aboutToExitFullscreen");

	BaseGuiPlus::aboutToExitFullscreen();

	// Hide floating_control
	if (!pref->compact_mode || !pref->floating_display_in_compact_mode) {
		floating_control->deactivate();
	}

	if (!pref->compact_mode) {
		//menuBar()->show();
		//statusBar()->show();
		controlwidget->show();

		toolbar1->setVisible( fullscreen_toolbar1_was_visible );
		#ifdef LANGUAGE_TOOLBAR
		toolbar2->setVisible( fullscreen_toolbar2_was_visible );
		#endif
	}

	#ifdef ADD_QUICK_ACCESS
	if (pref->tablet_mode) menuBar()->hide();
	#endif
}

void DefaultGui::aboutToEnterCompactMode() {

	BaseGuiPlus::aboutToEnterCompactMode();

	// Show floating_control
	if (pref->floating_display_in_compact_mode) {
		reconfigureFloatingControl();
		QTimer::singleShot(100, floating_control, SLOT(activate()));
	}


	// Save visibility of toolbars
	compact_toolbar1_was_visible = toolbar1->isVisible();
#ifdef LANGUAGE_TOOLBAR
	compact_toolbar2_was_visible = toolbar2->isVisible();
#endif

	//menuBar()->hide();
	//statusBar()->hide();
	controlwidget->hide();
	controlwidget_mini->hide();
	toolbar1->hide();
#ifdef LANGUAGE_TOOLBAR
	toolbar2->hide();
#endif
}

void DefaultGui::aboutToExitCompactMode() {
	BaseGuiPlus::aboutToExitCompactMode();

	// Hide floating_control
	if (pref->floating_display_in_compact_mode) {
		floating_control->deactivate();
	}

	//menuBar()->show();
	//statusBar()->show();
	controlwidget->show();

	toolbar1->setVisible( compact_toolbar1_was_visible );
#ifdef LANGUAGE_TOOLBAR
	toolbar2->setVisible( compact_toolbar2_was_visible );
#endif

	#ifdef ADD_QUICK_ACCESS
	if (pref->tablet_mode) menuBar()->hide();
	#endif

	// Recheck size of controlwidget
	resizeEvent( new QResizeEvent( size(), size() ) );
}

void DefaultGui::resizeEvent( QResizeEvent * ) {
	/*
	qDebug("defaultGui::resizeEvent");
	qDebug(" controlwidget width: %d", controlwidget->width() );
	qDebug(" controlwidget_mini width: %d", controlwidget_mini->width() );
	*/

#if QT_VERSION < 0x040000
#define LIMIT 470
#else
#define LIMIT 570
#endif

	if ( (controlwidget->isVisible()) && (width() < LIMIT) ) {
		controlwidget->hide();
		controlwidget_mini->show();
	}
	else
	if ( (controlwidget_mini->isVisible()) && (width() > LIMIT) ) {
		controlwidget_mini->hide();
		controlwidget->show();
	}
}

#if USE_MINIMUMSIZE
QSize DefaultGui::minimumSizeHint() const {
	return QSize(controlwidget_mini->sizeHint().width(), 0);
}
#endif

void DefaultGui::adjustFloatingControlSize() {
	qDebug("DefaultGui::adjustFloatingControlSize");
	//floating_control->adjustSize();
	QWidget *iw = floating_control->internalWidget();
	if (iw) {
		QSize iws = iw->size();
		QMargins m = floating_control->contentsMargins();
		int new_height = iws.height() + m.top() + m.bottom();
		if (new_height < 32) new_height = 32;
		floating_control->resize(floating_control->width(), new_height);
	}
}

void DefaultGui::saveConfig() {
	qDebug("DefaultGui::saveConfig");

	QSettings * set = settings;

	set->beginGroup( "default_gui");

	set->setValue("video_info", viewVideoInfoAct->isChecked());
	set->setValue("frame_counter", viewFrameCounterAct->isChecked());
	set->setValue("format_info", viewFormatInfoAct->isChecked());
	set->setValue("bitrate_info", viewBitrateInfoAct->isChecked());
	set->setValue("use_milliseconds", useMillisecondsAct->isChecked());

	set->setValue("fullscreen_toolbar1_was_visible", fullscreen_toolbar1_was_visible);
	set->setValue("compact_toolbar1_was_visible", compact_toolbar1_was_visible);
#ifdef LANGUAGE_TOOLBAR
	set->setValue("fullscreen_toolbar2_was_visible", fullscreen_toolbar2_was_visible);
	set->setValue("compact_toolbar2_was_visible", compact_toolbar2_was_visible);
#endif

	if (pref->save_window_size_on_exit) {
		qDebug("DefaultGui::saveConfig: w: %d h: %d", width(), height());
		set->setValue( "pos", pos() );
		set->setValue( "size", size() );
		set->setValue( "state", (int) windowState() );
	}

	set->setValue( "toolbars_state", saveState(Helper::qtVersion()) );

#if USE_CONFIGURABLE_TOOLBARS
	set->beginGroup( "actions" );
	set->setValue("toolbar1/" TOOLBAR_VERSION, toolbar1->actionsToStringList() );
	set->setValue("controlwidget/" CONTROLWIDGET_VERSION, controlwidget->actionsToStringList() );
	set->setValue("controlwidget_mini/" CONTROLWIDGETMINI_VERSION, controlwidget_mini->actionsToStringList() );
	EditableToolbar * iw = static_cast<EditableToolbar *>(floating_control->internalWidget());
	set->setValue("floating_control/" FLOATING_CONTROL_VERSION, iw->actionsToStringList() );
	set->endGroup();

	set->beginGroup("toolbars_icon_size");
	set->setValue("toolbar1", toolbar1->iconSize());
	set->setValue("controlwidget", controlwidget->iconSize());
	set->setValue("controlwidget_mini", controlwidget_mini->iconSize());
	set->setValue("floating_control", iw->iconSize());
	set->endGroup();
#endif

	set->endGroup();
}

void DefaultGui::loadConfig() {
	qDebug("DefaultGui::loadConfig");

	QSettings * set = settings;

	set->beginGroup( "default_gui");

	viewVideoInfoAct->setChecked(set->value("video_info", false).toBool());
	viewFrameCounterAct->setChecked(set->value("frame_counter", false).toBool());
	viewFormatInfoAct->setChecked(set->value("format_info", false).toBool());
	viewBitrateInfoAct->setChecked(set->value("bitrate_info", false).toBool());
	useMillisecondsAct->setChecked(set->value("use_milliseconds", false).toBool());

	fullscreen_toolbar1_was_visible = set->value("fullscreen_toolbar1_was_visible", fullscreen_toolbar1_was_visible).toBool();
	compact_toolbar1_was_visible = set->value("compact_toolbar1_was_visible", compact_toolbar1_was_visible).toBool();
#ifdef LANGUAGE_TOOLBAR
	fullscreen_toolbar2_was_visible = set->value("fullscreen_toolbar2_was_visible", fullscreen_toolbar2_was_visible).toBool();
	compact_toolbar2_was_visible = set->value("compact_toolbar2_was_visible", compact_toolbar2_was_visible).toBool();
#endif

	if (pref->save_window_size_on_exit) {
		QPoint p = set->value("pos", pos()).toPoint();
		QSize s = set->value("size", size()).toSize();

		if ( (s.height() < 70) && (!pref->use_mplayer_window) ) {
			s = pref->default_size;
		}

		move(p);
		resize(s);

		setWindowState( (Qt::WindowStates) set->value("state", 0).toInt() );

		if (!DesktopInfo::isInsideScreen(this)) {
			QPoint tl = DesktopInfo::topLeftPrimaryScreen();
			move(tl);
			qWarning("DefaultGui::loadConfig: window is outside of the screen, moved to %d x %d", tl.x(), tl.y());
		}
	}

#if USE_CONFIGURABLE_TOOLBARS
	set->beginGroup( "actions" );
	toolbar1->setActionsFromStringList( set->value("toolbar1/" TOOLBAR_VERSION, toolbar1->defaultActions()).toStringList() );

	{
	QStringList l = set->value("controlwidget/" CONTROLWIDGET_VERSION, controlwidget->defaultActions()).toStringList();
	#ifdef ADD_QUICK_ACCESS
	if (l.indexOf("quick_access_menu") == -1) l << "quick_access_menu";
	#endif
	controlwidget->setActionsFromStringList(l);
	}

	controlwidget_mini->setActionsFromStringList( set->value("controlwidget_mini/" CONTROLWIDGETMINI_VERSION, controlwidget_mini->defaultActions()).toStringList() );

	EditableToolbar * iw = static_cast<EditableToolbar *>(floating_control->internalWidget());
	iw->setActionsFromStringList( set->value("floating_control/" FLOATING_CONTROL_VERSION, iw->defaultActions()).toStringList() );
	set->endGroup();

	set->beginGroup("toolbars_icon_size");
	toolbar1->setIconSize(set->value("toolbar1", toolbar1->iconSize()).toSize());
	controlwidget->setIconSize(set->value("controlwidget", controlwidget->iconSize()).toSize());
	controlwidget_mini->setIconSize(set->value("controlwidget_mini", controlwidget_mini->iconSize()).toSize());
	iw->setIconSize(set->value("floating_control", iw->iconSize()).toSize());
	set->endGroup();

	floating_control->adjustSize();
#endif

	restoreState( set->value( "toolbars_state" ).toByteArray(), Helper::qtVersion() );

#if DOCK_PLAYLIST
	qDebug("DefaultGui::loadConfig: playlist visible: %d", playlistdock->isVisible());
	qDebug("DefaultGui::loadConfig: playlist position: %d, %d", playlistdock->pos().x(), playlistdock->pos().y());
	qDebug("DefaultGui::loadConfig: playlist size: %d x %d", playlistdock->size().width(), playlistdock->size().height());
#endif

	set->endGroup();

	updateWidgets();
}

#include "moc_defaultgui.cpp"
