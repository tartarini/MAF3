#
#  mafMacroInitProject.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

################### PUBLIC INTERFACE
MACRO(mafMacroInitProject type)
  
  if( ${type} STREQUAL "application" )
      InitializeApplication()
  elseif ( ${type} STREQUAL "library" )
      InitializeLibrary()
  elseif( ${type} STREQUAL "test")
      InitializeTest()
  elseif( ${type} STREQUAL "plugin")
      InitializePlugin()
  else() 
      message(FATAL_ERROR "${type} not recognized. Select between application, library, plugin, test")
  endif()

ENDMACRO()

################### PRIVATE IMPLEMENTATION (don't call from outside)
MACRO(filterForMoc outputList inputList)
set(${outputList})

foreach(file ${inputList})
  file(READ ${file} _contents)
  
  string(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
  if(_match)
    LIST(APPEND ${outputList} ${file})
  endif(_match)
    
endforeach(file ${inputList})

ENDMACRO()


MACRO(InternalPreInitialize)
  # Extract current directory name to use as project name

  file(GLOB CUR_FILE "CMakeLists.txt")

  get_filename_component(CUR_ABSOLUTE_DIR ${CUR_FILE} PATH)

  get_filename_component(DIR_NAME ${CUR_ABSOLUTE_DIR} NAME)

  PROJECT(${DIR_NAME})

  FILE(GLOB input_include_file_list "${PROJECT_SOURCE_DIR}/*.h")
  filterForMoc(include_file_list "${input_include_file_list}")

  FILE(GLOB implementation_file_list "${PROJECT_SOURCE_DIR}/*.cpp")
  FILE(GLOB implementation_file_list_vtkMAF "${PROJECT_SOURCE_DIR}/*.cxx")

  FILE(GLOB templete_file_list1 "${PROJECT_SOURCE_DIR}/*.txx")
  FILE(GLOB templete_file_list2 "${PROJECT_SOURCE_DIR}/*.tpp")
  FILE(GLOB resource_file_list "${PROJECT_SOURCE_DIR}/*.qrc")
  FILE(GLOB ui_file_list "${PROJECT_SOURCE_DIR}/*.ui")

  set(decorator_file_list)
  
  if(BUILD_WRAP)
    mafMacroWrapTargetFiles()
    FILE(GLOB decorator_file_list "${MAF_ROOT_BINARY_DIR}/wrap/mafPythonQtDecorators/${PROJECT_NAME}/*.cpp")
    FILE(GLOB decorator_file_header_list "${MAF_ROOT_BINARY_DIR}/wrap/mafPythonQtDecorators/${PROJECT_NAME}/*.h")
    set(implementation_file_list ${implementation_file_list} ${decorator_file_list})
    set(include_file_list ${include_file_list} ${decorator_file_header_list})
  endif(BUILD_WRAP)

   
  SET(PROJECT_SRCS
  ${implementation_file_list}
  ${implementation_file_list_vtkMAF}

  ${input_include_file_list}
  ${templete_file_list1}

  ${templete_file_list2}
  ${ui_file_list}
    ${uis_hdrs}
  ${resource_file_list}

  )
  
  IF(WIN32)
    ADD_DEFINITIONS(-DUNICODE)
  ELSE(WIN32)
    ADD_DEFINITIONS(-w)
  ENDIF(WIN32)

  #ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
  
  ## Add the project binary dir as include dir for the .moc files.
  INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
ENDMACRO()

MACRO(InternalInitialize)
  if(BUILD_QA)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
      
      find_program( CODECOV_GCOV gcov )
      add_definitions( -fprofile-arcs -ftest-coverage )
      link_libraries( gcov )
      set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} " )
      set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} " )
      
    ELSE(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
            message("gcov not present.")
    ENDIF(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    endif(BUILD_QA)
ENDMACRO()

MACRO(InternalPostInitialize)
  SET(PROJECT_SRCS 
    ${PROJECT_SRCS}
    )
  if(UNIX)
    if(BUILD_QA AND valgrind_FOUND)
      add_definitions( -g -O0 )
    set(valgrind_ENABLE 1)
    endif(BUILD_QA AND valgrind_FOUND)
  endif(UNIX)

  # configure files
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in")
      message(STATUS "Creating FROM ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in TO ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.h")
      CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.h)
      INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
  endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in")
  
  #here put if need for include in all projects
  INCLUDE_DIRECTORIES("${MAF_INTERNAL_BUILD_DIR}/src/mafCore")
  
  # List libraries that are needed by this project.
  mafMacroGetTargetLibraries(dependency_libraries)
  
  SET(PROJECT_LIBS ${dependency_libraries})
ENDMACRO()

MACRO(InitializeApplication)
  InternalPreInitialize()
  InternalInitialize()
  InternalPostInitialize()
ENDMACRO()

MACRO(InitializeLibrary)
  InternalPreInitialize()
  InternalInitialize()
  InternalPostInitialize()
ENDMACRO()

MACRO(InitializeTest)
  InternalPreInitialize()
  #InternalInitialize()
  InternalPostInitialize()
ENDMACRO()

MACRO(InitializePlugin)
  InternalPreInitialize()
  InternalInitialize()
  InternalPostInitialize()
ENDMACRO()



