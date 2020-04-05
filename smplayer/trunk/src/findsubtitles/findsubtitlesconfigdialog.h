/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef FINDSUBTITLESCONFIGDIALOG_H
#define FINDSUBTITLESCONFIGDIALOG_H

#include "ui_findsubtitlesconfigdialog.h"

class FindSubtitlesConfigDialog : public QDialog, public Ui::FindSubtitlesConfigDialog
{
	Q_OBJECT

public:
	FindSubtitlesConfigDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~FindSubtitlesConfigDialog();

#ifdef FS_USE_SERVER_CONFIG
	// Server
	void setServer(QString server);
	QString server();
#endif

	void setSearchMethod(int m);
	int searchMethod();

	void setUsername(QString username);
	QString username();

	void setPassword(QString password);
	QString password();

#ifdef OS_SEARCH_WORKAROUND
	void setRetries(int n);
	int retries();
#endif

#ifdef DOWNLOAD_SUBS
	void setAppendLang(bool b);
	bool appendLang();
#endif

#ifdef FS_USE_PROXY
	// Proxy
	void setUseProxy(bool b);
	bool useProxy();

	void setProxyHostname(QString host);
	QString proxyHostname();

	void setProxyPort(int port);
	int proxyPort();

	void setProxyUsername(QString username);
	QString proxyUsername();

	void setProxyPassword(QString password);
	QString proxyPassword();

	void setProxyType(int type);
	int proxyType();
#endif
};

#endif
