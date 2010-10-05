CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(MAF)

#############################################################################################
### where to search modules
#############################################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

include(mafDependencies)
include(mafMacroSetupQt)

#foundation libraries
include(FindQtSOAP)



#-----------------------------------------------------------------------------
# QT
#

mafMacroSetupQt()
# Update CTK_BASE_LIBRARIES with QT libraries
IF(QT4_FOUND)
  SET(CTK_BASE_LIBRARIES ${CTK_BASE_LIBRARIES} ${QT_LIBRARIES} CACHE INTERNAL "MAF base libraries" FORCE)
ENDIF()

#############################################################################################
### where to place output binaries
#############################################################################################
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Install/bin/ CACHE INTERNAL
     "Single output directory for building all libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Install/bin/ CACHE INTERNAL 
     "Single output directory for building all executables.")

MARK_AS_ADVANCED (
  LIBRARY_OUTPUT_PATH 
  EXECUTABLE_OUTPUT_PATH
)

INCLUDE_DIRECTORIES(BEFORE ".")

#############################################################################################
### find project subdirs avoiding the svn directory and make the SUBDIR CMake command on them.
#############################################################################################
FILE(GLOB dir_list "${PROJECT_SOURCE_DIR}/*")
FOREACH(dir_name ${dir_list})
  FILE(RELATIVE_PATH local_dir_name "${PROJECT_SOURCE_DIR}" ${dir_name})
	IF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".git")
    INCLUDE_DIRECTORIES("${dir_name}")
		SUBDIRS("${local_dir_name}")
	ENDIF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".git")
ENDFOREACH(dir_name ${dir_list})

#############################################################################################
### Export Build settings and library dependences
#############################################################################################
INCLUDE(CMakeExportBuildSettings)
