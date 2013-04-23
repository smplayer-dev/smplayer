/*
 * libMaia - maiaObject.h
 * Copyright (c) 2003 Frerich Raabe <raabe@kde.org> and
 *                    Ian Reinhart Geiser <geiseri@kde.org>
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MAIAOBJECT_H
#define MAIAOBJECT_H

#include <QtCore>
#include <QtXml>
#include <QNetworkReply>

class MaiaObject : public QObject {
	Q_OBJECT
	
	public:
		MaiaObject(QObject* parent = 0);
		static QDomElement toXml(QVariant arg);
		static QVariant fromXml(const QDomElement &elem);
		QString prepareCall(QString method, QList<QVariant> args);
		static QString prepareResponse(QVariant arg);
		
	public slots:
		void parseResponse(QString response, QNetworkReply* reply);
	
	signals:
		void aresponse(QVariant &, QNetworkReply* reply);
		void call(const QString, const QList<QVariant>);
		void fault(int, const QString &, QNetworkReply* reply);
		
};

#endif
