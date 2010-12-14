# import qxmlrpc foundation library references

# Foundation libraries dependencies
INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/qxmlrpc/include/)
LIBS += -L$$quote($$(MAF3_FOUNDATION_LIB_DIR)/qxmlrpc/lib) \
    -lqxmlrpc$$BUILD_EXT
