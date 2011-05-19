#
#  CMakeLists.txt
#  MAF
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## find doxygen
## #################################################################

set(DOXYGEN_FIND_QUIETLY TRUE)

find_package(Doxygen)

## #################################################################
## Apply doxygen
## #################################################################

if (DOXYGEN_FOUND)
  configure_file(MAF3Doxyfile.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3Doxyfile IMMEDIATE @ONLY)
  configure_file(MAF3DoxyfileWithTests.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3DoxyfileWithTests IMMEDIATE @ONLY)
  configure_file(MAF3PythonDoxyfile.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3PythonDoxyfile IMMEDIATE @ONLY)

add_custom_target(doc
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mafDoxygenFooter.html ${CMAKE_CURRENT_BINARY_DIR}/mafDoxygenFooter.html
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/DocOutput
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/DocWithTestsOutput
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/PythonDocOutput
  COMMAND ${DOXYGEN_EXECUTABLE} MAF3Doxyfile
  COMMAND ${DOXYGEN_EXECUTABLE} MAF3DoxyfileWithTests
  COMMAND ${DOXYGEN_EXECUTABLE} MAF3PythonDoxyfile
)

else (DOXYGEN_FOUND)
  message(INFO "Install doxygen for creating documentation.")
endif (DOXYGEN_FOUND)