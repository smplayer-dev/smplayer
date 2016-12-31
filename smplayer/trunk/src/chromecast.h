/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef CHROMECAST_H
#define CHROMECAST_H

#include <QString>
#include <QStringList>
#include <QProcess>

class Chromecast : public QObject {
	Q_OBJECT

public:
	Chromecast(QObject * parent = 0);
	~Chromecast();

	void openStream(const QString & url, const QString & title);
	void openLocal(const QString & file, const QString & title);

	static Chromecast * instance();
	static void deleteInstance();

	static QStringList localAddresses();
	static QString defaultLocalAddress();

	void setServerPort(int port) { server_port = port; };
	int serverPort() { return server_port; };

protected:
	void startServer(const QString & doc_root);
	void stopServer();

protected slots:
	void readProcessOutput();
	void processFinished(int exit_code, QProcess::ExitStatus exit_status);
	void processError(QProcess::ProcessError error);

protected:
	QProcess * server_process;
	int server_port;

private:
	static Chromecast * instance_obj;
};

#endif

