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


#include "prefperformance.h"
#include "images.h"
#include "global.h"
#include "preferences.h"

#ifdef YOUTUBE_SUPPORT
#include "retrieveyoutubeurl.h"
#endif

using namespace Global;

PrefPerformance::PrefPerformance(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	// Priority is only for windows, so we disable for other systems
#ifndef Q_OS_WIN
	priority_group->hide();
#endif

#if SMART_DVD_CHAPTERS
	fast_chapter_check->hide();
#endif

#ifdef YOUTUBE_SUPPORT
	yt_quality_combo->addItem( "240p (flv)", RetrieveYoutubeUrl::FLV_240p );

	yt_quality_combo->addItem( "360p (flv)", RetrieveYoutubeUrl::FLV_360p );
	yt_quality_combo->addItem( "360p (mp4)", RetrieveYoutubeUrl::MP4_360p );
	yt_quality_combo->addItem( "360p (webm)", RetrieveYoutubeUrl::WEBM_360p );

	yt_quality_combo->addItem( "480p (flv)", RetrieveYoutubeUrl::FLV_480p );
	yt_quality_combo->addItem( "480p (webm)", RetrieveYoutubeUrl::WEBM_480p );

	yt_quality_combo->addItem( "720p (mp4)", RetrieveYoutubeUrl::MP4_720p );
	yt_quality_combo->addItem( "720p (webm)", RetrieveYoutubeUrl::WEBM_720p );

	yt_quality_combo->addItem( "1080p (mp4)", RetrieveYoutubeUrl::MP4_1080p );
	yt_quality_combo->addItem( "1080p (webm)", RetrieveYoutubeUrl::WEBM_1080p );
#else
	yt_label->hide();
	yt_quality_combo->hide();
	yt_line->hide();
#endif

	retranslateStrings();
}

PrefPerformance::~PrefPerformance()
{
}

QString PrefPerformance::sectionName() {
	return tr("Performance");
}

QPixmap PrefPerformance::sectionIcon() {
    return Images::icon("pref_performance", 22);
}


void PrefPerformance::retranslateStrings() {
	int priority = priority_combo->currentIndex();
	int loop_filter = loopfilter_combo->currentIndex();

	retranslateUi(this);

	loopfilter_combo->clear();
	loopfilter_combo->addItem( tr("Enabled"), Preferences::LoopEnabled );
	loopfilter_combo->addItem( tr("Skip (always)"), Preferences::LoopDisabled );
	loopfilter_combo->addItem( tr("Skip only on HD videos"), Preferences::LoopDisabledOnHD );

	priority_combo->setCurrentIndex(priority);
	loopfilter_combo->setCurrentIndex(loop_filter);

	createHelp();
}

void PrefPerformance::setData(Preferences * pref) {
	setCacheForFiles( pref->cache_for_files );
	setCacheForStreams( pref->cache_for_streams );
	setCacheForDVDs( pref->cache_for_dvds );
	setCacheForAudioCDs( pref->cache_for_audiocds );
	setCacheForVCDs( pref->cache_for_vcds );
	setCacheForTV( pref->cache_for_tv );

	setPriority( pref->priority );
	setFrameDrop( pref->frame_drop );
	setHardFrameDrop( pref->hard_frame_drop );
	setCoreavcUsage( pref->coreavc );
	setSkipLoop( pref->h264_skip_loop_filter );
#if !SMART_DVD_CHAPTERS
	setFastChapterSeeking( pref->fast_chapter_change );
#endif
	setFastAudioSwitching( pref->fast_audio_change );
	setThreads( pref->threads );

#ifdef YOUTUBE_SUPPORT
	setYTQuality( pref->yt_quality );
#endif
}

void PrefPerformance::getData(Preferences * pref) {
	requires_restart = false;

	TEST_AND_SET(pref->cache_for_files, cacheForFiles());
	TEST_AND_SET(pref->cache_for_streams, cacheForStreams());
	TEST_AND_SET(pref->cache_for_dvds, cacheForDVDs());
	TEST_AND_SET(pref->cache_for_audiocds, cacheForAudioCDs());
	TEST_AND_SET(pref->cache_for_vcds, cacheForVCDs());
	TEST_AND_SET(pref->cache_for_tv, cacheForTV());

	TEST_AND_SET(pref->priority, priority());
	TEST_AND_SET(pref->frame_drop, frameDrop());
	TEST_AND_SET(pref->hard_frame_drop, hardFrameDrop());
	TEST_AND_SET(pref->coreavc, coreavcUsage())
	TEST_AND_SET(pref->h264_skip_loop_filter, skipLoop());
#if !SMART_DVD_CHAPTERS
	TEST_AND_SET(pref->fast_chapter_change, fastChapterSeeking());
#endif
	pref->fast_audio_change = fastAudioSwitching();
	TEST_AND_SET(pref->threads, threads());

#ifdef YOUTUBE_SUPPORT
	pref->yt_quality = YTQuality();
#endif
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

void PrefPerformance::setCacheForAudioCDs(int n) {
	cache_cds_spin->setValue(n);
}

int PrefPerformance::cacheForAudioCDs() {
	return cache_cds_spin->value();
}

void PrefPerformance::setCacheForVCDs(int n) {
	cache_vcds_spin->setValue(n);
}

int PrefPerformance::cacheForVCDs() {
	return cache_vcds_spin->value();
}

void PrefPerformance::setCacheForTV(int n) {
	cache_tv_spin->setValue(n);
}

int PrefPerformance::cacheForTV() {
	return cache_tv_spin->value();
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

void PrefPerformance::setCoreavcUsage(bool b) {
	coreavc_check->setChecked(b);
}

bool PrefPerformance::coreavcUsage() {
	return coreavc_check->isChecked();
}

void PrefPerformance::setSkipLoop(Preferences::H264LoopFilter value) {
	loopfilter_combo->setCurrentIndex(loopfilter_combo->findData(value));
}

Preferences::H264LoopFilter PrefPerformance::skipLoop() {
	return (Preferences::H264LoopFilter) loopfilter_combo->itemData(loopfilter_combo->currentIndex()).toInt();
}

#if !SMART_DVD_CHAPTERS
void PrefPerformance::setFastChapterSeeking(bool b) {
	fast_chapter_check->setChecked(b);
}

bool PrefPerformance::fastChapterSeeking() {
	return fast_chapter_check->isChecked();
}
#endif

void PrefPerformance::setFastAudioSwitching(Preferences::OptionState value) {
	fast_audio_combo->setState(value);
}

Preferences::OptionState PrefPerformance::fastAudioSwitching() {
	return fast_audio_combo->state();
}

void PrefPerformance::setThreads(int v) {
	threads_spin->setValue(v);
}

int PrefPerformance::threads() {
	return threads_spin->value();
}

#ifdef YOUTUBE_SUPPORT
void PrefPerformance::setYTQuality(int q) {
	yt_quality_combo->setCurrentIndex(yt_quality_combo->findData(q));
}

int PrefPerformance::YTQuality() {
	int index = yt_quality_combo->currentIndex();
    return yt_quality_combo->itemData(index).toInt();
}
#endif

void PrefPerformance::createHelp() {
	clearHelp();

	addSectionTitle(tr("Performance"));
	
	// Performance tab
#ifdef Q_OS_WIN
	setWhatsThis(priority_combo, tr("Priority"), 
		tr("Set process priority for mplayer according to the predefined "
           "priorities available under Windows.<br>"
           "<b>Warning:</b> Using realtime priority can cause system lockup."));
#endif

	setWhatsThis(framedrop_check, tr("Allow frame drop"),
		tr("Skip displaying some frames to maintain A/V sync on slow systems." ) );

	setWhatsThis(hardframedrop_check, tr("Allow hard frame drop"),
		tr("More intense frame dropping (breaks decoding). "
           "Leads to image distortion!") );

	setWhatsThis(threads_spin, tr("Threads for decoding"),
		tr("Sets the number of threads to use for decoding. Only for "
           "MPEG-1/2 and H.264") );

	setWhatsThis(coreavc_check, tr("Use CoreAVC if no other codec specified"),
		tr("Try to use non-free CoreAVC codec with no other codec is specified and non-VDPAU video output selected. Requires MPlayer build with CoreAVC support."));

	setWhatsThis(loopfilter_combo, tr("Skip loop filter"),
		tr("This option allows to skips the loop filter (AKA deblocking) "
           "during H.264 decoding. "
           "Since the filtered frame is supposed to be used as reference "
           "for decoding dependent frames this has a worse effect on quality "
           "than not doing deblocking on e.g. MPEG-2 video. But at least for "
           "high bitrate HDTV this provides a big speedup with no visible "
           "quality loss.") +"<br>"+
           tr("Possible values:") +"<br>" +
           tr("<b>Enabled</b>: the loop filter is not skipped")+"<br>"+
           tr("<b>Skip (always)</b>: the loop filter is skipped no matter the "
           "resolution of the video")+"<br>"+
           tr("<b>Skip only on HD videos</b>: the loop filter will be "
           "skipped only on videos which height is %1 or "
           "greater.").arg(pref->HD_height) +"<br>" );

	setWhatsThis(fast_audio_combo, tr("Fast audio track switching"),
		tr("Possible values:<br> "
           "<b>Yes</b>: it will try the fastest method "
           "to switch the audio track (it might not work with some formats).<br> "
           "<b>No</b>: the MPlayer process will be restarted whenever you "
           "change the audio track.<br> "
           "<b>Auto</b>: SMPlayer will decide what to do according to the "
           "MPlayer version." ) );

#if !SMART_DVD_CHAPTERS
	setWhatsThis(fast_chapter_check, tr("Fast seek to chapters in dvds"),
		tr("If checked, it will try the fastest method to seek to chapters "
           "but it might not work with some discs.") );
#endif

#ifdef YOUTUBE_SUPPORT
	setWhatsThis(yt_quality_combo, tr("Youtube quality"),
		tr("Select the preferred quality for youtube videos.") );
#endif

	addSectionTitle(tr("Cache"));

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

	setWhatsThis(cache_cds_spin, tr("Cache for audio CDs"), 
		tr("This option specifies how much memory (in kBytes) to use when "
           "precaching an audio CD.") );

	setWhatsThis(cache_vcds_spin, tr("Cache for VCDs"), 
		tr("This option specifies how much memory (in kBytes) to use when "
           "precaching a VCD.") );
}

#include "moc_prefperformance.cpp"
