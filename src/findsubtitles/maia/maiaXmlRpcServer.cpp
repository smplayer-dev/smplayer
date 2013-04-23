/*
 * libMaia - maiaXmlRpcServer.cpp
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

#include "maiaXmlRpcServer.h"
#include "maiaFault.h"

MaiaXmlRpcServer::MaiaXmlRpcServer(const QHostAddress &address, quint16 port, QObject* parent) : QObject(parent) {
	allowedAddresses = NULL;
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	server.listen(address, port);
}

MaiaXmlRpcServer::MaiaXmlRpcServer(quint16 port, QObject* parent) : QObject(parent) {
	allowedAddresses = NULL;
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	server.listen(QHostAddress::Any, port);
}

MaiaXmlRpcServer::MaiaXmlRpcServer(const QHostAddress &address, quint16 port, QList<QHostAddress> *allowedAddresses, QObject *parent) : QObject(parent) {
	this->allowedAddresses = allowedAddresses;
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	server.listen(address, port);
}

void MaiaXmlRpcServer::addMethod(QString method,
	 QObject* responseObject, const char* responseSlot) {
	objectMap[method] = responseObject;
	slotMap[method] = responseSlot;
}

void MaiaXmlRpcServer::removeMethod(QString method) {
	objectMap.remove(method);
	slotMap.remove(method);
}

void MaiaXmlRpcServer::getMethod(QString method, QObject **responseObject, const char **responseSlot) {
	if(!objectMap.contains(method)) {
		*responseObject = NULL;
		*responseSlot = NULL;
		return;
	}
	*responseObject = objectMap[method];
	*responseSlot = slotMap[method];
}

void MaiaXmlRpcServer::newConnection() {
	QTcpSocket *connection = server.nextPendingConnection();
	if (!this->allowedAddresses || this->allowedAddresses->isEmpty() || this->allowedAddresses->contains(connection->peerAddress())) {
		MaiaXmlRpcServerConnection *client = new MaiaXmlRpcServerConnection(connection, this);
		connect(client, SIGNAL(getMethod(QString, QObject **, const char**)),
			this, SLOT(getMethod(QString, QObject **, const char**)));
	} else {
		qWarning() << "Rejected connection attempt from" << connection->peerAddress().toString();
		connection->disconnectFromHost();
	}
}

QHostAddress MaiaXmlRpcServer::getServerAddress() {
	return server.serverAddress();
}

