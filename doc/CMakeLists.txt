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
INCLUDE(ExternalProject)

if (DOXYGEN_FOUND)
  configure_file(MAF3Doxyfile.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3Doxyfile IMMEDIATE @ONLY)
  configure_file(MAF3DoxyfileWithTests.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3DoxyfileWithTests IMMEDIATE @ONLY)
  configure_file(MAF3PythonDoxyfile.in     ${CMAKE_CURRENT_BINARY_DIR}/MAF3PythonDoxyfile IMMEDIATE @ONLY)

ExternalProject_Add(doc
      DOWNLOAD_COMMAND ""
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
)

ExternalProject_Add_Step(doc create_xml
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_COMMAND} -E copy ${MAF_ROOT_DIR}/doc/mafDoxygenFooter.html ${MAF_ROOT_BINARY_DIR}/doc/mafDoxygenFooter.html
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${MAF_ROOT_BINARY_DIR}/doc/DocOutput
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${MAF_ROOT_BINARY_DIR}/doc/DocWithTestsOutput
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${MAF_ROOT_BINARY_DIR}/doc/PythonDocOutput
      COMMAND ${DOXYGEN_EXECUTABLE} MAF3Doxyfile
      COMMAND ${DOXYGEN_EXECUTABLE} MAF3DoxyfileWithTests
      COMMAND ${DOXYGEN_EXECUTABLE} MAF3PythonDoxyfile
      )

else (DOXYGEN_FOUND)
  message(INFO "Install doxygen for creating documentation.")
endif (DOXYGEN_FOUND)