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

#include "inforeadermplayer.h"
#include <QStringList>
#include <QApplication>
#include <QRegExp>
#include <QProcess>

#include "colorutils.h"
#include "global.h"
#include "preferences.h"
#include "mplayerversion.h"

using namespace Global;

#define NOME 0
#define VO 1
#define AO 2
#define DEMUXER 3
#define VC 4
#define AC 5


InfoReaderMplayer::InfoReaderMplayer( QString mplayer_bin, QObject * parent )
	: QObject(parent)
	, proc(0)
	, mplayer_svn(0)
	, is_mplayer2(false)
{
	mplayerbin = mplayer_bin;

	proc = new QProcess(this);
	proc->setProcessChannelMode( QProcess::MergedChannels );
}

InfoReaderMplayer::~InfoReaderMplayer() {
}

void InfoReaderMplayer::getInfo() {
	waiting_for_key = true;
	vo_list.clear();
	ao_list.clear();
#if ALLOW_DEMUXER_CODEC_CHANGE
	demuxer_list.clear();
#endif
	mplayer_svn = -1;

#if ALLOW_DEMUXER_CODEC_CHANGE
	run("-identify -vo help -ao help -demuxer help -vc help -ac help");
#else
	run("-identify -vo help -ao help");
#endif

	//list();
}

void InfoReaderMplayer::list() {
	qDebug("InfoReaderMplayer::list");

	InfoList::iterator it;

	qDebug(" vo_list:");
	for ( it = vo_list.begin(); it != vo_list.end(); ++it ) {
		qDebug( "driver: '%s', desc: '%s'", (*it).name().toUtf8().data(), (*it).desc().toUtf8().data());
	}

	qDebug(" ao_list:");
	for ( it = ao_list.begin(); it != ao_list.end(); ++it ) {
		qDebug( "driver: '%s', desc: '%s'", (*it).name().toUtf8().data(), (*it).desc().toUtf8().data());
	}

#if ALLOW_DEMUXER_CODEC_CHANGE
	qDebug(" demuxer_list:");
	for ( it = demuxer_list.begin(); it != demuxer_list.end(); ++it ) {
		qDebug( "demuxer: '%s', desc: '%s'", (*it).name().toUtf8().data(), (*it).desc().toUtf8().data());
	}

	qDebug(" vc_list:");
	for ( it = vc_list.begin(); it != vc_list.end(); ++it ) {
		qDebug( "codec: '%s', desc: '%s'", (*it).name().toUtf8().data(), (*it).desc().toUtf8().data());
	}

	qDebug(" ac_list:");
	for ( it = ac_list.begin(); it != ac_list.end(); ++it ) {
		qDebug( "codec: '%s', desc: '%s'", (*it).name().toUtf8().data(), (*it).desc().toUtf8().data());
	}
#endif

}

static QRegExp rx_vo_key("^ID_VIDEO_OUTPUTS");
static QRegExp rx_ao_key("^ID_AUDIO_OUTPUTS");

#if ALLOW_DEMUXER_CODEC_CHANGE
static QRegExp rx_demuxer_key("^ID_DEMUXERS");
static QRegExp rx_ac_key("^ID_AUDIO_CODECS");
static QRegExp rx_vc_key("^ID_VIDEO_CODECS");
#endif

static QRegExp rx_driver("\\t(.*)\\t(.*)");
static QRegExp rx_demuxer("^\\s+([A-Z,a-z,0-9]+)\\s+(\\d+)\\s+(\\S.*)");
static QRegExp rx_demuxer2("^\\s+([A-Z,a-z,0-9]+)\\s+(\\S.*)");
static QRegExp rx_codec("^([A-Z,a-z,0-9]+)\\s+([A-Z,a-z,0-9]+)\\s+([A-Z,a-z,0-9]+)\\s+(\\S.*)");

void InfoReaderMplayer::readLine(QByteArray ba) {
#if COLOR_OUTPUT_SUPPORT
    QString line = ColorUtils::stripColorsTags(QString::fromLocal8Bit(ba));
#else
	QString line = QString::fromLocal8Bit(ba);
#endif

	if (line.isEmpty()) return;

	//qDebug("InfoReaderMplayer::readLine: line: '%s'", line.toUtf8().data());
	//qDebug("waiting_for_key: %d", waiting_for_key);

	if (!waiting_for_key) {
		if ((reading_type == VO) || (reading_type == AO)) {
			if ( rx_driver.indexIn(line) > -1 ) {
				QString name = rx_driver.cap(1);
				QString desc = rx_driver.cap(2);
				qDebug("InfoReaderMplayer::readLine: found driver: '%s' '%s'", name.toUtf8().data(), desc.toUtf8().data());
				if (reading_type==VO) {
					vo_list.append( InfoData(name, desc) );
				} 
				else
				if (reading_type==AO) {
					ao_list.append( InfoData(name, desc) );
				}
			} else {
				qWarning("InfoReaderMplayer::readLine: can't parse output driver from line '%s'", line.toUtf8().constData());
			}
		}
#if ALLOW_DEMUXER_CODEC_CHANGE
		else
		if (reading_type == DEMUXER) {
			if ( rx_demuxer.indexIn(line) > -1 ) {
				QString name = rx_demuxer.cap(1);
				QString desc = rx_demuxer.cap(3);
				qDebug("InfoReaderMplayer::readLine: found demuxer: '%s' '%s'", name.toUtf8().data(), desc.toUtf8().data());
				demuxer_list.append( InfoData(name, desc) );
			}
			else 
			if ( rx_demuxer2.indexIn(line) > -1 ) {
				QString name = rx_demuxer2.cap(1);
				QString desc = rx_demuxer2.cap(2);
				qDebug("InfoReaderMplayer::readLine: found demuxer: '%s' '%s'", name.toUtf8().data(), desc.toUtf8().data());
				demuxer_list.append( InfoData(name, desc) );
			}
			else {
				qWarning("InfoReaderMplayer::readLine: can't parse demuxer from line '%s'", line.toUtf8().constData());
			}
		}
		else
		if ((reading_type == VC) || (reading_type == AC)) {
			if ( rx_codec.indexIn(line) > -1 ) {
				QString name = rx_codec.cap(1);
				QString desc = rx_codec.cap(4);
				qDebug("InfoReaderMplayer::readLine: found codec: '%s' '%s'", name.toUtf8().data(), desc.toUtf8().data());
				if (reading_type==VC) {
					vc_list.append( InfoData(name, desc) );
				} 
				else
				if (reading_type==AC) {
					ac_list.append( InfoData(name, desc) );
				}
			} else {
				qWarning("InfoReaderMplayer::readLine: can't parse codec from line '%s'", line.toUtf8().constData());
			}
		}
#endif
	}

	if ( rx_vo_key.indexIn(line) > -1 ) {
		reading_type = VO;
		waiting_for_key = false;
		qDebug("InfoReaderMplayer::readLine: found key: vo");
	}

	if ( rx_ao_key.indexIn(line) > -1 ) {
		reading_type = AO;
		waiting_for_key = false;
		qDebug("InfoReaderMplayer::readLine: found key: ao");
	}

#if ALLOW_DEMUXER_CODEC_CHANGE
	if ( rx_demuxer_key.indexIn(line) > -1 ) {
		reading_type = DEMUXER;
		waiting_for_key = false;
		qDebug("InfoReaderMplayer::readLine: found key: demuxer");
	}

	if ( rx_ac_key.indexIn(line) > -1 ) {
		reading_type = AC;
		waiting_for_key = false;
		qDebug("InfoReaderMplayer::readLine: found key: ac");
	}

	if ( rx_vc_key.indexIn(line) > -1 ) {
		reading_type = VC;
		waiting_for_key = false;
		qDebug("InfoReaderMplayer::readLines: found key: vc");
	}
#endif

	if (line.startsWith("MPlayer ") || line.startsWith("MPlayer2 ")) {
		mplayer_svn = MplayerVersion::mplayerVersion(line);
		#ifdef MPLAYER2_SUPPORT
		mplayer2_version = MplayerVersion::mplayer2Version();
		is_mplayer2 = MplayerVersion::isMplayer2();
		#endif
	}
}

bool InfoReaderMplayer::run(QString options) {
	qDebug("InfoReaderMplayer::run: '%s'", options.toUtf8().data());

	if (proc->state() == QProcess::Running) {
		qWarning("InfoReaderMplayer::run: process already running");
		return false;
	}

	QStringList args = options.split(" ");

	proc->start(mplayerbin, args);
	if (!proc->waitForStarted()) {
		qWarning("InfoReaderMplayer::run: process can't start!");
		return false;
	}

	//Wait until finish
	if (!proc->waitForFinished()) {
		qWarning("InfoReaderMplayer::run: process didn't finish. Killing it...");
		proc->kill();
	}

	qDebug("InfoReaderMplayer::run : terminating");

	QByteArray ba;
	while (proc->canReadLine()) {
		ba = proc->readLine();
		ba.replace("\n", "");
		ba.replace("\r", "");
		readLine( ba );
	}

	return true;
}

#include "moc_inforeadermplayer.cpp"
