#
# Copyright and description to be placed here
#

# Extract current directory name to use as project name
file(GLOB CUR_FILE "CMakeLists.txt")
get_filename_component(CUR_ABSOLUTE_DIR ${CUR_FILE} PATH)
get_filename_component(DIR_NAME ${CUR_ABSOLUTE_DIR} NAME)
PROJECT(${DIR_NAME})

FILE(GLOB include_file_list "${PROJECT_SOURCE_DIR}/*.h")
FILE(GLOB implementation_file_list "${PROJECT_SOURCE_DIR}/*.cpp")
FILE(GLOB templete_file_list1 "${PROJECT_SOURCE_DIR}/*.txx")
FILE(GLOB templete_file_list2 "${PROJECT_SOURCE_DIR}/*.tpp")
# Set your list of sources here.
SET(PROJECT_SRCS
	${implementation_file_list}
	${include_file_list}
	${templete_file_list1}
	${templete_file_list2}
)

QT4_WRAP_CPP(MY_MOC_CXX ${include_file_list})

# List libraries that are needed by this project.
SET(PROJECT_LIBS ${CTK_BASE_LIBRARIES})

## #################################################################
## Wrapping
## #################################################################

set(${PROJECT_NAME}_SOURCES_WRAP)

if(SWIG_FOUND)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mafObjectBase.h)
  
  if(PYTHONLIBS_FOUND)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP core python ${CMAKE_CURRENT_SOURCE_DIR}/mafCore.i ${${PROJECT_NAME}_WRAP_DEPENDS})
  endif(PYTHONLIBS_FOUND)
  
  if(TCL_FOUND)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP core tcl    ${CMAKE_CURRENT_SOURCE_DIR}/mafCore.i ${${PROJECT_NAME}_WRAP_DEPENDS})
  endif(TCL_FOUND)
  
endif(SWIG_FOUND)

## #################################################################
## Build rules
## #################################################################

##in
# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${MY_MOC_CXX} ${${PROJECT_NAME}_SOURCES_WRAP})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
##in

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mafCore.i
  COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/modules
  COMMAND ${CMAKE_COMMAND} ARGS -E  copy ${${PROJECT_NAME}_BINARY_DIR}/core.py ${CMAKE_BINARY_DIR}/modules
  COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/modules/core.py)

endif(SWIG_FOUND AND PYTHONLIBS_FOUND)

## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)



