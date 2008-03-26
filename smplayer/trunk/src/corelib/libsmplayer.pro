TEMPLATE = lib
LANGUAGE = c++
CONFIG += qt warn_on release

DEFINES += LIBGLOBAL

HEADERS += config.h \
        global.h \
        helper.h \
        subtracks.h \
        trackdata.h \
        tracks.h \
        mediadata.h \
        mediasettings.h \
        preferences.h \
        myprocess.h \
        mplayerversion.h \
        mplayerprocess.h


SOURCES += global.cpp \
        helper.cpp \
        subtracks.cpp \
        trackdata.cpp \
        tracks.cpp \
        mediadata.cpp \
        mediasettings.cpp \
        preferences.cpp \
        myprocess.cpp \
        mplayerversion.cpp \
        mplayerprocess.cpp


unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

