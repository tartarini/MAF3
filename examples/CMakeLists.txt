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


#############################################################################################
### find project subdirs avoiding the svn directory and make the SUBDIR CMake command on them.
#############################################################################################

include(${MAF_USE_FILE_CONFIG})

if(BUILD_EXAMPLES)

  file(GLOB CUR_FILE "CMakeLists.txt")
  get_filename_component(CUR_ABSOLUTE_DIR ${CUR_FILE} PATH)  

  FILE(GLOB dir_list "${CUR_ABSOLUTE_DIR}/*")
  
  FOREACH(dir_name ${dir_list})
    FILE(RELATIVE_PATH local_dir_name "${CUR_ABSOLUTE_DIR}" ${dir_name})
	  IF(IS_DIRECTORY ${dir_name} AND NOT ${dir_name} STREQUAL ".git")
        INCLUDE_DIRECTORIES("${dir_name}")
		SUBDIRS("${local_dir_name}")
	  ENDIF(IS_DIRECTORY ${dir_name} AND NOT ${dir_name} STREQUAL ".git")
  ENDFOREACH(dir_name ${dir_list})

endif(BUILD_EXAMPLES)
