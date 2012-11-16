/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "skingui.h"
#include "helper.h"
#include "colorutils.h"
#include "core.h"
#include "global.h"
#include "widgetactions.h"
#include "playlist.h"
#include "mplayerwindow.h"
#include "myaction.h"
#include "images.h"
#include "floatingwidget.h"
#include "desktopinfo.h"
#include "editabletoolbar.h"
#include "mediabarpanel.h"
#include "actionseditor.h"

#if DOCK_PLAYLIST
#include "playlistdock.h"
#endif

#include <QMenu>
#include <QSettings>
#include <QLabel>
#include <QStatusBar>
#include <QPushButton>
#include <QToolButton>
#include <QMenuBar>
#include <QLayout>
#include <QApplication>
#include <QDir>

#define TOOLBAR_VERSION 1

using namespace Global;

SkinGui::SkinGui( QWidget * parent, Qt::WindowFlags flags )
	: BaseGuiPlus( parent, flags )
{
	connect( this, SIGNAL(timeChanged(QString)),
             this, SLOT(displayTime(QString)) );

	connect( this, SIGNAL(cursorNearBottom(QPoint)), 
             this, SLOT(showFloatingControl(QPoint)) );
	connect( this, SIGNAL(cursorNearTop(QPoint)), 
             this, SLOT(showFloatingMenu(QPoint)) );
	connect( this, SIGNAL(cursorFarEdges()), 
             this, SLOT(hideFloatingControls()) );

	createActions();
	createMainToolBars();
	createControlWidget();
#if SKIN_CONTROLWIDGET_OVER_VIDEO
	createFloatingControl();
#endif
	createMenus();

#if USE_CONFIGURABLE_TOOLBARS
	connect( editToolbar1Act, SIGNAL(triggered()),
             toolbar1, SLOT(edit()) );
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	floating_control->toolbar()->takeAvailableActionsFrom(this);
	connect( editFloatingControlAct, SIGNAL(triggered()),
             floating_control->toolbar(), SLOT(edit()) );
	#endif
#endif

	retranslateStrings();

	loadConfig();

	//if (playlist_visible) showPlaylist(true);

	if (pref->compact_mode) {
		controlwidget->hide();
		toolbar1->hide();
	}

	statusBar()->hide();

	changeStyleSheet(pref->iconset);
}

SkinGui::~SkinGui() {
	saveConfig();
}

void SkinGui::changeStyleSheet(QString style) {
	if (style.isEmpty())  {
		qApp->setStyleSheet("");
	} 
	else {
		QString qss = Images::styleSheet();
		QDir current = QDir::current();
		QString td = Images::themesDirectory();
		QString relativePath = current.relativeFilePath(td);
		qss.replace(QRegExp("url\\s*\\(\\s*([^\\);]+)\\s*\\)", Qt::CaseSensitive, QRegExp::RegExp2),
							QString("url(%1\\1)").arg(relativePath + "/"));
		//qDebug("qss: %s", qss.toLatin1().constData());
		qApp->setStyleSheet(qss);
	}
}

void SkinGui::createActions() {
	qDebug("SkinGui::createActions");

	timeslider_action = createTimeSliderAction(this);
	timeslider_action->disable();

	volumeslider_action = createVolumeSliderAction(this);
	volumeslider_action->disable();

	// Create the time label
	time_label_action = new TimeLabelAction(this);
	time_label_action->setObjectName("timelabel_action");

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

#if USE_CONFIGURABLE_TOOLBARS
	editToolbar1Act = new MyAction( this, "edit_main_toolbar" );
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	editFloatingControlAct = new MyAction( this, "edit_floating_control" );
	#endif
#endif

	playOrPauseAct->setCheckable(true);
}

#if AUTODISABLE_ACTIONS
void SkinGui::enableActionsOnPlaying() {
	qDebug("SkinGui::enableActionsOnPlaying");
	BaseGuiPlus::enableActionsOnPlaying();

	timeslider_action->enable();
	volumeslider_action->enable();
}

void SkinGui::disableActionsOnStop() {
	qDebug("SkinGui::disableActionsOnStop");
	BaseGuiPlus::disableActionsOnStop();

	timeslider_action->disable();
	volumeslider_action->disable();
}

void SkinGui::togglePlayAction(Core::State state) {
	qDebug("SkinGui::togglePlayAction");
	BaseGuiPlus::togglePlayAction(state);

	if (state == Core::Playing) {
		playOrPauseAct->setChecked(true);
	}
	else {
		playOrPauseAct->setChecked(false);
	}
}
#endif // AUTODISABLE_ACTIONS

void SkinGui::createMenus() {
	menuBar()->setObjectName("menubar");
	QFont font = menuBar()->font();
	font.setPixelSize(11);
	menuBar()->setFont(font);
	menuBar()->setFixedHeight(21);

	toolbar_menu = new QMenu(this);
	toolbar_menu->addAction(toolbar1->toggleViewAction());
#if USE_CONFIGURABLE_TOOLBARS
	toolbar_menu->addSeparator();
	toolbar_menu->addAction(editToolbar1Act);
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	toolbar_menu->addAction(editFloatingControlAct);
	#endif
#endif
	optionsMenu->addSeparator();
	optionsMenu->addMenu(toolbar_menu);
}

QMenu * SkinGui::createPopupMenu() {
	QMenu * m = new QMenu(this);
#if USE_CONFIGURABLE_TOOLBARS
	m->addAction(editToolbar1Act);
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	m->addAction(editFloatingControlAct);
	#endif
#else
	m->addAction(toolbar1->toggleViewAction());
#endif
	return m;
}

void SkinGui::createMainToolBars() {
	toolbar1 = new EditableToolbar( this );
	toolbar1->setObjectName("toolbar");
	toolbar1->setMovable(false);
	toolbar1->setFixedHeight(35);
	addToolBar(Qt::TopToolBarArea, toolbar1);
#if USE_CONFIGURABLE_TOOLBARS
	QStringList toolbar1_actions;
	toolbar1_actions << "open_file" << "open_url" << "favorites_menu" << "separator"
                     << "screenshot" << "separator" << "show_file_properties" 
                     << "show_find_sub_dialog" << "show_tube_browser" << "show_preferences";
	toolbar1->setDefaultActions(toolbar1_actions);
#else
	toolbar1->addAction(openFileAct);
	toolbar1->addAction(openURLAct);
	toolbar1->addSeparator();
	toolbar1->addAction(compactAct);
	toolbar1->addAction(fullscreenAct);
	toolbar1->addSeparator();
	toolbar1->addAction(screenshotAct);
	toolbar1->addSeparator();
	toolbar1->addAction(showPropertiesAct);
	toolbar1->addAction(showFindSubtitlesDialogAct);
	toolbar1->addAction(showPreferencesAct);
	// Test:
	//toolbar1->addSeparator();
	//toolbar1->addAction(timeslider_action);
	//toolbar1->addAction(volumeslider_action);
#endif

	// Modify toolbars' actions
	QAction *tba;
	tba = toolbar1->toggleViewAction();
	tba->setObjectName("show_main_toolbar");
	tba->setShortcut(Qt::Key_F5);
}


void SkinGui::createControlWidget() {
	qDebug("SkinGui::createControlWidget");

	controlwidget = new QToolBar( this );
	controlwidget->setObjectName("controlwidget");
	controlwidget->setStyleSheet("QToolBar { spacing: 0px; }");
	controlwidget->setMovable(false);
	addToolBar(Qt::BottomToolBarArea, controlwidget);

	mediaBarPanel = new MediaBarPanel(panel);
	mediaBarPanel->setObjectName("mediabar-panel");
	mediaBarPanel->setCore(core);
	/* panel->layout()->addWidget(mediaBarPanel); */

	QList<QAction*> actions;
	//actions << halveSpeedAct << playPrevAct << playOrPauseAct << stopAct << recordAct << playNextAct << doubleSpeedAct;
	actions << rewind1Act << playPrevAct << playOrPauseAct << stopAct << playNextAct << forward1Act;
	mediaBarPanel->setPlayControlActionCollection(actions);

	actions.clear();
	//actions << timeslider_action << shuffleAct << repeatPlaylistAct;
	QAction * shuffleAct = ActionsEditor::findAction(playlist, "pl_shuffle");
	QAction * repeatPlaylistAct = ActionsEditor::findAction(playlist, "pl_repeat");
	if (shuffleAct) actions << shuffleAct;
	if (repeatPlaylistAct) actions << repeatPlaylistAct;
	mediaBarPanel->setMediaPanelActionCollection(actions);
	connect(core, SIGNAL(stateChanged(Core::State)), mediaBarPanel, SLOT(setMplayerState(Core::State)));

	actions.clear();
	//actions << volumeslider_action << showPlaylistAct << fullscreenAct << equalizerAct;
	actions << volumeslider_action << showPlaylistAct << fullscreenAct << videoEqualizerAct;
	mediaBarPanel->setVolumeControlActionCollection(actions);

	actions.clear();
	actions << openFileAct << openDirectoryAct << openDVDAct << openURLAct << screenshotAct << showPropertiesAct;
#ifdef FIND_SUBTITLES
	actions << showFindSubtitlesDialogAct;
#endif
	actions << showPreferencesAct;
	mediaBarPanel->setToolbarActionCollection(actions);

	connect(mediaBarPanel, SIGNAL(volumeChanged(int)), core, SLOT(setVolume(int)));
	connect(core, SIGNAL(volumeChanged(int)), mediaBarPanel, SLOT(setVolume(int)));

#ifdef SEEKBAR_RESOLUTION
	connect(mediaBarPanel, SIGNAL(seekerChanged(int)), core, SLOT(goToPosition(int)));
	connect(core, SIGNAL(positionChanged(int)), mediaBarPanel, SLOT(setSeeker(int)));
#else
	connect(mediaBarPanel, SIGNAL(seekerChanged(int)), core, SLOT(goToPos(int)));
	connect(core, SIGNAL(posChanged(int)),mediaBarPanel, SLOT(setSeeker(int)));
#endif

	controlwidget->addWidget(mediaBarPanel);
}

#if SKIN_CONTROLWIDGET_OVER_VIDEO
void SkinGui::createFloatingControl() {
	// Floating control
	floating_control = new FloatingWidget(this);

#if USE_CONFIGURABLE_TOOLBARS
	QStringList floatingcontrol_actions;
	floatingcontrol_actions << "play" << "pause" << "stop" << "separator";
	#if MINI_ARROW_BUTTONS
	floatingcontrol_actions << "rewindbutton_action";
	#else
	floatingcontrol_actions << "rewind3" << "rewind2" << "rewind1";
	#endif
	floatingcontrol_actions << "timeslider_action";
	#if MINI_ARROW_BUTTONS
	floatingcontrol_actions << "forwardbutton_action";
	#else
	floatingcontrol_actions << "forward1" << "forward2" << "forward3";
	#endif
	floatingcontrol_actions << "separator" << "fullscreen" << "mute" << "volumeslider_action" << "separator" << "timelabel_action";
	floating_control->toolbar()->setDefaultActions(floatingcontrol_actions);
#else
	floating_control->toolbar()->addAction(playAct);
	floating_control->toolbar()->addAction(pauseAct);
	floating_control->toolbar()->addAction(stopAct);
	floating_control->toolbar()->addSeparator();

	#if MINI_ARROW_BUTTONS
	floating_control->toolbar()->addAction( rewindbutton_action );
	#else
	floating_control->toolbar()->addAction(rewind3Act);
	floating_control->toolbar()->addAction(rewind2Act);
	floating_control->toolbar()->addAction(rewind1Act);
	#endif

	floating_control->toolbar()->addAction(timeslider_action);

	#if MINI_ARROW_BUTTONS
	floating_control->toolbar()->addAction( forwardbutton_action );
	#else
	floating_control->toolbar()->addAction(forward1Act);
	floating_control->toolbar()->addAction(forward2Act);
	floating_control->toolbar()->addAction(forward3Act);
	#endif

	floating_control->toolbar()->addSeparator();
	floating_control->toolbar()->addAction(fullscreenAct);
	floating_control->toolbar()->addAction(muteAct);
	floating_control->toolbar()->addAction(volumeslider_action);
	floating_control->toolbar()->addSeparator();
	floating_control->toolbar()->addAction(time_label_action);
#endif // USE_CONFIGURABLE_TOOLBARS

#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
	// To make work the ESC key (exit fullscreen) and Ctrl-X (close) in Windows and OS2
	floating_control->addAction(exitFullscreenAct);
	floating_control->addAction(exitAct);
#endif

#if !USE_CONFIGURABLE_TOOLBARS
	floating_control->adjustSize();
#endif
}
#endif

void SkinGui::retranslateStrings() {
	BaseGuiPlus::retranslateStrings();

	toolbar_menu->menuAction()->setText( tr("&Toolbars") );
	toolbar_menu->menuAction()->setIcon( Images::icon("toolbars") );

	toolbar1->setWindowTitle( tr("&Main toolbar") );
	toolbar1->toggleViewAction()->setIcon(Images::icon("main_toolbar"));

#if USE_CONFIGURABLE_TOOLBARS
	editToolbar1Act->change( tr("Edit main &toolbar") );
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	editFloatingControlAct->change( tr("Edit &floating control") );
	#endif
#endif

	mediaBarPanel->setVolume(core->mset.volume);
}

void SkinGui::displayTime(QString text) {
	time_label_action->setText(text);
}

void SkinGui::displayState(Core::State state) {
	BaseGuiPlus::displayState(state);

	switch (state) {
		case Core::Playing:		mediaBarPanel->displayMessage( tr("Playing %1").arg(core->mdat.filename)); break;
		case Core::Paused:		mediaBarPanel->displayMessage( tr("Pause") ); break;
		case Core::Stopped:		mediaBarPanel->displayMessage( tr("Stop") ); break;
	}
}

void SkinGui::displayMessage(QString message) {
	BaseGuiPlus::displayMessage(message);
	mediaBarPanel->displayMessage(message);
}

void SkinGui::updateWidgets() {
	qDebug("SkinGui::updateWidgets");

	BaseGuiPlus::updateWidgets();

	panel->setFocus();
}

void SkinGui::aboutToEnterFullscreen() {
	qDebug("SkinGui::aboutToEnterFullscreen");

	BaseGuiPlus::aboutToEnterFullscreen();

	// Save visibility of toolbars
	fullscreen_toolbar1_was_visible = toolbar1->isVisible();

	if (!pref->compact_mode) {
		controlwidget->hide();
		toolbar1->hide();
	}
}

void SkinGui::aboutToExitFullscreen() {
	qDebug("SkinGui::aboutToExitFullscreen");

	BaseGuiPlus::aboutToExitFullscreen();

#if SKIN_CONTROLWIDGET_OVER_VIDEO
	floating_control->hide();
#endif

	if (!pref->compact_mode) {
		statusBar()->hide();
		controlwidget->show();
		toolbar1->setVisible( fullscreen_toolbar1_was_visible );
	}
}

void SkinGui::aboutToEnterCompactMode() {

	BaseGuiPlus::aboutToEnterCompactMode();

	// Save visibility of toolbars
	compact_toolbar1_was_visible = toolbar1->isVisible();

	controlwidget->hide();
	toolbar1->hide();
}

void SkinGui::aboutToExitCompactMode() {
	BaseGuiPlus::aboutToExitCompactMode();

	statusBar()->hide();
	controlwidget->show();
	toolbar1->setVisible( compact_toolbar1_was_visible );

	// Recheck size of controlwidget
	/* resizeEvent( new QResizeEvent( size(), size() ) ); */
}

void SkinGui::showFloatingControl(QPoint /*p*/) {
	qDebug("SkinGui::showFloatingControl");

#if SKIN_CONTROLWIDGET_OVER_VIDEO
	if ((pref->compact_mode) && (!pref->fullscreen)) {
		floating_control->setAnimated( false );
	} else {
		floating_control->setAnimated( pref->floating_control_animated );
	}
	floating_control->setMargin(pref->floating_control_margin);
	#ifndef Q_OS_WIN
	floating_control->setBypassWindowManager(pref->bypass_window_manager);
	#endif
	floating_control->showOver(panel, pref->floating_control_width);
#else
	if (!controlwidget->isVisible()) {
		controlwidget->show();
	}
#endif
}

void SkinGui::showFloatingMenu(QPoint /*p*/) {
#if !SKIN_CONTROLWIDGET_OVER_VIDEO
	qDebug("SkinGui::showFloatingMenu");

	if (!menuBar()->isVisible())
		menuBar()->show();
#endif
}

void SkinGui::hideFloatingControls() {
	qDebug("SkinGui::hideFloatingControls");

#if SKIN_CONTROLWIDGET_OVER_VIDEO
	floating_control->hide();
#else
	if (controlwidget->isVisible())
		controlwidget->hide();

	if (menuBar()->isVisible())
		menuBar()->hide();
#endif
}

void SkinGui::saveConfig() {
	qDebug("SkinGui::saveConfig");

	QSettings * set = settings;

	set->beginGroup( "skin_gui");

	set->setValue("fullscreen_toolbar1_was_visible", fullscreen_toolbar1_was_visible);
	set->setValue("compact_toolbar1_was_visible", compact_toolbar1_was_visible);

	if (pref->save_window_size_on_exit) {
		qDebug("SkinGui::saveConfig: w: %d h: %d", width(), height());
		set->setValue( "pos", pos() );
		set->setValue( "size", size() );
	}

	set->setValue( "toolbars_state", saveState(Helper::qtVersion()) );

#if USE_CONFIGURABLE_TOOLBARS
	set->beginGroup( "actions" );
	set->setValue("toolbar1", toolbar1->actionsToStringList() );
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	set->setValue("floating_control", floating_control->toolbar()->actionsToStringList() );
	#endif
	set->setValue("toolbar1_version", TOOLBAR_VERSION);
	set->endGroup();
#endif

	set->endGroup();
}

void SkinGui::loadConfig() {
	qDebug("SkinGui::loadConfig");

	QSettings * set = settings;

	set->beginGroup( "skin_gui");

	fullscreen_toolbar1_was_visible = set->value("fullscreen_toolbar1_was_visible", fullscreen_toolbar1_was_visible).toBool();
	compact_toolbar1_was_visible = set->value("compact_toolbar1_was_visible", compact_toolbar1_was_visible).toBool();

	if (pref->save_window_size_on_exit) {
		QPoint p = set->value("pos", pos()).toPoint();
		QSize s = set->value("size", size()).toSize();

		if ( (s.height() < 200) && (!pref->use_mplayer_window) ) {
			s = pref->default_size;
		}

		move(p);
		resize(s);

		if (!DesktopInfo::isInsideScreen(this)) {
			move(0,0);
			qWarning("SkinGui::loadConfig: window is outside of the screen, moved to 0x0");
		}
	}

#if USE_CONFIGURABLE_TOOLBARS
	set->beginGroup( "actions" );
	int toolbar_version = set->value("toolbar1_version", 0).toInt();
	if (toolbar_version >= TOOLBAR_VERSION) {
		toolbar1->setActionsFromStringList( set->value("toolbar1", toolbar1->defaultActions()).toStringList() );
	} else {
		qDebug("SkinGui::loadConfig: toolbar too old, loading default one");
		toolbar1->setActionsFromStringList( toolbar1->defaultActions() );
	}
	#if SKIN_CONTROLWIDGET_OVER_VIDEO
	floating_control->toolbar()->setActionsFromStringList( set->value("floating_control", floating_control->toolbar()->defaultActions()).toStringList() );
	floating_control->adjustSize();
	#endif
	set->endGroup();
#endif

	restoreState( set->value( "toolbars_state" ).toByteArray(), Helper::qtVersion() );

#if DOCK_PLAYLIST
	qDebug("SkinGui::loadConfig: playlist visible: %d", playlistdock->isVisible());
	qDebug("SkinGui::loadConfig: playlist position: %d, %d", playlistdock->pos().x(), playlistdock->pos().y());
	qDebug("SkinGui::loadConfig: playlist size: %d x %d", playlistdock->size().width(), playlistdock->size().height());
#endif

	set->endGroup();

	updateWidgets();
}

#include "moc_skingui.cpp"
