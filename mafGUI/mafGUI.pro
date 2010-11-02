# -------------------------------------------------
# Project created by QtCreator 2010-10-26T11:13:20
# -------------------------------------------------
TARGET = mafGUI
TEMPLATE = lib
CONFIG += uitools
win32:CONFIG += dll
DEFINES += mafGUI_EXPORTS
SOURCES += mafGUIManager.cpp \
    mafGUIRegistration.cpp \
    mafUILoader.cpp \
    mafUILoaderQt.cpp
HEADERS += mafGUI_global.h \
    mafGUIManager.h \
    mafGUIRegistration.h \
    mafGUIDefinitions.h \
    mafUILoader.h \
    mafUILoaderQt.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafCore \
    ../mafEventBus \
    ../mafApplicationLogic
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT \
    -lmafEventBus$$LIB_EXT \
    -lmafApplicationLogic$$LIB_EXT
RESOURCES += mafAppImages.qrc
