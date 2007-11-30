/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2007 Ricardo Villalba <rvm@escomposlinux.org>

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


#include "prefperformance.h"
#include "images.h"
#include "preferences.h"


PrefPerformance::PrefPerformance(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	// Priority is only for windows, so we disable for other systems
#ifndef Q_OS_WIN
	priority_group->hide();
#endif

	// This option is not good. Playing is awful, even with no H.264 videos.
	skip_frames_check->hide();

	createHelp();
}

PrefPerformance::~PrefPerformance()
{
}

QString PrefPerformance::sectionName() {
	return tr("Performance");
}

QPixmap PrefPerformance::sectionIcon() {
    return Images::icon("pref_performance");
}


void PrefPerformance::retranslateStrings() {
	int priority = priority_combo->currentIndex();

	retranslateUi(this);

	priority_combo->setCurrentIndex(priority);

	createHelp();
}

void PrefPerformance::setData(Preferences * pref) {
	setCacheForFiles( pref->cache_for_files );
	setCacheForStreams( pref->cache_for_streams );
	setCacheForDVDs( pref->cache_for_dvds );

	setPriority( pref->priority );
	setFrameDrop( pref->frame_drop );
	setHardFrameDrop( pref->hard_frame_drop );
	setSkipLoop( pref->h264_skip_loop );
	/* setSkipFrames( pref->h264_skip_frames ); */
	setAutoSyncActivated( pref->autosync );
	setAutoSyncFactor( pref->autosync_factor );
	setFastChapterSeeking( pref->fast_chapter_change );
	setFastAudioSwitching( pref->fast_audio_change );
	setUseIdx( pref->use_idx );
}

void PrefPerformance::getData(Preferences * pref) {
	requires_restart = false;

	TEST_AND_SET(pref->cache_for_files, cacheForFiles());
	TEST_AND_SET(pref->cache_for_streams, cacheForStreams());
	TEST_AND_SET(pref->cache_for_dvds, cacheForDVDs());

	TEST_AND_SET(pref->priority, priority());
	TEST_AND_SET(pref->frame_drop, frameDrop());
	TEST_AND_SET(pref->hard_frame_drop, hardFrameDrop());
	TEST_AND_SET(pref->h264_skip_loop, skipLoop());
	/* TEST_AND_SET(pref->h264_skip_frames, skipFrames()); */
	TEST_AND_SET(pref->autosync, autoSyncActivated());
	TEST_AND_SET(pref->autosync_factor, autoSyncFactor());
	TEST_AND_SET(pref->fast_chapter_change, fastChapterSeeking());
	pref->fast_audio_change = fastAudioSwitching();
	TEST_AND_SET(pref->use_idx, useIdx());
}

void PrefPerformance::setCacheForFiles(int n) {
	cache_files_spin->setValue(n);
}

int PrefPerformance::cacheForFiles() {
	return cache_files_spin->value();
}

void PrefPerformance::setCacheForStreams(int n) {
	cache_streams_spin->setValue(n);
}

int PrefPerformance::cacheForStreams() {
	return cache_streams_spin->value();
}

void PrefPerformance::setCacheForDVDs(int n) {
	cache_dvds_spin->setValue(n);
}

int PrefPerformance::cacheForDVDs() {
	return cache_dvds_spin->value();
}


void PrefPerformance::setPriority(int n) {
	priority_combo->setCurrentIndex(n);
}

int PrefPerformance::priority() {
	return priority_combo->currentIndex();
}

void PrefPerformance::setFrameDrop(bool b) {
	framedrop_check->setChecked(b);
}

bool PrefPerformance::frameDrop() {
	return framedrop_check->isChecked();
}

void PrefPerformance::setHardFrameDrop(bool b) {
	hardframedrop_check->setChecked(b);
}

bool PrefPerformance::hardFrameDrop() {
	return hardframedrop_check->isChecked();
}

void PrefPerformance::setSkipLoop(bool b) {
	skip_loop_filter_check->setChecked(b);
}

bool PrefPerformance::skipLoop() {
	return skip_loop_filter_check->isChecked();
}

void PrefPerformance::setSkipFrames(bool b) {
	skip_frames_check->setChecked(b);
}

bool PrefPerformance::skipFrames() {
	return skip_frames_check->isChecked();
}

void PrefPerformance::setAutoSyncFactor(int factor) {
	autosync_spin->setValue(factor);
}

int PrefPerformance::autoSyncFactor() {
	return autosync_spin->value();
}

void PrefPerformance::setAutoSyncActivated(bool b) {
	autosync_check->setChecked(b);
}

bool PrefPerformance::autoSyncActivated() {
	return autosync_check->isChecked();
}

void PrefPerformance::setFastChapterSeeking(bool b) {
	fast_chapter_check->setChecked(b);
}

bool PrefPerformance::fastChapterSeeking() {
	return fast_chapter_check->isChecked();
}

void PrefPerformance::setFastAudioSwitching(Preferences::OptionState value) {
	fast_audio_combo->setState(value);
}

Preferences::OptionState PrefPerformance::fastAudioSwitching() {
	return fast_audio_combo->state();
}

void PrefPerformance::setUseIdx(bool b) {
	idx_check->setChecked(b);
}

bool PrefPerformance::useIdx() {
	return idx_check->isChecked();
}

void PrefPerformance::createHelp() {
	clearHelp();

	// Performance tab
#ifdef Q_OS_WIN
	setWhatsThis(priority_combo, tr("Priority"), 
		tr("Set process priority for mplayer according to the predefined "
           "priorities available under Windows.<br>"
           "<b>WARNING:</b> Using realtime priority can cause system lockup."));
#endif

	setWhatsThis(framedrop_check, tr("Allow frame drop"),
		tr("Skip displaying some frames to maintain A/V sync on slow systems." ) );

	setWhatsThis(hardframedrop_check, tr("Allow hard frame drop"),
		tr("More intense frame dropping (breaks decoding). "
           "Leads to image distortion!") );

	setWhatsThis(skip_loop_filter_check, tr("Skip loop filter"),
		tr("Skips the loop filter (AKA deblocking) during H.264 decoding. "
           "Since the filtered frame is supposed to be used as reference "
           "for decoding dependent frames this has a worse effect on quality "
           "than not doing deblocking on e.g. MPEG-2 video. But at least for "
           "high bitrate HDTV this provides a big speedup with no visible "
           "quality loss.") );

	/*
	setWhatsThis(skip_frames_check, tr("Skip frames"),
		tr("Skips decoding of frames completely. Big speedup, but jerky "
           "motion and sometimes bad artifacts.") );
	*/

	setWhatsThis(autosync_check, tr("Audio/video auto synchronization"),
		tr("Gradually adjusts the A/V sync based on audio delay "
           "measurements.") );

	setWhatsThis(fast_audio_combo, tr("Fast audio track switching"),
		tr("Possible values:<br> "
           "<b>Yes</b>: it will try the fastest method "
           "to switch the audio track (it might not work with some formats).<br> "
           "<b>No</b>: the MPlayer process will be restarted whenever you "
           "change the audio track.<br> "
           "<b>Auto</b>: SMPlayer will decide what to do according to the "
           "MPlayer version." ) );

	setWhatsThis(fast_chapter_check, tr("Fast seek to chapters in dvds"),
		tr("If checked, it will try the fastest method to seek to chapters "
           "but it might not work with some discs.") );

	setWhatsThis(idx_check, tr("Create index if needed"),
		tr("Rebuilds index of files if no index was found, allowing seeking. "
		   "Useful with broken/incomplete downloads, or badly created files. "
           "This option only works if the underlying media supports "
           "seeking (i.e. not with stdin, pipe, etc).<br> "
           "Note: the creation of the index may take some time.") );

	setWhatsThis(cache_files_spin, tr("Cache for files"), 
		tr("This option specifies how much memory (in kBytes) to use when "
           "precaching a file.") );

	setWhatsThis(cache_streams_spin, tr("Cache for streams"), 
		tr("This option specifies how much memory (in kBytes) to use when "
           "precaching a URL.") );

	setWhatsThis(cache_dvds_spin, tr("Cache for DVDs"), 
		tr("This option specifies how much memory (in kBytes) to use when "
           "precaching a DVD.<br><b>Warning:</b> Seeking might not work "
           "properly (including chapter switching) when using a cache for DVDs.") );
}

#include "moc_prefperformance.cpp"
