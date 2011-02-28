###################################################
# this file defines the common variables and/or commands
# used into the mafInstallModule.pri (used by libraries)
# mafInstallTest.pri (used by test suite executables) and
# Example applications.

# console commands definition
win32 {
    CP_CMD = copy
    MV_CMD = rename
}
!win32 {
    CP_CMD = cp
    MV_CMD = mv
}

# Define the build extension for the library or the executable.
BUILD_DIR = Release
CONFIG(debug, debug|release) {
    unix{
        BUILD_EXT = _debug
    }
    win32{
        BUILD_EXT = _d
    }
    BUILD_DIR = Debug
}

TARGET = $$join(TARGET,,,$$BUILD_EXT)

INSTALL_DIR = $$(MAF3_INSTALL_DIR)




# Initialize the directory that will contains the binary
# this directory takes in account of the definition of
# the environment variable MAF3_INSTALL_DIR

# variable containing the library prefix
LIB_PREFIX = lib

win32 {
    DESTDIR = $$INSTALL_DIR\bin\\$$BUILD_DIR
#    OBJECTS_DIR = $$DESTDIR\\OBJ
#    MOC_DIR = $$DESTDIR\\MOC
    LIB_PREFIX = ""
}
unix {
    DESTDIR = $$INSTALL_DIR/bin/$$BUILD_DIR
#    OBJECTS_DIR = $$DESTDIR/OBJ
#    MOC_DIR = $$DESTDIR/MOC
}

# Definitions of gcov and lcov flags.
maf_use_lcov {
    QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs
    QMAKE_CXXFLAGS_DEBUG += -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -fprofile-arcs
    QMAKE_LFLAGS_DEBUG += -ftest-coverage
}

# Add the Makefile to the clean command
QMAKE_CLEAN += ./MakeFile

# definitions of variable containing the MAF3 Framework libraries needed when the framework is deployed
MAF_CORE = $$DESTDIR/$${LIB_PREFIX}mafCore$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_EVENTBUS = $$DESTDIR/$${LIB_PREFIX}mafEventBus$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_QA = $$DESTDIR/$${LIB_PREFIX}mafQA$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_RESOURCES = $$DESTDIR/$${LIB_PREFIX}mafResources$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_SERIALIZATION = $$DESTDIR/$${LIB_PREFIX}mafSerialization$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_TIMESPACE = $$DESTDIR/$${LIB_PREFIX}mafTimeSpace$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_APPLICATIONLOGIC = $$DESTDIR/$${LIB_PREFIX}mafApplicationLogic$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB
MAF_GUI = $$DESTDIR/$${LIB_PREFIX}mafGUI$${BUILD_EXT}.$$QMAKE_EXTENSION_SHLIB

