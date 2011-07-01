/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "myserver.h"
#include "version.h"
#include <QHostAddress>
#include <QRegExp>

Connection::Connection(QTcpSocket * s) 
{
	socket = s;

	//connect(s, SIGNAL(disconnected()), this, SLOT(deleteLater()));
	connect(s, SIGNAL(readyRead()), this, SLOT(readData()));

	sendText(QString("SMPlayer %1").arg(smplayerVersion()));
	sendText("Type help for a list of commands");
}

Connection::~Connection() {
	delete socket;
}

void Connection::sendText(QString l) {
	qDebug("Connection::sendText: '%s'", l.toUtf8().data());

    socket->write( l.toUtf8() + "\r\n" );
    socket->flush();
}

void Connection::readData() {
	while (socket->canReadLine()) {
		QString l = QString::fromUtf8( socket->readLine() );
		l.remove( QRegExp("[\r\n]") );
		parseLine( l );
	}
}

void Connection::parseLine(QString str) {
	qDebug("Connection::parseLine: '%s'", str.toUtf8().data());

	QRegExp rx_open("^open (.*)");
	QRegExp rx_open_files("(^open_files|^add_files) (.*)");
	QRegExp rx_function("^(function|f) (.*)");
	QRegExp rx_loadsub("^load_sub (.*)");
	QRegExp rx_playItem("^play item ([0-9]+)");
	QRegExp rx_moveItem("^move item ([0-9]+) ([+-]?[0-9]+)");
	QRegExp rx_removeItem("^remove item (\\*|[0-9]+)");
	QRegExp rx_seek("^seek ([0-9]*\\.*[0-9]*)");
	QRegExp rx_get("^get (.*)");
	QRegExp rx_setVol("^set volume ([0-9]+)");

	if (str.toLower() == "hello") {
		sendText(QString("Hello, this is SMPlayer %1").arg(smplayerVersion()));
	}
	else
	if (str.toLower() == "help") {
		sendText("Available commands:");
		sendText(" help");
		sendText(" quit");
		sendText(" list functions");
		sendText(" function [function_name]");
		sendText(" f [function_name]");
		sendText(" open [file]");
		sendText(" open_files_start \\n open_files [file1] \\n open_files [file2]  \\n ... open_files_end");
		sendText(" add_files_start  \\n  add_files [file1] \\n  add_files [file2]  \\n ...  add_files_end");
		sendText(" remove item [index]");
		sendText(" play item [index]");
		sendText(" move item [index] [shift]");
		sendText(" view playlist");
		sendText(" view status");
		sendText(" view clip info");
		sendText(" seek [position]");
		sendText(" get [action]");
		sendText(" get volume");
		sendText(" set volume [value]");
	}
	else
	if (str.toLower() == "quit") {
		sendText("Goodbye");
		socket->disconnectFromHost();
	}
	else
	if (str.toLower() == "list functions") {
		for (int n=0; n < actions_list.count(); n++) {
			sendText( actions_list[n] );
		}
	}
	else
	if (rx_playItem.indexIn(str) > -1) {
		QString index = rx_playItem.cap(1);
		qDebug("Connection::parseLine: asked to play file #%s.", index.toUtf8().data());
		sendText("OK, command sent to GUI.");
		emit receivedPlayItem(index.toInt());
	}
	else
	if (rx_removeItem.indexIn(str) > -1) {
		QString index = rx_removeItem.cap(1);
		qDebug("Connection::parseLine: asked to remove file %s.", index.toUtf8().data());
		sendText("OK, command sent to GUI.");
		emit receivedRemoveItem(index == "*" ? -1 : index.toInt());
	}
	else
	if (rx_moveItem.indexIn(str) > -1) {
		QString index = rx_moveItem.cap(1);
		QString shift = rx_moveItem.cap(2);
		qDebug("Connection::parseLine: asked to move file %s %s.", index.toUtf8().data(), shift.toUtf8().data());
		sendText("OK, command sent to GUI.");
		emit receivedMoveItem(index.toInt(), shift.toInt());
	}
	else
	if (rx_open.indexIn(str) > -1) {
		QString file = rx_open.cap(1);
		qDebug("Connection::parseLine: asked to open '%s'", file.toUtf8().data());
		sendText("OK, file sent to GUI");
		emit receivedOpen(file);
	} 
	else 
	if (rx_loadsub.indexIn(str) > -1) {
		QString file = rx_loadsub.cap(1);
		qDebug("Connection::parseLine: asked to load subtitle '%s'", file.toUtf8().data());
		sendText("OK, subtitle file sent to GUI");
		emit receivedLoadSubtitle(file);
	} 
	else
	if ( (str.toLower() == "open_files_start") ||
         (str.toLower() == "add_files_start") ) 
	{
		files_to_open.clear();
		sendText("OK, send first file");
	}
	else
	if ( (str.toLower() == "open_files_end") ||
         (str.toLower() == "add_files_end") ) 
	{
		qDebug("Connection::parseLine: files_to_open:");
		for (int n=0; n < files_to_open.count(); n++) 
			qDebug("Connection::parseLine: %d: '%s'", n, files_to_open[n].toUtf8().data());
		sendText("OK, sending files to GUI");

		if (str.toLower() == "open_files_end")
			emit receivedOpenFiles(files_to_open);
		else
			emit receivedAddFiles(files_to_open);
	}
	else
	if (rx_open_files.indexIn(str) > -1) {
		QString file = rx_open_files.cap(2);
		qDebug("Connection::parseLine: file: '%s'", file.toUtf8().data());
		files_to_open.append(file);
		sendText("OK, file received");
	}
	else
	if (rx_function.indexIn(str) > -1) {
		QString function = rx_function.cap(2).toLower();
		qDebug("Connection::parseLine: asked to process function '%s'", function.toUtf8().data());
		sendText("OK, function sent to GUI");
		emit receivedFunction(function);
	}
	else
	if(str.toLower() == "get volume"){
		int output = 0;
		emit receivedGetVolume(&output);
		sendText(QString::number(output));

	}
	else
	if(rx_setVol.indexIn(str) > -1){
		int value = rx_setVol.cap(1).toInt();
		emit receivedSetVolume(value);

	}
	else
	if(str.toLower() == "view playlist"){
		QString output = "";
		emit receivedViewPlaylist(&output);
		sendText(output);

	}
	else
	if(str.toLower() == "view status"){
		QString output = "";
		emit receivedViewStatus(&output);
		sendText(output);

	}
	else
	if(str.toLower() == "view clip info"){
		QString output = "";
		emit receivedViewClipInfo(&output);
		sendText(output);

	}
	else
	if(rx_seek.indexIn(str) > -1){
		qDebug("Connection::parseLine: asked to seek to %s", rx_seek.cap(1).toUtf8().data());
		emit receivedSeek(rx_seek.cap(1).toDouble());
	}
	else
	if(rx_get.indexIn(str) > -1){
		qDebug("Connection::parseLine: asked to get state of action '%s'", rx_get.cap(1).toUtf8().data());
		QString value = "";
		emit receivedGetChecked(rx_get.cap(1), &value);
		sendText(value);
	}
	else {
		sendText("Unknown command");
	}
}


MyServer::MyServer( QObject * parent ) : QTcpServer(parent)
{
	connect(this, SIGNAL(newConnection()), this, SLOT(newConnection_slot()));
}

bool MyServer::listen( quint16 port ) {
	return QTcpServer::listen(QHostAddress::LocalHost, port);
}

void MyServer::newConnection_slot() {
	Connection * c = new Connection( nextPendingConnection() );
	c->setActionsList( actionsList() );

	connect(c, SIGNAL(receivedPlayItem(int)),
			this, SIGNAL(receivedPlayItem(int)));
	connect(c, SIGNAL(receivedRemoveItem(int)),
			this, SIGNAL(receivedRemoveItem(int)));
	connect(c, SIGNAL(receivedOpen(QString)),
			this, SIGNAL(receivedOpen(QString)));
	connect(c, SIGNAL(receivedOpenFiles(QStringList)),
            this, SIGNAL(receivedOpenFiles(QStringList)));
	connect(c, SIGNAL(receivedAddFiles(QStringList)),
            this, SIGNAL(receivedAddFiles(QStringList)));
	connect(c, SIGNAL(receivedFunction(QString)),
            this, SIGNAL(receivedFunction(QString)));
	connect(c, SIGNAL(receivedLoadSubtitle(QString)),
            this, SIGNAL(receivedLoadSubtitle(QString)));
	connect(c, SIGNAL(receivedViewPlaylist(QString*)),
			this, SIGNAL(receivedViewPlaylist(QString*)));
	connect(c, SIGNAL(receivedViewStatus(QString*)),
			this, SIGNAL(receivedViewStatus(QString*)));
	connect(c, SIGNAL(receivedViewClipInfo(QString*)),
			this, SIGNAL(receivedViewClipInfo(QString*)));
	connect(c, SIGNAL(receivedSeek(double)),
			this, SIGNAL(receivedSeek(double)));
	connect(c, SIGNAL(receivedGetChecked(QString,QString*)),
			this, SIGNAL(receivedGetChecked(QString,QString*)));
	connect(c, SIGNAL(receivedMoveItem(int,int)),
			this, SIGNAL(receivedMoveItem(int,int)));
	connect(c, SIGNAL(receivedGetVolume(int*)),
			this, SIGNAL(receivedGetVolume(int*)));
	connect(c, SIGNAL(receivedSetVolume(int)),
			this, SIGNAL(receivedSetVolume(int)));
}

#include "moc_myserver.cpp"
