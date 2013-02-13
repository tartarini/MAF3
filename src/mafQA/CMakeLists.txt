#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

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
#if(${BUILD_SHARED_LIBS})
  ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
#else(${BUILD_SHARED_LIBS})
#  ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SRCS})
#endif(${BUILD_SHARED_LIBS})  


TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(0)
