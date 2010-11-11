# -------------------------------------------------
# Project created by QtCreator 2010-03-01T14:14:10
# -------------------------------------------------
QT += xml
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafVisualPipeVTKSurfaceTest.cpp \
    mafViewVTKTest.cpp \
    mafDataPipeImageThresholdTest.cpp \
    mafExternalDataCodecVTKTest.cpp \
    mafSerializationVTKTest.cpp \
    mafSerializationExtDataTest.cpp \
    mafSerializationRawBinaryExtDataTest.cpp \
    mafSerializationRawASCIIExtDataTest.cpp \
    mafEventBridgeVTKTest.cpp \
    mafVisualPipeSelectionTest.cpp
include(../../mafInstallTest.pri)
include(../../mafImportVXL.pri)
include(../../mafImportVTK.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
LIBS += -L$$DESTDIR \
    -lmafPluginVTK$$LIB_EXT
INCLUDEPATH += ../../mafResources
LIBS += -L$$DESTDIR \
    -lmafResources$$LIB_EXT
INCLUDEPATH += ../../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
