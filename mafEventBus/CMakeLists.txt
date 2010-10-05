#
# Copyright and description to be placed here
#

# Extract current directory name to use as project name
file(GLOB CUR_FILE "CMakeLists.txt")
get_filename_component(CUR_ABSOLUTE_DIR ${CUR_FILE} PATH)
get_filename_component(DIR_NAME ${CUR_ABSOLUTE_DIR} NAME)
PROJECT(${DIR_NAME})

FIND_PACKAGE(QtSOAP)
IF(NOT QtSOAP_FOUND)
  MESSAGE(FATAL_ERROR "error: QtSOAP package is required to build ${PROJECT_NAME}" )
ENDIF()


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

SET(PROJECT_LIBS ${CTK_BASE_LIBRARIES} mafCore )

# Create the library.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${MY_MOC_CXX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
