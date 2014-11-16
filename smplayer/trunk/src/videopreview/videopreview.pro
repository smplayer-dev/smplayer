CONFIG += debug

HEADERS = ../lineedit_with_icon.h ../filechooser.h ../playerid.h videopreviewconfigdialog.h videopreview.h 
SOURCES = ../lineedit_with_icon.cpp ../filechooser.cpp ../playerid.cpp videopreviewconfigdialog.cpp videopreview.cpp main.cpp

FORMS = videopreviewconfigdialog.ui

INCLUDEPATH += ..
DEPENDPATH += ..
DEFINES += NO_SMPLAYER_SUPPORT

isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets gui
	#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040000
}

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

