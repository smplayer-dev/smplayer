CONFIG += debug

HEADERS = videopreview.h
SOURCES = videopreview.cpp main.cpp

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}

