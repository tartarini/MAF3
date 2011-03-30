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
## Find the qxmlrpc package.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)

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
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()

## #################################################################
## Test suite.
## #################################################################
if(BUILD_TEST_SUITE)
  #SUBDIRS("mafPluginTest")
  #SUBDIRS("${PROJECT_NAME}Test")
endif(BUILD_TEST_SUITE)

