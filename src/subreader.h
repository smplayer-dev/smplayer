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

#ifndef SUBREADER_H
#define SUBREADER_H

#include <QString>
#include <QList>

//#define SRT_OUTPUT

class Subtitle
{
public:
	Subtitle(const QString & time, const QByteArray & text) { this->time = time; this->text = text; };

	QString time;
	QByteArray text;
};

class SubReader
{
public:
	SubReader();
	~SubReader();

	void setInputCodec(const QByteArray & codec) { input_codec = codec; };
	QByteArray inputCodec() { return input_codec; };

	void setVTTLinePosition(int pos) { vtt_line_position = pos; }
	int VTTLinePosition() { return vtt_line_position; };

	void setTextFilter(const QString & f) { filter = f; }
	QString textFilter() { return filter; }

	void setOverwriteVTT(bool b) { overwrite_vtt = b; }
	bool overwriteVTT() { return overwrite_vtt; };

	void parseSRT(const QString & filename);

	QString convertToVTT();

#ifdef SRT_OUTPUT
	QByteArray convertToSRT();
#endif

	bool autoConvertToVTT(const QString & filename);

	bool saveFile(const QString & filename, const QByteArray & content);
	bool saveFile(const QString & filename, const QString & content);

	void dump();

private:
	QList<Subtitle> entries;
	QByteArray input_codec;
	QString filter;
	bool overwrite_vtt;
	int vtt_line_position;
	bool has_bom;
};

#endif
