TEMPLATE = app
LANGUAGE = C++

CONFIG  += qt warn_on release

INCLUDEPATH += ../corelib

HEADERS = timeslider.h test.h
SOURCES = timeslider.cpp test.cpp

LIBS += -L../corelib -lsmplayercore

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}
