#
#  mafMacroSetupQt.cmake
#  maf
#
#  mafMacroSetupQt.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroSetupQt)

  SET(minimum_required_qt_version "4.7")

  FIND_PACKAGE(Qt4)

  # This option won't show up in the main CMake configure panel
  MARK_AS_ADVANCED(QT_QMAKE_EXECUTABLE)
  
  IF(QT4_FOUND)

    IF("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      MESSAGE(FATAL_ERROR "error: MAF requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
    ENDIF()

    SET(QT_USE_QTNETWORK ON)
    SET(QT_USE_QTGUI ON)
    SET(QT_USE_QTUITOOLS ON)
    SET(QT_USE_QTWEBKIT ON)
    SET(QT_USE_QTXML ON)
    SET(QT_USE_QTSQL ON)
	SET(QT_USE_QTOPENGL ON)
    SET(QT_USE_QTTEST ${BUILD_TESTING})
    SET(QT_USE_QTTEST ON)
    SET(QT_USE_QTMAIN ON)
    SET(QT_USE_QTSCRIPT ON)
    INCLUDE(${QT_USE_FILE})

    # Set variable QT_INSTALLED_LIBRARY_DIR that will contains
    # Qt shared library
    SET(QT_INSTALLED_LIBRARY_DIR ${QT_LIBRARY_DIR})
    IF (WIN32)
      GET_FILENAME_COMPONENT(QT_INSTALLED_LIBRARY_DIR ${QT_QMAKE_EXECUTABLE} PATH)
    ENDIF()

  ELSE(QT4_FOUND)
    MESSAGE(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  ENDIF(QT4_FOUND)

ENDMACRO()
