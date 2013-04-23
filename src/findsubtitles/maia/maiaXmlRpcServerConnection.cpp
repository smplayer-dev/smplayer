/*
 * libMaia - maiaXmlRpcServerConnection.cpp
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

#include "maiaXmlRpcServerConnection.h"
#include "maiaXmlRpcServer.h"

MaiaXmlRpcServerConnection::MaiaXmlRpcServerConnection(QTcpSocket *connection, QObject* parent) : QObject(parent) {
	header = NULL;
	clientConnection = connection;
	connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

MaiaXmlRpcServerConnection::~MaiaXmlRpcServerConnection() {
	clientConnection->deleteLater();
	delete header;
}

void MaiaXmlRpcServerConnection::readFromSocket() {
	QString lastLine;

	while(clientConnection->canReadLine() && !header) {
		lastLine = clientConnection->readLine();
		headerString += lastLine;
		if(lastLine == "\r\n") { /* http header end */
			header = new QHttpRequestHeader(headerString);
			if(!header->isValid()) {
				/* return http error */
				qDebug() << "Invalid Header";
				return;
			} else if(header->method() != "POST") {
				/* return http error */
				qDebug() << "No Post!";
				return;
			} else if(!header->contentLength()) {
				/* return fault */
				qDebug() << "No Content Length";
				return;
			}
		}
	}
	
	if(header) {
        if(header->contentLength() <= clientConnection->bytesAvailable()) {
			/* all data complete */
			parseCall(clientConnection->readAll());
		}
    }
}

void MaiaXmlRpcServerConnection::sendResponse(QString content) {
	QHttpResponseHeader header(200, "Ok");
	QByteArray block;
	header.setValue("Server", "MaiaXmlRpc/0.1");
	header.setValue("Content-Type", "text/xml");
	header.setValue("Connection","close");
	block.append(header.toString().toUtf8());
	block.append(content.toUtf8());
	clientConnection->write(block);
	clientConnection->disconnectFromHost();
}

void MaiaXmlRpcServerConnection::parseCall(QString call) {
	QDomDocument doc;
	QList<QVariant> args;
	QVariant ret;
	QString response;
	QObject *responseObject;
	const char *responseSlot;
	
	if(!doc.setContent(call)) { /* recieved invalid xml */
		MaiaFault fault(-32700, "parse error: not well formed");
		sendResponse(fault.toString());
		return;
	}
	
	QDomElement methodNameElement = doc.documentElement().firstChildElement("methodName");
	QDomElement params = doc.documentElement().firstChildElement("params");
	if(methodNameElement.isNull()) { /* invalid call */
		MaiaFault fault(-32600, "server error: invalid xml-rpc. not conforming to spec");
		sendResponse(fault.toString());
		return;
	}
	
	QString methodName = methodNameElement.text();
	
	emit getMethod(methodName, &responseObject, &responseSlot);
	if(!responseObject) { /* unknown method */
		MaiaFault fault(-32601, "server error: requested method not found");
		sendResponse(fault.toString());
		return;
	}
	
	QDomNode paramNode = params.firstChild();
	while(!paramNode.isNull()) {
		args << MaiaObject::fromXml( paramNode.firstChild().toElement());
		paramNode = paramNode.nextSibling();
	}
	
	
	if(!invokeMethodWithVariants(responseObject, responseSlot, args, &ret)) { /* error invoking... */
		MaiaFault fault(-32602, "server error: invalid method parameters");
		sendResponse(fault.toString());
		return;
	}
	
	
	if(ret.canConvert<MaiaFault>()) {
		response = ret.value<MaiaFault>().toString();
	} else {
		response = MaiaObject::prepareResponse(ret);
	}
	
	sendResponse(response);
}


/*	taken from http://delta.affinix.com/2006/08/14/invokemethodwithvariants/
	thanks to Justin Karneges once again :) */
bool MaiaXmlRpcServerConnection::invokeMethodWithVariants(QObject *obj,
			const QByteArray &method, const QVariantList &args,
			QVariant *ret, Qt::ConnectionType type) {

	// QMetaObject::invokeMethod() has a 10 argument maximum
	if(args.count() > 10)
		return false;

	QList<QByteArray> argTypes;
	for(int n = 0; n < args.count(); ++n)
		argTypes += args[n].typeName();

	// get return type
	int metatype = 0;
	QByteArray retTypeName = getReturnType(obj->metaObject(), method, argTypes);
	if(!retTypeName.isEmpty()  && retTypeName != "QVariant") {
		metatype = QMetaType::type(retTypeName.data());
		if(metatype == 0) // lookup failed
			return false;
	}

	QGenericArgument arg[10];
	for(int n = 0; n < args.count(); ++n)
		arg[n] = QGenericArgument(args[n].typeName(), args[n].constData());

	QGenericReturnArgument retarg;
	QVariant retval;
	if(metatype != 0) {
		retval = QVariant(metatype, (const void *)0);
		retarg = QGenericReturnArgument(retval.typeName(), retval.data());
	} else { /* QVariant */
		retarg = QGenericReturnArgument("QVariant", &retval);
	}

	if(retTypeName.isEmpty()) { /* void */
		if(!QMetaObject::invokeMethod(obj, method.data(), type,
						arg[0], arg[1], arg[2], arg[3], arg[4],
						arg[5], arg[6], arg[7], arg[8], arg[9]))
			return false;
	} else {
		if(!QMetaObject::invokeMethod(obj, method.data(), type, retarg,
						arg[0], arg[1], arg[2], arg[3], arg[4],
						arg[5], arg[6], arg[7], arg[8], arg[9]))
			return false;
	}

	if(retval.isValid() && ret)
		*ret = retval;
	return true;
}

QByteArray MaiaXmlRpcServerConnection::getReturnType(const QMetaObject *obj,
			const QByteArray &method, const QList<QByteArray> argTypes) {
	for(int n = 0; n < obj->methodCount(); ++n) {
		QMetaMethod m = obj->method(n);
#if QT_VERSION >= 0x050000
		QByteArray sig = m.methodSignature();
#else
		QByteArray sig = m.signature();
#endif
		int offset = sig.indexOf('(');
		if(offset == -1)
			continue;
		QByteArray name = sig.mid(0, offset);
		if(name != method)
			continue;
		if(m.parameterTypes() != argTypes)
			continue;

		return m.typeName();
	}
	return QByteArray();
}

/*
  simple Qt4 class emulater
*/

#if QT_VERSION >= 0x050000
QHttpRequestHeader::QHttpRequestHeader(QString headerString)
{
    this->mHeaderString = headerString;

    QStringList hdrs = headerString.split("\r\n");
    QStringList hdrkv;
    for (int i = 0; i < hdrs.size(); i++) {
        if (hdrs.at(i).trimmed().isEmpty()) break;
        if (i == 0) {
            hdrkv = hdrs.at(i).split(" ");
            this->mMethod = hdrkv.at(0);
        } else {
            hdrkv = hdrs.at(i).split(":");
            this->mHeaders[hdrkv.at(0)] = hdrkv.at(1).trimmed();
        }
    }
}

bool QHttpRequestHeader::isValid()
{
    if (this->mHeaderString.isEmpty()) return false;
    if (this->mMethod != "GET" && this->mMethod != "POST") return false;
    if (this->mHeaders.size() < 2) return false;
    return true;
}

QString QHttpRequestHeader::method()
{
    return this->mMethod;
}

uint QHttpRequestHeader::contentLength() const
{
    uint clen = 0;

    clen = this->mHeaders.value("Content-Length").toUInt();

    return clen;
}

QHttpResponseHeader::QHttpResponseHeader(int code, QString text)
{
    this->mCode = code;
    this->mText = text;
}

void QHttpResponseHeader::setValue(const QString &key, const QString &value)
{
    this->mHeaders[key] = value;
}

QString QHttpResponseHeader::toString() const
{
    QMapIterator<QString, QString> it(this->mHeaders);
    QString hdrstr;

    hdrstr += QString("HTTP/1.1 %1 %2\r\n").arg(this->mCode).arg(this->mText);
    while (it.hasNext()) {
        it.next();
        hdrstr += it.key() + ": " + it.value() + "\r\n";
    }
    hdrstr += "\r\n";

    return hdrstr;
}

#endif
