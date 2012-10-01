#
#  mafMacroGetTargetLibraries.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroGetTargetLibraries varname)

  SET(filepath ${CMAKE_CURRENT_SOURCE_DIR}/target_libraries.cmake)
  
  # Check if "target_libraries.cmake" file exists
#  IF(NOT EXISTS ${filepath} )
#    MESSAGE(FATAL_ERROR "${filepath} doesn't exists !")
#  ENDIF()

  # Make sure the variable is cleared
  SET(target_libraries )
  
  # Check if "target_libraries.cmake" file exists
  # if the file does not exists, no external dependences are assigned.
  IF(EXISTS ${filepath})
    # Let's make sure target_libraries contains only strings
    FILE(STRINGS "${filepath}" stringtocheck) # read content of 'filepath' into 'stringtocheck'
    #STRING(REGEX MATCHALL "[^\\#]\\$\\{.*\\}" incorrect_elements ${stringtocheck})
    FOREACH(incorrect_element ${incorrect_elements})
      STRING(REGEX REPLACE "\\$|\\{|\\}" "" correct_element ${incorrect_element})
      MESSAGE(FATAL_ERROR "In ${filepath}, ${incorrect_element} should be replaced by ${correct_element}")
    ENDFOREACH()

    INCLUDE(${filepath})

    # Loop over all target library
    # Loop over all target files, let's resolve the variable to access its content
    FOREACH(target_library ${target_libraries})
      LIST(APPEND ${varname} "${target_library}")
    ENDFOREACH()
    
  ENDIF()
  
ENDMACRO()
