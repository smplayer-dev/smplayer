TEMPLATE = app
LANGUAGE = C++

CONFIG  += qt warn_on debug

INCLUDEPATH += ../corelib

HEADERS = test.h
SOURCES = test.cpp

LIBS += -L../corelib -lsmplayercore

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}
