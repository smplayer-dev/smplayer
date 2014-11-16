
#include "cleanconfig.h"
#include <QFile>
#include <QDir>

#define DO_REMOVE

void CleanConfig::clean(const QString & config_path) {
	qDebug("CleanConfig::clean");

	QStringList files_to_delete;

	QString s = config_path + "/smplayer.ini";
	if (QFile::exists(s)) files_to_delete << s;

	s = config_path + "/styles.ass";
	if (QFile::exists(s)) files_to_delete << s;

	s = config_path + "/smplayer_files.ini";
	if (QFile::exists(s)) files_to_delete << s;

	s = config_path + "/ytcode.script";
	if (QFile::exists(s)) files_to_delete << s;

	s = config_path + "/yt.js";
	if (QFile::exists(s)) files_to_delete << s;
	
	s = config_path + "/player_info.ini";
	if (QFile::exists(s)) files_to_delete << s;

	s = config_path + "/file_settings";
	if (QFile::exists(s)) files_to_delete << listDir(s);

	printf("Deleting files:\n");
	for (int n = 0; n < files_to_delete.count(); n++) {
		printf("Delete: %s\n", files_to_delete[n].toUtf8().constData());
		#ifdef DO_REMOVE
		QFile::remove(files_to_delete[n]);
		#endif
	}
}

QStringList CleanConfig::listDir(const QString &path) {
	QDir dir(path);
	QStringList file_list;

	foreach(QString file, dir.entryList(QDir::Files)) {
		file_list << QFileInfo(dir, file).absoluteFilePath();
	}

	foreach(QString sub_dir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		file_list << listDir(path +"/"+ sub_dir);
	}

	return file_list;
}

