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
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${${PROJECT_NAME}_SOURCES_WRAP})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## Wrapping Build rules
## #################################################################
IF(${WRAP_LIST_FOUND})
    mafMacroWrapBuild()
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
ENDIF(${WRAP_LIST_FOUND})


## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()

## #################################################################
## Test suite.
## #################################################################
if(BUILD_TEST_SUITE)
  SUBDIRS("${PROJECT_NAME}Test")
endif(BUILD_TEST_SUITE)

