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
MACRO(mafCopySharedLibrariesInInstallDir vardir)
#copy all the libraries inside Install/bin

SET(result)
SET(erro)
FILE(GLOB file_list ${vardir}/*.*)

foreach(file ${file_list})
get_filename_component(fileName ${file} NAME)

set(result)
set(erro)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.i
        COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/modules
        COMMAND ${CMAKE_COMMAND} ARGS -E  copy ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}.py ${CMAKE_BINARY_DIR}/modules
        COMMAND ${CMAKE_COMMAND} ARGS -E  copy ${SHARED_LIB_COPY_SOURCE} ${CMAKE_BINARY_DIR}/modules/${wrap_lib_name}
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

EXECUTE_PROCESS(
   COMMAND ${CMAKE_COMMAND} -E copy
   ${file}
   ${CMAKE_BINARY_DIR}/Install/bin/${CMAKE_CFG_INTDIR}/${fileName}
   RESULT_VARIABLE result
   ERROR_VARIABLE erro)
endforeach(file ${filelist})
ENDMACRO()