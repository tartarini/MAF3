#-----------------------------------------------------------------------------
# CTK
#-----------------------------------------------------------------------------

set(proj CTK)

execute_process(COMMAND ${GIT_EXECUTABLE} remote show "http://github.com/commontk/CTK.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
IF(RESULT)
    #to be implemented  
ELSE(RESULT)
    # Sanity checks
  IF(DEFINED CTK_DIR AND NOT EXISTS ${CTK_DIR})
    MESSAGE(FATAL_ERROR "CTK_DIR variable is defined but corresponds to non-existing directory")
  ENDIF()

  SET(proj_DEPENDENCIES )
  SET(CTK_DEPENDS ${proj})
  
  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)


  IF(NOT DEFINED CTK_DIR)
    
    #SET(revision_tag 107ffad7)

    #SET(revision_tag 3a63315)
    IF(${proj}_REVISION_TAG)
      SET(revision_tag ${${proj}_REVISION_TAG})
    ENDIF()
    
    SET(ctk_optional_cache_args )
    FOREACH(type RUNTIME ARCHIVE LIBRARY)
      IF(DEFINED CTK_PLUGIN_${type}_OUTPUT_DIRECTORY)
        LIST(APPEND optional_cache_args -DCTK_PLUGIN_${type}_OUTPUT_DIRECTORY:PATH=${CTK_PLUGIN_${type}_OUTPUT_DIRECTORY})
      ENDIF()
    ENDFOREACH()
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "http://github.com/commontk/CTK.git"
      GIT_TAG ${revision_tag}
      DOWNLOAD_DIR "${ep_build_dir}/${proj}"
      #BINARY_DIR ${proj}-build
      UPDATE_COMMAND ""
      INSTALL_COMMAND ""
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${ep_common_args}
        ${ctk_optional_cache_args}
        -DDESIRED_QT_VERSION:STRING=4
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        -DCTK_LIB_DICOM/Widgets:BOOL=ON
        -DCTK_USE_GIT_PROTOCOL:BOOL=OFF
      DEPENDS ${proj_DEPENDENCIES}
     )
  SET(CTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
  
  ELSE()

    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
    
  ENDIF()

ENDIF(RESULT) 

