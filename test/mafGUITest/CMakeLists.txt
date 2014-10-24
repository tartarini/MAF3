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

mafMacroInitProject("test")

## #################################################################
## Build rules
## #################################################################

# Create the executable.
mafMacroConfigureProject("test")

## #################################################################
## copy ui files
## #################################################################

#mafCopyFilesFromExtension("ui" "mafGUITest")

## #################################################################
## Installation
## #################################################################

mafMacroBuildProject("test")
