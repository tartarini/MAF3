###################################################
# this file defines the variables and/or commands
# used to create the test suite executables

include(mafCommon.pri)

# for Mac OSX Operative system create a unix executable.
mac: CONFIG-=app_bundle

# Add library dependencies path to the destdir (where all the libraries are generated)
DEPENDPATH += $$DESTDIR

# Add the mafQA library to the include path
win32 {
    INCLUDEPATH += ..\..\mafQA
}
unix {
    INCLUDEPATH += ../../mafQA
}

# Add the tested library directory to the include path
INCLUDEPATH += ..

LIBS += -L$$DESTDIR -lmafQA$$BUILD_EXT
