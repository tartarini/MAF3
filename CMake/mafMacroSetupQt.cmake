#
#  mafMacroSetupQt.cmake
#  maf
#
#  mafMacroSetupQt.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroSetupQt)

  SET(minimum_required_qt_version "5.0")

  find_package(Qt5 COMPONENTS Core Widgets Sql Xml Gui Concurrent UiTools Test)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  if (DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH $ENV{QTDIR} ${CMAKE_PREFIX_PATH})
  endif (DEFINED ENV{QTDIR})
    
    
  include_directories(${Qt5Widgets_INCLUDES})
  add_definitions(${Qt5Widgets_DEFINITIONS})
  set(CMAKE_CXX_FLAGS "${Qt5_EXECUTABLE_COMPILE_FLAGS}")

ENDMACRO()
