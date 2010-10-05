# -------------------------------------------------
# Project created by QtCreator 2009-10-26T18:49:18
# -------------------------------------------------
CONFIG += qtestlib
CONFIG += link_prl
SOURCES += main.cpp \
    mafProfilerTest.cpp \
    mafQAManagerTest.cpp
include(../../mafInstallTest.pri)
LIBS += -L$$DESTDIR \
    -lmafQA$$LIB_EXT
INCLUDEPATH += ../../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$LIB_EXT
