#
#  mafCopySharedLibrariesInInstallDir.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#
MACRO(mafCopySharedLibrariesInInstallDir vardir proj)
#copy all the libraries inside Install/bin
FILE(GLOB file_list ${vardir}/*.*)
#get_target_property(TARGET_LOC ${PROJECT_NAME} LOCATION)
#get_filename_component(TARGET_ABSOLUTE_DIR ${TARGET_LOC} PATH)
foreach(file ${file_list})
get_filename_component(fileName ${file} NAME)

add_custom_command(TARGET ${${proj}} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${file} ${CMAKE_BINARY_DIR}/build/bin/${fileName} )
endforeach(file ${filelist})
ENDMACRO()
