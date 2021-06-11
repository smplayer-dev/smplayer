HEADERS += ../src/extensions.h ../src/version.h
SOURCES += ../src/extensions.cpp ../src/version.cpp main.cpp

mac {
  CONFIG -= app_bundle
}
