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

#include "filesettingshash.h"
#include "mediasettings.h"
#include "mediadata.h"
#include "filehash.h" // hash function
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

FileSettingsHash::FileSettingsHash(QString directory) : FileSettingsBase(directory) 
{
	base_dir = directory + "/file_settings";
}

FileSettingsHash::~FileSettingsHash() {
}


QString FileSettingsHash::configFile(const QString & filename, int type, QString * output_dir) {
	QString res;

	QString hash;
	if (type == TYPE_FILE) {
		hash = FileHash::calculateHash(filename);
	} else {
		QByteArray ba;
		for (int n = filename.count()-1; n >= 0; --n) {
			ba += filename.at(n);
		}
		//qDebug() << "FileSettingsHash::configFile: ba:" << ba;
		hash = ba.toBase64();
	}

	if (!hash.isEmpty()) {
		if (output_dir != 0) (*output_dir) = hash[0];
		res = base_dir +"/"+ hash[0] +"/"+ hash + ".ini";
	}
	return res;
}

bool FileSettingsHash::existSettingsFor(QString filename, int type) {
	qDebug() << "FileSettingsHash::existSettingsFor:" << filename;

	if (type != TYPE_FILE && type != TYPE_STREAM) return false;

	QString config_file = configFile(filename, type);

	qDebug() << "FileSettingsHash::existSettingsFor: config_file:" << config_file;

	return QFile::exists(config_file);
}

void FileSettingsHash::loadSettingsFor(QString filename, int type, MediaSettings & mset, int player) {
	qDebug() << "FileSettings::loadSettingsFor:" << filename << "type:" << type;

	if (type != TYPE_FILE && type != TYPE_STREAM) return;

	QString config_file = configFile(filename, type);

	qDebug() << "FileSettingsHash::loadSettingsFor: config_file:" << config_file;

	mset.reset();

	if ((!config_file.isEmpty()) && (QFile::exists(config_file))) {
		QSettings settings(config_file, QSettings::IniFormat);

		settings.beginGroup("file_settings");
		mset.load(&settings, player);
		settings.endGroup();
	}
}

void FileSettingsHash::saveSettingsFor(QString filename, int type, MediaSettings & mset, int player) {
	qDebug() << "FileSettingsHash::saveSettingsFor:" << filename << "type:" << type;

	if (type != TYPE_FILE && type != TYPE_STREAM) return;

	QString output_dir;
	QString config_file = configFile(filename, type, &output_dir);

	qDebug() << "FileSettingsHash::saveSettingsFor: config_file:" << config_file;
	qDebug() << "FileSettingsHash::saveSettingsFor: output_dir:" << output_dir;

	if (!config_file.isEmpty()) {
		QDir d(base_dir);
		if (!d.exists(output_dir)) {
			if (!d.mkpath(output_dir)) {
				qWarning() << "FileSettingsHash::saveSettingsFor: can't create directory" << QString(base_dir + "/" + output_dir);
				return;
			}
		}

		QSettings settings(config_file, QSettings::IniFormat);

		/* settings.setValue("filename", filename); */

		settings.beginGroup("file_settings");
		mset.save(&settings, player);
		settings.endGroup();
		settings.sync();
	}
}

