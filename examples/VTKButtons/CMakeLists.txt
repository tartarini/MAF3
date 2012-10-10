#
#  CMakeLists.txt
#  MSVTK
#
#  Created by Paolo Quadrani on 24/10/11.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


## #################################################################
## Find the external packages.
## #################################################################

include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_OpenCV.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_VTK.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_MSVTK.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_fervor.cmake)

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject(0)

## #################################################################
## Build rules
## #################################################################

# Set some Win32 Specific Settings
IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

# Create the executable.
ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

## #################################################################
## copy MSVTK libraries
## #################################################################

mafCopySharedLibrariesInInstallDir("${MSVTK_DIR}/MSVTK-build/bin")

## #################################################################
## copy ui files
## #################################################################

mafCopyFilesFromExtension("ui" ${PROJECT_NAME})
mafCopyFilesFromExtension("xml" ${PROJECT_NAME})

## #################################################################
## Installation
## #################################################################

mafMacroInstallProject(1)

INSTALL(TARGETS ${PROJECT_NAME}
  DESTINATION ${PROJECT_NAME}/bin
  RUNTIME DESTINATION ${PROJECT_NAME}/bin COMPONENT RuntimeLibraries
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  #Copy shared library
  FILE(GLOB SOLIBS "${MAF_BINARY_DIR}/bin/*.so")

  string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UP)
  
  #Need install also qt libraries
  list(APPEND qtlibs ${QT_QTCORE_LIBRARY_${CMAKE_BUILD_TYPE_UP}})
  list(APPEND qtlibs ${QT_QTGUI_LIBRARY_${CMAKE_BUILD_TYPE_UP}})
  list(APPEND qtlibs ${QT_QTSQL_LIBRARY_${CMAKE_BUILD_TYPE_UP}})
  list(APPEND qtlibs ${QT_QTXML_LIBRARY_${CMAKE_BUILD_TYPE_UP}})
  list(APPEND qtlibs ${QT_QTWEBKIT_LIBRARY_${CMAKE_BUILD_TYPE_UP}})
  list(APPEND qtlibs ${QT_QTOPENGL_LIBRARY_${CMAKE_BUILD_TYPE_UP}})

  FOREACH(qtlib ${qtlibs})
    get_filename_component(r_qtlib ${qtlib} REALPATH)
    set(SOLIBS ${SOLIBS};${r_qtlib})
  ENDFOREACH(qtlib ${qtlibs})

  # before installing library inside the package rename it has in its soname
  FOREACH(SOLIB ${SOLIBS})
    message(STATUS ">>>> ${SOLIB}")
    execute_process(COMMAND objdump ${SOLIB} -p COMMAND grep SONAME
            OUTPUT_VARIABLE OUT_SONAME
    )
    # adapt output string
    string(REPLACE "SONAME" " " OUT_SONAME ${OUT_SONAME})
    string(STRIP ${OUT_SONAME} OUT_SONAME)
    install(FILES ${SOLIB} DESTINATION ${PROJECT_NAME}/bin NAMELINK_SKIP RENAME ${OUT_SONAME})
  ENDFOREACH(SOLIB in ${SHARED_LIBS})

  #Copy ui files
  FILE(GLOB UI_FILES "${MAF_BINARY_DIR}/bin/*.ui")
  INSTALL(FILES ${UI_FILES} DESTINATION ${PROJECT_NAME}/bin)

  #Copy MNU files
  FILE(GLOB MNU_FILES "${MAF_BINARY_DIR}/bin/*.ui")
  INSTALL(FILES ${MNU_FILES} DESTINATION ${PROJECT_NAME}/bin)

  #Copy Plugin directory
  INSTALL(DIRECTORY "${MAF_BINARY_DIR}/bin/plugins" DESTINATION ${PROJECT_NAME}/bin)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include(../../CMake/mafCPack.cmake)
