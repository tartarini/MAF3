# -------------------------------------------------
# Project created by QtCreator 2010-09-21T13:47:18
# -------------------------------------------------
TARGET = SimpleApp
TEMPLATE = app
CONFIG -= app_bundle
SOURCES += main.cpp \
    mafMainWindow.cpp \
    mafMementoApplication.cpp
HEADERS += mafMainWindow.h \
    mafMementoApplication.h \
    mafGUIRegistration.h
FORMS += mafMainWindow.ui
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
INCLUDEPATH += ../../mafEventBus
INCLUDEPATH += ../../mafApplicationLogic
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
LIBS += -lmafEventBus$$LIB_EXT
LIBS += -lmafApplicationLogic$$LIB_EXT
RESOURCES += mafAppImages.qrc
QMAKE_CLEAN += ./MakeFile
include(../../mafImportVTK.pri)
