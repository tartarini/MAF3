#
#  mafMacroWrapTargetFiles.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
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
## Tcl
## #################################################################

find_package(TCL QUIET)

if(TCL_FOUND)
  include_directories(${TCL_INCLUDE_PATH})
endif(TCL_FOUND)

if(TCL_FOUND)
  add_definitions(-DHAVE_TCL)
endif(TCL_FOUND)

## #################################################################
## Python
## #################################################################

find_package(PythonLibs QUIET)

if(PYTHONLIBS_FOUND)
  include_directories(${PYTHON_INCLUDE_PATH})
  get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)
  link_directories(${PYTHON_PATH})
endif(PYTHONLIBS_FOUND)

if(PYTHONLIBS_FOUND)
  add_definitions(-DHAVE_PYTHON)
endif(PYTHONLIBS_FOUND)


MACRO(mafMacroWrapTargetFiles)

  SET(filepath ${CMAKE_CURRENT_SOURCE_DIR}/target_wrap_files.cmake)

  # Check if "target_wrap_files.cmake" file exists
  IF(NOT EXISTS ${filepath})
    MESSAGE(FATAL_ERROR "${filepath} doesn't exists !")
  ENDIF()

  # Make sure the variable is cleared
  SET(file_to_wrap )
  SET(wrap_list )
  set(WRAP_LIST_FOUND )

  IF(EXISTS ${filepath})
    # Let's make sure target_wrap_files contains only strings
    FILE(STRINGS "${filepath}" stringtocheck) # read content of 'filepath' into 'stringtocheck'
    STRING(REGEX MATCHALL "[^\\#]\\$\\{.*\\}" incorrect_elements ${stringtocheck})
    FOREACH(incorrect_element ${incorrect_elements})
      STRING(REGEX REPLACE "\\$|\\{|\\}" "" correct_element ${incorrect_element})
      MESSAGE(FATAL_ERROR "In ${filepath}, ${incorrect_element} should be replaced by ${correct_element}")
    ENDFOREACH()

    INCLUDE(${filepath})

    # Loop over all target files, let's resolve the variable to access its content
    FOREACH(wfile ${file_to_wrap})
      LIST(APPEND wrap_list ${CMAKE_CURRENT_SOURCE_DIR}/${wfile})
    ENDFOREACH()
  ENDIF()
  
  # Wrapping step
  set(${PROJECT_NAME}_SOURCES_WRAP)

  list(LENGTH wrap_list length)
  
  if(SWIG_FOUND AND ${length})
    set(WRAP_LIST_FOUND 1)
    set(${PROJECT_NAME}_WRAP_DEPENDS ${wrap_list})
   
    SET(i_filepath ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i)
    IF(EXISTS ${i_filepath})
      if(PYTHONLIBS_FOUND)
        mafMacroWrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP ${PROJECT_NAME} python ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i ${${PROJECT_NAME}_WRAP_DEPENDS})
      endif(PYTHONLIBS_FOUND)
  
      if(TCL_FOUND)
        mafMacroWrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP ${PROJECT_NAME} tcl    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i ${${PROJECT_NAME}_WRAP_DEPENDS})
      endif(TCL_FOUND)
    ENDIF()
    
  endif(SWIG_FOUND AND ${length})

ENDMACRO()

MACRO(mafMacroWrapBuild)

  if(TCL_FOUND)
    target_link_libraries(${PROJECT_NAME} ${TCL_LIBRARY})
  endif(TCL_FOUND)
	
  if(PYTHONLIBS_FOUND)
    target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
  endif(PYTHONLIBS_FOUND)

  ## #################################################################
  ## Handling of generated script modules
  ## #################################################################

  set(${PROJECT_NAME}_MODULES)

  if(SWIG_FOUND AND PYTHONLIBS_FOUND)
    if(WIN32)
      set(lib_ext ".dll")
      set(lib_prefix )
      set(wrap_lib_ext ".pyd")
    else(WIN32)
      set(lib_prefix "lib")
      set(wrap_lib_ext ".so")
      if(APPLE)
        set(lib_ext ".dylib")
      else(APPLE)
        set(lib_ext ".so")
      endif(APPLE)
    endif(WIN32)

    set(lib_name ${lib_prefix}${PROJECT_NAME}${lib_ext})
	set(wrap_lib_prefix "_")
    set(wrap_lib_name ${wrap_lib_prefix}${PROJECT_NAME}${wrap_lib_ext})
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i
      COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/modules
      COMMAND ${CMAKE_COMMAND} ARGS -E  copy ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}.py ${CMAKE_BINARY_DIR}/modules
      COMMAND ${CMAKE_COMMAND} ARGS -E  copy ${LIBRARY_OUTPUT_PATH}${lib_name} ${CMAKE_BINARY_DIR}/modules/${wrap_lib_name}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

    set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/modules/${PROJECT_NAME}.py)

  endif(SWIG_FOUND AND PYTHONLIBS_FOUND)

ENDMACRO()
