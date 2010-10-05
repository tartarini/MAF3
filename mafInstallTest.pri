# install
# target.path is the directory used for the make install step

mac: CONFIG-=app_bundle

BUILD_DIR = Release

CONFIG(debug, debug|release) {
    unix:LIB_EXT = _debug
    win32:LIB_EXT = _d
	BUILD_DIR = Debug
}

TARGET = $$join(TARGET,,,$$LIB_EXT)

win32 {
    INSTALLBASE = ..\..\..\Install
    INCLUDEPATH += ..\..\mafQA
    DESTDIR = $$INSTALLBASE\bin\\$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE\bin\\$$BUILD_DIR
}
unix {
    INSTALLBASE = $$PWD/../Install
    INCLUDEPATH += ../../mafQA
    DESTDIR = $$INSTALLBASE/bin/$$BUILD_DIR
    DEPENDPATH += $$INSTALLBASE/bin/$$BUILD_DIR
}
INCLUDEPATH += ..

LIBS += -L$$DESTDIR -lmafQA$$LIB_EXT

maf_use_lcov {
    message("adding lcov compiler flags")
    QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs
    QMAKE_CXXFLAGS_DEBUG += -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -fprofile-arcs
    QMAKE_LFLAGS_DEBUG += -ftest-coverage
}

QMAKE_CLEAN += ./MakeFile
