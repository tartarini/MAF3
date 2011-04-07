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

set(lib_ext "*")
if(WIN32)
  set(lib_ext "dll")
else(WIN32)
   if(APPLE)
     set(lib_ext "dylib")
   else(APPLE)
     set(lib_ext "so")
   endif(APPLE)
endif(WIN32)

FILE(GLOB file_list ${vardir}/*.${lib_ext})
FILE(GLOB file_list_debug ${vardir}/Debug/*.${lib_ext})
FILE(GLOB file_list_release ${vardir}/Release/*.${lib_ext})

#if dirs don't exist, create them
add_custom_command(TARGET ${${proj}} POST_BUILD
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/Debug
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/Release
          )


foreach(file ${file_list})
  add_custom_command(TARGET ${${proj}} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${file} ${CMAKE_BINARY_DIR}/build/bin/${fileName} )
endforeach(file ${file_list})

foreach(file_debug ${file_list_debug})
  add_custom_command(TARGET ${${proj}} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${file_debug} ${CMAKE_BINARY_DIR}/build/bin/Debug/${fileName} )
endforeach(file_debug ${file_list_debug})


foreach(file_release ${file_list_release})
  add_custom_command(TARGET ${${proj}} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${file_release} ${CMAKE_BINARY_DIR}/build/bin/Release/${fileName} )
endforeach(file_release ${file_list_release})


ENDMACRO()
