# -------------------------------------------------
# Project created by QtCreator 2010-03-01T14:14:10
# -------------------------------------------------
QT += xml
CONFIG += qtestlib
CONFIG += link_prl
DEFINES +=VTK_BUILD_SHARED_LIBS
SOURCES += main.cpp \
    mafPipeVisualVTKSurfaceTest.cpp \
    mafViewVTKTest.cpp \
    mafPipeDataImageThresholdTest.cpp \
    mafExternalDataCodecVTKTest.cpp \
    mafSerializationVTKTest.cpp \
    mafSerializationExtDataTest.cpp \
    mafSerializationRawBinaryExtDataTest.cpp \
    mafSerializationRawASCIIExtDataTest.cpp \
    mafVTKWidgetTest.cpp \
    mafPipeVisualSelectionTest.cpp \
    mafDataBoundaryAlgorithmVTKTest.cpp \
    mafVTKInteractorPickerTest.cpp \
    mafVTKParametricSurfaceSphereTest.cpp \
    mafVTKParametricSurfaceCubeTest.cpp \
    mafVTKParametricSurfaceConeTest.cpp \
    mafVTKParametricSurfaceCylinderTest.cpp \
    mafVTKParametricSurfaceEllipsoidTest.cpp \
    mafVTKParametricSurfaceTest.cpp \
    mafOpParametricSurfaceTest.cpp
include(../../mafInstallTest.pri)
include(../../mafImportVXL.pri)
include(../../mafImportVTK.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)
INCLUDEPATH += ../../mafResources \
    ../../mafEventBus \
    ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafResources$$BUILD_EXT \
    -lmafEventBus$$BUILD_EXT \
    -lmafCore$$BUILD_EXT \
    -lmafPluginVTK$${BUILD_EXT}
