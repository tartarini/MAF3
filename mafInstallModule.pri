###################################################
# this file defines the variables and/or commands
# used to create the framework libraries
# it remove the definition of symbolic links to the
# versioned library and rename it according to the name
# assigned to the TARGET variable.

include(mafCommon.pri)

# Remove the symbolic links
#QMAKE_LN_SHLIB = :

# remove the version number
#QMAKE_POST_LINK = $$MV_CMD $$DESTDIR/$${LIB_PREFIX}$${TARGET}.1.0.0.$$QMAKE_EXTENSION_SHLIB $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$$QMAKE_EXTENSION_SHLIB

# MAF3 deployment step.
headers.files = $$HEADERS
headers.files += ../mafImportQtSoap.pri
headers.files += ../mafImportQXMLRPC.pri
headers.files += ../mafImportQScxml.pri
headers.files += ../mafImportVXL.pri
headers.files += ../mafImportVTK.pri
headers.path = $$(MAF3_INSTALL_DIR)/include
INSTALLS += headers
PRE_TARGETDEPS += install_headers
