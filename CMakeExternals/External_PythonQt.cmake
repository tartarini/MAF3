#
# PythonQt
#
#IF(${add_project})

  # Sanity checks
  IF(DEFINED PYTHONQT_INSTALL_DIR AND NOT EXISTS ${PYTHONQT_INSTALL_DIR})
    MESSAGE(FATAL_ERROR "PYTHONQT_INSTALL_DIR variable is defined but corresponds to non-existing directory")
  ENDIF()

  SET(PythonQt_enabling_variable PYTHONQT_LIBRARIES)

  SET(proj PythonQt)
  SET(proj_DEPENDENCIES)

  
  IF(NOT DEFINED PYTHONQT_INSTALL_DIR)
  #   MESSAGE(STATUS "Adding project:${proj}")

    SET(ep_PythonQt_args)

    # Should PythonQt use VTK
    #IF(CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK)
    #  LIST(APPEND proj_DEPENDENCIES VTK)
    #  LIST(APPEND ep_PythonQt_args -DVTK_DIR:PATH=${VTK_DIR})
    #ENDIF()
    LIST(APPEND ep_PythonQt_args -DPythonQt_USE_VTK:BOOL=${CTK_LIB_Scripting/Python/Core_PYTHONQT_USE_VTK})
    
    # Enable Qt libraries PythonQt wrapping if required
    foreach(qtlib core gui network opengl sql svg uitools webkit xml xmlpatterns)
      STRING(TOUPPER ${qtlib} qtlib_uppercase)
      LIST(APPEND ep_PythonQt_args -DPythonQt_Wrap_Qt${qtlib}:BOOL=${CTK_LIB_Scripting/Python/Core_PYTHONQT_WRAP_QT${qtlib_uppercase}})
    endforeach()

    # Python is required
    FIND_PACKAGE(PythonLibs)
    FIND_PACKAGE(PythonInterp)
    IF(NOT PYTHONLIBS_FOUND)
      MESSAGE(FATAL_ERROR "error: Python is required to build ${PROJECT_NAME}")
    ENDIF()
    
    #need to install lxml and jinja2 python packages
    #lxml
    SET ( EI_EXECUTABLE
    "easy_install"
    )
    
    #lxml
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import lxml"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RTL)
             
    if(RTL MATCHES "0")
    else(RTL MATCHES "0")
      execute_process(COMMAND ${EI_EXECUTABLE} " lxml==2.2.2 "
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE RTLI)
      if(RTLI MATCHES "0")
      else(RTLI MATCHES "0")
	    message(FATAL_ERROR "Install in Python distribution library lxml, automatic install failed. Install setuptools, check the permissions, and then type : easy_install lxml")
      endif(RTLI MATCHES "0")    
    endif(RTL MATCHES "0")

    #jinja2
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import jinja2"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RTJ)
                  
    if(RTJ MATCHES "0")
    else(RTJ MATCHES "0")
      execute_process(COMMAND ${EI_EXECUTABLE} " jinja2 "
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE RTJI)
      if(RTJI MATCHES "0")
      else(RTJI MATCHES "0")
	    message(FATAL_ERROR "Install in Python distribution library jinja2, automatic install failed. Install setuptools, check the permissions , and then type : easy_install jinja2")
      endif(RTJI MATCHES "0")    
    endif(RTJ MATCHES "0")

    # Set CMake OSX variable to pass down the external project
    set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
    if(APPLE)
      list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
        -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
        -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
        -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET})
    endif()
    
    SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})

  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)
  

    SET(revision_tag 099805b447d54dac22b2e548c078ba39fd724f50)
    #IF(${proj}_REVISION_TAG)
    #  SET(revision_tag ${${proj}_REVISION_TAG})
    #ENDIF()
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/b3c/PythonQt.git"
      #GIT_TAG "origin/master"
      GIT_TAG ${revision_tag}
      CMAKE_GENERATOR ${gen}
      UPDATE_COMMAND ""
      BUILD_COMMAND ""
      CMAKE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
        #-DCMAKE_C_FLAGS:STRING=${ep_common_c_flags} # Not used
        -DCMAKE_INSTALL_PREFIX:PATH=${ep_install_dir}
        ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR}
        -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}
        ${ep_PythonQt_args}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(PYTHONQT_DIR ${ep_build_dir}/${proj})
    SET(PYTHONQT_SOURCE_DIR ${ep_source_dir}/${proj})

    # Since the full path of PythonQt library is used, there is not need to add
    # its corresponding library output directory to CTK_EXTERNAL_LIBRARY_DIRS

  ELSE()
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  LIST(APPEND CTK_SUPERBUILD_EP_VARS
    PYTHONQT_INSTALL_DIR:PATH
    PYTHON_EXECUTABLE:FILEPATH # FindPythonInterp expects PYTHON_EXECUTABLE variable to be defined
    PYTHON_INCLUDE_DIR:PATH # FindPythonQt expects PYTHON_INCLUDE_DIR variable to be defined
    PYTHON_LIBRARY:FILEPATH # FindPythonQt expects PYTHON_LIBRARY variable to be defined
    )

  SET(${PythonQt_enabling_variable}_INCLUDE_DIRS PYTHONQT_INCLUDE_DIR PYTHON_INCLUDE_DIRS)
  SET(${PythonQt_enabling_variable}_FIND_PACKAGE_CMD PythonQt)
#ENDIF()
