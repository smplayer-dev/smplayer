/*
 * libMaia - maiaXmlRpcServerConnection.h
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
 *                and Karl Glatz
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

#ifndef MAIAXMLRPCSERVERCONNECTION_H
#define MAIAXMLRPCSERVERCONNECTION_H

#include <QtCore>
#include <QtXml>
#include <QtNetwork>
#include "maiaFault.h"

#if QT_VERSION >= 0x050000
class QHttpRequestHeader
{
public:
    explicit QHttpRequestHeader(QString headerString);
    virtual ~QHttpRequestHeader() {}

    bool isValid();
    QString method();
    uint contentLength() const;

private:
    QString mHeaderString;
    QString mMethod;
    QMap<QString, QString> mHeaders;
};

class QHttpResponseHeader
{
public:
    explicit QHttpResponseHeader(int code, QString text);
    virtual ~QHttpResponseHeader() {}
    void setValue(const QString &key, const QString &value);
    virtual QString toString() const;

private:
    int mCode;
    QString mText;
    QMap<QString, QString> mHeaders;
};
#endif


class MaiaXmlRpcServerConnection : public QObject {
	Q_OBJECT
	
	public:
		MaiaXmlRpcServerConnection(QTcpSocket *connection, QObject *parent = 0);
		~MaiaXmlRpcServerConnection();
		
	signals:
		void getMethod(QString method, QObject **responseObject, const char **responseSlot);

	private slots:
		void readFromSocket();
	
	private:
		void sendResponse(QString content);
		void parseCall(QString call);
		bool invokeMethodWithVariants(QObject *obj,
		        const QByteArray &method, const QVariantList &args,
		        QVariant *ret, Qt::ConnectionType type = Qt::AutoConnection);
		static QByteArray getReturnType(const QMetaObject *obj,
			        const QByteArray &method, const QList<QByteArray> argTypes);
		

		QTcpSocket *clientConnection;
		QString headerString;
		QHttpRequestHeader *header;
		
};

#endif
