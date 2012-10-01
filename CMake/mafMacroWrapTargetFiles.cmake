#
#  mafMacroWrapTargetFiles.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroWrapTargetFiles) 

IF(BUILD_WRAP)
	find_package(PythonInterp)
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MAF_ROOT_BINARY_DIR}/wrap/mafPythonQtDecorators/${PROJECT_NAME})
	execute_process(COMMAND ${PYTHON_EXECUTABLE} ${MAF_ROOT_BINARY_DIR}/wrap/pyMAF/mafPythonWrapper.py ${MAF_SOURCE_DIR}/src/${PROJECT_NAME}/target_wrap_files ${MAF_ROOT_BINARY_DIR}/doc/DocOutput/xml ${MAF_ROOT_BINARY_DIR}/wrap/mafPythonQtDecorators/${PROJECT_NAME})
	ENDIF(BUILD_WRAP)
ENDMACRO()

MACRO(mafMacroWrapBuild)
  
  
ENDMACRO()
