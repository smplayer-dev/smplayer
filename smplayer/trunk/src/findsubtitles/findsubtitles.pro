TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on release

QT += network xml

INCLUDEPATH += .. filedownloader
DEPENDPATH += .. filedownloader

HEADERS += simplehttp.h \
           osparser.h \
           ../filechooser.h \
           ../languages.h \
           filedownloader/filedownloader.h \
           findsubtitleswindow.h

SOURCES += simplehttp.cpp \
           osparser.cpp \
           ../filechooser.cpp \
           ../languages.cpp \
           filedownloader/filedownloader.cpp \
           findsubtitleswindow.cpp \
           main.cpp

FORMS += ../filechooser.ui findsubtitleswindow.ui

DEFINES += NO_SMPLAYER_SUPPORT USE_FILEDOWNLOADER

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32 {
  CONFIG += console
}

