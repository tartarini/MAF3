#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(MAF)
set(MafFoundationDir $ENV{MAF3_FOUNDATION_LIB_DIR})
STRING(REPLACE "\\" "/" MafFoundationLibDir ${MafFoundationDir})

#############################################################################################
### MAF3 Options
#############################################################################################

OPTION(BUILD_TEST_SUITE "Build Module's Tests" ON)
OPTION(BUILD_EXAMPLES "Build Examples" ON)
OPTION(MAF_USE_GIT_PROTOCOL "If ON use git protocol, otherwise http" OFF)

set(git_protocol "git")
if(NOT MAF_USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()

# modify CMAKE_BUILD_TYPE in order to set default configuration for non-IDE build
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles") 
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles")

#############################################################################################
### where to search modules
#############################################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

include(mafMacroInitProject)
include(mafMacroInstallProject)

include(mafMacroSetupQt)
include(mafMacroWrapTargetFiles)
include(mafMacroTargetLibraries)
include(mafCopyFilesFromExtension)

#-----------------------------------------------------------------------------
# MAF3 Foundation Libraries.
#
include(mafMacroFoundationLibraries)
mafMacroFoundationLibraries()

#-----------------------------------------------------------------------------
# QT
#

mafMacroSetupQt()
# Update MAF_BASE_LIBRARIES with QT libraries
IF(QT4_FOUND)
  SET(MAF_BASE_LIBRARIES ${QT_LIBRARIES} CACHE INTERNAL "MAF base libraries" FORCE)
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
	IF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".git" AND NOT ${local_dir_name} STREQUAL "CMake")
    INCLUDE_DIRECTORIES("${dir_name}")
		SUBDIRS("${local_dir_name}")
	ENDIF(IS_DIRECTORY ${dir_name} AND NOT ${local_dir_name} STREQUAL ".git" AND NOT ${local_dir_name} STREQUAL "CMake")
ENDFOREACH(dir_name ${dir_list})

#############################################################################################
### Export Build settings and library dependences
#############################################################################################
INCLUDE(CMakeExportBuildSettings)
