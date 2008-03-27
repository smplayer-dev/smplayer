TEMPLATE = lib
LANGUAGE = c++
CONFIG += qt warn_on debug staticlib

DEFINES += MINILIB

HEADERS += config.h \
        constants.h \
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
        mplayerprocess.h \
        desktopinfo.h \
        mplayerwindow.h \
        core.h \
        smplayercorelib.h


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
        mplayerprocess.cpp \
        desktopinfo.cpp \
        mplayerwindow.cpp \
        core.cpp \
        smplayercorelib.cpp


unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

win32 {
	HEADERS += screensaver.h
	SOURCES += screensaver.cpp
}
