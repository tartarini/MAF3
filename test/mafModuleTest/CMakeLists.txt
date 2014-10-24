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

mafMacroInitProject("plugin")

## #################################################################
## Build rules
## #################################################################

mafMacroConfigureProject("plugin")

## #################################################################
## Installation
## #################################################################

mafMacroBuildProject("test")

