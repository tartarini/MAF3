# import utility foundation library references

# Foundation libraries dependencies
INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/utility/include)
LIBS += -L$$quote($$(MAF3_FOUNDATION_LIB_DIR)/utility/lib) \
    -lutility

