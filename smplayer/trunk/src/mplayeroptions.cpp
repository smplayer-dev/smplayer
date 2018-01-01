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

#include "subtracks.h"
#include "deviceinfo.h"
#include <QDir>
#include <QDebug>

void MplayerProcess::initializeOptionVars() {
	qDebug("MplayerProcess::initializeOptionVars");
	PlayerProcess::initializeOptionVars();
}

void MplayerProcess::setMedia(const QString & media, bool is_playlist) {
	if (is_playlist) arg << "-playlist";
	arg << media;
}

void MplayerProcess::setFixedOptions() {
	arg << "-noquiet" << "-slave" << "-identify";
}

void MplayerProcess::disableInput() {
	arg << "-nomouseinput";

#if !defined(Q_OS_WIN) && !defined(Q_OS_OS2)
	arg << "-input" << "nodefault-bindings:conf=/dev/null";
#endif
}

#ifdef CAPTURE_STREAM
void MplayerProcess::setCaptureDirectory(const QString & dir) {
	PlayerProcess::setCaptureDirectory(dir);
	if (!capture_filename.isEmpty()) {
		arg << "-capture" << "-dumpfile" << capture_filename;
	}
}
#endif

void MplayerProcess::enableScreenshots(const QString & dir, const QString & /* templ */, const QString & /* format */) {
	QString f = "screenshot";
	if (!dir.isEmpty()) {
		QString d = QDir::toNativeSeparators(dir);
		if (MplayerVersion::isMplayerAtLeast(36848)) {
			f += "="+ d + "/shot";
		} else {
			// Keep compatibility with older versions
			qDebug() << "MplayerProcess::enableScreenshots: this version of mplayer is very old";
			qDebug() << "MplayerProcess::enableScreenshots: changing working directory to" << d;
			setWorkingDirectory(d);
		}
	}
	arg << "-vf-add" << f;
}

void MplayerProcess::setOption(const QString & option_name, const QVariant & value) {
	if (option_name == "ao") {
		QString ao = value.toString();
		if (ao.contains(":")) {
			QStringList l = DeviceInfo::extractDevice(ao);
			qDebug() << "MplayerProcess::setOption: ao:" << l;
			if (l.count() > 1) {
				#ifndef Q_OS_WIN
				if (l[0] == "alsa") {
					ao = "alsa:device=hw=" + l[1];
				}
				else
				if (l[0] == "pulse") {
					ao = "pulse::" + l[1];
				}
				#else
				if (l[0] == "dsound") {
					ao = "dsound:device=" + l[1];
				}
				#endif
			}
		}
		arg << "-ao" << ao + ",";
	}
	else
	if (option_name == "cache") {
		int cache = value.toInt();
		if (cache > 31) {
			arg << "-cache" << value.toString();
		} else {
			arg << "-nocache";
		}
	}
	else
	if (option_name == "cache_auto") {
		// Nothing to do
	}
	else
	if (option_name == "stop-xscreensaver") {
		bool stop_ss = value.toBool();
		if (stop_ss) arg << "-stop-xscreensaver"; else arg << "-nostop-xscreensaver";
	}
	else
	if (option_name == "correct-pts") {
		bool b = value.toBool();
		if (b) arg << "-correct-pts"; else arg << "-nocorrect-pts";
	}
	else
	if (option_name == "framedrop") {
		QString o = value.toString();
		if (o.contains("vo"))  arg << "-framedrop";
		if (o.contains("decoder")) arg << "-hardframedrop";
	}
	else
	if (option_name == "osd-level") {
		arg << "-osdlevel" << value.toString();
	}
	else
	if (option_name == "osd-fractions") {
	}
	else
	if (option_name == "osd-bar-pos") {
		int position = value.toInt();
		if (position >= 0 && position <= 100) {
			arg << "-progbar-align" << QString::number(position);
		}
	}
	else
	if (option_name == "osd-scale") {
		arg << "-subfont-osd-scale" << value.toString();
	}
	else
	if (option_name == "verbose") {
		arg << "-v";
	}
	else
	if (option_name == "enable_streaming_sites_support") {
		// Not supported
	}
	else
	if (option_name == "ytdl_quality") {
		// Not supported
	}
	else
	if (option_name == "hwdec") {
		// Not supported
	}
	else
	if (option_name == "fontconfig") {
		bool b = value.toBool();
		if (b) arg << "-fontconfig"; else arg << "-nofontconfig";
	}
	else
	if (option_name == "mute") {
		// Not supported
	}
	else
	if (option_name == "softvol") {
		if (value.toString() != "off") {
			int v = value.toInt();
			if (v < 100) v = 100;
			arg << "-softvol" << "-softvol-max" << QString::number(v);
		}
	}
	else
	if (option_name == "keepaspect" ||
	    option_name == "dr" || option_name == "double" ||
	    option_name == "fs" || option_name == "slices" ||
	    option_name == "flip-hebrew")
	{
		bool b = value.toBool();
		if (b) arg << "-" + option_name; else arg << "-no" + option_name;
	}
	else {
		arg << "-" + option_name;
		if (!value.isNull()) arg << value.toString();
	}
}

void MplayerProcess::addUserOption(const QString & option) {
	arg << option;
}

void MplayerProcess::setSubEncoding(const QString & codepage, const QString & enca_lang) {
	QString encoding;
	if (!enca_lang.isEmpty()) {
		encoding = "enca:"+ enca_lang;
		if (!codepage.isEmpty()) {
			encoding += ":"+ codepage;
		}
	}
	else
	if (!codepage.isEmpty()) {
		encoding = codepage;
	}

	if (!encoding.isEmpty()) {
		arg << "-subcp" << encoding;
	}
}

void MplayerProcess::addVF(const QString & filter_name, const QVariant & value) {
	QString option = value.toString();

	if (filter_name == "blur" || filter_name == "sharpen") {
		arg << "-vf-add" << "unsharp=" + option;
	}
	else
	if (filter_name == "deblock") {
		arg << "-vf-add" << "pp=" + option;
	}
	else
	if (filter_name == "dering") {
		arg << "-vf-add" << "pp=dr";
	}
	else
	if (filter_name == "postprocessing") {
		arg << "-vf-add" << "pp";
	}
	else
	if (filter_name == "lb" || filter_name == "l5") {
		arg << "-vf-add" << "pp=" + filter_name;
	}
	else
	if (filter_name == "subs_on_screenshots") {
		if (option == "ass") {
			arg << "-vf-add" << "ass";
		} else {
			arg << "-vf-add" << "expand=osd=1";
		}
	}
	else
	if (filter_name == "flip") {
		// expand + flip doesn't work well, a workaround is to add another
		// filter between them, so that's why harddup is here
		arg << "-vf-add" << "harddup,flip";
	}
	else
	if (filter_name == "letterbox") {
		QSize desktop_size = value.toSize();
		double aspect = (double) desktop_size.width() / desktop_size.height();
		arg << "-vf-add" << "expand=aspect=" + QString::number(aspect) + ",harddup";
		// harddup fixes subtitles not disappearing
	}
	/*
	else
	if (filter_name == "expand") {
		arg << "-vf-add" << "expand=" + option + ",harddup";
		// Note: on some videos (h264 for instance) the subtitles doesn't disappear,
		// appearing the new ones on top of the old ones. It seems adding another
		// filter after expand fixes the problem. I chose harddup 'cos I think
		// it will be harmless in mplayer.
	}
	*/
	else {
		QString s = filter_name;
		QString option = value.toString();
		if (!option.isEmpty()) s += "=" + option;
		arg << "-vf-add" << s;
	}
}

void MplayerProcess::addStereo3DFilter(const QString & in, const QString & out) {
	QString filter = "stereo3d=" + in + ":" + out;
	filter += ",scale"; // In my PC it doesn't work without scale :?
	arg << "-vf-add" << filter;
}

void MplayerProcess::setVideoEqualizerOptions(int contrast, int brightness, int hue, int saturation, int gamma, bool soft_eq) {
	if (contrast != 0) arg << "-contrast" << QString::number(contrast);
	if (brightness != 0) arg << "-brightness" << QString::number(brightness);
	if (hue != 0) arg << "-hue" << QString::number(hue);
	if (saturation != 0) arg << "-saturation" << QString::number(saturation);
	if (gamma != 0) arg << "-gamma" << QString::number(gamma);

	if (soft_eq) {
		arg << "-vf-add" << "eq2,hue";
	}
}

void MplayerProcess::addAF(const QString & filter_name, const QVariant & value) {
	QString s = filter_name;
	if (filter_name == "earwax") {
		// Not supported
	}
	else
	if (filter_name == "stereo-mode") {
		QString o = value.toString();
		if (o == "left") arg << "-af-add" << "channels=2:2:0:1:0:0";
		else
		if (o == "right") arg << "-af-add" << "channels=2:2:1:0:1:1";
		else
		if (o == "reverse") arg << "-af-add" << "channels=2:2:0:1:1:0";
		else
		if (o == "mono") arg << "-af-add" << "pan=1:0.5:0.5";
	}
	else
	if (filter_name == "equalizer") {
		AudioEqualizerList l = value.toList();
		QString o = AudioEqualizerHelper::equalizerListToString(l);
		arg << "-af-add" << "equalizer=" + o;
	} else {
		if (!value.isNull()) s += "=" + value.toString();
		arg << "-af-add" << s;
	}
}

void MplayerProcess::quit() {
	writeToStdin("quit");
}

void MplayerProcess::setVolume(int v) {
	writeToStdin("volume " + QString::number(v) + " 1");
}

void MplayerProcess::setOSD(int o) {
	writeToStdin(pausing_prefix + " osd " + QString::number(o));
}

void MplayerProcess::setAudio(int ID) {
	writeToStdin("switch_audio " + QString::number(ID));
}

void MplayerProcess::setVideo(int ID) {
	writeToStdin("set_property switch_video " + QString::number(ID));
}

void MplayerProcess::setSubtitle(int type, int ID) {
	switch (type) {
		case SubData::Vob:
			writeToStdin( "sub_vob " + QString::number(ID) );
			break;
		case SubData::Sub:
			writeToStdin( "sub_demux " + QString::number(ID) );
			break;
		case SubData::File:
			writeToStdin( "sub_file " + QString::number(ID) );
			break;
		default: {
			qWarning("MplayerProcess::setSubtitle: unknown type!");
		}
	}
}

void MplayerProcess::disableSubtitles() {
	writeToStdin("sub_source -1");
}

void MplayerProcess::setSubtitlesVisibility(bool b) {
	writeToStdin(QString("sub_visibility %1").arg(b ? 1 : 0));
}

void MplayerProcess::seek(double secs, int mode, bool precise) {
	QString s = QString("seek %1 %2").arg(secs).arg(mode);
	if (precise) s += " 1"; else s += " -1";
	writeToStdin(s);
}

void MplayerProcess::mute(bool b) {
	writeToStdin(pausing_prefix + " mute " + QString::number(b ? 1 : 0));
}

void MplayerProcess::setPause(bool /*b*/) {
	writeToStdin("pause"); // pauses / unpauses
}

void MplayerProcess::frameStep() {
	writeToStdin("frame_step");
}

void MplayerProcess::frameBackStep() {
	qDebug("MplayerProcess::frameBackStep: function not supported by mplayer");
	showOSDText(tr("This option is not supported by MPlayer"), 3000, 1);
}


void MplayerProcess::showOSDText(const QString & text, int duration, int level) {
	QString str = QString("osd_show_text \"%1\" %2 %3").arg(text).arg(duration).arg(level);
	if (!pausing_prefix.isEmpty()) str = pausing_prefix + " " + str;
	writeToStdin(str);
}

void MplayerProcess::showFilenameOnOSD() {
	QString s = "${filename}";

	if (!osd_media_info.isEmpty()) s = osd_media_info;

	writeToStdin("osd_show_property_text \"" + s + "\" 2000 0");
}

void MplayerProcess::showMediaInfoOnOSD() {
	showFilenameOnOSD();
}

void MplayerProcess::showTimeOnOSD() {
	writeToStdin("osd_show_property_text \"${time_pos} / ${length} (${percent_pos}%)\" 2000 0");
}

void MplayerProcess::setContrast(int value) {
	writeToStdin(pausing_prefix + " contrast " + QString::number(value) + " 1");
}

void MplayerProcess::setBrightness(int value) {
	writeToStdin(pausing_prefix + " brightness " + QString::number(value) + " 1");
}

void MplayerProcess::setHue(int value) {
	writeToStdin(pausing_prefix + " hue " + QString::number(value) + " 1");
}

void MplayerProcess::setSaturation(int value) {
	writeToStdin(pausing_prefix + " saturation " + QString::number(value) + " 1");
}

void MplayerProcess::setGamma(int value) {
	writeToStdin(pausing_prefix + " gamma " + QString::number(value) + " 1");
}

void MplayerProcess::setChapter(int ID) {
	writeToStdin("seek_chapter " + QString::number(ID) +" 1");
}

void MplayerProcess::nextChapter() {
	writeToStdin("seek_chapter 1 0");
}

void MplayerProcess::previousChapter() {
	writeToStdin("seek_chapter -1 0");
}

void MplayerProcess::setExternalSubtitleFile(const QString & filename) {
	writeToStdin("sub_load \""+ filename +"\"");
}

void MplayerProcess::setSubPos(int pos) {
	writeToStdin("sub_pos " + QString::number(pos) + " 1");
}

void MplayerProcess::setSubScale(double value) {
	writeToStdin("sub_scale " + QString::number(value) + " 1");
}

void MplayerProcess::setSubStep(int value) {
	writeToStdin("sub_step " + QString::number(value));
}

#ifdef MPV_SUPPORT
void MplayerProcess::seekSub(int /*value*/) {
	/* Not supported */
	showOSDText(tr("This option is not supported by MPlayer"), 3000, 1);
};
#endif

void MplayerProcess::setSubForcedOnly(bool b) {
	writeToStdin(QString("forced_subs_only %1").arg(b ? 1 : 0));
}

void MplayerProcess::setSpeed(double value) {
	writeToStdin("speed_set " + QString::number(value));
}

void MplayerProcess::enableKaraoke(bool b) {
	if (b) writeToStdin("af_add karaoke"); else writeToStdin("af_del karaoke");
}

void MplayerProcess::enableExtrastereo(bool b) {
	if (b) writeToStdin("af_add extrastereo"); else writeToStdin("af_del extrastereo");
}

void MplayerProcess::enableVolnorm(bool b, const QString & option) {
	if (b) writeToStdin("af_add volnorm=" + option); else writeToStdin("af_del volnorm");
}

void MplayerProcess::setAudioEqualizer(AudioEqualizerList l) {
	QString values = AudioEqualizerHelper::equalizerListToString(l);
	writeToStdin("af_cmdline equalizer " + values);
}

void MplayerProcess::setAudioDelay(double delay) {
	writeToStdin(pausing_prefix + " audio_delay " + QString::number(delay) +" 1");
}

void MplayerProcess::setSubDelay(double delay) {
	writeToStdin(pausing_prefix + " sub_delay " + QString::number(delay) +" 1");
}

void MplayerProcess::setLoop(int v) {
	writeToStdin(QString("loop %1 1").arg(v));
}

void MplayerProcess::setAMarker(int /*sec*/) {
	/* Not supported */
}

void MplayerProcess::setBMarker(int /*sec*/) {
	/* Not supported */
}

void MplayerProcess::clearABMarkers() {
	/* Not supported */
}

void MplayerProcess::takeScreenshot(ScreenshotType t, bool /*include_subtitles*/) {
	if (t == Single) {
		writeToStdin(pausing_prefix + " screenshot 0");
	} else {
		writeToStdin("screenshot 1");
	}
}

#ifdef CAPTURE_STREAM
void MplayerProcess::switchCapturing() {
	writeToStdin("capturing");
}
#endif

void MplayerProcess::setTitle(int ID) {
	writeToStdin("switch_title " + QString::number(ID));
}

#if DVDNAV_SUPPORT
void MplayerProcess::discSetMousePos(int x, int y) {
	writeToStdin(QString("set_mouse_pos %1 %2").arg(x).arg(y));
}

void MplayerProcess::discButtonPressed(const QString & button_name) {
	writeToStdin("dvdnav " + button_name);
}
#endif

void MplayerProcess::setAspect(double aspect) {
	writeToStdin("switch_ratio " + QString::number(aspect));
}

void MplayerProcess::setFullscreen(bool b) {
	writeToStdin(QString("vo_fullscreen %1").arg(b ? "1" : "0"));
}

#if PROGRAM_SWITCH
void MplayerProcess::setTSProgram(int ID) {
	writeToStdin("set_property switch_program " + QString::number(ID) );
	writeToStdin("get_property switch_audio");
	writeToStdin("get_property switch_video");
}
#endif

void MplayerProcess::toggleDeinterlace() {
	writeToStdin("step_property deinterlace");
}

void MplayerProcess::askForLength() {
	writeToStdin(pausing_prefix + " get_property length");
}

void MplayerProcess::setOSDScale(double /*value*/) {
	// not available
	/* writeToStdin("set_property subfont-osd-scale " + QString::number(value)); */
}

void MplayerProcess::changeVF(const QString & filter, bool enable, const QVariant & option) {
	// not supported
	Q_UNUSED(filter);
	Q_UNUSED(enable);
	Q_UNUSED(option);
}

void MplayerProcess::changeAF(const QString & filter, bool enable, const QVariant & option) {
	/* Not used yet */
	Q_UNUSED(filter);
	Q_UNUSED(enable);
	Q_UNUSED(option);
};

void MplayerProcess::changeStereo3DFilter(bool /*enable*/, const QString & /*in*/, const QString & /*out*/) {
	// not supported
}

void MplayerProcess::setSubStyles(const AssStyles & styles, const QString & assStylesFile) {
	if (assStylesFile.isEmpty()) {
		qWarning("MplayerProcess::setSubStyles: assStylesFile is invalid");
		return;
	}

	// Load the styles.ass file
	if (!QFile::exists(assStylesFile)) {
		// If file doesn't exist, create it
		styles.exportStyles(assStylesFile);
	}
	if (QFile::exists(assStylesFile)) {
		setOption("ass-styles", assStylesFile);
	} else {
		qWarning("MplayerProcess::setSubStyles: '%s' doesn't exist", assStylesFile.toUtf8().constData());
	}
}
