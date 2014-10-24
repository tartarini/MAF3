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
mafMacroInitProject("library")

## #################################################################
## Wrapping
## #################################################################

mafMacroWrapTargetFiles()

## #################################################################
## Build rules
## #################################################################

set(RESOURCE1 ${CMAKE_SOURCE_DIR}/src/mafGUI/mafGUIImages.qrc)
set(RESOURCE2 ${CMAKE_SOURCE_DIR}/src/mafGUI/mafGUIVMEImages.qrc)
qt5_add_resources(RESOURCE_ADDED ${RESOURCE1} ${RESOURCE2})


# Create the library.
#if(${BUILD_SHARED_LIBS})
  ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${RESOURCE_ADDED})
#else(${BUILD_SHARED_LIBS})
#  ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SRCS} ${RESOURCE_ADDED})
#endif(${BUILD_SHARED_LIBS})  


TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS} Qt5::UiTools)

## #################################################################
## copy mnu file
## #################################################################

mafCopyFilesFromExtension("mnu" ${PROJECT_NAME})
mafCopyFilesFromExtension("xml" ${PROJECT_NAME})

## #################################################################
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()

## #################################################################
## Installation
## #################################################################

mafMacroBuildProject("library")
