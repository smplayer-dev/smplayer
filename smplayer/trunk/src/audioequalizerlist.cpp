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
#include <QDebug>

#define ANEQUALIZER_CHANNELS 2
#define ANEQUALIZER_WIDTH 1000
#define ANEQUALIZER_MUL 20 / 240

#define FIREQUALIZER_MUL 0.1

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
		for (int ch = 0; ch < ANEQUALIZER_CHANNELS; ch++) {
			double freq = 31.25;
			for (int f = 0; f < 10; f++) {
				double v = (double) values[f].toInt() * ANEQUALIZER_MUL;
				s += QString("c%1 f=%2 w=%4 g=%3|").arg(ch).arg(freq).arg(v).arg(ANEQUALIZER_WIDTH);
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
			double v = (double) values[f].toInt() * FIREQUALIZER_MUL;
			s += QString("entry(%1,%2)").arg(freq).arg(v);
			if (f < 9) s += ";";
			freq = freq * 2;
		}
		s += "'";
	}
	else
	if (type == Superequalizer) {
		for (int n = 0; n < 10; n++) {
			double v = (double) (values[n].toInt() + 120) * 20 / 240;
			if (n == 1) s += "2b=" + QString::number(v) + ":";
			if (n == 2) s += "3b=" + QString::number(v) + ":";
			if (n == 3) s += "5b=" + QString::number(v) + ":";
			if (n == 4) s += "7b=" + QString::number(v) + ":";
			if (n == 5) s += "9b=" + QString::number(v) + ":";
			if (n == 6) s += "11b=" + QString::number(v) + ":";
			if (n == 7) s += "13b=" + QString::number(v) + ":";
			if (n == 8) s += "15b=" + QString::number(v) + ":";
			if (n == 9) s += "17b=" + QString::number(v) + ":";
		}
	}

	return s;
}

QStringList AudioEqualizerHelper::equalizerListForCommand(AudioEqualizerList values,
	AudioEqualizerList old_values, AudioEqualizerType type) 
{
	QStringList l;

	if (type == Anequalizer) {
		double freq = 31.25;
		for (int f = 0; f < 10; f++) {
			if (!old_values.isEmpty() && values[f] != old_values[f]) {
				double v = (double) values[f].toInt() * ANEQUALIZER_MUL;
				for (int ch = 0; ch < ANEQUALIZER_CHANNELS; ch++) {
					l << QString("%1|f=%2|w=%4|g=%3").arg(f + ch*10).arg(freq).arg(v).arg(ANEQUALIZER_WIDTH);
				}
			}
			freq = freq * 2;
		}
	}
	else
	if (type == Firequalizer) {
		double freq = 31.25;
		for (int f = 0; f < 10; f++) {
			if (!old_values.isEmpty() && values[f] != old_values[f]) {
				double v = (double) values[f].toInt() * FIREQUALIZER_MUL;
				// This is probably wrong
				l << QString("entry(%1,%2)").arg(freq).arg(v);
			}
		freq = freq * 2;
		}
	}

	return l;
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
	else
	if (type == FEqualizer) {
		double freq = 31.25;
		for (int f = 0; f < 10; f++) {
			double v = (double) values[f].toInt() / 4;
			QString s = QString("f=%1:width_type=q:width=1:g=%2").arg(freq).arg(v);
			freq = freq * 2;
			l << s;
		}
	}

	return l;
}
