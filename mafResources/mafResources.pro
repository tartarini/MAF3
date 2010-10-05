# -------------------------------------------------
# Project created by QtCreator 2009-12-30T13:52:24
# -------------------------------------------------
QT -= gui
TARGET = mafResources
TEMPLATE = lib
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
DEFINES += MAFRESOURCES_LIBRARY
SOURCES += mafResource.cpp \
    mafMementoResource.cpp \
    mafVME.cpp \
    mafInterpolator.cpp \
    mafInterpolatorBefore.cpp \
    mafInterpolatorNearest.cpp \
    mafDataSet.cpp \
    mafPipe.cpp \
    mafDataPipe.cpp \
    mafVisualPipe.cpp \
    mafPlugin.cpp \
    mafPluginManager.cpp \
    mafOperation.cpp \
    mafViewManager.cpp \
    mafResourcesSingletons.cpp \
    mafDataSetCollection.cpp \
    mafView.cpp \
    mafResourcesRegistration.cpp \
    mafSceneNode.cpp \
    mafVMEManager.cpp \
    mafOperationManager.cpp \
    mafVisitorFindSceneNodeByVMEHash.cpp \
    mafUndoStackCommand.cpp \
    mafMementoVME.cpp \
    mafMementoDataSet.cpp
HEADERS += mafResource.h \
    mafResources_global.h \
    mafResourcesDefinitions.h \
    mafMementoResource.h \
    mafVME.h \
    mafResourcesRegistration.h \
    mafInterpolator.h \
    mafInterpolatorBefore.h \
    mafInterpolatorNearest.h \
    mafDataSet.h \
    mafPipe.h \
    mafDataPipe.h \
    mafVisualPipe.h \
    mafPlugin.h \
    mafPluginManager.h \
    mafOperation.h \
    mafViewManager.h \
    mafResourcesSingletons.h \
    mafDataSetCollection.h \
    mafView.h \
    mafSceneNode.h \
    mafVMEManager.h \
    mafDataBoundaryAlgorithm.h \
    mafOperationManager.h \
    mafUndoStackCommand.h \
    mafVisitorFindSceneNodeByVMEHash.h \
    mafMementoVME.h \
    mafMementoDataSet.h
include(../mafInstallModule.pri)
include(../mafImportVXL.pri)
include(../mafImportQtSoap.pri)

INCLUDEPATH += ../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT

