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
## Find the VTK package.
## #################################################################
include(FindVTK)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Build rules
## #################################################################

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" "SimpleApp")

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject()
