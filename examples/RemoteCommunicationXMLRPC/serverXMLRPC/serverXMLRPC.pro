# -------------------------------------------------
# Project created by QtCreator 2010-04-22T12:56:11
# -------------------------------------------------
QT -= gui
TARGET = serverXMLRPC
CONFIG += console
mac: CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    mafServerXMLRPC.cpp \
    mafAlgorithm.cpp
HEADERS += mafServerXMLRPC.h \
    mafAlgorithm.h

# include dirs related to the framework
BUILD_DIR = Release

CONFIG(debug, debug|release) {
    unix:LIB_EXT = _debug
    win32:LIB_EXT = _d
	BUILD_DIR = Debug
}

win32 {
    INSTALLBASE = ..\..\..\..\Install
    DESTDIR = $$INSTALLBASE\bin\\$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE\bin\\$$BUILD_DIR
}
unix {
    INSTALLBASE = $$PWD/../../../../Install
    DESTDIR = $$INSTALLBASE/bin/$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE/bin/$$BUILD_DIR
}

include(../../../mafImportVXL.pri)
include(../../../mafImportQXMLRPC.pri)

INCLUDEPATH += ../../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
INCLUDEPATH += ../../../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../../../mafResources
LIBS += -L$$DESTDIR \
    -lmafResources$$LIB_EXT
INCLUDEPATH += ../../../mafApplicationLogic
LIBS += -L$$DESTDIR \
    -lmafApplicationLogic$$LIB_EXT

QMAKE_CLEAN += ./MakeFile

win32:!exists( $$DESTDIR\qxmlrpc.dll ):QMAKE_PRE_LINK += copy \
    $$(MAF3_FOUNDATION_LIB_DIR)\qxmlrpc\lib\*qxmlrpc* \
    $$DESTDIR
unix:!exists( $$DESTDIR/*qxmlrpc.* ):QMAKE_PRE_LINK += cp \
    $$(MAF3_FOUNDATION_LIB_DIR)/qxmlrpc/lib/* \
    $$DESTDIR
