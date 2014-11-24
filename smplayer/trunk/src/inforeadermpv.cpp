/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "inforeadermpv.h"
#include "mplayerversion.h"
#include <QStringList>
#include <QProcess>
#include <QDebug>


InfoReaderMPV::InfoReaderMPV( QString mplayer_bin, QObject * parent )
	: QObject(parent)
	, proc(0)
	, mplayer_svn(0)
{
	mplayerbin = mplayer_bin;

	proc = new QProcess(this);
	proc->setProcessChannelMode( QProcess::MergedChannels );
}

InfoReaderMPV::~InfoReaderMPV() {
}

void InfoReaderMPV::getInfo() {
	vo_list.clear();
	ao_list.clear();
#if ALLOW_DEMUXER_CODEC_CHANGE
	demuxer_list.clear();
	vc_list.clear();
	ac_list.clear();
#endif
	mplayer_svn = -1;

	vo_list = run("--vo help");
	ao_list = run("--ao help");
#if ALLOW_DEMUXER_CODEC_CHANGE
	demuxer_list = run("--demuxer help");
	vc_list = run("--vd help");
	ac_list = run("--ad help");
#endif

	InfoList i = run("--version");
	QString mpv_version_line;
	if (i.count() >= 1) {
		mpv_version_line = i[0].name();
		mplayer_svn = MplayerVersion::mplayerVersion(mpv_version_line);
		mpv_version = MplayerVersion::mpvVersion();
	}

	qDebug() << "InfoReaderMPV::getInfo: version_line" << mpv_version_line;
	qDebug() << "InfoReaderMPV::getInfo: mplayer_svn" << mplayer_svn;

	//list();
}

void InfoReaderMPV::list() {
	qDebug("InfoReaderMPV::list");

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

InfoList InfoReaderMPV::run(QString options) {
	qDebug("InfoReaderMPV::run: '%s'", options.toUtf8().data());

	InfoList l;

	if (proc->state() == QProcess::Running) {
		qWarning("InfoReaderMPV::run: process already running");
		return l;
	}

	QStringList args = options.split(" ");

	proc->start(mplayerbin, args);
	if (!proc->waitForStarted()) {
		qWarning("InfoReaderMPV::run: process can't start!");
		return l;
	}

	//Wait until finish
	if (!proc->waitForFinished()) {
		qWarning("InfoReaderMPV::run: process didn't finish. Killing it...");
		proc->kill();
	}

	qDebug("InfoReaderMPV::run : terminating");

	QByteArray line;
	QList<QByteArray> f;
	while (proc->canReadLine()) {
		line = proc->readLine();
		line.replace("\n", "");
		line.replace("\r", "");
		line = line.simplified();
		if (line.startsWith("mpv")) {
			l.clear();
			l.append(InfoData(line,""));
			return l;
		}
		if (line.startsWith("Available") || line.startsWith("demuxer:") ||
			line.startsWith("Video decoders:") || line.startsWith("Audio decoders:"))
		{
			line = QByteArray();
		}
		if (!line.isEmpty()) {
			int pos = line.indexOf(' ');
			if (pos > -1) {
				QString name = line.left(pos);
				if (name.endsWith(':')) name = name.left(name.count()-1);
				QString desc = line.mid(pos+1);
				desc = desc.replace(": ", "").replace("- ", "");
				qDebug() << "InfoReaderMPV::run: name:" << name << "desc:" << desc;
				l.append(InfoData(name, desc));
			}
		}
	}

	return l;
}

#include "moc_inforeadermpv.cpp"
