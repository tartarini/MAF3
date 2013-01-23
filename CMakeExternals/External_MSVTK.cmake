#-----------------------------------------------------------------------------
# MSVTK
#-----------------------------------------------------------------------------

set(proj MSVTK)

#execute_process(COMMAND ${GIT_EXECUTABLE} remote show "https://github.com/MSV-Project/MSVTK.git"
#                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#                  RESULT_VARIABLE RESULT)
#IF(RESULT)
    #to be implemented  
#ELSE(RESULT)
    # Sanity checks
  IF(DEFINED MSVTK_DIR AND NOT EXISTS ${MSVTK_DIR})
    MESSAGE(FATAL_ERROR "MSVTK_DIR variable is defined but corresponds to non-existing directory")
  ENDIF()

  SET(proj_DEPENDENCIES VTK)
  SET(MSVTK_DEPENDS ${proj})
  
  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)


  IF(NOT DEFINED MSVTK_DIR)
    
    #SET(revision_tag 107ffad7)

    #SET(revision_tag 824ccb42e1d8d387b2ee4ce7e650efe9e67717f9)
    IF(${proj}_REVISION_TAG)
      SET(revision_tag ${${proj}_REVISION_TAG})
    ENDIF()
    
    SET(MSVTK_optional_cache_args )
    FOREACH(type RUNTIME ARCHIVE LIBRARY)
      IF(DEFINED MSVTK_PLUGIN_${type}_OUTPUT_DIRECTORY)
        LIST(APPEND optional_cache_args -DMSVTK_PLUGIN_${type}_OUTPUT_DIRECTORY:PATH=${MSVTK_PLUGIN_${type}_OUTPUT_DIRECTORY})
      ENDIF()
    ENDFOREACH()

    ExternalProject_Add(${proj}
      GIT_REPOSITORY "https://github.com/MSV-Project/MSVTK.git"
      #GIT_TAG ${revision_tag}
      #BINARY_DIR ${proj}-build
      INSTALL_COMMAND ""
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${ep_common_args}
        ${MSVTK_optional_cache_args}
        -DDESIRED_QT_VERSION:STRING=4
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        -DMSVTK_USE_GIT_PROTOCOL:BOOL=OFF
        -DVTK_DIR=${VTK_DIR}
      DEPENDS ${proj_DEPENDENCIES}
     )
  SET(MSVTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)

  ELSE()

    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
    
  ENDIF()

#ENDIF(RESULT) 

