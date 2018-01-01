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

#include "inforeadermpv.h"
#include "mplayerversion.h"
#include <QStringList>
#include <QProcess>
#include <QDebug>


InfoReaderMPV::InfoReaderMPV( QString mplayer_bin, QObject * parent )
	: QObject(parent)
	, mplayer_svn(0)
{
	mplayerbin = mplayer_bin;
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
	vf_list.clear();
	mplayer_svn = -1;

	vo_list = getList(run("--vo help"));
	ao_list = getList(run("--ao help"));
#if ALLOW_DEMUXER_CODEC_CHANGE
	demuxer_list = getList(run("--demuxer help"));
	vc_list = getList(run("--vd help"));
	ac_list = getList(run("--ad help"));
#endif
	{
		InfoList list = getList(run("--vf help"));
		for (int n = 0; n < list.count(); n++) {
			vf_list.append(list[n].name());
		}
	}

	QList<QByteArray> lines = run("--version");

	QString mpv_version_line;
	if (lines.count() >= 1) {
		mpv_version_line = lines[0];
		mplayer_svn = MplayerVersion::mplayerVersion(mpv_version_line);
		mpv_version = MplayerVersion::mpvVersion();
	}

	qDebug() << "InfoReaderMPV::getInfo: version_line" << mpv_version_line;
	qDebug() << "InfoReaderMPV::getInfo: mplayer_svn" << mplayer_svn;

	option_list = getOptionsList(run("--list-options"));

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

QList<QByteArray> InfoReaderMPV::run(QString options) {
	qDebug("InfoReaderMPV::run: '%s'", options.toUtf8().data());

	QList<QByteArray> r;

	QProcess proc(this);
	proc.setProcessChannelMode( QProcess::MergedChannels );

	QStringList args = options.split(" ");

	proc.start(mplayerbin, args);
	if (!proc.waitForStarted()) {
		qWarning("InfoReaderMPV::run: process can't start!");
		return r;
	}

	//Wait until finish
	if (!proc.waitForFinished()) {
		qWarning("InfoReaderMPV::run: process didn't finish. Killing it...");
		proc.kill();
	}

	QByteArray data = proc.readAll().replace("\r", "");
	r = data.split('\n');
	return r;
}

InfoList InfoReaderMPV::getList(const QList<QByteArray> & lines) {
	InfoList l;

	foreach(QByteArray line, lines) {
		//qDebug() << "InfoReaderMPV::getList: line:" << line;

		line.replace("\n", "");
		line = line.simplified();
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
				//qDebug() << "InfoReaderMPV::getList: name:" << name << "desc:" << desc;
				l.append(InfoData(name, desc));
			}
		}
	}

	return l;
}

QStringList InfoReaderMPV::getOptionsList(const QList<QByteArray> & lines) {
	QStringList l;

	foreach(QByteArray line, lines) {
		//qDebug() << "InfoReaderMPV::getOptionsList: line:" << line;
		line.replace("\n", "");
		line = line.simplified();
		if (line.startsWith("--")) {
			int pos = line.indexOf(' ');
			if (pos > -1) {
				QString option_name = line.left(pos);
				//qDebug() << "InfoReaderMPV::getOptionsList: option:" << option_name;
				l << option_name;
			}
		}
	}

	return l;
}

#include "moc_inforeadermpv.cpp"
