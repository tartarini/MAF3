# -------------------------------------------------
# Project created by QtCreator 2010-04-29T11:10:57
# -------------------------------------------------
QT -= gui
TARGET = qaCheck
CONFIG += console
mac:CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    mafQAChecker.cpp

# include dirs related to the framework
BUILD_DIR = Release

CONFIG(debug, debug|release) {
    unix:LIB_EXT = _debug
    win32:LIB_EXT = _d
	BUILD_DIR = Debug
}

win32 {
    INSTALLBASE = ..\..\..\Install
    INCLUDEPATH += ..\..\mafQA
    DESTDIR = $$INSTALLBASE\bin\\$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE\bin\\$$BUILD_DIR
}
unix {
    INSTALLBASE = $$PWD/../../../Install
    INCLUDEPATH += $$PWD/../../mafQA
    DESTDIR = $$INSTALLBASE/bin/$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE/bin/$$BUILD_DIR
}

INCLUDEPATH += ../../mafQA
LIBS += -L$$DESTDIR \
    -lmafQA$$LIB_EXT

INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
HEADERS += mafQAChecker.h
