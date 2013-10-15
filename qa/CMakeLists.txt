#execute_process(COMMAND ${GIT_EXECUTABLE} clone -b refactorExternalTools "git://github.com/dgiunchi/QAT.git"
execute_process(COMMAND ${GIT_EXECUTABLE} clone -b refactorExternalTools "git://github.com/INSIGNEO/QAT.git"
			  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
			  RESULT_VARIABLE RESULT)

SET(SOURCE_DIR ${MAF_SOURCE_DIR})
SET(BINARY_DIR ${MAF_BINARY_DIR})
SET(QA_SOURCE_DIR "${MAF_SOURCE_DIR}/qa")

configure_file("${CMAKE_CURRENT_BINARY_DIR}/QAT/qa/QATConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/QAT/qa/QATConfig.cmake" @ONLY)

subdirs("${CMAKE_CURRENT_BINARY_DIR}/QAT")
