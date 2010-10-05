# import utility foundation library references

# Foundation libraries dependencies
INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/qscxml/include/)
LIBS += -L$$quote($$(MAF3_FOUNDATION_LIB_DIR)/qscxml/lib) \
    -lqscxml

