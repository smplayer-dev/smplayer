/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "mpvprocess.h"
#include <QRegExp>
#include <QStringList>
#include <QApplication>
#include <QDebug>

#include "global.h"
#include "preferences.h"
#include "mplayerversion.h"
#include "colorutils.h"

using namespace Global;

#define CUSTOM_STATUS

#define TOO_CHAPTERS_WORKAROUND

MPVProcess::MPVProcess(QObject * parent)
	: PlayerProcess(parent)
	, notified_mplayer_is_running(false)
	, received_end_of_file(false)
	, last_sub_id(-1)
	, mplayer_svn(-1) // Not found yet
	, verbose(false)
#if NOTIFY_SUB_CHANGES
	, subtitle_info_received(false)
	, subtitle_info_changed(false)
#endif
#if NOTIFY_AUDIO_CHANGES
	, audio_info_changed(false)
#endif
#if NOTIFY_VIDEO_CHANGES
	, video_info_changed(false)
#endif
#if NOTIFY_CHAPTER_CHANGES
	, chapter_info_changed(false)
#endif
	, dvd_current_title(-1)
	, br_current_title(-1)
{
	player_id = PlayerID::MPV;

	connect( this, SIGNAL(lineAvailable(QByteArray)),
			 this, SLOT(parseLine(QByteArray)) );

	connect( this, SIGNAL(finished(int,QProcess::ExitStatus)), 
             this, SLOT(processFinished(int,QProcess::ExitStatus)) );

	connect( this, SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(gotError(QProcess::ProcessError)) );

	int svn = MplayerVersion::mplayerVersion("mpv unknown version (C)");
}

MPVProcess::~MPVProcess() {
}

bool MPVProcess::start() {
	md.reset();
	notified_mplayer_is_running = false;
	last_sub_id = -1;
	mplayer_svn = -1; // Not found yet
	received_end_of_file = false;

#if NOTIFY_SUB_CHANGES
	subs.clear();
	subtitle_info_received = false;
	subtitle_info_changed = false;
#endif

#if NOTIFY_AUDIO_CHANGES
	audios.clear();
	audio_info_changed = false;
#endif

#if NOTIFY_VIDEO_CHANGES
	videos.clear();
	video_info_changed = false;
#endif

#if NOTIFY_CHAPTER_CHANGES
	chapters.clear();
	chapter_info_changed = false;
#endif

	dvd_current_title = -1;
	br_current_title = -1;

	MyProcess::start();

	return waitForStarted();
}

#ifdef CUSTOM_STATUS
static QRegExp rx_mpv_av("^STATUS: ([0-9\\.-]+) / ([0-9\\.-]+) P: (yes|no) B: (yes|no) I: (yes|no)");
#else
static QRegExp rx_mpv_av("^(\\((.*)\\) |)(AV|V|A): ([0-9]+):([0-9]+):([0-9]+) / ([0-9]+):([0-9]+):([0-9]+)"); //AV: 00:02:15 / 00:09:56
#endif

static QRegExp rx_mpv_dsize("^INFO_VIDEO_DSIZE=(\\d+)x(\\d+)");
static QRegExp rx_mpv_vo("^VO: \\[(.*)\\]");
static QRegExp rx_mpv_ao("^AO: \\[(.*)\\]");
static QRegExp rx_mpv_paused("^\\(Paused\\)");
static QRegExp rx_mpv_endoffile("^Exiting... \\(End of file\\)");

//static QRegExp rx_mpv_audio("^\\[stream\\] Audio .* --aid=(\\d+)( --alang=([a-z]+)|)([ \\(\\)\\*]+)('(.*)'|)");
static QRegExp rx_mpv_audio("^.* Audio\\s+--aid=(\\d+)( --alang=([a-z]+)|)([ \\(\\)\\*]+)('(.*)'|)");
//static QRegExp rx_mpv_subs("^\\[stream\\] Subs .* --sid=(\\d+)( --slang=([a-z]+)|)([ \\(\\)\\*]+)('(.*)'|)");
static QRegExp rx_mpv_subs("^.* Subs\\s+--sid=(\\d+)( --slang=([a-z]+)|)([ \\(\\)\\*]+)('(.*)'|)");
#if !NOTIFY_VIDEO_CHANGES
//static QRegExp rx_mpv_video("^\\[stream\\] Video .* --vid=(\\d+)([ \\(\\)\\*]+)('(.*)'|)");
static QRegExp rx_mpv_video("^.* Video\\s+--vid=(\\d+)([ \\(\\)\\*]+)('(.*)'|)");
#endif

#if 0
static QRegExp rx_mpv_subs2("^Sub:( >|) \\((\\d+)\\) '(.*)'");
#endif

static QRegExp rx_mpv_chaptername("^INFO_CHAPTER_(\\d+)_NAME=(.*)");
static QRegExp rx_mpv_trackinfo("^INFO_TRACK_(\\d+): (audio|video|sub) (\\d+) '(.*)' '(.*)' (yes|no)");

#if 0
static QRegExp rx_mpv_videoinfo("^\\[vd\\] VIDEO: .* (\\d+)x(\\d+) .* ([0-9.]+) fps"); // [vd] VIDEO:  624x352  25.000 fps  1018.5 kbps (127.3 kB/s)
#endif

static QRegExp rx_mpv_videocodec("^INFO_VIDEO_CODEC=(.*) \\[(.*)\\]");
static QRegExp rx_mpv_audiocodec("^INFO_AUDIO_CODEC=(.*) \\[(.*)\\]");

static QRegExp rx_mpv_forbidden("HTTP error 403 Forbidden");

#if DVDNAV_SUPPORT
static QRegExp rx_mpv_switch_title("^\\[dvdnav\\] DVDNAV, switched to title: (\\d+)");
#endif

static QRegExp rx_mpv_playing("^Playing:.*|^\\[ytdl_hook\\].*");

//static QRegExp rx_mpv_generic("^(.*)=(.*)");
static QRegExp rx_mpv_generic("^([A-Z_]+)=(.*)");

static QRegExp rx_stream_title("icy-title: (.*)");


void MPVProcess::parseLine(QByteArray ba) {
	//qDebug("MPVProcess::parseLine: '%s'", ba.data() );

	if (ba.isEmpty()) return;

	QString tag;
	QString value;

#if COLOR_OUTPUT_SUPPORT
    QString line = ColorUtils::stripColorsTags(QString::fromLocal8Bit(ba));
#else
	#ifdef Q_OS_WIN
	QString line = QString::fromUtf8(ba);
	#else
	QString line = QString::fromLocal8Bit(ba);
	#endif
#endif

	if (verbose) {
		line = line.replace("[statusline] ", "");
		line = line.replace("[cplayer] ", "");
	}

	// Parse A: V: line
	//qDebug("MPVProcess::parseLine: %s", line.toUtf8().data());
	if (rx_mpv_av.indexIn(line) > -1) {
		#ifdef CUSTOM_STATUS
		double sec = rx_mpv_av.cap(1).toDouble();
		double length = rx_mpv_av.cap(2).toDouble();
		bool paused = (rx_mpv_av.cap(3) == "yes");
		bool buffering = (rx_mpv_av.cap(4) == "yes");
		bool idle = (rx_mpv_av.cap(5) == "yes");

		if (length != md.duration) {
			md.duration = length;
			#if DVDNAV_SUPPORT
			emit receivedDuration(length);
			#endif
		}

		if (paused) {
			qDebug("MPVProcess::parseLine: paused");
			receivedPause();
			return;
		}
		else
		if (buffering) {
			qDebug("MPVProcess::parseLine: buffering");
			receivedBuffering();
			return;
		}
		else
		if (idle) {
			qDebug("MPVProcess::parseLine: idle");
			receivedBuffering();
			return;
		}

		#else

		//qDebug() << rx_mpv_av.cap(1);
		//qDebug() << rx_mpv_av.cap(2);

		QString status = rx_mpv_av.cap(2).trimmed();

		int i = 4;
		int h = rx_mpv_av.cap(i++).toInt();
		int m = rx_mpv_av.cap(i++).toInt();
		int s = rx_mpv_av.cap(i++).toInt();
		//qDebug("%d:%d:%d", h, m, s);
		double sec = h * 3600 + m * 60 + s;

		h = rx_mpv_av.cap(i++).toInt();
		m = rx_mpv_av.cap(i++).toInt();
		s = rx_mpv_av.cap(i++).toInt();
		double length = h * 3600 + m * 60 + s;
		if (length != md.duration) {
			md.duration = length;
			emit receivedDuration(length);
		}

		if (status == "Paused") {
			receivedPause();
			return;
		}
		else
		if ((status == "...") || (status == "Buffering")) {
			receivedBuffering();
			return;
		}

		if (!status.isEmpty()) {
			qDebug() << "MPVProcess::parseLine: status:" << status;
		}

		#endif

#if NOTIFY_SUB_CHANGES
		if (notified_mplayer_is_running) {
			if (subtitle_info_changed) {
				qDebug("MPVProcess::parseLine: subtitle_info_changed");
				subtitle_info_changed = false;
				subtitle_info_received = false;
				emit subtitleInfoChanged(subs);
			}
			if (subtitle_info_received) {
				qDebug("MPVProcess::parseLine: subtitle_info_received");
				subtitle_info_received = false;
				emit subtitleInfoReceivedAgain(subs);
			}
		}
#endif

#if NOTIFY_AUDIO_CHANGES
		if (notified_mplayer_is_running) {
			if (audio_info_changed) {
				qDebug("MPVProcess::parseLine: audio_info_changed");
				audio_info_changed = false;
				emit audioInfoChanged(audios);
			}
		}
#endif

#if NOTIFY_VIDEO_CHANGES
		if (notified_mplayer_is_running) {
			if (video_info_changed) {
				qDebug("MPVProcess::parseLine: video_info_changed");
				video_info_changed = false;
				emit videoInfoChanged(videos);
			}
		}
#endif

#if NOTIFY_CHAPTER_CHANGES
		if (notified_mplayer_is_running) {
			if (chapter_info_changed) {
				qDebug("MPVProcess::parseLine: chapter_info_changed");
				chapter_info_changed = false;
				emit chaptersChanged(chapters);
			}
		}
#endif

		if (!notified_mplayer_is_running) {
			qDebug("MPVProcess::parseLine: starting sec: %f", sec);

			/*
			if ( (md.n_chapters <= 0) && (dvd_current_title > 0) && 
                 (md.titles.find(dvd_current_title) != -1) )
			{
				int idx = md.titles.find(dvd_current_title);
				md.n_chapters = md.titles.itemAt(idx).chapters();
				qDebug("MPVProcess::parseLine: setting chapters to %d", md.n_chapters);
			}
			*/

			/*
			#if CHECK_VIDEO_CODEC_FOR_NO_VIDEO
			// Another way to find out if there's no video
			if (md.video_codec.isEmpty()) {
				md.novideo = true;
				emit receivedNoVideo();
			}
			#endif
			*/

			emit receivedStartingTime(sec);
			emit mplayerFullyLoaded();

			emit receivedCurrentFrame(0); // Ugly hack: set the frame counter to 0

			notified_mplayer_is_running = true;

			// Wait some secs to ask for bitrate
			QTimer::singleShot(12000, this, SLOT(requestBitrateInfo()));
		}

		emit receivedCurrentSec( sec );
	}
	else {
		emit lineAvailable(line);

		// Parse other things
		qDebug("MPVProcess::parseLine: '%s'", line.toUtf8().data() );

		// End of file
		if (rx_mpv_endoffile.indexIn(line) > -1)  {
			qDebug("MVPProcess::parseLine: detected end of file");
			if (!received_end_of_file) {
				// In case of playing VCDs or DVDs, maybe the first title
				// is not playable, so the GUI doesn't get the info about
				// available titles. So if we received the end of file
				// first let's pretend the file has started so the GUI can have
				// the data.
				if ( !notified_mplayer_is_running) {
					emit mplayerFullyLoaded();
				}
				// Send signal once the process is finished, not now!
				received_end_of_file = true;
			}
		}
		else

		// Window resolution
		if (rx_mpv_dsize.indexIn(line) > -1) {
			int w = rx_mpv_dsize.cap(1).toInt();
			int h = rx_mpv_dsize.cap(2).toInt();
			emit receivedWindowResolution( w, h );
		}
		else
	
		// VO
		if (rx_mpv_vo.indexIn(line) > -1) {
			emit receivedVO( rx_mpv_vo.cap(1) );
			// Ask for window resolution
			writeToStdin("print_text INFO_VIDEO_DSIZE=${=dwidth}x${=dheight}");
		}
		else
		
		// AO
		if (rx_mpv_ao.indexIn(line) > -1) {
			emit receivedAO( rx_mpv_ao.cap(1) );
		}
		else


		// Audio
		if (rx_mpv_audio.indexIn(line) > -1) {
			int ID = rx_mpv_audio.cap(1).toInt();
			QString lang = rx_mpv_audio.cap(3);
			QString title = rx_mpv_audio.cap(6);
			qDebug("MPVProcess::parseLine: audio id: %d, lang: '%s', name: '%s'", ID, lang.toUtf8().constData(), title.toUtf8().constData());

			#if NOTIFY_AUDIO_CHANGES
			updateAudioTrack(ID, title, lang);
			#else
			if (md.audios.find(ID) == -1) {
				md.audios.addID(ID);
				md.audios.addName(ID, title);
				md.audios.addLang(ID, lang);
			}
			#endif
		}
		else

		if (rx_stream_title.indexIn(line) > -1) {
			QString s = rx_stream_title.cap(1);
			qDebug("MPVProcess::parseLine: stream_title: '%s'", s.toUtf8().data());
			md.stream_title = s;
			emit receivedStreamTitle(s);
		}
		else

		// Subtitles
		if (rx_mpv_subs.indexIn(line) > -1) {
			int ID = rx_mpv_subs.cap(1).toInt();
			QString lang = rx_mpv_subs.cap(3);
			QString title = rx_mpv_subs.cap(6);
			qDebug("MPVProcess::parseLine: sub id: %d, lang: '%s', name: '%s'", ID, lang.toUtf8().constData(), title.toUtf8().constData());

			#if NOTIFY_SUB_CHANGES
			updateSubtitleTrack(ID, title, lang);
			#else
			if (md.subs.find(SubData::Sub, ID) == -1) {
				md.subs.add(SubData::Sub, ID);
				md.subs.changeName(SubData::Sub, ID, title);
				md.subs.changeLang(SubData::Sub, ID, lang);
			}
			#endif
		}
		else
		#if 0
		if (rx_mpv_subs2.indexIn(line) > -1) {
			bool selected = (rx_mpv_subs2.cap(1) == " >");
			int ID = rx_mpv_subs2.cap(2).toInt();
			QString title = rx_mpv_subs2.cap(3);
			qDebug("MPVProcess::parseLine: sub id: %d title: '%s' selected: %d", ID, title.toUtf8().constData(), selected);
			if (md.subs.find(SubData::Sub, ID) == -1) {
				md.subs.add(SubData::Sub, ID);
				md.subs.changeName(SubData::Sub, ID, title);
			}
		}
		else
		#endif

#if !NOTIFY_VIDEO_CHANGES
		// Video
		if (rx_mpv_video.indexIn(line) > -1) {
			int ID = rx_mpv_video.cap(1).toInt();
			QString title = rx_mpv_video.cap(4);
			qDebug("MPVProcess::parseLine: video id: %d, name: '%s'", ID, title.toUtf8().constData());
			//md.videos.addID(ID);
			md.videos.addName(ID, title);
		}
		else
#endif

#if 0
		// Video info
		if (rx_mpv_videoinfo.indexIn(line) > -1) {
			md.video_width = rx_mpv_videoinfo.cap(1).toInt();
			md.video_height = rx_mpv_videoinfo.cap(2).toInt();
			md.video_fps = rx_mpv_videoinfo.cap(3);
			qDebug("MPVProcess::parseLine: width: %d height: %d fps: %s", md.video_width, md.video_height, md.video_fps.toUtf8().constData());
		}
		else
#endif

		// Chapters
		if (rx_mpv_chaptername.indexIn(line) > -1) {
			int ID = rx_mpv_chaptername.cap(1).toInt();
			QString title = rx_mpv_chaptername.cap(2);
			if (title.isEmpty()) title = QString::number(ID + 1);
			#if NOTIFY_CHAPTER_CHANGES
			chapters.addName(ID, title);
			chapter_info_changed = true;
			#else
			md.chapters.addName(ID, title);
			#endif
			qDebug("MPVProcess::parseLine: chapter id: %d title: %s", ID, title.toUtf8().constData());
			//md.chapters.list();
		}
		else

#if NOTIFY_VIDEO_CHANGES || NOTIFY_AUDIO_CHANGES || NOTIFY_SUB_CHANGES
		// Track info
		if (rx_mpv_trackinfo.indexIn(line) > -1) {
			int ID = rx_mpv_trackinfo.cap(3).toInt();
			QString type = rx_mpv_trackinfo.cap(2);
			QString name = rx_mpv_trackinfo.cap(5);
			QString lang = rx_mpv_trackinfo.cap(4);
			QString selected = rx_mpv_trackinfo.cap(6);
			qDebug("MPVProcess::parseLine: ID: %d type: %s name: %s lang: %s selected: %s", ID, type.toUtf8().constData(), 
				name.toUtf8().constData(), lang.toUtf8().constData(), selected.toUtf8().constData());
			/*
			if (lang == "(unavailable)") lang = "";
			if (name == "(unavailable)") name = "";
			*/
			if (type == "video") {
				#if NOTIFY_VIDEO_CHANGES
				int idx = videos.find(ID);
				if (idx == -1) {
					video_info_changed = true;
					videos.addName(ID, name);
					videos.addLang(ID, lang);
				} else {
					// Track already existed
					if (videos.itemAt(idx).name() != name) {
						video_info_changed = true;
						videos.addName(ID, name);
					}
					if (videos.itemAt(idx).lang() != lang) {
						video_info_changed = true;
						videos.addLang(ID, lang);
					}
				}
				#endif
			}
			else
			if (type == "audio") {
				#if NOTIFY_AUDIO_CHANGES
				updateAudioTrack(ID, name, lang);
				#endif
			}
			else
			if (type == "sub") {
				#if NOTIFY_SUB_CHANGES
				updateSubtitleTrack(ID, name, lang);
				#endif
			}
		}
		else
#endif

#if DVDNAV_SUPPORT
		if (rx_mpv_switch_title.indexIn(line) > -1) {
			int title = rx_mpv_switch_title.cap(1).toInt();
			qDebug("MPVProcess::parseLine: title changed to %d", title);
			// Ask for track info
			// Wait 10 secs. because it can take a while until the title start to play
			QTimer::singleShot(10000, this, SLOT(requestChapterInfo()));
		}
		else
#endif

		//Playing
		if (rx_mpv_playing.indexIn(line) > -1) {
			emit receivedPlaying();
		}
		else

		if (rx_mpv_videocodec.indexIn(line) > -1) {
			md.video_codec = rx_mpv_videocodec.cap(2);
			qDebug("MPVProcess::parseLine: md.video_codec '%s'", md.video_codec.toUtf8().constData());
		}
		else
		if (rx_mpv_audiocodec.indexIn(line) > -1) {
			md.audio_codec = rx_mpv_audiocodec.cap(2);
			qDebug("MPVProcess::parseLine: md.audio_codec '%s'", md.audio_codec.toUtf8().constData());
		}
		else

		if (rx_mpv_forbidden.indexIn(line) > -1) {
			qDebug("MVPProcess::parseLine: 403 forbidden");
			emit receivedForbiddenText();
		}

		//Generic things
		if (rx_mpv_generic.indexIn(line) > -1) {
			tag = rx_mpv_generic.cap(1);
			value = rx_mpv_generic.cap(2);
			//qDebug("MPVProcess::parseLine: tag: %s", tag.toUtf8().constData());
			//qDebug("MPVProcess::parseLine: value: %s", value.toUtf8().constData());

			if (tag == "INFO_VIDEO_WIDTH") {
				md.video_width = value.toInt();
				qDebug("MPVProcess::parseLine: md.video_width set to %d", md.video_width);
			}
			else
			if (tag == "INFO_VIDEO_HEIGHT") {
				md.video_height = value.toInt();
				qDebug("MPVProcess::parseLine: md.video_height set to %d", md.video_height);
			}
			else
			if (tag == "INFO_VIDEO_ASPECT") {
				md.video_aspect = value.toDouble();
				if ( md.video_aspect == 0.0 ) {
					// I hope width & height are already set.
					md.video_aspect = (double) md.video_width / md.video_height;
				}
				qDebug("MPVProcess::parseLine: md.video_aspect set to %f", md.video_aspect);
			}
			if (tag == "INFO_VIDEO_BITRATE") {
				int bitrate = value.toInt();
				emit receivedVideoBitrate(bitrate);
			}
			else
			if (tag == "INFO_LENGTH") {
				md.duration = value.toDouble();
				qDebug("MPVProcess::parseLine: md.duration set to %f", md.duration);
			}
			else
			if (tag == "INFO_DEMUXER") {
				md.demuxer = value;
			}
			else
			if (tag == "INFO_VIDEO_FORMAT") {
				md.video_format = value;
			}
			if (tag == "INFO_VIDEO_FPS") {
				md.video_fps = value;
			}
			else
			/*
			if (tag == "INFO_VIDEO_CODEC") {
				md.video_codec = value;
			}
			else
			if (tag == "INFO_AUDIO_CODEC") {
				md.audio_codec = value;
			}
			else
			*/
			if (tag == "INFO_AUDIO_BITRATE") {
				int bitrate = value.toInt();
				emit receivedAudioBitrate(bitrate);
			}
			else
			if (tag == "INFO_AUDIO_RATE") {
				md.audio_rate = value.toInt();
			}
			else
			if (tag == "INFO_AUDIO_NCH") {
				md.audio_nch = value.toInt();
			}
			else
			if (tag == "INFO_AUDIO_FORMAT") {
				md.audio_format = value;
			}
			else
			if (tag == "INFO_CHAPTERS") {
				md.n_chapters = value.toInt();
				#ifdef TOO_CHAPTERS_WORKAROUND
				if (md.n_chapters > 1000) {
					qDebug("MPVProcess::parseLine: warning too many chapters: %d", md.n_chapters);
					qDebug("                       chapters will be ignored"); 
					md.n_chapters = 0;
				}
				#endif
				for (int n = 0; n < md.n_chapters; n++) {
					writeToStdin(QString("print_text INFO_CHAPTER_%1_NAME=${chapter-list/%1/title}").arg(n));
				}
			}
			else
			if (tag == "INFO_TITLES") {
				int n_titles = value.toInt();
				for (int ID = 0; ID < n_titles; ID++) {
					md.titles.addName(ID, QString::number(ID+1));
				}
			}
			else
			if (tag == "METADATA_TITLE") {
				if (!value.isEmpty()) md.clip_name = value;
			}
			else
			if (tag == "METADATA_ARTIST") {
				if (!value.isEmpty()) md.clip_artist = value;
			}
			else
			if (tag == "METADATA_DATE") {
				if (!value.isEmpty()) md.clip_date = value;
			}
			else
			if (tag == "METADATA_ALBUM") {
				if (!value.isEmpty()) md.clip_album = value;
			}
			else
			if (tag == "METADATA_COPYRIGHT") {
				if (!value.isEmpty()) md.clip_copyright = value;
			}
			else
			if (tag == "METADATA_TRACK") {
				if (!value.isEmpty()) md.clip_track = value;
			}
			else
			if (tag == "METADATA_GENRE") {
				if (!value.isEmpty()) md.clip_genre = value;
			}
			else
			if (tag == "INFO_MEDIA_TITLE") {
				if (!value.isEmpty() && value != "mp4" && !value.startsWith("mp4&") /*&& md.clip_name.isEmpty()*/) {
					md.clip_name = value;
				}
			}
			else
			if (tag == "MPV_VERSION") {
				mpv_version = value;
				//qDebug("MPVProcess::parseLine: mpv version: %s", mpv_version.toUtf8().constData());
			}
#if NOTIFY_VIDEO_CHANGES || NOTIFY_AUDIO_CHANGES || NOTIFY_SUB_CHANGES
			else
			if (tag == "INFO_TRACKS_COUNT") {
				int tracks = value.toInt();
				for (int n = 0; n < tracks; n++) {
					writeToStdin(QString("print_text \"INFO_TRACK_%1: "
						"${track-list/%1/type} "
						"${track-list/%1/id} "
						"'${track-list/%1/lang:}' "
						"'${track-list/%1/title:}' "
						"${track-list/%1/selected}\"").arg(n));
				}
			}
#endif
		}
	}
}

void MPVProcess::requestChapterInfo() {
	writeToStdin("print_text \"INFO_CHAPTERS=${=chapters}\"");
}

void MPVProcess::requestBitrateInfo() {
	writeToStdin("print_text INFO_VIDEO_BITRATE=${=video-bitrate}");
	writeToStdin("print_text INFO_AUDIO_BITRATE=${=audio-bitrate}");
}

#if NOTIFY_AUDIO_CHANGES
void MPVProcess::updateAudioTrack(int ID, const QString & name, const QString & lang) {
	qDebug("MPVProcess::updateAudioTrack: ID: %d", ID);

	int idx = audios.find(ID);
	if (idx == -1) {
		audio_info_changed = true;
		audios.addName(ID, name);
		audios.addLang(ID, lang);
	} else {
		// Track already existed
		if (audios.itemAt(idx).name() != name) {
			audio_info_changed = true;
			audios.addName(ID, name);
		}
		if (audios.itemAt(idx).lang() != lang) {
			audio_info_changed = true;
			audios.addLang(ID, lang);
		}
	}
}
#endif

#if NOTIFY_SUB_CHANGES
void MPVProcess::updateSubtitleTrack(int ID, const QString & name, const QString & lang) {
	qDebug("MPVProcess::updateSubtitleTrack: ID: %d", ID);

	int idx = subs.find(SubData::Sub, ID);
	if (idx == -1) {
		subtitle_info_changed = true;
		subs.add(SubData::Sub, ID);
		subs.changeName(SubData::Sub, ID, name);
		subs.changeLang(SubData::Sub, ID, lang);
	}
	else {
		// Track already existed
		if (subs.itemAt(idx).name() != name) {
			subtitle_info_changed = true;
			subs.changeName(SubData::Sub, ID, name);
		}
		if (subs.itemAt(idx).lang() != lang) {
			subtitle_info_changed = true;
			subs.changeLang(SubData::Sub, ID, lang);
		}
	}
}
#endif

// Called when the process is finished
void MPVProcess::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	qDebug("MPVProcess::processFinished: exitCode: %d, status: %d", exitCode, (int) exitStatus);
	// Send this signal before the endoffile one, otherwise
	// the playlist will start to play next file before all
	// objects are notified that the process has exited.
	emit processExited();
	if (received_end_of_file) emit receivedEndOfFile();
}

void MPVProcess::gotError(QProcess::ProcessError error) {
	qDebug("MPVProcess::gotError: %d", (int) error);
}

#include "mpvoptions.cpp"
#include "moc_mpvprocess.cpp"
