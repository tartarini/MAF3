# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:49:18
# -------------------------------------------------
CONFIG += uitools
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafGUIManagerTest.cpp \
    mafUILoaderTest.cpp \
    mafUILoaderQtTest.cpp
include(../../mafInstallTest.pri)

# Foundation libraries dependencies
INCLUDEPATH += ../../mafEventBus \
    ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafEventBus$$LIB_EXT \
    -lmafCore$$LIB_EXT \
    -lmafGUI$$LIB_EXT
