#
# nanogear
#

SET(nanogear_DEPENDS)
  
  SET(nanogear_enabling_variable nanogear_LIBRARY)
  
  SET(proj nanogear)
  SET(proj_DEPENDENCIES)
  
  SET(nanogear_DEPENDS ${proj})
  execute_process(COMMAND ${GIT_EXECUTABLE} remote show "${git_protocol}://github.com/SCS-B3C/nanogear.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
                  
  IF(RESULT)
  IF(NOT DEFINED nanogear_DIR)
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
    SET(nanogear_DIR ${ep_build_dir}/${proj})
    
    # Since nanogear is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ELSE(RESULT)
  IF(NOT DEFINED nanogear_DIR)
    #MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/SCS-B3C/nanogear.git"
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
    SET(nanogear_DIR ${ep_build_dir}/${proj})
    
    # Since nanogear is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  ENDIF(RESULT)

  SET(${nanogear_enabling_variable}_INCLUDE_DIRS nanogear_INCLUDE_DIRS)
  SET(${nanogear_enabling_variable}_FIND_PACKAGE_CMD nanogear)
    


