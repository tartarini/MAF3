# Generate the MAFConfig.cmake file in the build tree and configure one
# for the installation tree.  This file tells external project how to use
# MAF.  This provides a mechanism for third party developers to build 
# modules against a MAF installation.
#
#

# The configuration process is very different for a build tree and an
# installation. The resulting directory structures are vastly
# different. So, the two configured files not only have different
# settings, they have a different structure.

# Settings that are the same for build trees and installation trees
#
#

# Settings specific to build trees
#
#

SET(MAF_ROOT_DIR ${MAF_SOURCE_DIR})
SET(MAF_INTERNAL_BUILD_DIR ${MAF_BINARY_DIR}/build)
SET(MAF_EXTERNAL_BUILD_DIR ${MAF_BINARY_DIR}/ExternalLibraries)
SET(MAF_USE_FILE_CONFIG ${MAF_BINARY_DIR}/UseMAF.cmake)
SET(MAF_SRC_DIRS_CONFIG ${MAF_SOURCE_DIR}/src)
SET(MAF_BIN_DIRS_CONFIG ${MAF_INTERNAL_BUILD_DIR}/bin)

# pass options to mafConfig.cmake
SET(MAF_CORE ${MAF_CORE})
SET(MAF_EVENT_BUS ${MAF_EVENT_BUS})
SET(MAF_TIME_SPACE ${MAF_TIME_SPACE})
SET(MAF_SERIALIZATION ${MAF_SERIALIZATION})
SET(MAF_RESOURCES ${MAF_RESOURCES})
SET(MAF_PLUGINVTK ${MAF_PLUGINVTK})
SET(VTK_MAF ${VTK_MAF})
SET(MAF_APPLICATION_LOGIC ${MAF_APPLICATION_LOGIC})
SET(MAF_QTGUI ${MAF_QTGUI})

SET(BUILD_TEST_SUITE ${BUILD_TEST_SUITE})
SET(BUILD_EXAMPLES ${BUILD_EXAMPLES})
 
SET(VTK_DIR_CONFIG ${VTK_DIR})
SET(PYTHONLIBS_FOUND ${PYTHONLIBS_FOUND})
SET(PYTHON_INCLUDE_PATH ${PYTHON_INCLUDE_PATH})
SET(PYTHON_LIBRARIES ${PYTHON_LIBRARIES})
SET(PYTHON_LIBRARY ${PYTHON_LIBRARY})

# Configure MAFConfig.cmake for the install tree.
CONFIGURE_FILE(
  ${MAF_SOURCE_DIR}/MAFConfig.cmake.in
  ${MAF_BINARY_DIR}/MAFConfig.cmake @ONLY)
      
