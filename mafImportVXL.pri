# import vxl foundation library references

# Foundation libraries dependencies
INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/vxl/include/vcl)
INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/vxl/include/core)

win32: INCLUDEPATH += $$quote($$(MAF3_FOUNDATION_LIB_DIR)/vxl/include/vcl/config.win32)

LIBS += -L$$quote($$(MAF3_FOUNDATION_LIB_DIR)/vxl/lib) \
    -lvnl \
    -lvcl

