# -------------------------------------------------
# Project created by QtCreator 2010-21-04T13:15:13
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafLogicLightTest.cpp
include(../../mafInstallTest.pri)
unix:LIB_EXT = _debug
win32:LIB_EXT = _d
LIBS += -L$$DESTDIR \
    -lmafApplicationLogic$$LIB_EXT
INCLUDEPATH += ../../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
include(../../mafImportVXL.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
