#
#  mafMacroInstallProject.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroInstallProject)
  # AWFUL CODE!!!!!! 
  # to be removed as soon as git repositories for foundation libraries comes !!!!!
  # 
  FOREACH(foundation_library ${foundation_libraries})
    if(${foundation_library} MATCHES "qxmlrpc")
      mafCopySharedLibrariesInInstallDir("${QXMLRPC_DIR}lib")
    endif(${foundation_library} MATCHES "qxmlrpc")
    
    if(${foundation_library} MATCHES "QtSolutions_SOAP-2.7")
      mafCopySharedLibrariesInInstallDir("${QtSOAP_DIR}lib")
    endif(${foundation_library} MATCHES "QtSolutions_SOAP-2.7")
    
    if(${foundation_library} MATCHES "qscxml")
      mafCopySharedLibrariesInInstallDir("${QSCXML_DIR}lib")
    endif(${foundation_library} MATCHES "qscxml")

  ENDFOREACH()
  
  FOREACH(target_library ${target_libraries})
    if(${target_library} MATCHES "vtkCommon")
      mafCopySharedLibrariesInInstallDir("${VTK_DIR}lib")
    endif(${target_library} MATCHES "vtkCommon")
  ENDFOREACH()
  
  # END AWFUL CODE

  SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES PREFIX "" )
  if (CMAKE_BUILD_TYPE MATCHES Release)
      SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" )
  endif(CMAKE_BUILD_TYPE MATCHES Release)

  if(CMAKE_BUILD_TYPE MATCHES Debug)
      if(WIN32)
          SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_d" )
      else(WIN32)
          SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_debug" )
      endif(WIN32)
  else(CMAKE_BUILD_TYPE MATCHES Debug)
          SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" )
  endif(CMAKE_BUILD_TYPE MATCHES Debug)

  install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
  
  if(${WRAP_LIST_FOUND})
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
  endif(${WRAP_LIST_FOUND})

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

ENDMACRO()
