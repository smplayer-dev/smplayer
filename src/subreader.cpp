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

#include "subreader.h"
#include <QFile>
#include <QRegExp>
#include <QTextCodec>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#define NL QString("\r\n")
#define BOM QString(0xFEFF)

SubReader::SubReader() {
	input_codec = "ISO-8859-1";
	vtt_line_position = -1;
	overwrite_vtt = false;
	has_bom = false;
}

SubReader::~SubReader() {
}

void SubReader::parseSRT(const QString & filename) {
	entries.clear();

	QRegExp rx_number("^(\\d+)$");
	QRegExp rx_time("^(.* --> .*)$");

	bool first_line = true;
	has_bom = false;

	QFile file(filename);
	if (file.open(QIODevice::ReadOnly)) {
		while (!file.atEnd()) {
			QByteArray line = file.readLine().trimmed();

			if (first_line) {
				first_line = false;
				if (line.count() > 2 && line.left(3) == QByteArray("\xEF\xBB\xBF")) {
					qDebug("SubReader::parseSRT: found BOM");
					has_bom = true;
				}
			}

			//qDebug() << "line:" << line;
			if (rx_number.indexIn(line) != -1) {
				// Ignore
			}
			else
			if (rx_time.indexIn(line) != -1) {
				QString time = rx_time.cap(1);
				//qDebug() << "time:" << time;
				// Read text
				QByteArray text;
				QByteArray line2;
				do {
					line2 = file.readLine().trimmed();
					//qDebug() << "line2:" << line2;
					if (!text.isEmpty() && !line2.isEmpty()) text += NL;
					if (!line2.isEmpty()) text += line2;
				} while (!file.atEnd() && !line2.isEmpty());
				//qDebug() << "text:" << text;

				Subtitle sub(time, text);
				entries << sub;
			}
		}
	}
}

void SubReader::dump() {
	for (int n = 0; n < entries.count(); n++) {
		qDebug() << "sub:" << n;
		qDebug() << "time:" << entries[n].time;
		qDebug() << "text:" << entries[n].text;
	}
}

QString SubReader::convertToVTT() {
	QByteArray codec_name = input_codec;
	if (has_bom) codec_name = "UTF-8";
	QTextCodec *codec = QTextCodec::codecForName(codec_name);

	qDebug() << "SubReader::convertToVTT: codec_name:" << codec_name;

	QRegExp rx(filter, Qt::CaseInsensitive);

	QString res = BOM + "WEBVTT" + NL + NL;

	int filtered_lines = 0;

	for (int n = 0; n < entries.count(); n++) {
		QString time = entries[n].time;
		time.replace(",", ".");
		QString text = codec->toUnicode(entries[n].text).trimmed();
		if (!text.isEmpty()) {
			if (!filter.isEmpty() && rx.indexIn(text) != -1) {
				//qDebug() << "SubReader::convertToVTT: filtered:" << text;
				filtered_lines++;
				res += "NOTE " + time + NL + "NOTE " + text.replace("\r\n", "\r\nNOTE ") + NL + NL;
			} else {
				res += time;
				if (vtt_line_position > -1) res += " line:" + QString::number(vtt_line_position) + "%";
				res += NL;
				res += text + NL + NL;
			}
		}
	}

	if (filtered_lines != 0) {
		qDebug() << "SubReader::convertToVTT: filtered lines:" << filtered_lines;
	}

	return res;
}

#ifdef SRT_OUTPUT
QByteArray SubReader::convertToSRT() {
	QByteArray res;

	QRegExp rx(filter, Qt::CaseInsensitive);

	for (int n = 0; n < entries.count(); n++) {
		res += QString::number(n+1) + NL;
		res += entries[n].time + NL;
		if (!filter.isEmpty() && rx.indexIn(QString::fromLatin1(entries[n].text)) != -1) {
			qDebug() << "SubReader::convertToSRT: filtered:" << entries[n].text;
			res += "   " + NL + NL;
		} else {
			res += entries[n].text + NL + NL;
		}
	}

	return res;
}
#endif

bool SubReader::saveFile(const QString & filename, const QByteArray & content) {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "SubReader::saveFile: can't open" << filename << "for writing";
		return false;
	}

	file.write(content);
	file.close();

	return true;
}

bool SubReader::saveFile(const QString & filename, const QString & content) {
	return saveFile(filename, content.toUtf8());
}

bool SubReader::autoConvertToVTT(const QString & filename) {
	QFileInfo fi(filename);

	if (!fi.exists()) {
		qDebug() << "SubReader::autoConvertToVTT: error: file" << filename << "doesn't exist";
		return false;
	}

	QString path = fi.absolutePath();
	QString basename = fi.completeBaseName();
	QString extension = fi.suffix();
	qDebug() << "SubReader::autoConvertToVTT: path:" << path << "basename:" << basename << "extension:" << extension;

	if (extension.toLower() != "srt") {
		qDebug() << "SubReader::autoConvertToVTT: input filename isn't a srt file. Exiting";
		return false;
	}

	QString output_basename = basename + ".vtt";
	QString output_filename = path + QDir::separator() + output_basename;
	qDebug() << "SubReader::autoConvertToVTT: output:" << output_filename;

	if (QFile::exists(output_filename)) {
		if (!overwrite_vtt) {
			qDebug() << "SubReader::autoConvertToVTT: file" << output_filename << "already exists. Skipping.";
			return true;
		} else {
			qDebug() << "SubReader::autoConvertToVTT: overwriting " << output_filename;
		}
	}

	parseSRT(filename);
	QString content = convertToVTT();
	return saveFile(output_filename, content);
}

