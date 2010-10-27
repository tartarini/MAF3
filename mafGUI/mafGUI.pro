# -------------------------------------------------
# Project created by QtCreator 2010-10-26T11:13:20
# -------------------------------------------------
TARGET = mafGUI
TEMPLATE = lib
win32:CONFIG += dll
DEFINES += mafGUI_EXPORTS
SOURCES += mafGUIManager.cpp \
    mafGUIRegistration.cpp
HEADERS += mafGUI_global.h \
    mafGUIManager.h \
    mafGUIRegistration.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafCore \
    ../mafEventBus \
    ../mafApplicationLogic
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT \
    -lmafEventBus$$LIB_EXT \
    -lmafApplicationLogic$$LIB_EXT
