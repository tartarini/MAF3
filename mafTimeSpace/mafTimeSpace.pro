# -------------------------------------------------
# Project created by QtCreator 2009-11-23T19:35:09
# -------------------------------------------------
QT -= gui
TARGET = mafTimeSpace
TEMPLATE = lib
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
DEFINES += MAFTIMESPACE_LIBRARY
SOURCES += mafTimeManager.cpp \
    mafTimer.cpp \
    mafThreadedTimer.cpp \
    mafModuleRegistrator.cpp
HEADERS += mafTimeManager.h \
    mafTimeSpace_global.h \
    mafTimeSpaceDefinitions.h \
    mafTimer.h \
    mafThreadedTimer.h \
    mafModuleRegistrator.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafEventBus ../mafCore
LIBS += -L$$DESTDIR \
    -lmafEventBus$$BUILD_EXT -lmafCore$$BUILD_EXT
