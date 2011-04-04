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

#############################################################################################
### MAF3 Options
#############################################################################################

OPTION(BUILD_TEST_SUITE "Build Module's Tests" OFF)
OPTION(BUILD_EXAMPLES "Build Examples" ON)
OPTION(MAF_USE_GIT_PROTOCOL "If ON use git protocol, otherwise http" OFF)
OPTION(MAF_SUPERBUILD "Build MAF and the projects it depends on via SuperBuild.cmake." ON)
MARK_AS_ADVANCED(MAF_SUPERBUILD)

FIND_PACKAGE(Git)
IF(NOT GIT_FOUND)
  MESSAGE(FATAL_ERROR "error: Install Git and try to re-configure")
ENDIF()

# git protocol to be used
set(git_protocol "git")
if(NOT MAF_USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()


#############################################################################################
### Compilers
#############################################################################################

# modify CMAKE_BUILD_TYPE in order to set default configuration for non-IDE build
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles") 
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles")

#############################################################################################
### Available Modules
#############################################################################################

SET(MAF_CORE ON)
SET(MAF_EVENT_BUS ON)
SET(MAF_QA ON)
OPTION(MAF_TIME_SPACE "Build mafTimeSpace module. mafCore and mafEventBus will be built anyway." ON)
OPTION(MAF_SERIALIZATION "Build mafSerialization module. mafCore and mafEventBus will be built anyway." ON)
OPTION(MAF_RESOURCES "Build mafResources module. mafCore and mafEventBus will be built anyway." ON)
OPTION(MAF_PLUGINVTK "Build mafPluginVTK module. mafCore and mafEventBus will be built anyway." ON)
OPTION(MAF_APPLICATION_LOGIC "Build mafApplicationLogic module.mafCore and mafEventBus will be built anyway." ON)
OPTION(MAF_QTGUI "Build mafGUI module. mafCore and mafEventBus will be built anyway." ON)

### set modules interdependencies
include(CMake/mafUtilities.cmake)
if(MAF_PLUGINVTK)
  SET( MAF_RESOURCES ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINVTK ON)
endif(MAF_PLUGINVTK)

### tests and examples dependencies 
IF(BUILD_TEST_SUITE OR BUILD_EXAMPLES)
    SET( MAF_TIME_SPACE ON ) 
   mafDependentOption(MAF_TIME_SPACE "Build Test module for mafTimeSpace." ON BUILD_EXAMPLES ON)
    
   SET( MAF_SERIALIZATION ON ) 
   mafDependentOption(MAF_SERIALIZATION "Build Test module for mafSerialization." ON BUILD_EXAMPLES ON)
    
   SET( MAF_RESOURCES ON ) 
   mafDependentOption(MAF_RESOURCES "Build Test module for mafResources." ON BUILD_EXAMPLES ON)
    
   SET( MAF_PLUGINVTK ON ) 
   mafDependentOption(MAF_PLUGINVTK "Build Test module for mafPluginVTK." ON BUILD_EXAMPLES ON)
   
   SET( MAF_RESOURCES ON ) 
   mafDependentOption(MAF_RESOURCES "Build mafResources module." ON BUILD_EXAMPLES ON)
    
   SET( MAF_APPLICATION_LOGIC ON ) 
   mafDependentOption(MAF_APPLICATION_LOGIC "Build Test module for mafApplicationLogic." ON BUILD_EXAMPLES ON)
    
   SET( MAF_QTGUI ON ) 
   mafDependentOption(MAF_QTGUI "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)
ENDIF(BUILD_TEST_SUITE OR BUILD_EXAMPLES)

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

IF(BUILD_TEST_SUITE)
  ADD_SUBDIRECTORY(test)
ENDIF(BUILD_TEST_SUITE)

#############################################################################################
### EXAMPLES
#############################################################################################

IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF(BUILD_EXAMPLES)

