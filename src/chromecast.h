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

#ifndef CHROMECAST_H
#define CHROMECAST_H

#include <QString>
#include <QStringList>
#include <QProcess>

#define SERVE_FILE_DIR_ONLY
#define CONVERT_TO_VTT

class QSettings;

class Chromecast : public QObject {
	Q_OBJECT

public:
	Chromecast(QObject * parent = 0);
	~Chromecast();

	void openStream(const QString & url, const QString & title);
	void openLocal(const QString & file, const QString & title, const QString & subtitle = QString::null);

	static Chromecast * instance();
	static void deleteInstance();

	static QStringList localAddresses();
	static QString findLocalAddress();

	// Server settings
	void setServerPort(int port) {
		if (port != server_port) server_needs_restart = true;
		server_port = port;
	}
	int serverPort() { return server_port; };

	void setLocalAddress(const QString & address) {
		if (address != local_address) server_needs_restart = true;
		local_address = address;
	};
	QString localAddress() { return local_address; };

	void setDirectoryListing(bool enabled) {
		if (enabled != directory_listing) server_needs_restart = true;
		directory_listing = enabled;
	};
	bool directoryListing() { return directory_listing; };

	void setSettings(QSettings * set) { settings = set; loadSettings(); };

#ifdef CONVERT_TO_VTT
	void setAutoConvertToVTT(bool b) { autoconvert_to_vtt = b; };
	bool autoConvertToVTT() { return autoconvert_to_vtt; };

	void setSubtitleEncoding(const QString& encoding) { sub_encoding = encoding; };
	QString subtitleEncoding() { return sub_encoding; };

	void setSubtitlePosition(int position) { sub_position = position; };
	int subtitlePosition() { return sub_position; };

	void setSubtitleFilter(const QString & filter) { sub_filter = filter; };
	QString subtitleFilter() { return sub_filter; };

	void setOverwriteVTT(bool b) { overwrite_vtt = b; };
	bool overwriteVTT() { return overwrite_vtt; };

	void enableSubtitleFilter(bool b) { use_sub_filter = b; };
	bool isSubtitleFilterEnabled() { return use_sub_filter; };
#endif

protected:
	void startServer(QString doc_root);
	void stopServer();

	void loadSettings();
	void saveSettings();

#ifndef SERVE_FILE_DIR_ONLY
	QString filepathWithoutRoot(const QString & filepath);
#endif

	QString checkForVTT(const QString & video_path, const QString & subtitle_file);

protected slots:
	void readProcessOutput();
	void processFinished(int exit_code, QProcess::ExitStatus exit_status);
	void processError(QProcess::ProcessError error);

protected:
	QProcess * server_process;
	QSettings * settings;

	int server_port;
	QString local_address;
	bool directory_listing;

	bool server_needs_restart;

#ifdef CONVERT_TO_VTT
	bool autoconvert_to_vtt;
	QString sub_encoding;
	int sub_position;
	QString sub_filter;
	bool overwrite_vtt;
	bool use_sub_filter;
#endif

private:
	static Chromecast * instance_obj;
};

#endif
