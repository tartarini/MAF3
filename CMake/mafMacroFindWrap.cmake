#
#  mafMacroFindWrap.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Swig
## #################################################################

find_package(SWIG QUIET)

if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  set(CMAKE_SWIG_FLAGS "")
  
  macro(mafMacroWrap project target name language input deps)
    set(wrap_output ${project}_wrap_${language}.cpp)
    
    add_custom_command(
      OUTPUT ${wrap_output}
      COMMAND ${SWIG_EXECUTABLE}
      ARGS
      "-${language}"
      "-c++"
      "-module" ${name}
      "-I${CMAKE_SOURCE_DIR}/${name}"
      "-outdir" ${CMAKE_CURRENT_BINARY_DIR}
      "-o" ${wrap_output}
      ${input}
      MAIN_DEPENDENCY ${input}
      COMMENT "Wrapping ${input} to ${language}")
    set(${target} ${${target}} ${wrap_output})
  endmacro(mafMacroWrap)
  
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
endif(SWIG_FOUND)

if(SWIG_FOUND)
  add_definitions(-DHAVE_SWIG)
endif(SWIG_FOUND)

## #################################################################
## Other language
## #################################################################


## #################################################################
## Python
## #################################################################

include(mafMacroFindPython.cmake)
