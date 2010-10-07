# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:49:18
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafResourceTest.cpp \
    mafVMETest.cpp \
    mafInterpolatorBeforeTest.cpp \
    mafInterpolatorNearestTest.cpp \
    mafDataSetTest.cpp \
    mafPluginManagerTest.cpp \
    mafOperationTest.cpp \
    mafDataSetCollectionTest.cpp \
    mafResourcesRegistrationTest.cpp \
    mafHierarchyTest.cpp \
    mafViewManagerTest.cpp \
    mafDataPipeTest.cpp \
    mafInterpolatorTest.cpp \
    mafMementoResourceTest.cpp \
    mafPipeTest.cpp \
    mafPluginTest.cpp \
    mafSceneNodeTest.cpp \
    mafVisualPipeTest.cpp \
    mafViewTest.cpp \
    mafDataBoundaryAlgorithmTest.cpp \
    mafVMEManagerTest.cpp \
    mafOperationManagerTest.cpp \
    mafMementoDataSetTest.cpp \
    mafMementoVMETest.cpp \
    mafInteractorTest.cpp
include(../../mafInstallTest.pri)
include(../../mafImportVXL.pri)
include(../../mafImportQXMLRPC.pri)
include(../../mafImportQtSoap.pri)

# Foundation libraries dependencies
LIBS += -L$$DESTDIR \
    -lmafResources$$LIB_EXT
INCLUDEPATH += ../../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
