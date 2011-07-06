#
# qxmlrpc
#

SET(qxmlrpc_DEPENDS)
  
  SET(qxmlrpc_enabling_variable qxmlrpc_LIBRARY)
  
  SET(proj qxmlrpc)
  SET(proj_DEPENDENCIES)
  
  SET(qxmlrpc_DEPENDS ${proj})
  execute_process(COMMAND git remote show "${git_protocol}://github.com/SCS-B3C/qxmlrpc.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
                  
  IF(RESULT)
  MESSAGE("qxmlrpc can not be downloaded with git, try in local filesystem")
  IF(NOT DEFINED qxmlrpc_DIR)
    #MESSAGE("Adding project:${proj}")
    
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
    SET(qxmlrpc_DIR ${ep_build_dir}/${proj})
    
    # Since qxmlrpc is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ELSE(RESULT)
    MESSAGE("qxmlrpc will be download with git, if in future some network problem occurs, rerun Cmake for using local filesystem sources already downloaded")
  IF(NOT DEFINED qxmlrpc_DIR)
    #MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/SCS-B3C/qxmlrpc.git"
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
    SET(qxmlrpc_DIR ${ep_build_dir}/${proj})
    
    # Since qxmlrpc is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ENDIF(RESULT)

  SET(${qxmlrpc_enabling_variable}_INCLUDE_DIRS qxmlrpc_INCLUDE_DIRS)
  SET(${qxmlrpc_enabling_variable}_FIND_PACKAGE_CMD qxmlrpc)
    


