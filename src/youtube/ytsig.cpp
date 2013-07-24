/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifdef YT_USE_SCRIPT
#include <QtScript>
#endif

#ifdef YT_USE_SCRIPT
QString YTSig::aclara(const QString & text) {
	int dot = text.indexOf('.');
	qDebug("YTSig::aclara: length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

	if (script.isEmpty()) script = default_script;

	QScriptEngine engine;

	//QScriptSyntaxCheckResult r = engine.checkSyntax(script);
	//qDebug() << (int) r.state();

	engine.evaluate(script);
	QScriptValue aclarar = engine.globalObject().property("aclara");
	QString res = aclarar.call(QScriptValue(), QScriptValueList() << text).toString();

	//qDebug() << res;

	if (res.isEmpty()) {
		qDebug("YTSig::aclara: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
	}

	return res;
}

void YTSig::reloadScriptFile() {
	qDebug("YTSig::reloadScriptFile: %s", script_file.toUtf8().constData());

	if (!QFile::exists(script_file)) {
		qDebug("YTSig::reloadScriptFile: file doesn't exist.");
		return;
	}

	QFile f(script_file);
	f.open(QIODevice::ReadOnly);
	QByteArray bytes = f.readAll();
	f.close();

	if (!bytes.isEmpty()) {
		script = bytes;
	}
}

QString YTSig::script;
QString YTSig::script_file;

// Algorithms from youtube-dl (http://rg3.github.io/youtube-dl/)

QString YTSig::default_script = 
"function aclara(s) {"
"  var r = \"\";"
"  var d = s.split(\".\");"
"  var id = d[0].length + \".\" + (s.length - d[0].length - 1);"
""
"  if (s.length == 87) {"
"    r = s.substr(4,19) + s.substr(86,1) + s.substr(24,61);"
"  }"
"  return r;"
"}";

#else

QString YTSig::rev(const QString & orig) {
	QString r;
	for (int n = orig.size()-1; n >= 0; n--) {
		r.append(orig.at(n));
	}
	return r;
}

QString YTSig::aclara(const QString & text) {
	QString res;

	int dot = text.indexOf('.');
	qDebug("YTSig::aclara: length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

	if (text.size() == 87) {
		res = text.mid(4,19) + text.mid(86,1) + text.mid(24,61);
	}
	else {
		qDebug("YTSig::aclara: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
		return res;
	}

	/*
	qDebug("%d: orig: %s", text.size(), text.toLatin1().constData());
	qDebug("%d: conv: %s", text.size(), res.toLatin1().constData());
	*/

	return res;
}

#endif
