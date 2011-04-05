#
# VXL
#

SET(VXL_DEPENDS)

  SET(VXL_enabling_variable VXL_LIBRARIES)

  SET(proj VXL)
  SET(proj_DEPENDENCIES)

  SET(VXL_DEPENDS ${proj})

  IF(NOT DEFINED VXL_DIR)
  
    #     MESSAGE(STATUS "Adding project:${proj}")
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/SCS-B3C/VXL.git"
      GIT_TAG "origin/master"
      CMAKE_GENERATOR ${gen}
      INSTALL_COMMAND ""
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_CONTRIB:BOOL=OFF
        -DBUILD_CORE_GEOMETRY:BOOL=ON
        -DBUILD_CORE_IMAGING:BOOL=OFF
        -DBUILD_CORE_NUMERICS:BOOL=ON
        -DBUILD_CORE_PROBABILITY:BOOL=OFF
        -DBUILD_CORE_SERIALISATION:BOOL=OFF
        -DBUILD_CORE_UTILITIES:BOOL=ON
        -DBUILD_DOCUMENTATION:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_FOR_VXL_DASHBOARD:BOOL=OFF
        -DBUILD_TESTING:BOOL=OFF
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DBUILD_UNMANTAINED_LIBRARIES:BOOL=OFF
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(VXL_DIR "${ep_build_dir}/${proj}")
    
    # Since VXL is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS

  ELSE()
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

  SET(${VXL_enabling_variable}_LIBRARY_DIRS VXL_LIBRARY_DIRS)
  SET(${VXL_enabling_variable}_INCLUDE_DIRS VXL_INCLUDE_DIRS)
  SET(${VXL_enabling_variable}_FIND_PACKAGE_CMD VXL)

