#-------------------------------------------------
#
# Project created by QtCreator 2010-08-24T11:11:30
#
#-------------------------------------------------

TARGET = vtkExample
TEMPLATE = app
CONFIG -= app_bundle

SOURCES += main.cpp\
        mafMainWindow.cpp

HEADERS  += mafMainWindow.h

FORMS    += mafMainWindow.ui

BUILD_DIR = Release

CONFIG(debug, debug|release) {
    unix:LIB_EXT = _debug
    win32:LIB_EXT = _d
	BUILD_DIR = Debug
}

win32 {
    INSTALLBASE = ..\..\..\Install
    DESTDIR = $$INSTALLBASE\bin\\$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE\bin\\$$BUILD_DIR
}
unix {
    INSTALLBASE = ../../../Install
    DESTDIR = $$INSTALLBASE/bin/$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE/bin/$$BUILD_DIR
}

include(../../mafImportVTK.pri)
