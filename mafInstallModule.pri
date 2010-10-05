# install
# target.path is the directory used for the make install step

BUILD_DIR = Release

CONFIG(debug, debug|release) {
    unix{
        LIB_EXT = _debug
    }
    win32{
        LIB_EXT = _d
    }
    BUILD_DIR = Debug
}

TARGET = $$join(TARGET,,,$$LIB_EXT)

win32 {
    INSTALLBASE = ..\..\Install
    DESTDIR = $$INSTALLBASE\bin\\$$BUILD_DIR
}
unix {
    INSTALLBASE = $$PWD/../Install
    DESTDIR = $$INSTALLBASE/bin/$$BUILD_DIR
}

maf_use_lcov {
    QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs
    QMAKE_CXXFLAGS_DEBUG += -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -fprofile-arcs
    QMAKE_LFLAGS_DEBUG += -ftest-coverage
}

QMAKE_CLEAN += ./MakeFile

# MAF3 deployment step.
headers.files = $$HEADERS
headers.files += ../mafImportQtSoap.pri
headers.files += ../mafImportQXMLRPC.pri
headers.files += ../mafImportQScxml.pri
headers.files += ../mafImportVXL.pri
headers.files += ../mafImportVTK.pri
headers.path = $$INSTALLBASE/include
INSTALLS += headers
PRE_TARGETDEPS += install_headers
