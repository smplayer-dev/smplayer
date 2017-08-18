/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "audioequalizerlist.h"

QString AudioEqualizerHelper::equalizerListToString(AudioEqualizerList values, AudioEqualizerType type) {
	double v0 = (double) values[0].toInt() / 10;
	double v1 = (double) values[1].toInt() / 10;
	double v2 = (double) values[2].toInt() / 10;
	double v3 = (double) values[3].toInt() / 10;
	double v4 = (double) values[4].toInt() / 10;
	double v5 = (double) values[5].toInt() / 10;
	double v6 = (double) values[6].toInt() / 10;
	double v7 = (double) values[7].toInt() / 10;
	double v8 = (double) values[8].toInt() / 10;
	double v9 = (double) values[9].toInt() / 10;

	QString s;
	if (type == Equalizer) {
		s = QString::number(v0) + ":" + QString::number(v1) + ":" +
				QString::number(v2) + ":" + QString::number(v3) + ":" +
				QString::number(v4) + ":" + QString::number(v5) + ":" +
				QString::number(v6) + ":" + QString::number(v7) + ":" +
				QString::number(v8) + ":" + QString::number(v9);
	}
	else
	if (type == Anequalizer) {
		for (int ch = 0; ch < 2; ch++) {
			double freq = 31.25;
			for (int f = 0; f < 10; f++) {
				double v = (double) values[f].toInt() / 10;
				s += QString("c%1 f=%2 w=1000 g=%3|").arg(ch).arg(freq).arg(v);
				freq = freq * 2;
			}
		}
		//qDebug() << "AudioEqualizerHelper::equalizerListToString:" << s;
	}
	else
	if (type == Firequalizer) {
		s = "gain_entry='";
		double freq = 31.25;
		for (int f = 0; f < 10; f++) {
			double v = (double) values[f].toInt() / 10;
			s += QString("entry(%1,%2)").arg(freq).arg(v);
			if (f < 9) s += ";";
			freq = freq * 2;
		}
		s += "'";
	}

	return s;
}

QStringList AudioEqualizerHelper::equalizerListToStringList(AudioEqualizerList values, AudioEqualizerType type) {
	QStringList l;

	if (type == Firequalizer) {
		double freq = 31.25;
		for (int f = 0; f < 10; f++) {
			double v = (double) values[f].toInt() / 10;
			QString s = QString("gain_entry='entry(%1,%2)'").arg(freq).arg(v);
			freq = freq * 2;
			l << s;
		}
	}

	return l;
}
