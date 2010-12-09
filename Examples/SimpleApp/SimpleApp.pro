# -------------------------------------------------
# Project created by QtCreator 2010-09-21T13:47:18
# -------------------------------------------------
QT += webkit \
    network
TARGET = SimpleApp
TEMPLATE = app
CONFIG += uitools

# Removing the line below, uncomment the post link instruction at the end
CONFIG -= app_bundle
SOURCES += main.cpp \
    mafMainWindow.cpp \
    googlechat.cpp
HEADERS += mafMainWindow.h \
    googlechat.h
FORMS += mafMainWindow.ui \
    form.ui
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
INCLUDEPATH += ../../mafCore \
    ../../mafEventBus \
    ../../mafApplicationLogic \
    ../../mafGUI
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT \
    -lmafEventBus$$LIB_EXT \
    -lmafApplicationLogic$$LIB_EXT \
    -lmafGUI$$LIB_EXT
RESOURCES += mafAppImages.qrc
QMAKE_CLEAN += ./MakeFile
include(../../mafImportVTK.pri)
