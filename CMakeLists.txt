#
#  CMakeLists.txt
#  MAF
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

cmake_minimum_required(VERSION 2.8.4)

project(MAF)

#############################################################################################
### MAF3 Options
#############################################################################################

option(BUILD_TEST_SUITE "Build Module's Tests"      OFF)
option(BUILD_EXAMPLES "Build Examples"              OFF)
option(BUILD_SHARED_LIBS "Build Shared Libs"        ON)
option(BUILD_QA "Build Q&A"                         OFF)
option(BUILD_DOCUMENTATION "Build documentation"    OFF)
option(BUILD_WRAP "Build Wrap"                      OFF)
option(DATA_TEST "Download MAF Data Test"           OFF)


option(MAF_USE_GIT_PROTOCOL "If ON use git protocol, otherwise http"                   OFF)

option(MAF_SUPERBUILD "Always keep ON superbuild" ON)
mark_as_advanced(MAF_SUPERBUILD)
mark_as_advanced(BUILD_SHARED_LIBS)

FIND_PACKAGE(Git)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "error: Install Git and try to re-configure")
endif()

if(BUILD_TEST_SUITE)
	FIND_PACKAGE(Subversion)
	if(NOT Subversion_FOUND)
	  message(STATUS "error: Install Subversion for enable tests.")
	  set(BUILD_TEST_SUITE OFF)
	  set(DATA_TEST OFF)
	endif()
endif(BUILD_TEST_SUITE)

# git protocol to be used
set(git_protocol "git")
if(NOT MAF_USE_GIT_PROTOCOL)
  set(git_protocol "https")
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
option(VTK_MAF "Build vtkMAF VTK extension library."                                                          ON)
option(MAF_APPLICATION_LOGIC "Build mafApplicationLogic module. mafCore and mafEventBus will be built anyway." ON)
option(MAF_QTGUI "Build mafGUI module. mafCore and mafEventBus will be built anyway."                         ON)
option(MAF_SCRIPT_INTERPRETER "Build mafScriptInterpreter module. mafCore, mafEventBus and mafGUI will be built anyway." OFF)
option(MAF_PLUGINCTK "Build mafPluginCTK module. mafCore and mafEventBus will be built anyway."               ON)
option(MAF_PLUGIN_OUTOFCORE "Build mafPluginOutOfCore module. mafCore and mafEventBus will be built anyway."               ON)
option(MAF_PLUGIN_ZIP "Build mafPluginZip module. mafCore and mafEventBus will be built anyway."               OFF)
option(MAF_MSV_EXTENSION "Build MSV Extension. mafCore and mafEventBus will be built anyway."               ON)


set(MAF_DATA_DIR)
find_path(MAF_DATA_DIR "README.txt" PATHS "${MAF_SOURCE_DIR}/../maf3data")

if(${MAF_DATA_DIR})
else(${MAF_DATA_DIR})
  set(MAF_DATA_DIR "${MAF_SOURCE_DIR}/../maf3data")
endif(${MAF_DATA_DIR})
  
include(CMake/mafUtilities.cmake)
### set modules interdependencies
if(MAF_PLUGINVTK)
  SET( MAF_RESOURCES ON ) 
  SET( VTK_MAF ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINVTK ON)
  mafDependentOption(VTK_MAF "Build vtkMAF module." ON MAF_PLUGINVTK ON)
endif(MAF_PLUGINVTK)

### set modules interdependencies
if(MAF_PLUGIN_OUTOFCORE)
  SET( MAF_RESOURCES ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGIN_OUTOFCORE ON)
endif(MAF_PLUGIN_OUTOFCORE)

if(MAF_PLUGIN_ZIP)
  SET( MAF_RESOURCES ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGIN_ZIP ON)
endif(MAF_PLUGIN_ZIP)

if(MAF_PLUGINCTK)
  SET( MAF_RESOURCES ON ) 
  SET( MAF_QTGUI ON ) 
  mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINCTK ON)
  mafDependentOption(MAF_QTGUI "Build vtkMAF module." ON MAF_PLUGINCTK ON)
endif(MAF_PLUGINCTK)

if(MAF_SCRIPT_INTERPRETER)
  SET( MAF_QTGUI ON )
  mafDependentOption(MAF_QTGUI "Build mafGUI module." ON MAF_SCRIPT_INTERPRETER ON)
endif(MAF_SCRIPT_INTERPRETER)

if(MAF_QTGUI)
  SET( MAF_APPLICATION_LOGIC ON )
  mafDependentOption(MAF_APPLICATION_LOGIC "Build mafApplicationLogic module." ON MAF_QTGUI ON)
endif(MAF_QTGUI)

### examples dependencies 
if(BUILD_EXAMPLES)
    set( MAF_TIME_SPACE ON ) 
    mafDependentOption(MAF_TIME_SPACE "Build module for mafTimeSpace." ON BUILD_EXAMPLES ON)
    
    set( MAF_SERIALIZATION ON ) 
    mafDependentOption(MAF_SERIALIZATION "Build module for mafSerialization." ON BUILD_EXAMPLES ON)
    
    set( MAF_RESOURCES ON ) 
    mafDependentOption(MAF_RESOURCES "Build module for mafResources." ON BUILD_EXAMPLES ON)
    
    set( MAF_PLUGINVTK ON ) 
    mafDependentOption(MAF_PLUGINVTK "Build module for mafPluginVTK." ON BUILD_EXAMPLES ON)
   
    set( VTK_MAF ON ) 
    mafDependentOption(VTK_MAF "Build module for vtkMAF." ON BUILD_EXAMPLES ON)
    
    set( MAF_APPLICATION_LOGIC ON ) 
    mafDependentOption(MAF_APPLICATION_LOGIC "Build module for mafApplicationLogic." ON BUILD_EXAMPLES ON)
    
    set( MAF_QTGUI ON ) 
    mafDependentOption(MAF_QTGUI "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)
	
	set( MAF_SCRIPT_INTERPRETER ON ) 
    mafDependentOption(MAF_SCRIPT_INTERPRETER "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)
    
    set( MAF_PLUGINCTK ON ) 
    mafDependentOption(MAF_PLUGINCTK "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)

    set( MAF_PLUGIN_OUTOFCORE ON ) 
    mafDependentOption(MAF_PLUGIN_OUTOFCORE "Build Test module for mafGUI." ON BUILD_EXAMPLES ON)

endif(BUILD_EXAMPLES)

### examples dependencies 
if(BUILD_TEST_SUITE)
    set( DATA_TEST ON ) 
    mafDependentOption(DATA_TEST "Download Data Test." ON BUILD_TEST_SUITE ON)
endif(BUILD_TEST_SUITE)

if(BUILD_QA)
    set( BUILD_DOCUMENTATION ON ) 
    mafDependentOption(BUILD_DOCUMENTATION "Build Documentation for QA." ON BUILD_QA ON)
endif(BUILD_QA)

#############################################################################################
### Wrapping
#############################################################################################
if(BUILD_WRAP)
  set( BUILD_DOCUMENTATION ON ) 
  mafDependentOption(BUILD_DOCUMENTATION "Build XML doxygen output for Wrapping." ON BUILD_WRAP ON)
  include(CMake/mafMacroFindPython.cmake)
endif(BUILD_WRAP)

#############################################################################################
### Scripting (find python) If Wrap is enabled, don't need to find Python again!!
#############################################################################################
if(MAF_SCRIPT_INTERPRETER AND NOT BUILD_WRAP)
  include(CMake/mafMacroFindPython.cmake)
endif(MAF_SCRIPT_INTERPRETER AND NOT BUILD_WRAP)

include(${MAF_SOURCE_DIR}/GenerateMAFConfig.cmake)

#############################################################################################
### DOCUMENTATION
#############################################################################################
if(BUILD_DOCUMENTATION)
  add_subdirectory(doc)
  if(BUILD_WRAP)
    add_subdirectory(wrap)
  endif(BUILD_WRAP)
endif(BUILD_DOCUMENTATION)

#############################################################################################
### Superbuild execution
#############################################################################################

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
### DATA
#############################################################################################

if(DATA_TEST)
  include(CMake/mafDataTestDownload.cmake)
  mafDataTestDownload()
endif(DATA_TEST)

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

