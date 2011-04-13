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

cmake_minimum_required(VERSION 2.8)

project(MAF)

#############################################################################################
### MAF3 Options
#############################################################################################

option(BUILD_TEST_SUITE "Build Module's Tests"      OFF)
option(BUILD_EXAMPLES "Build Examples"              OFF)
option(BUILD_QA "Build Q&A"                         OFF)
option(BUILD_DOCUMENTATION "Build documentation"    OFF)

option(MAF_USE_GIT_PROTOCOL "If ON use git protocol, otherwise http"                   OFF)

option(MAF_SUPERBUILD "Always keep ON superbuild" ON)
mark_as_advanced(MAF_SUPERBUILD)

FIND_PACKAGE(Git)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "error: Install Git and try to re-configure")
endif()

# git protocol to be used
set(git_protocol "git")
if(NOT MAF_USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()


#############################################################################################
### Compilers
#############################################################################################

# modify CMAKE_BUILD_TYPE in order to set default configuration for non-IDE build
if(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles" AND NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(CMAKE_GENERATOR STREQUAL "Unix Makefiles" OR CMAKE_GENERATOR STREQUAL "MinGW Makefiles" AND NOT CMAKE_BUILD_TYPE)

#############################################################################################
### Available Modules
#############################################################################################

set(MAF_CORE ON)
set(MAF_EVENT_BUS ON)
set(MAF_QA ON)
option(MAF_TIME_SPACE "Build mafTimeSpace module. mafCore and mafEventBus will be built anyway."              ON)
option(MAF_SERIALIZATION "Build mafSerialization module. mafCore and mafEventBus will be built anyway."       ON)
option(MAF_RESOURCES "Build mafResources module. mafCore and mafEventBus will be built anyway."               ON)
option(MAF_PLUGINVTK "Build mafPluginVTK module. mafCore and mafEventBus will be built anyway."               ON)
option(MAF_APPLICATION_LOGIC "Build mafApplicationLogic module.mafCore and mafEventBus will be built anyway." ON)
option(MAF_QTGUI "Build mafGUI module. mafCore and mafEventBus will be built anyway."                         ON)

### set modules interdependencies
include(CMake/mafUtilities.cmake)
if(MAF_PLUGINVTK)
  SET( MAF_RESOURCES ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINVTK ON)
endif(MAF_PLUGINVTK)

### tests and examples dependencies 
if(BUILD_TEST_SUITE OR BUILD_EXAMPLES)
    set( MAF_TIME_SPACE ON ) 
    mafDependentOption(MAF_TIME_SPACE "Build Test module for mafTimeSpace." ON BUILD_EXAMPLES ON)
    
    set( MAF_SERIALIZATION ON ) 
    mafDependentOption(MAF_SERIALIZATION "Build Test module for mafSerialization." ON BUILD_EXAMPLES ON)
    
    set( MAF_RESOURCES ON ) 
    mafDependentOption(MAF_RESOURCES "Build Test module for mafResources." ON BUILD_EXAMPLES ON)
    
    set( MAF_PLUGINVTK ON ) 
    mafDependentOption(MAF_PLUGINVTK "Build Test module for mafPluginVTK." ON BUILD_EXAMPLES ON)
   
    set( MAF_RESOURCES ON ) 
    mafDependentOption(MAF_RESOURCES "Build mafResources module." ON BUILD_EXAMPLES ON)
    
    set( MAF_APPLICATION_LOGIC ON ) 
    mafDependentOption(MAF_APPLICATION_LOGIC "Build Test module for mafApplicationLogic." ON BUILD_EXAMPLES ON)
    
    set( MAF_QTGUI ON ) 
    mafDependentOption(MAF_QTGUI "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)
endif(BUILD_TEST_SUITE OR BUILD_EXAMPLES)

if(BUILD_QA)
    set( BUILD_DOCUMENTATION ON ) 
    mafDependentOption(BUILD_DOCUMENTATION "Build Documentation for QA." ON BUILD_QA ON)

endif(BUILD_QA)

#############################################################################################
### Wrapping (find swig, python ...)
#############################################################################################

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  include(CMake/mafMacroFindWrap.cmake)
else(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(PYTHONLIBS_FOUND OFF)
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

#############################################################################################
### Superbuild execution
#############################################################################################

include(${MAF_SOURCE_DIR}/GenerateMAFConfig.cmake)
configure_file(${MAF_SOURCE_DIR}/UseMAF.cmake.in
  ${MAF_BINARY_DIR}/UseMAF.cmake COPYONLY IMMEDIATE)
  
if(MAF_SUPERBUILD)
  include(SuperBuild.cmake)
  return()
else(MAF_SUPERBUILD)
endif(MAF_SUPERBUILD)

#############################################################################################
### SOURCES
#############################################################################################
add_subdirectory(src)

#############################################################################################
### TESTS
#############################################################################################

if(BUILD_TEST_SUITE)
  add_subdirectory(test)
endif(BUILD_TEST_SUITE)

#############################################################################################
### EXAMPLES
#############################################################################################

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_EXAMPLES)

#############################################################################################
### EXAMPLES
#############################################################################################

if(BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)
