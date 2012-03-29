#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2012 B3C. All rights reserved.
#
#  See License at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Find the external packages.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_OpenCV.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_FieldML.cmake)

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

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".mafplugin")

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(0)

