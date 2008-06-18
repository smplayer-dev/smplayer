TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on release

QT += network xml

HEADERS += simplehttp.h osparser.h ../filechooser.h findsubtitlesdialog.h
SOURCES += simplehttp.cpp osparser.cpp ../filechooser.cpp findsubtitlesdialog.cpp \
           main.cpp
FORMS += ../filechooser.ui findsubtitlesdialog.ui

INCLUDEPATH += ..
DEPENDPATH += ..

DEFINES += NO_SMPLAYER_SUPPORT

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32 {
  CONFIG += console
}

