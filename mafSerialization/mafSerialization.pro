# -------------------------------------------------
# Project created by QtCreator 2009-12-11T10:12:39
# -------------------------------------------------
QT -= gui
QT += xml
TARGET = mafSerialization
TEMPLATE = lib
CONFIG += create_prl
CONFIG += link_prl
win32:CONFIG += dll
DEFINES += MAFSERIALIZATION_LIBRARY
SOURCES += mafSerializationManager.cpp \
    mafCodec.cpp \
    mafCodecRawBinary.cpp \
    mafCodecXML.cpp \
    mafSerializer.cpp \
    mafSerializerFileSystem.cpp \
    mafSerializationRegistration.cpp \
    mafCodecRawASCII.cpp \
    mafCodecRaw.cpp \
    mafCodecBrickedVolume.cpp \
    mafCodecVolume.cpp
    #mafCSVReader.cpp \
    #mafCodecCSV.cpp
HEADERS += mafSerializationManager.h \
    mafSerialization_global.h \
    mafSerializationDefinitions.h \
    mafCodec.h \
    mafCodecRawBinary.h \
    mafCodecXML.h \
    mafSerializer.h \
    mafSerializerFileSystem.h \
    mafSerializationRegistration.h \
    mafCodecRawASCII.h \
    mafCodecRaw.h \
    mafCodecBrickedVolume.h \
    mafCodecVolume.h
   # mafCSVReader.h \
   # mafCodecCSV.h
include(../mafInstallModule.pri)
INCLUDEPATH += ../mafEventBus
LIBS += -L$$DESTDIR \
    -lmafEventBus$$BUILD_EXT
INCLUDEPATH += ../mafCore
LIBS += -L$$DESTDIR \
    -lmafCore$$BUILD_EXT
