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
mafMacroGetTargetLibraries(dependency_libraries)
SET(PROJECT_LIBS ${dependency_libraries})

## #################################################################
## Wrapping
## #################################################################

mafMacroWrapTargetFiles()

## #################################################################
## Build rules
## #################################################################

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${MY_MOC_CXX} ${${PROJECT_NAME}_SOURCES_WRAP})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## Wrapping Build rules
## #################################################################

mafMacroWrapBuild()


## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)



