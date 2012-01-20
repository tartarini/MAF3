#
#  mafCopyFilesFromExtension.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#
MACRO(mafCopyFilesFromExtension extension project)
if(${BUILD_SHARED_LIBS})
set(TARGET_LOC)
set(DEST_ABSOLUTE_DIR)
get_target_property(TARGET_LOC ${project} LOCATION)
get_filename_component(DEST_ABSOLUTE_DIR ${TARGET_LOC} PATH)

FILE(GLOB tocopy_file_list "${PROJECT_SOURCE_DIR}/*.${extension}")

foreach(file ${tocopy_file_list})
  get_filename_component(fileName ${file} NAME)
  add_custom_command(TARGET ${project} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${file} ${DEST_ABSOLUTE_DIR}/${fileName} )
endforeach(file ${tocopy_file_list})
endif(${BUILD_SHARED_LIBS})
ENDMACRO()
