/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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


#include "prefadvanced.h"
#include "images.h"
#include "preferences.h"
#include "paths.h"
#include "playerid.h"
#include <QColorDialog>

#define LOGS_TAB 3

PrefAdvanced::PrefAdvanced(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

#ifndef Q_OS_WIN
	shortnames_check->hide();
#endif

#if !USE_COLORKEY
	colorkey_label->hide();
	colorkey_view->hide();
	changeButton->hide();
#endif

#if !REPAINT_BACKGROUND_OPTION
	repaint_video_background_check->hide();
#endif

#ifndef LOG_MPLAYER
	mplayer_log_box->hide();
#endif

#ifndef LOG_SMPLAYER
	smplayer_log_box->hide();
#endif

#ifndef MPLAYER_SUPPORT
	use_playlist_check->hide();
#endif

#if !defined(LOG_MPLAYER) && !defined(LOG_SMPLAYER)
	advanced_tab->setTabEnabled(LOGS_TAB, false);
#endif

	// Monitor aspect
	monitoraspect_combo->addItem("Auto");
	monitoraspect_combo->addItem("4:3");
	monitoraspect_combo->addItem("16:9");
	monitoraspect_combo->addItem("5:4");
	monitoraspect_combo->addItem("16:10");

	retranslateStrings();
}

PrefAdvanced::~PrefAdvanced()
{
}

QString PrefAdvanced::sectionName() {
	return tr("Advanced");
}

QPixmap PrefAdvanced::sectionIcon() {
	return Images::icon("pref_advanced");
}


void PrefAdvanced::retranslateStrings() {
	retranslateUi(this);

	monitor_aspect_icon->setPixmap( Images::icon("monitor") );

	monitoraspect_combo->setItemText(0, tr("Auto") );

	mplayer_use_window_check->setText( tr("&Run %1 in its own window").arg(PLAYER_NAME) );
	shortnames_check->setText( tr("&Pass short filenames (8+3) to %1").arg(PLAYER_NAME) );
	mplayer_crashes_check->setText( tr("Notify %1 crash&es").arg(PLAYER_NAME) );
	advanced_tab->setTabText(1, PLAYER_NAME);
	player_group->setTitle(tr("Options for %1").arg(PLAYER_NAME));
	options_info_label->setText( tr("Here you can pass options and filters to %1.").arg(PLAYER_NAME) );
	mplayer_log_box->setTitle(PLAYER_NAME);
	log_mplayer_check->setText( tr("Log %1 &output").arg(PLAYER_NAME) );
	log_mplayer_save_check->setText( tr("A&utosave %1 log to file").arg(PLAYER_NAME) );

	use_playlist_check->setText( tr("Pa&ss the %1 option to MPlayer (security risk)").arg("-playlist") );

	osd_pos_label->setNum(osd_pos_slider->value());

	createHelp();
}

void PrefAdvanced::setData(Preferences * pref) {
	setMonitorAspect( pref->monitor_aspect );

#if REPAINT_BACKGROUND_OPTION
	setRepaintVideoBackground( pref->repaint_video_background );
#endif
	setUseMplayerWindow( pref->use_mplayer_window );
	setMplayerAdditionalArguments( pref->mplayer_additional_options );
	setMplayerAdditionalVideoFilters( pref->mplayer_additional_video_filters );
	setMplayerAdditionalAudioFilters( pref->mplayer_additional_audio_filters );
#if USE_COLORKEY
	setColorKey( pref->color_key );
#endif
	setPreferIpv4( pref->prefer_ipv4 );
	setUseIdx( pref->use_idx );

	setUseLavfDemuxer(pref->use_lavf_demuxer);

	setUseCorrectPts( pref->use_correct_pts );
	setActionsToRun( pref->actions_to_run );
	setShowTagInTitle( pref->show_tag_in_window_title );

	osd_pos_slider->setValue(pref->osd_bar_pos);

#ifdef LOG_MPLAYER
	setLogMplayer( pref->log_mplayer );
	setMplayerLogVerbose( pref->verbose_log );
	setSaveMplayerLog( pref->autosave_mplayer_log );
	setMplayerLogName( pref->mplayer_log_saveto );
#endif

#ifdef LOG_SMPLAYER
	setLogSmplayer( pref->log_smplayer );
	setLogFilter( pref->log_filter );
	setSaveSmplayerLog( pref->save_smplayer_log );
#endif

	setUseShortNames( pref->use_short_pathnames );

	setMplayerCrashes( pref->report_mplayer_crashes );

#ifdef MPLAYER_SUPPORT
	use_playlist_check->setChecked(pref->use_playlist_option);
#endif
}

void PrefAdvanced::getData(Preferences * pref) {
	requires_restart = false;

#if REPAINT_BACKGROUND_OPTION
	repaint_video_background_changed = false;
#endif

	monitor_aspect_changed = false;
#if USE_COLORKEY
	colorkey_changed = false;
#endif
	lavf_demuxer_changed = false;

	pref->prefer_ipv4 = preferIpv4();
	TEST_AND_SET(pref->use_idx, useIdx());

	if (pref->use_lavf_demuxer != useLavfDemuxer()) {
		pref->use_lavf_demuxer = useLavfDemuxer();
		lavf_demuxer_changed = true;
		requires_restart = true;
	}

	TEST_AND_SET(pref->use_correct_pts, useCorrectPts());
	pref->actions_to_run = actionsToRun();
	//TEST_AND_SET(pref->show_tag_in_window_title, showTagInTitle());
	pref->show_tag_in_window_title = showTagInTitle(); // TODO: detect change and apply

	TEST_AND_SET(pref->osd_bar_pos, osd_pos_slider->value());

	if (pref->monitor_aspect != monitorAspect()) {
		pref->monitor_aspect = monitorAspect();
		monitor_aspect_changed = true;
		requires_restart = true;
	}

#if REPAINT_BACKGROUND_OPTION
	if (pref->repaint_video_background != repaintVideoBackground()) {
		pref->repaint_video_background = repaintVideoBackground();
		repaint_video_background_changed = true;
    }
#endif

	TEST_AND_SET(pref->use_mplayer_window, useMplayerWindow());
	TEST_AND_SET(pref->mplayer_additional_options, mplayerAdditionalArguments());
	TEST_AND_SET(pref->mplayer_additional_video_filters, mplayerAdditionalVideoFilters());
	TEST_AND_SET(pref->mplayer_additional_audio_filters, mplayerAdditionalAudioFilters());
#if USE_COLORKEY
	if (pref->color_key != colorKey()) {
		pref->color_key = colorKey();
		colorkey_changed = true;
		requires_restart = true;
	}
#endif

#ifdef LOG_MPLAYER
	pref->log_mplayer = logMplayer();
	TEST_AND_SET( pref->verbose_log, mplayerLogVerbose() );
	pref->autosave_mplayer_log = saveMplayerLog();
	pref->mplayer_log_saveto = mplayerLogName();
#endif

#ifdef LOG_SMPLAYER
	pref->log_smplayer = logSmplayer();
	pref->log_filter = logFilter();
	pref->save_smplayer_log = saveSmplayerLog();
#endif

	pref->use_short_pathnames = useShortNames();

	pref->report_mplayer_crashes = mplayerCrashes();

#ifdef MPLAYER_SUPPORT
	pref->use_playlist_option = use_playlist_check->isChecked();
#endif
}

void PrefAdvanced::setMonitorAspect(QString asp) {
	if (asp.isEmpty())
		monitoraspect_combo->setCurrentIndex( 0 );
	else
		monitoraspect_combo->setCurrentText(asp);
		//monitoraspect_combo->setEditText(asp);
}

QString PrefAdvanced::monitorAspect() {
	if (monitoraspect_combo->currentIndex() == 0 ) 
		return "";
	else
		return monitoraspect_combo->currentText();
}

#if REPAINT_BACKGROUND_OPTION
void PrefAdvanced::setRepaintVideoBackground(bool b) {
	repaint_video_background_check->setChecked(b);
}

bool PrefAdvanced::repaintVideoBackground() {
	return repaint_video_background_check->isChecked();
}
#endif

void PrefAdvanced::setUseMplayerWindow(bool v) {
	mplayer_use_window_check->setChecked(v);
}

bool PrefAdvanced::useMplayerWindow() {
	return mplayer_use_window_check->isChecked();
}

void PrefAdvanced::setUseShortNames(bool b) {
	shortnames_check->setChecked(b);
}

bool PrefAdvanced::useShortNames() {
	return shortnames_check->isChecked();
}

void PrefAdvanced::setMplayerCrashes(bool b) {
	mplayer_crashes_check->setChecked(b);
}

bool PrefAdvanced::mplayerCrashes() {
	return mplayer_crashes_check->isChecked();
}

void PrefAdvanced::setMplayerAdditionalArguments(QString args) {
	mplayer_args_edit->setText(args);
}

QString PrefAdvanced::mplayerAdditionalArguments() {
	return mplayer_args_edit->text();
}

void PrefAdvanced::setMplayerAdditionalVideoFilters(QString s) {
	mplayer_vfilters_edit->setText(s);
}

QString PrefAdvanced::mplayerAdditionalVideoFilters() {
	return mplayer_vfilters_edit->text();
}

void PrefAdvanced::setMplayerAdditionalAudioFilters(QString s) {
	mplayer_afilters_edit->setText(s);
}

QString PrefAdvanced::mplayerAdditionalAudioFilters() {
	return mplayer_afilters_edit->text();
}

#if USE_COLORKEY
void PrefAdvanced::setColorKey(unsigned int c) {
	QString color = QString::number(c, 16);
	while (color.length() < 6) color = "0"+color;
	colorkey_view->setText( "#" + color );
}

unsigned int PrefAdvanced::colorKey() {
	QString c = colorkey_view->text();
	if (c.startsWith("#")) c = c.mid(1);

	bool ok;
	unsigned int color = c.toUInt(&ok, 16);

	if (!ok) 
		qWarning("PrefAdvanced::colorKey: cannot convert color to uint");

	qDebug("PrefAdvanced::colorKey: color: %s", QString::number(color,16).toUtf8().data() );

	return color;
}
#endif

void PrefAdvanced::setPreferIpv4(bool b) {
	if (b) 
		ipv4_button->setChecked(true);
	else 
		ipv6_button->setChecked(true);
}

bool PrefAdvanced::preferIpv4() {
	return ipv4_button->isChecked();
}

void PrefAdvanced::setUseIdx(bool b) {
	idx_check->setChecked(b);
}

bool PrefAdvanced::useIdx() {
	return idx_check->isChecked();
}

void PrefAdvanced::setUseLavfDemuxer(bool b) {
	lavf_demuxer_check->setChecked(b);
}

bool PrefAdvanced::useLavfDemuxer() {
	return lavf_demuxer_check->isChecked();
}

void PrefAdvanced::setUseCorrectPts(Preferences::OptionState value) {
	correct_pts_combo->setState(value);
}

Preferences::OptionState PrefAdvanced::useCorrectPts() {
	return correct_pts_combo->state();
}

void PrefAdvanced::setActionsToRun(QString actions) {
	actions_to_run_edit->setText(actions);
}

QString PrefAdvanced::actionsToRun() {
	return actions_to_run_edit->text();
}

void PrefAdvanced::setShowTagInTitle(bool b) {
	show_tag_in_title_check->setChecked(b);
}

bool PrefAdvanced::showTagInTitle() {
	return show_tag_in_title_check->isChecked();
}

void PrefAdvanced::on_changeButton_clicked() {
	//bool ok;
	//int color = colorkey_view->text().toUInt(&ok, 16);
	QColor color( colorkey_view->text() );
	QColor c = QColorDialog::getColor ( color, this );
	if (c.isValid()) {
		//colorkey_view->setText( QString::number( c.rgb(), 16 ) );
		colorkey_view->setText( c.name() );
	}
}

// Log options
#ifdef LOG_MPLAYER
void PrefAdvanced::setLogMplayer(bool b) {
	log_mplayer_check->setChecked(b);
}

bool PrefAdvanced::logMplayer() {
	return log_mplayer_check->isChecked();
}

void PrefAdvanced::setMplayerLogVerbose(bool b) {
	verbose_check->setChecked(b);
}

bool PrefAdvanced::mplayerLogVerbose() {
	return verbose_check->isChecked();
}

void PrefAdvanced::setSaveMplayerLog(bool b) {
    log_mplayer_save_check->setChecked(b);
}

bool PrefAdvanced::saveMplayerLog() {
    return log_mplayer_save_check->isChecked();
}

void PrefAdvanced::setMplayerLogName(QString filter) {
    log_mplayer_save_name->setText(filter);
}

QString PrefAdvanced::mplayerLogName() {
    return log_mplayer_save_name->text();
}
#endif

#ifdef LOG_SMPLAYER
void PrefAdvanced::setLogSmplayer(bool b) {
	log_smplayer_check->setChecked(b);
}

bool PrefAdvanced::logSmplayer() {
	return log_smplayer_check->isChecked();
}

void PrefAdvanced::setLogFilter(QString filter) {
	log_filter_edit->setText(filter);
}

QString PrefAdvanced::logFilter() {
	return log_filter_edit->text();
}

void PrefAdvanced::setSaveSmplayerLog(bool b) {
	log_smplayer_save_check->setChecked(b);
}

bool PrefAdvanced::saveSmplayerLog(){
    return log_smplayer_save_check->isChecked();
}
#endif

void PrefAdvanced::createHelp() {
	clearHelp();

	addSectionTitle(tr("Advanced"));

	setWhatsThis(monitoraspect_combo, tr("Monitor aspect"),
        tr("Select the aspect ratio of your monitor.") );

	setWhatsThis(idx_check, tr("Rebuild index if needed"),
		tr("Rebuilds index of files if no index was found, allowing seeking. "
		   "Useful with broken/incomplete downloads, or badly created files. "
           "This option only works if the underlying media supports "
           "seeking (i.e. not with stdin, pipe, etc).<br> "
           "<b>Note:</b> the creation of the index may take some time.") );

	setWhatsThis(lavf_demuxer_check, tr("Use the lavf demuxer by default"),
		tr("If this option is checked, the lavf demuxer will be used for all formats."));

#if REPAINT_BACKGROUND_OPTION
	setWhatsThis(repaint_video_background_check,
		tr("Repaint the background of the video window"),
		tr("Unchecking this option may reduce flickering, but it can also produce "
           "strange artifacts under certain circumstances.") );
#endif


	setWhatsThis(correct_pts_combo, tr("Correct pts"),
		tr("Switches %1 to an experimental mode where timestamps for "
           "video frames are calculated differently and video filters which "
           "add new frames or modify timestamps of existing ones are "
           "supported. The more accurate timestamps can be visible for "
           "example when playing subtitles timed to scene changes with the "
           "SSA/ASS library enabled. Without correct pts the subtitle timing "
           "will typically be off by some frames. This option does not work "
           "correctly with some demuxers and codecs.").arg(PLAYER_NAME) );

	setWhatsThis(actions_to_run_edit, tr("Actions list"),
		tr("Here you can specify a list of <i>actions</i> which will be "
           "run every time a file is opened. You'll find all available "
           "actions in the key shortcut editor in the <b>Keyboard and mouse</b> "
           "section. The actions must be separated by spaces. Checkable "
           "actions can be followed by <i>true</i> or <i>false</i> to "
           "enable or disable the action.") +"<br>"+
		tr("Example:") +" <i>auto_zoom compact true</i><br>" +
		tr("Limitation: the actions are run only when a file is opened and "
           "not when the %1 process is restarted (e.g. you select an "
           "audio or video filter).").arg(PLAYER_NAME) );

	setWhatsThis(osd_pos_slider, tr("OSD bar position"),
		tr("Set the position of the screen where the OSD bar is displayed. 0 is top, 100 bottom."));

	setWhatsThis(show_tag_in_title_check, tr("Show tag info in window title"),
		tr("If this option is enabled, information from tags will be "
		   "shown in window title. "
           "Otherwise only the filename will be shown.") );

#if USE_COLORKEY
	setWhatsThis(colorkey_view, tr("Colorkey"),
        tr("If you see parts of the video over any other window, you can "
           "change the colorkey to fix it. Try to select a color close to "
           "black.") );
#endif


	addSectionTitle(PLAYER_NAME);

	setWhatsThis(mplayer_args_edit, tr("Options"),
        tr("Here you can type options for %1.").arg(PLAYER_NAME) +" "+
        tr("Write them separated by spaces.") +"<br>"+ tr("Example:") +" -flip -nosound");

	setWhatsThis(mplayer_vfilters_edit, tr("Video filters"),
        tr("Here you can add video filters for %1.").arg(PLAYER_NAME) +" "+
        tr("Write them separated by commas. Don't use spaces!") +"<br>"+ tr("Example:") +" scale=512:-2,mirror");

	setWhatsThis(mplayer_afilters_edit, tr("Audio filters"),
        tr("Here you can add audio filters for %1.").arg(PLAYER_NAME) +" "+
        tr("Write them separated by commas. Don't use spaces!") +"<br>"+ tr("Example:") +" extrastereo,karaoke");

	setWhatsThis(mplayer_use_window_check, tr("Run %1 in its own window").arg(PLAYER_NAME),
        tr("If you check this option, the %1 video window won't be "
           "embedded in SMPlayer's main window but instead it will use its "
           "own window. Note that mouse and keyboard events will be handled "
           "directly by %1, that means key shortcuts and mouse clicks "
           "probably won't work as expected when the %1 window has the "
           "focus.").arg(PLAYER_NAME) );

	setWhatsThis(mplayer_crashes_check,
		tr("Notify %1 crashes").arg(PLAYER_NAME),
		tr("If this option is checked, a popup window will be displayed to inform "
           "about %1 crashes. Otherwise those failures will be "
           "silently ignored.").arg(PLAYER_NAME) );

#ifdef MPLAYER_SUPPORT
	setWhatsThis(use_playlist_check, tr("Pass the %1 option to MPlayer (security risk)").arg("-playlist"),
		tr("This option may be needed to play playlist files (m3u, pls...). "
           "However it can involve a security risk when playing internet sources because "
           "the way MPlayer parses and uses playlist files is not "
           "safe against maliciously constructed files.") );
#endif

#ifdef Q_OS_WIN
	setWhatsThis(shortnames_check, tr("Pass short filenames (8+3) to %1").arg(PLAYER_NAME),
		tr("If this option is checked, SMPlayer will pass to %1 the short version of the filenames.").arg(PLAYER_NAME) );
#endif


	addSectionTitle(tr("Network"));

	setWhatsThis(ipv4_button, tr("IPv4"),
		tr("Use IPv4 on network connections. Falls back on IPv6 automatically."));

	setWhatsThis(ipv6_button, tr("IPv6"),
		tr("Use IPv6 on network connections. Falls back on IPv4 automatically."));

	addSectionTitle(tr("Logs"));

#ifdef LOG_SMPLAYER
	setWhatsThis(log_smplayer_check, tr("Log SMPlayer output"),
		tr("If this option is checked, SMPlayer will store the debugging "
           "messages that SMPlayer outputs "
           "(you can see the log in <b>Options -> View logs -> SMPlayer</b>). "
           "This information can be very useful for the developer in case "
           "you find a bug." ) );

	setWhatsThis(log_smplayer_save_check, tr("Save SMPlayer log to file"),
		tr("If this option is checked, the SMPlayer log wil be recorded to %1")
          .arg( "<i>"+ Paths::configPath() + "/smplayer_log.txt</i>" ) );
#endif

#ifdef LOG_MPLAYER
	setWhatsThis(log_mplayer_check, tr("Log %1 output").arg(PLAYER_NAME),
		tr("If checked, SMPlayer will store the output of %1 "
           "(you can see it in <b>Options -> View logs -> %1</b>). "
           "In case of problems this log can contain important information, "
           "so it's recommended to keep this option checked.").arg(PLAYER_NAME) );

	setWhatsThis(log_mplayer_save_check, tr("Autosave %1 log").arg(PLAYER_NAME),
		tr("If this option is checked, the %1 log will be saved to the "
           "specified file every time a new file starts to play. "
           "It's intended for external applications, so they can get "
           "info about the file you're playing.").arg(PLAYER_NAME) );

	setWhatsThis(log_mplayer_save_name, tr("Autosave %1 log filename").arg(PLAYER_NAME),
 		tr("Enter here the path and filename that will be used to save the "
           "%1 log.").arg(PLAYER_NAME) );
#endif

#ifdef LOG_SMPLAYER
	setWhatsThis(log_filter_edit, tr("Filter for SMPlayer logs"),
		tr("This option allows to filter the SMPlayer messages that will "
           "be stored in the log. Here you can write any regular expression.<br>"
           "For instance: <i>^Core::.*</i> will display only the lines "
           "starting with <i>Core::</i>") );
#endif
}

#include "moc_prefadvanced.cpp"
