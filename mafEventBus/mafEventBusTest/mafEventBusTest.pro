# -------------------------------------------------
# Project created by QtCreator 2009-11-09T12:51:13
# -------------------------------------------------
QT += network
QT += xml
DEFINES += EXCLUDE_SINGLETONS
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafEventBusManagerTest.cpp \
    mafNetworkConnectorTest.cpp \
    mafEventDispatcherLocalTest.cpp \
    mafEventDispatcherRemoteTest.cpp \
    mafEventDispatcherTest.cpp \
    mafNetworkConnectorQtSoapTest.cpp \
    mafNetworkConnectorQXMLRPCTest.cpp
include(../../mafInstallTest.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
