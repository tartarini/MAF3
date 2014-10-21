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

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_OpenCV.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_fervor.cmake)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject("application")

## #################################################################
## Build rules
## #################################################################

# Set some Win32 Specific Settings
IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)

SET(chartSample_RESOURCES chartSample.qrc)
QT4_ADD_RESOURCES(chartSample_RESOURCES_RCC ${chartSample_RESOURCES})

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS} ${chartSample_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" ${PROJECT_NAME})
mafCopyFilesFromExtension("xml" ${PROJECT_NAME})

## #################################################################
## Installation
## #################################################################

mafMacroBuildProject("application")
