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
## Find the VTK package.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_VTK.cmake)

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
LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})

## #################################################################
## copy ui files and xml configuration file
## #################################################################

mafCopyFilesFromExtension("ui" "mafPluginVTK")
mafCopyFilesFromExtension("xml" "mafPluginVTK")

## #################################################################
## copy VTK libraries
## #################################################################

mafCopySharedLibrariesInInstallDir("${VTK_DIR}/bin")

## #################################################################
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroInstallPlugin()

