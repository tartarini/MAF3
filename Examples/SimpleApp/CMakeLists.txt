#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
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

# List libraries that are needed by this project.
SET(PROJECT_LIBS ${CTK_BASE_LIBRARIES})
# Create the library.
add_executable(${PROJECT_NAME} ${PROJECT_SRCS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
