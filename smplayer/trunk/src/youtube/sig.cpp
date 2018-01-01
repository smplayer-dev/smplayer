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
#include <QtScript>
#include <QSettings>
#include <QDebug>

//#define ULTRAVERBOSE

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
	QRegExp rx_sig("([\"\'])signature\\1\\s*,\\s*([a-zA-Z0-9$]+)\\(");
	if (rx_sig.indexIn(text) != -1) {
		//sig_name = rx_sig.cap(1);
		sig_name = rx_sig.cap(2);
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

QString Sig::aclara(const QString & text) {
	int dot = text.indexOf('.');
	qDebug("Sig::aclara: length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

	QString res;

	if (!decrypt_function.isEmpty()) {
		QString fname = function_name;
		QScriptEngine engine;
		engine.evaluate(decrypt_function);

		QScriptValueList args;
		args << text;
		QScriptValue aclarar = engine.globalObject().property(fname);
		res = aclarar.call(QScriptValue(), args).toString();
	}

	qDebug() << "Sig::aclara: res:" << res;

	if (res.isEmpty()) {
		qDebug() << "Sig::aclara: signature length not supported:" << text.size() << ":" << text;
	}

	return res;
}

void Sig::save(QSettings * set) {
	qDebug() << "Sig::save";

	set->beginGroup("sig");
	set->setValue("player", html5_player);
	set->setValue("sts", sts);
	set->setValue("function_name", function_name);
	set->setValue("function", decrypt_function);
	set->endGroup();
}

void Sig::load(QSettings * set) {
	qDebug() << "Sig::load";

	set->beginGroup("sig");
	html5_player = set->value("player", "").toString();
	sts = set->value("sts", "").toString();
	function_name = set->value("function_name", "").toString();
	decrypt_function = set->value("function", "").toString();
	set->endGroup();
}
