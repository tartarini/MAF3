# -------------------------------------------------
# Project created by QtCreator 2010-04-20T17:30:12
# -------------------------------------------------
QT -= gui
TARGET = mafApplicationLogic
TEMPLATE = lib
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
DEFINES += MAFAPPLICATIONLOGIC_LIBRARY
SOURCES += mafLogicLight.cpp \
    mafLogic.cpp
HEADERS += mafLogicLight.h \
    mafApplicationLogic_global.h \
    mafLogic.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafEventBus \
                        ../mafCore
LIBS += -L$$DESTDIR \
    -lmafEventBus$$BUILD_EXT -lmafCore$$BUILD_EXT

include(../mafImportVXL.pri)
include(../mafImportQtSoap.pri)
