# Find PythonQt
#
# Sets PYTHONQT_FOUND, PYTHONQT_INCLUDE_DIR, PYTHONQT_LIBRARY, PYTHONQT_LIBRARIES
#

SET(PYTHONQT_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/PythonQt")
SET(PYTHONQT_SOURCE_DIR "${MAF_EXTERNAL_BUILD_DIR}/Source/PythonQt")
# Python is required
#FIND_PACKAGE(PythonLibs)
#FIND_PACKAGE(PythonInterp)
#IF(NOT PYTHONLIBS_FOUND)
#  MESSAGE(FATAL_ERROR "error: Python is required to build ${PROJECT_NAME}")
#ENDIF()

#python inclusion files
include_directories("${PYTHON_INCLUDE_DIR}")


include_directories("${PYTHONQT_SOURCE_DIR}/src")
link_directories(${PYTHONQT_DIR})