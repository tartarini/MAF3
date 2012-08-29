#
#  CMakeLists.txt
#  maf
#
#  Created by Matteo Giacomoni on 11/06/12.
#  Copyright 2012 B3C. All rights reserved.
#
#  See License at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Find the external packages.
## #################################################################

include(${MAF_ROOT_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_ROOT_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_ROOT_DIR}/CMakeExternals/Find_OpenCV.cmake)
include(${MAF_ROOT_DIR}/CMakeExternals/Find_QuaZIP.cmake)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Build rules
## #################################################################

# Create the executable.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files and xml configuration file
## #################################################################

mafCopyFilesFromExtension("xml" "mafPluginZip")

## #################################################################
## copy QuaZIP libraries
## #################################################################

mafCopySharedLibrariesInInstallDir(${QuaZip_LIBRARY_DIR})

## #################################################################
## Installation
## #################################################################

mafMacroInstallPlugin()

