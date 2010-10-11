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

#############################################################################################
### find project subdirs avoiding the svn directory and make the SUBDIR CMake command on them.
#############################################################################################
FILE(GLOB dir_list "${PROJECT_SOURCE_DIR}/*")
FOREACH(dir_name ${dir_list})
  FILE(RELATIVE_PATH local_dir_name "${PROJECT_SOURCE_DIR}" ${dir_name})
	IF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".svn")
    INCLUDE_DIRECTORIES("${dir_name}")
		SUBDIRS("${local_dir_name}")
	ENDIF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".svn")
ENDFOREACH(dir_name ${dir_list})
