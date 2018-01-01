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

#include "ytsig.h"
#include <QtScript>

QString YTSig::script_filename;

QString YTSig::script;
QString YTSig::default_script;

QString YTSig::parsed_ts;


QString YTSig::aclara(const QString & text, const QString & player, const QString & function_name) {
	int dot = text.indexOf('.');
	qDebug("YTSig::aclara: length: %d (%d.%d) p: %d", text.size(), dot, text.size()-dot-1, !player.isEmpty());

	if (script.isEmpty()) script = default_script;

	QScriptEngine engine;

	//QScriptSyntaxCheckResult r = engine.checkSyntax(script);
	//qDebug() << (int) r.state();

	engine.evaluate(script);

	QScriptValueList args;
	QString fname;

	if (!function_name.isEmpty()) {
		fname = function_name;
		args << text;
	}
	else
	if (player.isEmpty()) {
		fname = "aclara";
		args << text;
	}
	else {
		fname = "aclara_p";
		args << text << player;
	}

	//qDebug() << "YTSig::aclara: function_name:" << function_name;

	QScriptValue aclarar = engine.globalObject().property(fname);
	QString res = aclarar.call(QScriptValue(), args).toString();

	//qDebug() << res;

	if (res.isEmpty()) {
		qDebug() << "YTSig::aclara: signature length not supported:" << text.size() << ":" << text;
	}

	return res;
}

void YTSig::reloadScriptFile() {
	qDebug() << "YTSig::reloadScriptFile:" << script_filename;

	if (!QFile::exists(script_filename)) {
		qDebug() << "YTSig::reloadScriptFile: file doesn't exist.";
		return;
	}

	QFile f(script_filename);
	f.open(QIODevice::ReadOnly);
	QByteArray bytes = f.readAll();
	f.close();

	parsed_ts = "";

	if (!bytes.isEmpty()) {
		script = bytes;

		QRegExp rx("D: ([\\d,a-z,A-Z-]+)");
		if (rx.indexIn(bytes)) {
			QByteArray d = rx.cap(1).toLatin1();
			qDebug() << "YTSig::reloadScriptFile: d:" << d;
			parsed_ts = QByteArray::fromBase64(d);
		}

	}
}

void YTSig::check(QString & u) {
	if (!parsed_ts.isEmpty()) {
		u.append(QString("&%1").arg(parsed_ts));
	}
}

