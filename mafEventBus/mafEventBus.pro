# -------------------------------------------------
# Project created by QtCreator 2009-10-28T10:24:01
# -------------------------------------------------
QT -= gui
QT += network
QT += xml
TARGET = mafEventBus
TEMPLATE = lib
DEFINES += mafEventBus_EXPORTS
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
SOURCES += mafEventDispatcher.cpp \
    mafEventBusManager.cpp \
    mafEventDispatcherRemote.cpp \
    mafEventDispatcherLocal.cpp \
    mafNetworkConnector.cpp \
    mafEvent.cpp \
    mafNetworkConnectorQtSoap.cpp \
    mafNetworkConnectorQXMLRPC.cpp \
    mafTopicRegistry.cpp
HEADERS += mafEventBus_global.h \
    mafEventDispatcher.h \
    mafEventDefinitions.h \
    mafEventBusManager.h \
    mafEventDispatcherRemote.h \
    mafEventDispatcherLocal.h \
    mafNetworkConnector.h \
    mafEvent.h \
    mafNetworkConnectorQtSoap.h \
    mafNetworkConnectorQXMLRPC.h \
    mafTopicRegistry.h
include(../mafInstallModule.pri)
include(../mafImportQXMLRPC.pri)
include(../mafImportQtSoap.pri)
