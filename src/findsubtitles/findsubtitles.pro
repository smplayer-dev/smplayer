TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on 
#CONFIG += release
CONFIG += debug

QT += network xml

INCLUDEPATH += .. qrestapi

isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets gui
	QT += qml
	#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040000
} else {
	QT += script
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

HEADERS += qrestapi/qRestAPI_Export.h qrestapi/qRestAPI.h qrestapi/qRestAPI_p.h qrestapi/qRestResult.h qrestapi/qGirderAPI.h osclient.h
SOURCES += qrestapi/qRestAPI.cpp qrestapi/qRestResult.cpp qrestapi/qGirderAPI.cpp osclient.cpp

FORMS += findsubtitleswindow.ui findsubtitlesconfigdialog.ui

DEFINES += NO_SMPLAYER_SUPPORT
DEFINES += DOWNLOAD_SUBS
#DEFINES += FS_USE_PROXY
#DEFINES += OS_SEARCH_WORKAROUND
#DEFINES += FS_USE_SERVER_CONFIG

contains( DEFINES, DOWNLOAD_SUBS ) {
	INCLUDEPATH += filedownloader

	HEADERS += filedownloader/filedownloader.h subchooserdialog.h
	SOURCES += filedownloader/filedownloader.cpp subchooserdialog.cpp

	FORMS += subchooserdialog.ui

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

