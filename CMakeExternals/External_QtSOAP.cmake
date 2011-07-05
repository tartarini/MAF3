#
# QtSOAP
#

SET(QtSOAP_DEPENDS)

  SET(QtSOAP_enabling_variable QtSOAP_LIBRARIES)

  SET(proj QtSOAP)
  SET(proj_DEPENDENCIES)

  SET(QtSOAP_DEPENDS ${proj})
  execute_process(COMMAND git remote show "${git_protocol}://github.com/SCS-B3C/QtSOAP.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
                  
  IF(RESULT)
      MESSAGE("QtSOAP can not be downloaded with git, try in local filesystem")
      IF(NOT DEFINED QtSOAP_DIR)
      ExternalProject_Add(${proj}
      DOWNLOAD_DIR "${ep_build_dir}/${proj}"
      CMAKE_GENERATOR ${gen}
      INSTALL_COMMAND ""
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(QtSOAP_DIR "${ep_build_dir}/${proj}")
    
    # Since QtSOAP is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS

  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ELSE(RESULT)
      IF(NOT DEFINED QtSOAP_DIR)
    
      MESSAGE("QtSOAP will be download with git, if in future some network problem occurs, rerun Cmake for using local filesystem sources already downloaded")
    #MESSAGE("Adding project:${proj}")
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/SCS-B3C/QtSOAP.git"
      GIT_TAG "origin/master"
      CMAKE_GENERATOR ${gen}
      INSTALL_COMMAND ""
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(QtSOAP_DIR "${ep_build_dir}/${proj}")
    
    # Since QtSOAP is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS

  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ENDIF(RESULT)

  
  SET(${QtSOAP_enabling_variable}_LIBRARY_DIRS QtSOAP_LIBRARY_DIRS)
  SET(${QtSOAP_enabling_variable}_INCLUDE_DIRS QtSOAP_INCLUDE_DIRS)
  SET(${QtSOAP_enabling_variable}_FIND_PACKAGE_CMD QtSOAP)

