# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:49:18
# -------------------------------------------------
QT += xml
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafSerializationManagerTest.cpp \
    mafCodecRawBinaryTest.cpp \
    mafSerializerFileSystemTest.cpp \
    mafCodecXMLTest.cpp \
    mafCodecTest.cpp \
    mafSerializerTest.cpp \
    mafCodecRawASCIITest.cpp \
    mafCodecBrickedVolumeTest.cpp \
    mafCodecVolumeTest.cpp
    #mafCSVReaderTest.cpp
HEADERS += 
include(../../mafInstallTest.pri)
LIBS += -L$$DESTDIR \
    -lmafSerialization$$BUILD_EXT \
    -lmafCore$$BUILD_EXT \
    -lmafEventBus$$BUILD_EXT
INCLUDEPATH += ../../mafCore \
    ../../mafEventBus
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
