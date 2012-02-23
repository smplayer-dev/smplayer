/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _FINDSUBTITLESCONFIGDIALOG_H_
#define _FINDSUBTITLESCONFIGDIALOG_H_

#include "ui_findsubtitlesconfigdialog.h"

class FindSubtitlesConfigDialog : public QDialog, public Ui::FindSubtitlesConfigDialog
{
	Q_OBJECT

public:
	FindSubtitlesConfigDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~FindSubtitlesConfigDialog();

	// Server
	void setServer(QString server);
	QString server();

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
};

#endif
