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
FILE(GLOB file_list_debug ${vardir}/Debug/*.*)
FILE(GLOB file_list_release ${vardir}/Release/*.*)

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
