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
    mafCodecRawASCIITest.cpp
HEADERS += 
include(../../mafInstallTest.pri)
LIBS += -L$$DESTDIR \
    -lmafSerialization$$LIB_EXT
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
INCLUDEPATH += ../../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT

include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
