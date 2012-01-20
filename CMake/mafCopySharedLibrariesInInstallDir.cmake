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

if(lib_ext STREQUAL "*")
else(lib_ext STREQUAL "*")

FILE(GLOB file_list ${vardir}/*.${lib_ext})
FILE(GLOB file_list_debug ${vardir}/Debug/*.${lib_ext})
FILE(GLOB file_list_release ${vardir}/Release/*.${lib_ext})

#if dirs don't exist, create them
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/Debug
          COMMAND ${CMAKE_COMMAND} ARGS -E  make_directory ${CMAKE_BINARY_DIR}/build/bin/Release
          )

set(fileName)

foreach(filelib ${file_list})
  get_filename_component(fileName ${filelib} NAME)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${filelib} ${CMAKE_BINARY_DIR}/bin/${fileName} )
endforeach(filelib ${file_list})

foreach(file_debug ${file_list_debug})
  get_filename_component(fileName ${file_debug} NAME)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${file_debug} ${CMAKE_BINARY_DIR}/bin/Debug/${fileName} )
endforeach(file_debug ${file_list_debug})


foreach(file_release ${file_list_release})
  get_filename_component(fileName ${file_release} NAME)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${file_release} ${CMAKE_BINARY_DIR}/bin/Release/${fileName} )
endforeach(file_release ${file_list_release})

endif(lib_ext STREQUAL "*")
ENDMACRO()
