# -------------------------------------------------
# Project created by QtCreator 2010-09-07T09:46:23
# -------------------------------------------------
QT += script \
    xml
QT -= gui
TARGET = scxmlTest
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    mafStateMachine.cpp \
    mafModel.cpp
QMAKE_CLEAN += ./MakeFile
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


INCLUDEPATH += ../../mafCore
include(../../mafImportQScxml.pri)

win32:system(copy testWorkflow.scxml $$DESTDIR)
unix:system(cp testWorkflow.scxml $$DESTDIR/)

LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT

# Foundation libraries dependencies
HEADERS += mafStateMachine.h \
    mafModel.h
