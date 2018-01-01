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

#include "infofile.h"
#include "discname.h"
#include "images.h"

#include <QFileInfo>
#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>

InfoFile::InfoFile(QObject * parent)
	: QObject(parent)
#ifndef INFO_SIMPLE_LAYOUT
	, row(0)
#endif
{
}

InfoFile::~InfoFile() {
}

QString InfoFile::getInfo(MediaData md) {
	QString s;

	// General
	QFileInfo fi(md.filename);

	QString icon;
	switch (md.type) {
		case TYPE_FILE	:	if (md.novideo) 
								icon = "type_audio.png";
							else
								icon = "type_video.png"; 
							break;
		case TYPE_DVD	: 	icon = "type_dvd.png"; break;
		case TYPE_VCD	: 	icon = "type_vcd.png"; break;
		case TYPE_AUDIO_CD	: 	icon = "type_vcd.png"; break;
		case TYPE_TV	: 	icon = "type_tv.png"; break;
		case TYPE_STREAM : 	icon = "type_url.png"; break;
#ifdef BLURAY_SUPPORT
		case TYPE_BLURAY : 	icon = "type_bluray.png"; break;
#endif
		default 		: 	icon = "type_unknown.png";
	}
	icon = icon.replace(".png", ""); // FIXME
	//icon = "<img src=\"" + Images::file(icon) + "\"> ";

#ifdef BLURAY_SUPPORT
	if (md.type == TYPE_DVD || md.type == TYPE_BLURAY)
#else
	if (md.type == TYPE_DVD)
#endif
	{
		DiscData disc_data = DiscName::split(md.filename);
		s += title(disc_data.protocol + "://" + QString::number(disc_data.title), icon);
	} else {
		s += title(md.displayName(), icon);
	}

	s += openPar( tr("General") );
	if (fi.exists()) {
		//s += addItem( tr("Path"), fi.dirPath() );
		s += addItem( tr("File"), fi.absoluteFilePath() );
		s += addItem( tr("Size"), tr("%1 KB (%2 MB)").arg(fi.size()/1024)
                                  .arg(fi.size()/1048576) );
	} else {
		QString url = md.filename;
		s += addItem( tr("URL"), url );
		/*
		if (!md.stream_path.isEmpty() && md.stream_path != url) {
			s += addItem( tr("Video URL"), md.stream_path );
		}
		*/
	}
	s += addItem( tr("Length"), Helper::formatTime((int)md.duration) );
	s += addItem( tr("Demuxer"), md.demuxer );
	s += closePar();

	// Clip info
	QString c;
	if (!md.clip_name.isEmpty()) c+= addItem( tr("Name"), md.clip_name );
	if (!md.clip_artist.isEmpty()) c+= addItem( tr("Artist"), md.clip_artist );
	if (!md.clip_author.isEmpty()) c+= addItem( tr("Author"), md.clip_author );
	if (!md.clip_album.isEmpty()) c+= addItem( tr("Album"), md.clip_album );
	if (!md.clip_genre.isEmpty()) c+= addItem( tr("Genre"), md.clip_genre );
	if (!md.clip_date.isEmpty()) {
		QString s = md.clip_date;
		QDateTime d = QDateTime::fromString(md.clip_date, Qt::ISODate);
		if (d.isValid()) {
			s = d.toString("yyyy-MM-dd hh:mm:ss");
			/* s = QLocale::system().toString(d, QLocale::ShortFormat); */
		}
		c+= addItem( tr("Date"), s );
	}
	if (!md.clip_track.isEmpty()) c+= addItem( tr("Track"), md.clip_track );
	if (!md.clip_copyright.isEmpty()) c+= addItem( tr("Copyright"), md.clip_copyright );
	if (!md.clip_comment.isEmpty()) c+= addItem( tr("Comment"), md.clip_comment );
	if (!md.clip_software.isEmpty()) c+= addItem( tr("Software"), md.clip_software );
	if (!md.stream_title.isEmpty()) c+= addItem( tr("Stream title"), md.stream_title );
	if (!md.stream_url.isEmpty()) c+= addItem( tr("Stream URL"), md.stream_url );

	if (!c.isEmpty()) {
		s += openPar( tr("Clip info") );
		s += c;
		s += closePar();
	}

	// Video info
	if (!md.novideo) {
		s += openPar( tr("Video") );
		s += addItem( tr("Resolution"), QString("%1 x %2").arg(md.video_width).arg(md.video_height) );
		s += addItem( tr("Aspect ratio"), QString::number(md.video_aspect) );
		s += addItem( tr("Format"), md.video_format );
		s += addItem( tr("Bitrate"), tr("%1 kbps").arg(md.video_bitrate / 1000) );
		s += addItem( tr("Frames per second"), md.video_fps );
		s += addItem( tr("Selected codec"), md.video_codec );
		s += closePar();
	}

	// Audio info
	s += openPar( tr("Initial Audio Stream") );
	s += addItem( tr("Format"), md.audio_format );
	s += addItem( tr("Bitrate"), tr("%1 kbps").arg(md.audio_bitrate / 1000) );
	s += addItem( tr("Rate"), tr("%1 Hz").arg(md.audio_rate) );
	s += addItem( tr("Channels"), QString::number(md.audio_nch) );
	s += addItem( tr("Selected codec"), md.audio_codec );
	s += closePar();

	// Audio Tracks
	if (md.audios.numItems() > 0) {
		s += openPar( tr("Audio Streams") );
		s += addTrackColumns( QStringList() << "#" << tr("Language") << tr("Name") << "ID" );

		for (int n = 0; n < md.audios.numItems(); n++) {
			#ifndef INFO_SIMPLE_LAYOUT
			row++;
			#endif
			s += openItem();
			QString lang = md.audios.itemAt(n).lang();
			if (lang.isEmpty()) lang = "<i>&lt;"+tr("undefined")+"&gt;</i>";
			QString name = md.audios.itemAt(n).name();
			if (name.isEmpty()) name = "<i>&lt;"+tr("undefined")+"&gt;</i>";
			s += addTrack(n, lang, name, md.audios.itemAt(n).ID());
			s += closeItem();
		}
		s += closePar();
	}

	// Subtitles
	if (md.subs.numItems() > 0) {
		s += openPar( tr("Subtitles") );
		s += addTrackColumns( QStringList() << "#" << tr("Type") << tr("Language") << tr("Name") << "ID" );
		for (int n = 0; n < md.subs.numItems(); n++) {
			#ifndef INFO_SIMPLE_LAYOUT
			row++;
			#endif
			s += openItem();
			QString t;
			switch (md.subs.itemAt(n).type()) {
				case SubData::File: t = "FILE_SUB"; break;
				case SubData::Vob:	t = "VOB"; break;
				default:			t = "SUB";
			}
			QString lang = md.subs.itemAt(n).lang();
			if (lang.isEmpty()) lang = "<i>&lt;"+tr("undefined")+"&gt;</i>";
			QString name = md.subs.itemAt(n).name();
			if (name.isEmpty()) name = "<i>&lt;"+tr("undefined")+"&gt;</i>";
			s += addTrack(n, lang, name, md.subs.itemAt(n).ID(), t);
			s += closeItem();
		}
		s += closePar();
	}

	QString page = "<html><head><style type=\"text/css\">" + style() + "</style></head><body>"+ s + "</body></html>";
	//qDebug() << "InfoFile::getInfo:" << page;
	return page;
}


#ifdef INFO_SIMPLE_LAYOUT
QString InfoFile::title(QString text, QString /* icon */) {
	return QString("<h1>%1</h1>").arg(text);
}

QString InfoFile::openPar(QString text) {
	return "<h2>" + text + "</h2><ul>";
}

QString InfoFile::closePar() {
	return "</ul>";
}

QString InfoFile::openItem() {
	return "<li>";
}

QString InfoFile::closeItem() {
	return "</li>";
}

QString InfoFile::addItem( QString tag, QString value ) {
	return openItem() + QString("<b>%1</b>: %2").arg(tag).arg(value) + closeItem();
}

QString InfoFile::addTrackColumns(QStringList /* l */) {
	return "";
}

QString InfoFile::addTrack(int n, QString lang, QString name, int ID, QString type) {
	QString s = "<b>" + tr("Track %1").arg(n) + "</b>";
	#if 1
	s += "<ul>";
	s += "<li>" + tr("Language: %1").arg(lang) + "</li>";
	s += "<li>" + tr("Name: %1").arg(name) + "</li>";
	s += "<li>" + tr("ID: %1").arg(ID) + "</li>";
	if (!type.isEmpty()) {
		s += "<li>" + tr("Type: %1").arg(type) + "</li>";
	}
	s += "</ul>";
	#else
	s += "<br>&nbsp;&bull; " + tr("Language: %1").arg(lang);
	s += "<br>&nbsp;&bull; " + tr("Name: %1").arg(name);
	s += "<br>&nbsp;&bull; " + tr("ID: %1").arg(ID);
	if (!type.isEmpty()) {
		s += "<br>&nbsp;&bull; " + tr("Type: %1").arg(type);
	}
	#endif
	return s;
}

QString InfoFile::defaultStyle() {
	return
		"ul { margin: 0px; }"
		//"body { background-color: gray; }"
		"h2 { background-color: whitesmoke; color: navy;}"
	;
}

#else

QString InfoFile::title(QString text, QString icon) {
	return QString("<h1><img src=\"%1\">%2</h1>").arg(Images::file(icon)).arg(text);
}

QString InfoFile::openPar(QString text) {
	return "<h2>" + text + "</h2>"
           "<table width=\"100%\">";
}

QString InfoFile::closePar() {
	row = 0;
	return "</table>";
}

QString InfoFile::openItem() {
	if (row % 2 == 1)
		return "<tr bgcolor=\"lavender\">";
	else
		return "<tr bgcolor=\"powderblue\">";
}

QString InfoFile::closeItem() {
	return "</tr>";
}

QString InfoFile::addTrackColumns(QStringList l) {
	row = 0;
	QString s = openItem();
	foreach(QString i, l) { s += "<td>" + i + "</td>"; }
	s += closeItem();
	return s;
}

QString InfoFile::addItem( QString tag, QString value ) {
	row++;
	return openItem() + 
           "<td><b>" + tag + "</b></td>" +
           "<td>" + value + "</td>" +
           closeItem();
}

QString InfoFile::addTrack(int n, QString lang, QString name, int ID, QString type) {
	QString s = "<td>" + QString::number(n) + "</td>";
	if (!type.isEmpty()) s += "<td>" + type + "</td>";
	s += QString("<td>%1</td><td>%2</td><td>%3</td>").arg(lang).arg(name).arg(ID);
	return s;
}

QString InfoFile::defaultStyle() {
	return "";
}
#endif

QString InfoFile::style() {
	QString s = defaultStyle();

	QString stylesheet_file = Images::file("infofile.css");
	qDebug() << "InfoFile::style: stylesheet_file:" << stylesheet_file;

	QFile file(stylesheet_file);
	if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		s = file.readAll();
	}

	return s;
}

#include "moc_infofile.cpp"
