CONFIG += debug

HEADERS = ../lineedit_with_icon.h ../filechooser.h videopreviewconfigdialog.h videopreview.h 
SOURCES = ../lineedit_with_icon.cpp ../filechooser.cpp videopreviewconfigdialog.cpp videopreview.cpp main.cpp

FORMS = videopreviewconfigdialog.ui

INCLUDEPATH += ..
DEPENDPATH += ..
DEFINES += NO_SMPLAYER_SUPPORT

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

