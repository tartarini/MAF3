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
MACRO(mafCopyFilesFromExtension extension project_name)
if(${BUILD_SHARED_LIBS})
set(TARGET_LOC)
set(DEST_ABSOLUTE_DIR)
set(R_DEST_ABSOLUTE_DIR)
set(D_DEST_ABSOLUTE_DIR)

get_target_property(TARGET_LOC ${project_name} LOCATION)
get_filename_component(DEST_ABSOLUTE_DIR ${TARGET_LOC} PATH)

if( NOT ${DEST_ABSOLUTE_DIR} STREQUAL "" )
    string(REPLACE "$(Configuration)" "Release" R_DEST_ABSOLUTE_DIR ${DEST_ABSOLUTE_DIR} )
    string(REPLACE "$(Configuration)" "Debug" D_DEST_ABSOLUTE_DIR ${DEST_ABSOLUTE_DIR} )
endif( NOT ${DEST_ABSOLUTE_DIR} STREQUAL "" )

FILE(GLOB tocopy_file_list "${PROJECT_SOURCE_DIR}/*.${extension}")

foreach(file ${tocopy_file_list})
  get_filename_component(fileName ${file} NAME)
  if( NOT ${R_DEST_ABSOLUTE_DIR} STREQUAL "" AND NOT ${D_DEST_ABSOLUTE_DIR} STREQUAL "" )
      configure_file( ${file} ${R_DEST_ABSOLUTE_DIR}/${fileName} )
      if( NOT ${D_DEST_ABSOLUTE_DIR} STREQUAL ${R_DEST_ABSOLUTE_DIR} )
        #configure_file( ${file} ${D_DEST_ABSOLUTE_DIR}/${fileName} )
      endif( NOT ${D_DEST_ABSOLUTE_DIR} STREQUAL ${R_DEST_ABSOLUTE_DIR} )
  endif( NOT ${R_DEST_ABSOLUTE_DIR} STREQUAL "" AND NOT ${D_DEST_ABSOLUTE_DIR} STREQUAL "" )
endforeach(file ${tocopy_file_list})
endif(${BUILD_SHARED_LIBS})
ENDMACRO()
