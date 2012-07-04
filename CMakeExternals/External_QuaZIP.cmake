#-----------------------------------------------------------------------------
# Get and build QuaZIP
#

set(proj QuaZIP)
if(NOT DEFINED QuaZIP_DIR OR NOT DEFINED QuaZIP_SOURCE_DIR)

  SET(QuaZIP_enabling_variable QuaZIP_LIBRARIES)
  
  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_source_dir ${ep_base}/Source)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)
  
  
  SET(additional_vtk_cmakevars )
  IF(MINGW)
    LIST(APPEND additional_vtk_cmakevars -DCMAKE_USE_PTHREADS:BOOL=OFF)
  ENDIF()

  SET(proj QuaZIP)
  SET(proj_DEPENDENCIES)

  ExternalProject_Add(${proj}
    GIT_REPOSITORY "${git_protocol}://github.com/b3c/QuaZIP.git"
    #GIT_TAG "origin"
    INSTALL_COMMAND ""
    #INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/bin
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
    ${ep_common_args}
    DEPENDS
    ${proj_DEPENDENCIES}
    )
  set(QuaZIP_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/QuaZIP")
  set(QuaZIP_SOURCE_DIR ${CMAKE_BINARY_DIR}/ExternalLibraries/Source/${proj})

else()
  # The project is provided using QuaZIP_DIR and QuaZIP_SOURCE_DIR, nevertheless since other 
  # project may depend on QuaZIP, let's add an 'empty' one
  include(CMake/mafMacroEmptyExternalProject.cmake)
  mafMacroEmptyExternalProject(${proj} "${QuaZIP_DEPENDENCIES}")
endif()
#ENDIF(RESULT)
  SET(${QuaZIP_enabling_variable}_INCLUDE_DIRS QuaZIP_INCLUDE_DIRS)
  SET(${QuaZIP_enabling_variable}_FIND_PACKAGE_CMD QuaZIP)
  