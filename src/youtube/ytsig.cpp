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
	qDebug("ytsig: length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

	if (script.isEmpty()) script = default_script;

	QScriptEngine engine;

	//QScriptSyntaxCheckResult r = engine.checkSyntax(script);
	//qDebug() << (int) r.state();

	engine.evaluate(script);
	QScriptValue aclarar = engine.globalObject().property("aclara");
	QString res = aclarar.call(QScriptValue(), QScriptValueList() << text).toString();

	//qDebug() << res;

	if (res.isEmpty()) {
		qDebug("ytsig: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
	}

	return res;
}

QString YTSig::script;

QString YTSig::default_script = 
"function aclara(s) {"
"  var r = \"\";"
"  var d = s.split(\".\");"
"  var id = d[0].length + \".\" + (s.length - d[0].length - 1);"
""
"  if (s.length == 87) {"
"    r = s.substr(62,1) + s.substr(63,20).split(\"\").reverse().join(\"\") + s.substr(83,1) + s.substr(53,9).split(\"\").reverse().join(\"\") + s.substr(0,1) + s.substr(3,49).split(\"\").reverse().join(\"\");"
"  }"
"  else"
"  if (s.length == 86) {"
"    r = s.substr(2,61) + s.substr(82,1) + s.substr(64,18) + s.substr(63,1);"
"  }"
"  else"
"  if ((s.length == 83) && (id == \"40.42\")) {"
"    r = s.substr(0,81);"
"  }"
"  else"
"  if ((s.length == 83) && (id == \"42.40\")) {"
"    r = s.substr(6,1) + s.substr(3,3) + s.substr(33,1) + s.substr(7,17) + s.substr(0,1) + s.substr(25,8) + s.substr(53,1) + s.substr(34,19) + s.substr(24,1) + s.substr(54);"
"  }"
"  else"
"  if (s.length == 81) {"
"    r = s.substr(6,1) + s.substr(3,3) + s.substr(33,1) + s.substr(7,17) + s.substr(0,1) + s.substr(25,8) + s.substr(2,1) + s.substr(34,19) + s.substr(24,1) + s.substr(54,27);"
"  }"
"  else"
"  if (s.length == 92) {"
"    r = s.substr(25,1) + s.substr(3,22) + s.substr(0,1) + s.substr(26,16) + s.substr(79,1) + s.substr(43,36) + s.substr(91,1) + s.substr(80,3);"
"  }"
"  else"
"  if (s.length == 85) {"
"    r = s.substr(2,6) + s.substr(0,1) + s.substr(9,12) + s.substr(65,1) + s.substr(22,43) + s.substr(84,1) + s.substr(66,16) + s.substr(21,1);"
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

// Algorithms from youtube-dl (http://rg3.github.io/youtube-dl/)
QString YTSig::aclara(const QString & text) {
	QString res;

	int dot = text.indexOf('.');
	qDebug("ytsig: length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

	if (text.size() == 86) {
		res = text.mid(2,61) + text.mid(82,1) + text.mid(64,18) + text.mid(63,1);
	}
	else
	if (text.size() == 87) {
		res = text.mid(62,1) + rev(text.mid(63,20)) + text.mid(83,1) + rev(text.mid(53,9)) + text.mid(0,1) + rev(text.mid(3,49));
	}
	else
	if (text.size() == 83) {
		if (dot == 40) {
			// 40.42
			res = text.left(81);
		} else {
			// 42.40
			res = text.mid(6,1) + text.mid(3,3) + text.mid(33,1) + text.mid(7,17) + text.mid(0,1) + text.mid(25,8) + text.mid(53,1) + text.mid(34,19) + text.mid(24,1) + text.mid(54);
		}
	}
	else
	if (text.size() == 81) {
		res = text.mid(6,1) + text.mid(3,3) + text.mid(33,1) + text.mid(7,17) + text.mid(0,1) + text.mid(25,8) + text.mid(2,1) + text.mid(34,19) + text.mid(24,1) + text.mid(54,27);
	}
	else
	if (text.size() == 92) {
		res = text.mid(25,1) + text.mid(3,22) + text.mid(0,1) + text.mid(26,16) + text.mid(79,1) + text.mid(43,36) + text.mid(91,1) + text.mid(80,3);
	}
	else
	if (text.size() == 85) {
		res = text.mid(2,6) + text.mid(0,1) + text.mid(9,12) + text.mid(65,1) + text.mid(22,43) + text.mid(84,1) + text.mid(66,16) + text.mid(21,1);
	}
	else {
		qDebug("ytsig: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
		return res;
	}

	/*
	qDebug("%d: orig: %s", text.size(), text.toLatin1().constData());
	qDebug("%d: conv: %s", text.size(), res.toLatin1().constData());
	*/

	return res;
}

#endif
