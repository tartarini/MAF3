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

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Wrapping
## #################################################################

mafMacroWrapTargetFiles()

## #################################################################
## Build rules
## #################################################################

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" "mafResources")

## #################################################################
## copy OpenCV libraries
## #################################################################

mafCopySharedLibrariesInInstallDir("${OpenCV_DIR}/lib")

## #################################################################
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()


