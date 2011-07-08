################################################################################
#
#  Program: MAF
#
#  Copyright (c) 2010 Kitware Inc.
#
#  See Doc/copyright/copyright.txt
#  or http://www.MAF.org/copyright/copyright.txt for details.
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  This file was originally developed by
#   Dave Partyka and Jean-Christophe Fillion-Robin, Kitware Inc.
#  and was partially funded by NIH grant 3P41RR013218-12S1
#
################################################################################

#############################################################################################
### Git protocol option
#############################################################################################

option(MAF_USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)

#############################################################################################
### Qt - Let's check if a valid version of Qt is available
#############################################################################################

FIND_PACKAGE(Qt4)
IF(QT_FOUND)
  IF("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}" VERSION_LESS "${minimum_required_qt_version}")
    MESSAGE(FATAL_ERROR "error: MAF requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
  ENDIF()
ELSE()
  MESSAGE(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
ENDIF()

#############################################################################################
### Enable and setup External project global properties
#############################################################################################

INCLUDE(ExternalProject)

# this directory will be the root dir for external libraries.
SET(ep_base "${CMAKE_BINARY_DIR}/ExternalLibraries")
SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})

SET(ep_install_dir ${ep_base}/Install)

SET(ep_common_args
  -DCMAKE_INSTALL_PREFIX:PATH=${ep_install_dir}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DBUILD_TESTING:BOOL=${ep_build_testing}
  )
SET(ep_common_c_flags "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_INIT} ${ADDITIONAL_C_FLAGS}")
SET(ep_common_cxx_flags "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_INIT} ${ADDITIONAL_CXX_FLAGS}")
SET(ep_exe_linker_flags "${CMAKE_EXE_LINKER_FLAGS}")


# Compute -G arg for configuring external projects with the same CMake generator:
IF(CMAKE_EXTRA_GENERATOR)
  SET(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
ELSE()
  SET(gen "${CMAKE_GENERATOR}")
ENDIF()

# Use this value where semi-colons are needed in ep_add args:
set(sep "^^")

#############################################################################################
### Establish Target Dependencies based on Selected Options
#############################################################################################

#if(MAF_USE_PYTHONQT)
#  list(APPEND MAF_DEPENDENCIES python)
#endif()

#DEPENDENCIES
set(MAF_DEPENDENCIES)

if(MAF_EVENT_BUS)
  set(MAF_DEPENDENCIES ${MAF_DEPENDENCIES} qxmlrpc)
  include(CMakeExternals/External_qxmlrpc.cmake)
  
  set(MAF_DEPENDENCIES ${MAF_DEPENDENCIES} QtSOAP)
  include(CMakeExternals/External_QtSOAP.cmake)
endif(MAF_EVENT_BUS)

if(MAF_RESOURCES)
  set(MAF_DEPENDENCIES ${MAF_DEPENDENCIES} OpenCV)
  include(CMakeExternals/External_OpenCV.cmake)
endif(MAF_RESOURCES)

if(VTK_MAF)
  set(MAF_DEPENDENCIES ${MAF_DEPENDENCIES} VTK)
  include(CMakeExternals/External_VTK.cmake)
endif(VTK_MAF)

if(MAF_PLUGINVTK)
  set(MAF_DEPENDENCIES ${MAF_DEPENDENCIES} VTK)
  include(CMakeExternals/External_VTK.cmake)
endif(MAF_PLUGINVTK)


#remove duplicates
list(REMOVE_DUPLICATES MAF_DEPENDENCIES)

#############################################################################################
### Conditionnaly include ExternalProject Target
#############################################################################################

#if(MAF_USE_PYTHON OR MAF_USE_PYTHONQT)
#  include(SuperBuild/External_Python26.cmake)
#  if(MAF_BUILD_NUMPY)
#    include(SuperBuild/External_CLAPACK.cmake)
#    include(SuperBuild/External_NUMPY.cmake)
#    include(SuperBuild/External_weave.cmake)
#    if(MAF_BUILD_SCIPY)
#      include(SuperBuild/External_SciPy.cmake)
#    endif()
#  endif()
#endif()


#if(MAF_USE_QT)
#  include(SuperBuild/External_CTK.cmake)
#  if (MAF_USE_CTKAPPLAUNCHER)
#    include(SuperBuild/External_CTKAPPLAUNCHER.cmake)
#  endif()
#endif()

#############################################################################################
### Update external project dependencies
#############################################################################################

# For now, tk and itcl are used only when MAF_USE_KWWIDGETS is ON

#if(MAF_USE_QT)
#  list(APPEND MAF_DEPENDENCIES CTK)
#  if (MAF_USE_CTKAPPLAUNCHER)
#    list(APPEND MAF_DEPENDENCIES CTKAPPLAUNCHER)
#  endif()
#endif()

#if(MAF_USE_PYTHON OR MAF_USE_PYTHONQT)
#  list(APPEND MAF_DEPENDENCIES python)
  #if(MAF_BUILD_NUMPY)
  #  list(APPEND MAF_DEPENDENCIES NUMPY)
  #  if(MAF_BUILD_SCIPY)
  #    list(APPEND MAF_DEPENDENCIES scipy)
  #  endif()
  #endif()
#endif()


#############################################################################################
### Set superbuild boolean args
#############################################################################################

SET(MAF_cmake_boolean_args
  BUILD_DOCUMENTATION
  BUILD_QA
  BUILD_TESTING
  BUILD_SHARED_LIBS
  MAF_USE_QT
  #MAF_USE_PYTHONQT
  #MAF_BUILD_NUMPY
  # Deprecated
  MAF_USE_PYTHON
)
  
SET(MAF_superbuild_boolean_args)
FOREACH(MAF_cmake_arg ${MAF_cmake_boolean_args})
  LIST(APPEND MAF_superbuild_boolean_args -D${MAF_cmake_arg}:BOOL=${${MAF_cmake_arg}})
ENDFOREACH()

# MESSAGE("CMake args:")
# FOREACH(arg ${MAF_superbuild_boolean_args})
#   MESSAGE("  ${arg}")
# ENDFOREACH()

#############################################################################################
### Configure and build MAF
#############################################################################################

#message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")
set(proj MAF)
ExternalProject_Add(${proj}
  DEPENDS ${MAF_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${MAF_superbuild_boolean_args}
    -DBUILD_TEST_SUITE:BOOL=${BUILD_TEST_SUITE}
    -DBUILD_EXAMPLES:BOOL=${BUILD_EXAMPLES}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DBUILD_WRAP:BOOL=${BUILD_WRAP}
    #-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    #-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DSubversion_SVN_EXECUTABLE:FILEPATH=${Subversion_SVN_EXECUTABLE}
    -DGIT_EXECUTABLE:FILEPATH=${GIT_EXECUTABLE}
    -DMAF_SUPERBUILD:BOOL=OFF
    -DMAF_C_FLAGS:STRING=${MAF_C_FLAGS}
    -DMAF_CXX_FLAGS:STRING=${MAF_CXX_FLAGS}
    # ITK
    #-DITK_DIR:PATH=${ITK_DIR}
    # OpenIGTLink
    #-DOpenIGTLink_DIR:PATH=${OpenIGTLink_DIR}
    # Python
    #-DMAF_USE_SYSTEM_PYTHON:BOOL=OFF
    #-DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}
    -DPYTHONLIBS_FOUND:BOOL=${PYTHONLIBS_FOUND}
    -DPYTHON_LIBRARIES:FILEPATH=${PYTHON_LIBRARIES}
    -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR}
    -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}
    # Qt
    -DQT_QMAKE_EXECUTABLE:PATH=${QT_QMAKE_EXECUTABLE}
    # VTK
    -DVTK_DIR:PATH=${VTK_DIR}
    #-DVTK_DEBUG_LEAKS:BOOL=${MAF_USE_VTK_DEBUG_LEAKS}
    # qxmlrpc
    -Dqxmlrpc_DIR:PATH=${qxmlrpc_DIR} # Findqxmlrpc expects qxmlrpc_DIR variable to be defined
    # QtSOAP
    -DQtSOAP_DIR:PATH=${QtSOAP_DIR} # FindQtSOAP expects QtSOAP_DIR variable to be defined
    # OpenCV
    -DOpenCV_DIR:PATH=${OpenCV_DIR} # FindOpenCV expects OpenCV_DIR variable to be defined
    # CTK
    #-DCTK_DIR:PATH=${CTK_DIR}
    # CTKAppLauncher
    #-DCTKAPPLAUNCHER_DIR:PATH=${CTKAPPLAUNCHER_DIR}
    # Deprecated - KWWidgets
  INSTALL_COMMAND ""
  )
  
  
#message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")
