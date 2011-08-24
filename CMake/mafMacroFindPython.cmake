#
#  mafMacroFindPython.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 24/08/11.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Python
## #################################################################

find_package(PythonLibs QUIET)

if(PYTHONLIBS_FOUND)
  set(PYTHON_DEBUG_LIBRARY "")
  include_directories(${PYTHON_INCLUDE_PATH})
  get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)
  link_directories(${PYTHON_PATH})
endif(PYTHONLIBS_FOUND)

if(PYTHONLIBS_FOUND)
  add_definitions(-DHAVE_PYTHON)
endif(PYTHONLIBS_FOUND)
