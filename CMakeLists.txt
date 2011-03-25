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

cmake_minimum_required(VERSION 2.8)

PROJECT(MAF)
set(MafFoundationDir $ENV{MAF3_FOUNDATION_LIB_DIR})
STRING(REPLACE "\\" "/" MafFoundationLibDir ${MafFoundationDir})

#############################################################################################
### MAF3 Options
#############################################################################################

OPTION(BUILD_TEST_SUITE "Build Module's Tests" ON)
OPTION(BUILD_EXAMPLES "Build Examples" ON)
OPTION(MAF_USE_GIT_PROTOCOL "If ON use git protocol, otherwise http" OFF)
OPTION(MAF_SUPERBUILD "Build MAF and the projects it depends on via SuperBuild.cmake." ON)
MARK_AS_ADVANCED(MAF_SUPERBUILD)


set(git_protocol "git")
if(NOT MAF_USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()

# modify CMAKE_BUILD_TYPE in order to set default configuration for non-IDE build
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles") 
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles")

#############################################################################################
### Available Modules
#############################################################################################




SET(MAF_CORE ON)
SET(MAF_EVENT_BUS ON)
OPTION(MAF_TIME_SPACE "Build mafTimeSpace module." ON)
OPTION(MAF_SERIALIZATION "Build mafSerialization module." ON)
OPTION(MAF_RESOURCES "Build mafResources module." ON)
OPTION(MAF_PLUGINVTK "Build mafPluginVTK module." ON)
OPTION(MAF_APPLICATION_LOGIC "Build mafApplicationLogic module." ON)
OPTION(MAF_QTGUI "Build mafGUI module." ON)

include(CMake/mafUtilities.cmake)
if(MAF_PLUGINVTK)
  SET( MAF_RESOURCES ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINVTK ON)
endif(MAF_PLUGINVTK)


#############################################################################################
### Superbuild execution
#############################################################################################

INCLUDE(${MAF_SOURCE_DIR}/GenerateMAFConfig.cmake)
CONFIGURE_FILE(${MAF_SOURCE_DIR}/UseMAF.cmake.in
  ${MAF_BINARY_DIR}/UseMAF.cmake COPYONLY IMMEDIATE)
  
IF(MAF_SUPERBUILD)
  INCLUDE(SuperBuild.cmake)
  RETURN()
ELSE(MAF_SUPERBUILD)
ENDIF(MAF_SUPERBUILD)

#############################################################################################
### SOURCES
#############################################################################################

ADD_SUBDIRECTORY(src)

#############################################################################################
### TESTS
#############################################################################################

#ADD_SUBDIRECTORY(test)


