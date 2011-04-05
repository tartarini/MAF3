# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:49:18
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafTimerTest.cpp \
    mafTimeManagerTest.cpp
HEADERS += 
include(../../mafInstallTest.pri)
LIBS += -L$$DESTDIR \
    -lmafTimeSpace$$BUILD_EXT -lmafCore$$BUILD_EXT -lmafEventBus$$BUILD_EXT
INCLUDEPATH += ../../mafCore ../../mafEventBus
    
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
