#
#  mafMacroConfigureProject.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2011 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

################### PUBLIC INTERFACE
MACRO(mafMacroConfigureProject type)
  
  if( ${type} STREQUAL "application" )
      ConfigureApplication()
  elseif ( ${type} STREQUAL "library" )
      ConfigureLibrary()
  elseif( ${type} STREQUAL "test")
      ConfigureTest()
  elseif( ${type} STREQUAL "plugin")
      ConfigurePlugin()
  else() 
      message(FATAL_ERROR "${type} not recognized. Select between application, library, plugin, test")
  endif()

ENDMACRO()


################### PRIVATE IMPLEMENTATION (don't call from outside)
MACRO(ConfigureApplication)
    # Set some Win32 Specific Settings
    if(WIN32)
        SET(GUI_TYPE WIN32)
    endif(WIN32)
    # Set some Apple MacOS Specific settings
    if (APPLE)
        SET(GUI_TYPE MACOSX_BUNDLE)
    endif (APPLE)

    # Create the executable.
    ADD_EXECUTABLE(${PROJECT_NAME} ${GUI_TYPE} ${PROJECT_SRCS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
ENDMACRO()

MACRO(ConfigureLibrary)
    # Create the library.
    #if(${BUILD_SHARED_LIBS})
      ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
    #else(${BUILD_SHARED_LIBS})
    #  ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SRCS})
    #endif(${BUILD_SHARED_LIBS})  

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
ENDMACRO()

MACRO(ConfigureTest)
    ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_SRCS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
ENDMACRO()


MACRO(ConfigurePlugin)
  add_definitions(-DQT_PLUGIN)
  ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".mafplugin")
ENDMACRO()


