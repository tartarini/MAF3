# -------------------------------------------------
# Project created by QtCreator 2010-21-04T13:15:13
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafLogicLightTest.cpp \
    mafLogicTest.cpp
include(../../mafInstallTest.pri)

LIBS += -L$$DESTDIR \
    -lmafApplicationLogic$$BUILD_EXT -lmafEventBus$$BUILD_EXT -lmafCore$$BUILD_EXT
INCLUDEPATH += ../../mafEventBus ../../mafCore

include(../../mafImportVXL.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
