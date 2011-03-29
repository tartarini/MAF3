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

  SET(qxmlrpc_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/qxmlrpc")
  FIND_PACKAGE(qxmlrpc REQUIRED)
  include(${qxmlrpc_DIR}/qxmlrpcConfig.cmake)
  INCLUDE(${qxmlrpc_USE_FILE})
  #additional include directory
  INCLUDE_DIRECTORIES(${MAF_EXTERNAL_BUILD_DIR}/Source/qxmlrpc/xmlrpc)

## #################################################################
## Find the VTK package.
## #################################################################

  # need ${VTK_DIR} that is be set by ExternalProject_Add for vtk
  SET(VTK_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/VTK")
  FIND_PACKAGE(VTK REQUIRED)
  #this set is because, inside VTK 5, nobody fills it
  SET(VTK_CMAKE_DIR "${MAF_EXTERNAL_BUILD_DIR}/Source/VTK/Cmake")
  INCLUDE(${VTK_USE_FILE})

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
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()

