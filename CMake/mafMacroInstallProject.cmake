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
      if(WIN32)
          SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_d" )
      else(WIN32)
          SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_debug" )
      endif(WIN32)
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
