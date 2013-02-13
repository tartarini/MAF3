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
  configure_file(ProjectDoxyfile.in     ${QA_BINARY_DIR}/doc_output/ProjectDoxyfile IMMEDIATE @ONLY)
  configure_file(ProjectDoxyfileWithTests.in     ${QA_BINARY_DIR}/doc_output/ProjectDoxyfileWithTests IMMEDIATE @ONLY)
  configure_file(ProjectPythonDoxyfile.in     ${QA_BINARY_DIR}/doc_output/ProjectPythonDoxyfile IMMEDIATE @ONLY)


execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${CMAKE_COMMAND} -E remove_directory DocOutput
)

execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${CMAKE_COMMAND} -E remove_directory DocWithTestsOutput
)

execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${CMAKE_COMMAND} -E remove_directory PythonDocOutput
)

execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${DOXYGEN_EXECUTABLE} ProjectDoxyfile
)

execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${DOXYGEN_EXECUTABLE} ProjectDoxyfileWithTests
)

execute_process(
  WORKING_DIRECTORY ${QA_BINARY_DIR}/doc_output
  COMMAND ${DOXYGEN_EXECUTABLE} ProjectPythonDoxyfile
)


else (DOXYGEN_FOUND)
  message(INFO "Install doxygen for creating input for QA.")
endif (DOXYGEN_FOUND)
