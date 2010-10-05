# -------------------------------------------------
# Project created by QtCreator 2009-10-20T10:40:15
# -------------------------------------------------
QT -= gui
TARGET = mafCore
TEMPLATE = lib
CONFIG += create_prl
win32:CONFIG += dll
DEFINES += mafCore_EXPORTS
message($$DEFINES)
HEADERS += mafCore_global.h \
    mafObjectBase.h \
    mafIdProvider.h \
    mafSmartPointer.h \
    mafSmartPointer.txx \
    mafContracts.h \
    mafVisitor.h \
    mafObject.h \
    mafObjectRegistry.h \
    mafLogger.h \
    mafCoreDefinitions.h \
    mafMessageHandler.h \
    mafLoggerConsole.h \
    mafLoggerBuffer.h \
    mafLoggerFile.h \
    mafCoreSingletons.h \
    mafMementoObject.h \
    mafObjectFactoryInterface.h \
    mafObjectFactory.h \
    mafMemento.h \
    mafVisitorFindObjects.h \
    mafVisitorFindObjectsByHashCode.h \
    mafCoreRegistration.h \
    mafClassFactory.h \
    mafUtilities.h \
    mafContainerInterface.h \
    mafContainer.h \
    mafContainer.txx \
    mafMonitor.h \
    mafMonitorMemory.h \
    mafMonitorRAM.h \
    mafMonitorHardDisk.h \
    mafExternalDataCodec.h \
    mafCommand.h \
    mafHierarchy.h \
    mafOntology.h \
    mafTree.hpp \
    mafSafeIterator.hpp \
    mafExceptions.hpp \
    mafContainersFixes.hpp
SOURCES += mafObjectBase.cpp \
    mafIdProvider.cpp \
    mafObject.cpp \
    mafObjectRegistry.cpp \
    mafLogger.cpp \
    mafMessageHandler.cpp \
    mafLoggerConsole.cpp \
    mafLoggerBuffer.cpp \
    mafLoggerFile.cpp \
    mafVisitor.cpp \
    mafMementoObject.cpp \
    mafObjectFactory.cpp \
    mafMemento.cpp \
    mafVisitorFindObjects.cpp \
    mafVisitorFindObjectsByHashCode.cpp \
    mafUtilities.cpp \
    mafContainerInterface.cpp \
    mafMonitor.cpp \
    mafMonitorMemory.cpp \
    mafMonitorRAM.cpp \
    mafMonitorHardDisk.cpp \
    mafCoreSingletons.cpp \
    mafExternalDataCodec.cpp \
    mafCommand.cpp \
    mafHierarchy.cpp \
    mafOntology.cpp \
    mafExceptions.cpp
include(../mafInstallModule.pri)
include(../mafInstallFoundationLibraries.pri)
OTHER_FILES += mafTree.tpp \
    mafSafeIterator.tpp
