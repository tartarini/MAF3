# import qxmlrpc foundation library references

# Foundation libraries dependencies
COMPILER = $$QMAKE_CC
!contains(COMPILER, gcc) {
    DEFINES += QT_QTSOAP_IMPORT
}

INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/qtsoap/include/)
LIBS += -L$$quote($$(MAF3_FOUNDATION_LIB_DIR)/qtsoap/lib) \
    -lQtSolutions_SOAP-2.7$$BUILD_EXT
