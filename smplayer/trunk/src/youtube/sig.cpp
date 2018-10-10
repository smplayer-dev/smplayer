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

#include "sig.h"
#include <QRegExp>
#include <QStringList>
#include <QSettings>
#include <QDebug>

#ifdef SIG_USE_JSCODE
#include <QtScript>
#endif

//#define ULTRAVERBOSE

QString Sig::playerURL(const QString & player_name) {
#ifdef SIG_FROM_SMPLAYER_SITE
	QString p = player_name.toUtf8().toBase64();
	return QString("http://updates.smplayer.info/ytsig/?e=%1&c=1").arg(p);
#else
	return QString("http://s.ytimg.com/yts/jsbin/%1/base.js").arg(player_name);
#endif
}

#ifdef SIG_FROM_SMPLAYER_SITE
QString Sig::findFunctions(const QString & text) {
	qDebug() << "Sig::findFunctions:" << text;

	sts = "";
	function_name = "";
	decrypt_function = "";

	QRegExp rx;
	rx.setMinimal(true);

	rx.setPattern("sts:(\\d+)\n");
	if (rx.indexIn(text) != -1) {
		sts = rx.cap(1);
	}
	qDebug() << "Sig::findFunctions: sts:" << sts;

	rx.setPattern("function_name:(.*)\n");
	if (rx.indexIn(text) != -1) {
		function_name = rx.cap(1);
	}
	qDebug() << "Sig::findFunctions: function_name:" << function_name;

	rx.setPattern("function:(.*)\n");
	if (rx.indexIn(text) != -1) {
		decrypt_function = rx.cap(1);
	}
	qDebug() << "Sig::findFunctions: decrypt_function:" << decrypt_function;

	rx.setPattern("code:(.*)\n");
	if (rx.indexIn(text) != -1) {
		decrypt_code = rx.cap(1).trimmed();
	}
	qDebug() << "Sig::findFunctions: decrypt_code:" << decrypt_code;

	return decrypt_function;
}

#else

QString Sig::findText(const QString & text, const QString & begin, const QString & end) {
	QString res;

	int pos = text.indexOf(begin);
	if (pos > -1) {
		int pos2 = text.indexOf(end, pos);
		if (pos2 > -1) {
			pos = pos + begin.length();
			res = text.mid(pos, pos2 - pos);
		}
	}

	return res;
}

QString Sig::findFunctions(const QString & text) {
	QString res;

	//QString sts;
	QRegExp rx_sts("sts:(\\d+)");
	if (rx_sts.indexIn(text) != -1) {
		sts = rx_sts.cap(1);
	}
	qDebug() << "Sig::findFunctions: sts:" << sts;

	QString sig_name;
	//QRegExp rx_sig("\\.sig\\|\\|([a-zA-Z0-9\\$]+)\\(");
	//QRegExp rx_sig("([\"\'])signature\\1\\s*,\\s*([a-zA-Z0-9$]+)\\(");
	QRegExp rx_sig("yt\\.akamaized\\.net\\/\\).*\\.set\\(.*,([a-zA-Z0-9$]+)\\(");
	rx_sig.setMinimal(true);
	if (rx_sig.indexIn(text) != -1) {
		sig_name = rx_sig.cap(1);
		//sig_name = rx_sig.cap(2);
	}
	qDebug() << "Sig::findFunctions: sig_name:" << sig_name;

	QString sig_code;
	if (!sig_name.isEmpty()) {
		//int pos = text.indexOf("function " + sig_name);
		int pos = text.indexOf("var " + sig_name + "=function");
		if (pos == -1) pos = text.indexOf("," + sig_name + "=function");
		if (pos == -1) pos = text.indexOf(sig_name + "=function");
		if (pos > -1) {
			int endpos = text.indexOf("}", pos);
			#ifdef ULTRAVERBOSE
			qDebug() << "Sig::findFunctions: pos:" << pos << "endpos:" << endpos;
			#endif
			if (endpos > -1) {
				sig_code = text.mid(pos, (endpos-pos)+1);
				if (sig_code.startsWith("var ")) sig_code.replace("var " + sig_name + "=function", "function " + sig_name);
				else
				if (sig_code.startsWith(",")) sig_code.replace("," + sig_name + "=function", "function " + sig_name);
				else
				if (sig_code.startsWith(sig_name+"=")) sig_code.replace(sig_name + "=function", "function " + sig_name);
			}
		}
	}
	qDebug() << "Sig::findFunctions: sig_code:" << sig_code;

	// Find the 2nd function
	QString function2;
	if (!sig_code.isEmpty()) {
		QStringList parts = sig_code.split(";");
		#ifdef ULTRAVERBOSE
		qDebug() << "Sig::findFunctions: parts:" << parts;
		#endif
		foreach(QString part, parts) {
			//qDebug() << "Sig::findFunctions: part:" << part;
			QRegExp rx("([a-zA-Z\\$]+)\\.([a-zA-Z0-9]+)\\(");
			if (rx.indexIn(part) != -1) {
				QString possible_var = rx.cap(1);
				#ifdef ULTRAVERBOSE
				qDebug() << "Sig::findFunctions: possible var:" << possible_var;
				#endif
				if ((possible_var != "a") && (possible_var != "")) {
					QString s = findText(text, "var "+ possible_var +"=", "};");
					#ifdef ULTRAVERBOSE
					qDebug() << "Sig::findFunctions: s:" << s;
					#endif
					if (!s.isEmpty()) {
						function2 = "var "+ possible_var +" = "+ s +" };";
						break;
					}
				}
			}
		}
	}
	qDebug() << "Sig::findFunctions: function2:" << function2;

	if (!sig_code.isEmpty()) {
		//sig_code = sig_code.replace("function "+ sig_name + "(", "function aclara(");
		function_name = sig_name;
		#ifdef ULTRAVERBOSE
		qDebug() << "Sig::findFunctions: sig_code:" << sig_code;
		#endif
		res = sig_code + " " + function2;
	}

	qDebug() << "Sig::findFunctions: res:" << res;

	decrypt_function = res;
	return res;
}
#endif

QString Sig::aclara(const QString & signature) {
#if defined(SIG_USE_JSCODE) && defined(SIG_USE_NO_JSCODE)
	if (!decrypt_code.isEmpty()) {
		return aclaraNoJS(signature);
	} else {
		return aclaraJS(signature);
	}
#else
#ifdef SIG_USE_JSCODE
	return aclaraJS(signature);
#else
	return aclaraNoJS(signature);
#endif
#endif
}

#ifdef SIG_USE_JSCODE
QString Sig::aclaraJS(const QString & signature) {
	int dot = signature.indexOf('.');
	qDebug("Sig::aclaraJS: length: %d (%d.%d)", signature.size(), dot, signature.size()-dot-1);

	QString res;

	if (!decrypt_function.isEmpty()) {
		QString fname = function_name;
		QScriptEngine engine;
		engine.evaluate(decrypt_function);

		QScriptValueList args;
		args << signature;
		QScriptValue aclarar = engine.globalObject().property(fname);
		res = aclarar.call(QScriptValue(), args).toString();
	}

	#ifdef ULTRAVERBOSE
	qDebug() << "Sig::aclara: orig:" << signature;
	qDebug() << "Sig::aclara: res:" << res;
	#endif

	if (res.isEmpty()) {
		qDebug() << "Sig::aclara: signature length not supported:" << signature.size() << ":" << signature;
	}

	return res;
}
#endif

#ifdef SIG_USE_NO_JSCODE
QString Sig::reverseString(const QString & orig) {
	QString r;
	for (int n = orig.size()-1; n >= 0; n--) {
		r.append(orig.at(n));
	}
	return r;
}

QString Sig::aclaraNoJS(const QString & signature) {
	int dot = signature.indexOf('.');
	qDebug("Sig::aclaraNoJS: length: %d (%d.%d)", signature.size(), dot, signature.size()-dot-1);

	QString s = signature;

	if (decrypt_code.isEmpty()) {
		qDebug() << "Sig::aclara: decrypt_code not found";
		return "";
	}

	QStringList commands = decrypt_code.split(";", QString::SkipEmptyParts);
	foreach(QString command, commands) {
		#ifdef ULTRAVERBOSE
		qDebug() << "Sig::aclara: command:" << command;
		#endif
		QRegExp rx("(\\w+)\\((\\d+)\\)");
		if (rx.indexIn(command) != -1) {
			QString c = rx.cap(1);
			int v = rx.cap(2).toInt();
			//qDebug() << "c:" << c << "v:" << v;
			if (c == "slice") {
				s = s.mid(v);
			}
			else
			if (c == "swap") {
				int p = v % s.length();
				QChar c = s[0];
				s[0] = s[p];
				s[p] = c;
			}
			else
			if (c == "reverse") {
				s = reverseString(s);
			}
			else {
				qDebug() << "Sig::aclara: unknown command:" << command;
			}
		} else {
			// Error
			qDebug() << "Sig::aclara: failed to parse command:" << command;
		}
	}

	#ifdef ULTRAVERBOSE
	qDebug() << "Sig::aclara: orig:" << signature;
	qDebug() << "Sig::aclara: res:" << s;
	#endif

	if (s.isEmpty()) {
		qDebug() << "Sig::aclara: signature length not supported:" << signature.size() << ":" << signature;
	}

	return s;
}
#endif

void Sig::save(QSettings * set) {
	qDebug() << "Sig::save";

	set->beginGroup("sig");
	set->setValue("player", html5_player);
	set->setValue("sts", sts);
	set->setValue("function_name", function_name);
	set->setValue("function", decrypt_function);
	set->setValue("code", decrypt_code);
	set->endGroup();
}

void Sig::load(QSettings * set) {
	qDebug() << "Sig::load";

	set->beginGroup("sig");
	html5_player = set->value("player", "").toString();
	sts = set->value("sts", "").toString();
	function_name = set->value("function_name", "").toString();
	decrypt_function = set->value("function", "").toString();
	decrypt_code = set->value("code", "").toString();
	set->endGroup();
}
