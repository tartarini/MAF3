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

  install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
  
  if(${WRAP_LIST_FOUND})
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
  endif(${WRAP_LIST_FOUND})

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

ENDMACRO()
