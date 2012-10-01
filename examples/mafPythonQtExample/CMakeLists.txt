#
#  CMakeLists.txt
#  MSVTK
#
#  Created by Paolo Quadrani on 24/10/11.
#  Copyright 2011 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


## #################################################################
## Find the external packages.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_PythonQt.cmake)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

INCLUDE_DIRECTORIES("${MAF_ROOT_BINARY_DIR}/wrap/mafPythonQtDecorators/mafCore")

## #################################################################
## Build rules
## #################################################################

# Set some Win32 Specific Settings
IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" ${PROJECT_NAME})
mafCopyFilesFromExtension("xml" ${PROJECT_NAME})

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(1)
