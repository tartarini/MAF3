#
#  CMakeLists.txt
#  maf
#
#  Created by Paolo Quadrani on 01/07/11.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Find the VTK package.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_VTK.cmake)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Build rules
## #################################################################

# Create the library.
if(${BUILD_SHARED_LIBS})
  ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
else(${BUILD_SHARED_LIBS})
  ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SRCS})
endif(${BUILD_SHARED_LIBS})  

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})

## #################################################################
## copy VTK libraries
## #################################################################

mafCopySharedLibrariesInInstallDir("${VTK_DIR}/bin")

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(0)

