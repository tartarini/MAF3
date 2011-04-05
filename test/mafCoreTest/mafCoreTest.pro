# -------------------------------------------------
# Project created by QtCreator 2009-10-22T18:19:06
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafObjectBaseTest.cpp \
    mafIdProviderTest.cpp \
    mafObjectTest.cpp \
    mafObjectRegistryTest.cpp \
    mafLoggerConsoleTest.cpp \
    mafMessageHandlerTest.cpp \
    mafLoggerBufferTest.cpp \
    mafLoggerFileTest.cpp \
    mafMementoObjectTest.cpp \
    mafObjectFactoryTest.cpp \
    mafVisitorFindObjectsByHashCodeTest.cpp \
    mafContainerTest.cpp \
    mafMonitorRAMTest.cpp \
    mafMonitorHardDiskTest.cpp \
    mafVisitorTest.cpp \
    mafClassFactoryTest.cpp \
    mafContainerInterfaceTest.cpp \
    mafLoggerTest.cpp \
    mafMementoTest.cpp \
    mafMonitorTest.cpp \
    mafExternalDataCodecTest.cpp \
    mafVisitorFindObjectsTest.cpp \
    mafMonitorMemoryTest.cpp \
    mafCoreSingletonsTest.cpp \
    mafCoreRegistrationTest.cpp
include(../../mafInstallTest.pri)
LIBS += -L$$DESTDIR \
    -lmafCore$$BUILD_EXT
HEADERS += 
