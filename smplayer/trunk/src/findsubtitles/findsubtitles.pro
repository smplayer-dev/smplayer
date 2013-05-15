TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on 
#CONFIG += release
CONFIG += debug

QT += network xml

INCLUDEPATH += ..
DEPENDPATH += ..

INCLUDEPATH += maia
DEPENDPATH += maia

HEADERS += ../filehash.h \
           ../lineedit_with_icon.h \
           ../filechooser.h \
           ../languages.h \
           ../version.h \
           fixsubs.h \
           findsubtitlesconfigdialog.h \
           findsubtitleswindow.h

SOURCES += ../filehash.cpp \
           ../lineedit_with_icon.cpp \
           ../filechooser.cpp \
           ../languages.cpp \
           ../version.cpp \
           fixsubs.cpp \
           findsubtitlesconfigdialog.cpp \
           findsubtitleswindow.cpp \
           main.cpp

HEADERS += maiaObject.h maiaFault.h maiaXmlRpcClient.h osclient.h
SOURCES += maiaObject.cpp maiaFault.cpp maiaXmlRpcClient.cpp osclient.cpp

FORMS += findsubtitleswindow.ui findsubtitlesconfigdialog.ui

DEFINES += NO_SMPLAYER_SUPPORT DOWNLOAD_SUBS

contains( DEFINES, DOWNLOAD_SUBS ) {
	INCLUDEPATH += filedownloader quazip
	DEPENDPATH += filedownloader quazip

	HEADERS += filedownloader.h subchooserdialog.h
	SOURCES += filedownloader.cpp subchooserdialog.cpp

	FORMS += subchooserdialog.ui

	contains( DEFINES, USE_QUAZIP ) {
		HEADERS += crypt.h \
		           ioapi.h \
		           quazip.h \
		           quazipfile.h \
		           quazipfileinfo.h \
		           quazipnewinfo.h \
		           unzip.h \
		           zip.h

		SOURCES += ioapi.c \
		           quazip.cpp \
		           quazipfile.cpp \
		           quazipnewinfo.cpp \
		           unzip.c \
		           zip.c
	}

	LIBS += -lz
	
	win32 {
		INCLUDEPATH += c:\development\zlib-1.2.3
		LIBS += -Lc:\development\zlib-1.2.3
	}
}

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32 {
  CONFIG += console
}

