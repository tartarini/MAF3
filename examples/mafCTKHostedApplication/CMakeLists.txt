#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


## #################################################################
## Find the external packages.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_OpenCV.cmake)

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_CTK.cmake)
## #################################################################
## Additional Includes
## #################################################################
include_directories("${CTK_SOURCE_DIR}/Plugins/org.commontk.dah.core")
include_directories("${CTK_SOURCE_DIR}/Plugins/org.commontk.dah.hostedapp")
include_directories("${CTK_BINARY_DIR}/Plugins/org.commontk.dah.core")
include_directories("${CTK_BINARY_DIR}/Plugins/org.commontk.dah.hostedapp")
include_directories("${CTK_SOURCE_DIR}/Plugins/org.commontk.dah.app")
include_directories("${CTK_BINARY_DIR}/Plugins/org.commontk.dah.app")

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Build rules
## #################################################################

# Set some Win32 Specific Settings
IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
    #SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" "SimpleApp")

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()
