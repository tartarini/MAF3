# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:04:06
# -------------------------------------------------
QT -= gui
QT += testlib
TARGET = mafQA
TEMPLATE = lib
DEFINES += mafQA_EXPORTS
CONFIG += link_prl
win32:CONFIG += dll
SOURCES += mafQAManager.cpp \
    mafProfiler.cpp \
    mafTestRegistry.cpp
HEADERS += mafQA_global.h \
    mafProfiler.h \
    mafQAManager.h \
    mafQADefinitions.h \
    mafTestRegistration.h \
    mafTestRegistry.h \
    mafTestSuite.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$BUILD_EXT
