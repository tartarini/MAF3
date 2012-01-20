#-----------------------------------------------------------------------------
# Get and build VTK
#

set(proj VTK)

execute_process(COMMAND ${GIT_EXECUTABLE} remote show "${git_protocol}://github.com/SCS-B3C/VTK.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
                                    
  IF(RESULT)

if(NOT DEFINED VTK_DIR OR NOT DEFINED VTK_SOURCE_DIR)
#  message(STATUS "Adding project:${proj}")


  SET(VTK_enabling_variable VTK_LIBRARIES)
  
  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)
  
  
  SET(additional_vtk_cmakevars )
  IF(MINGW)
    LIST(APPEND additional_vtk_cmakevars -DCMAKE_USE_PTHREADS:BOOL=OFF)
  ENDIF()
  
  #IF(CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK)
  #  LIST(APPEND additional_vtk_cmakevars
  #    -DPYTHON_EXECUTABLE:PATH=${PYTHON_EXECUTABLE}
  #    -DPYTHON_LIBRARIES:FILEPATH=${PYTHON_LIBRARIES}
  #    -DPYTHON_DEBUG_LIBRARIES:FILEPATH=${PYTHON_DEBUG_LIBRARIES}
  #    )
  #ENDIF()

  SET(proj VTK)
  SET(proj_DEPENDENCIES)
  
  # Disable Tk when Python wrapping is enabled
  #if (MAF_USE_PYTHONQT)
  #  list(APPEND VTK_QT_ARGS -DVTK_USE_TK:BOOL=OFF)
  #endif()

  #MESSAGE("Adding project:${proj}")
  ExternalProject_Add(${proj}
    DOWNLOAD_DIR "${ep_build_dir}/${proj}"
    INSTALL_COMMAND ""
    #INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/bin
    CMAKE_GENERATOR ${gen}
CMAKE_ARGS
        ${ep_common_args}
        ${additional_vtk_cmakevars}
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_TESTING:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
        -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
        -DCMAKE_EXE_LINKER_FLAGS:STRING=${ep_exe_linker_flags}
        -DCMAKE_OSX_ARCHITECTURES=x86_64
        -DVTK_WRAP_TCL:BOOL=OFF
        -DVTK_USE_TK:BOOL=OFF
        -DVTK_WRAP_PYTHON:BOOL=${CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK}
        -DVTK_WRAP_JAVA:BOOL=OFF
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DDESIRED_QT_VERSION:STRING=4
        -DVTK_USE_GUISUPPORT:BOOL=ON
        -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
        -DVTK_USE_QT:BOOL=ON
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        ${VTK_PYTHON_ARGS}
        ${VTK_QT_ARGS}
        ${VTK_MAC_ARGS}
      DEPENDS
        ${proj_DEPENDENCIES}

    )
  set(VTK_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/VTK")
  set(VTK_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})

else()
  # The project is provided using VTK_DIR and VTK_SOURCE_DIR, nevertheless since other 
  # project may depend on VTK, let's add an 'empty' one
  include(CMake/mafMacroEmptyExternalProject.cmake)
  mafMacroEmptyExternalProject(${proj} "${VTK_DEPENDENCIES}")
endif()
ELSE(RESULT)
  if(NOT DEFINED VTK_DIR OR NOT DEFINED VTK_SOURCE_DIR)
#  message(STATUS "Adding project:${proj}")


  SET(VTK_enabling_variable VTK_LIBRARIES)
  
  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)
  
  
  SET(additional_vtk_cmakevars )
  IF(MINGW)
    LIST(APPEND additional_vtk_cmakevars -DCMAKE_USE_PTHREADS:BOOL=OFF)
  ENDIF()
  
  #IF(CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK)
  #  LIST(APPEND additional_vtk_cmakevars
  #    -DPYTHON_EXECUTABLE:PATH=${PYTHON_EXECUTABLE}
  #    -DPYTHON_LIBRARIES:FILEPATH=${PYTHON_LIBRARIES}
  #    -DPYTHON_DEBUG_LIBRARIES:FILEPATH=${PYTHON_DEBUG_LIBRARIES}
  #    )
  #ENDIF()

  SET(proj VTK)
  SET(proj_DEPENDENCIES)
  
  # Disable Tk when Python wrapping is enabled
  #if (MAF_USE_PYTHONQT)
  #  list(APPEND VTK_QT_ARGS -DVTK_USE_TK:BOOL=OFF)
  #endif()

  #MESSAGE("Adding project:${proj}")
  ExternalProject_Add(${proj}
    GIT_REPOSITORY "${git_protocol}://github.com/SCS-B3C/VTK.git"
    #GIT_TAG "origin"
    INSTALL_COMMAND ""
    #INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/bin
    CMAKE_GENERATOR ${gen}
CMAKE_ARGS
        ${ep_common_args}
        ${additional_vtk_cmakevars}
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_TESTING:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
        -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
        -DCMAKE_EXE_LINKER_FLAGS:STRING=${ep_exe_linker_flags}
        -DCMAKE_OSX_ARCHITECTURES=x86_64
        -DVTK_WRAP_TCL:BOOL=OFF
        -DVTK_USE_TK:BOOL=OFF
        -DVTK_WRAP_PYTHON:BOOL=${CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK}
        -DVTK_WRAP_JAVA:BOOL=OFF
        -DBUILD_SHARED_LIBS:BOOL=ON 
        -DDESIRED_QT_VERSION:STRING=4
        -DVTK_USE_GUISUPPORT:BOOL=ON
        -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
        -DVTK_USE_QT:BOOL=ON
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        ${VTK_PYTHON_ARGS}
        ${VTK_QT_ARGS}
        ${VTK_MAC_ARGS}
      DEPENDS
        ${proj_DEPENDENCIES}

    )
  set(VTK_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/VTK")
  set(VTK_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})

else()
  # The project is provided using VTK_DIR and VTK_SOURCE_DIR, nevertheless since other 
  # project may depend on VTK, let's add an 'empty' one
  include(CMake/mafMacroEmptyExternalProject.cmake)
  mafMacroEmptyExternalProject(${proj} "${VTK_DEPENDENCIES}")
endif()
ENDIF(RESULT)
  SET(${VTK_enabling_variable}_INCLUDE_DIRS VTK_INCLUDE_DIRS)
  SET(${VTK_enabling_variable}_FIND_PACKAGE_CMD VTK)
  