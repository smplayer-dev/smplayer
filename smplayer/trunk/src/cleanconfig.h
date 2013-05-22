
#ifndef CLEANCONFIG_H
#define CLEANCONFIG_H

#include <QString>
#include <QStringList>

class CleanConfig {
public:
	static void clean(const QString &config_path);

private:
	static QStringList listDir(const QString &path);
};

#endif
