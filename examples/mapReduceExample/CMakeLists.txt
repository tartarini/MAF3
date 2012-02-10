#
#  CMakeLists.txt
#  maf
#
#  Created by Paolo Quadrani on 12/10/11.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


## #################################################################
## Find the external packages.
## #################################################################

#include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)


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
    SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

#mafCopyFilesFromExtension("ui" "SimpleApp")

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(1)
