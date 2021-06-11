#include "../src/extensions.h"
#include "../src/version.h"
#include <QString>
#include <stdio.h>

void print_stringlist(const QStringList & l, int spaces) {
	QString sp;
	sp.fill(QChar(' '), spaces);

	foreach(QString v, l) {
		QString s = sp + v;
		printf("%s\n", s.toLatin1().constData());
	}
}

void print_string(const QString & s, int spaces = 4) {
	print_stringlist(QStringList() << s, spaces);
}

void print_key(const QString & key, const QString & value, int spaces = 4) {
	print_stringlist(QStringList() << QString("<key>%1</key>").arg(key) << QString("<string>%1</string>").arg(value), spaces);
}

void print_list(const QString & name, const QStringList & list, const QString & key = "CFBundleDocumentTypes", int spaces = 4) {
	print_stringlist(QStringList()
		<< QString("<key>%1</key>").arg(key)
		<< "<array>"
		<< "<dict>", spaces);

	print_stringlist(QStringList()
		<< "<key>CFBundleTypeIconFile</key>"
		<< "<string>smplayer.icns</string>"
		<< "<key>CFBundleTypeName</key>"
		<< QString("<string>%1</string>").arg(name)
		<< "<key>CFBundleTypeRole</key>"
		<< "<string>Viewer</string>"
		<< "<key>LSTypeIsPackage</key>"
		<< "<false/>"
		<< "<key>NSPersistentStoreTypeKey</key>"
		<< "<string>XML</string>"
		<< "<key>CFBundleTypeExtensions</key>"
		<< "<array>", spaces + 4);

	foreach (QString v, list) {
		print_string( QString("<string>%1</string>").arg(v), spaces + 8);
	}

	print_string("</array>", spaces + 4);
	print_string("</dict>", spaces);
	print_string("</array>", spaces);
}

int main( int argc, char ** argv )
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
           "<plist version=\"1.0\">\n"
           "<dict>\n");
	print_key("CFBundleDevelopmentRegion", "English");

	Extensions e;
	print_list("Video file", e.video());
	print_list("Audio file", e.audio());

	print_key("CFBundleExecutable", "smplayer");
	print_key("CFBundleHomepage", "https://www.smplayer.info");
	print_key("CFBundleIconFile", "smplayer.icns");
	print_key("CFBundleIdentifier", "info.smplayer.SMPlayer");
	print_key("CFBundleInfoDictionaryVersion", "6.0");
	print_key("CFBundleName", "SMPlayer");
	print_key("CFBundlePackageType", "APPL");
	print_key("CFBundleShortVersionString", Version::stable());

	printf("</dict>\n</plist>\n");

	return 0;
}

