# -------------------------------------------------
# Project created by QtCreator 2010-02-15T12:35:58
# -------------------------------------------------
QT -= gui
TARGET = mafPluginVTK
TEMPLATE = lib
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
DEFINES += MAFPLUGIN_VTK
SOURCES += mafDataPipeSurface.cpp \
    mafPluginRegistrator.cpp \
    mafVisualPipeVTKSurface.cpp \
    mafViewVTK.cpp \
    mafDataPipeImageThreshold.cpp \
    mafExternalDataCodecVTK.cpp \
    mafInteractorVTK.cpp \
    mafEventBridgeVTK.cpp \
    mafDataBoundaryAlgorithmVTK.cpp
HEADERS += mafPluginVTK_global.h \
    mafPluginVTKDefinitions.h \
    mafDataPipeSurface.h \
    mafPluginRegistrator.h \
    mafVisualPipeVTKSurface.h \
    mafViewVTK.h \
    mafDataPipeImageThreshold.h \
    mafExternalDataCodecVTK.h \
    mafInteractorVTK.h \
    mafEventBridgeVTK.h \
    mafDataBoundaryAlgorithmVTK.h
include(../mafInstallModule.pri)
include(../mafImportVXL.pri)
include(../mafImportVTK.pri)
include(../mafImportQtSoap.pri)
INCLUDEPATH += ../mafResources
LIBS += -L$$DESTDIR \
    -lmafResources$$LIB_EXT
INCLUDEPATH += ../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT
INCLUDEPATH += ../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
