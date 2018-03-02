TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on 
#CONFIG += release
CONFIG += debug

QT += network xml

INCLUDEPATH += .. maia

isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets gui
	#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040000
}

HEADERS += ../filehash.h \
           ../lineedit_with_icon.h \
           ../filechooser.h \
           ../languages.h \
           ../version.h \
           ../myscroller.h \
           fixsubs.h \
           findsubtitlesconfigdialog.h \
           findsubtitleswindow.h

SOURCES += ../filehash.cpp \
           ../lineedit_with_icon.cpp \
           ../filechooser.cpp \
           ../languages.cpp \
           ../version.cpp \
           ../myscroller.cpp \
           fixsubs.cpp \
           findsubtitlesconfigdialog.cpp \
           findsubtitleswindow.cpp \
           main.cpp

HEADERS += maia/maiaObject.h maia/maiaFault.h maia/maiaXmlRpcClient.h osclient.h
SOURCES += maia/maiaObject.cpp maia/maiaFault.cpp maia/maiaXmlRpcClient.cpp osclient.cpp

FORMS += findsubtitleswindow.ui findsubtitlesconfigdialog.ui

DEFINES += NO_SMPLAYER_SUPPORT
DEFINES += DOWNLOAD_SUBS
#DEFINES += USE_QUAZIP
#DEFINES += FS_USE_PROXY
#DEFINES += OS_SEARCH_WORKAROUND
#DEFINES += FS_USE_SERVER_CONFIG

contains( DEFINES, DOWNLOAD_SUBS ) {
	INCLUDEPATH += filedownloader

	HEADERS += filedownloader/filedownloader.h subchooserdialog.h
	SOURCES += filedownloader/filedownloader.cpp subchooserdialog.cpp

	FORMS += subchooserdialog.ui

	contains( DEFINES, USE_QUAZIP ) {
		INCLUDEPATH += quazip
		DEPENDPATH += quazip

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
		INCLUDEPATH += ..\\..\\zlib
		LIBS += -L..\\..\\zlib
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

