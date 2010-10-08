### mafDependencies.cmake --- 
## 
## Author: Daniele Giunchi

## #################################################################
## Qt
## #################################################################

set(QT_USE_QTOPENGL  TRUE)
set(QT_USE_QTXML     TRUE)
set(QT_USE_QTSQL     TRUE)
set(QT_USE_QTHELP    TRUE)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTWEBKIT  TRUE)

if(WIN32)
  set(QT_USE_QTMAIN TRUE)
endif(WIN32)

find_package(Qt4 4.6.0 REQUIRED)
include(${QT_USE_FILE})

mark_as_advanced(QT_QMAKE_EXECUTABLE)
mark_as_advanced(QT_QTMOTIF_INCLUDE_DIR)
mark_as_advanced(QT_QTMOTIF_LIBRARY_DEBUG)
mark_as_advanced(QT_QTMOTIF_LIBRARY_RELEASE)

## #################################################################
## Swig
## #################################################################

find_package(SWIG QUIET)

if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  set(CMAKE_SWIG_FLAGS "")
  
  macro(maf_wrap project target name language input deps)
    set(wrap_output ${project}_wrap_${language}.cpp)
    add_custom_command(
      OUTPUT ${wrap_output}
      COMMAND ${SWIG_EXECUTABLE}
      ARGS
      "-${language}"
      "-c++"
      "-module" ${name}
      "-I${CMAKE_SOURCE_DIR}/${name}"
      "-outdir" ${CMAKE_CURRENT_BINARY_DIR}
      "-o" ${wrap_output}
      ${input}
      MAIN_DEPENDENCY ${input}
      COMMENT "Wrapping ${input} to ${language}")
    set(${target} ${${target}} ${wrap_output})
  endmacro(maf_wrap)
  
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
endif(SWIG_FOUND)

if(SWIG_FOUND)
  add_definitions(-DHAVE_SWIG)
endif(SWIG_FOUND)

## #################################################################
## Tcl
## #################################################################

find_package(TCL QUIET)

if(TCL_FOUND)
  include_directories(${TCL_INCLUDE_PATH})
endif(TCL_FOUND)

if(TCL_FOUND)
  add_definitions(-DHAVE_TCL)
endif(TCL_FOUND)

## #################################################################
## Python
## #################################################################

find_package(PythonLibs QUIET)

if(PYTHONLIBS_FOUND)
  include_directories(${PYTHON_INCLUDE_PATH})
  get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)
  link_directories(${PYTHON_PATH})
endif(PYTHONLIBS_FOUND)

if(PYTHONLIBS_FOUND)
  add_definitions(-DHAVE_PYTHON)
endif(PYTHONLIBS_FOUND)

## #################################################################
## Editline
## #################################################################

mark_as_advanced(EDITLINE_INCLUDE_DIR)
mark_as_advanced(EDITLINE_LIBRARY)

find_path(EDITLINE_INCLUDE_DIR histedit.h
  /usr/include
  /usr/local/include
)

find_library(EDITLINE_LIBRARY edit
  /usr/lib
  /usr/local/lib
)

if(EDITLINE_LIBRARY)
  set(EDITLINE_FOUND "YES")
endif(EDITLINE_LIBRARY)

if(EDITLINE_FOUND)
  add_definitions(-DHAVE_EDITLINE)
endif(EDITLINE_FOUND)

## #################################################################
## Zlib
## #################################################################

find_package(ZLIB QUIET)

if(Z_LIBRARY)
include_directories(${ZLIB_INCLUDE_DIRS})
endif(Z_LIBRARY)

## #################################################################
## Ssl
## #################################################################

find_package(OpenSSL QUIET)

if(OPENSSL_FOUND)
include_directories(${OPENSSL_INCLUDE_DIR})
endif(OPENSSL_FOUND)

## #################################################################
## Mpi
## #################################################################

find_package(MPI QUIET)

if(MPI_FOUND)
include_directories(${MPI_INCLUDE_PATH})
set(COMPILE_FLAGS ${COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
endif(MPI_FOUND)

## #################################################################
## Vrpn
## #################################################################

find_path(QUAT_INCLUDES           quat.h /usr/include /usr/local/include)
find_path(VRPN_INCLUDES vrpn_Configure.h /usr/include /usr/local/include)

if(QUAT_INCLUDES AND VRPN_INCLUDES)
include_directories(${QUAT_INCLUDES})
include_directories(${VRPN_INCLUDES})
endif(QUAT_INCLUDES AND VRPN_INCLUDES)

mark_as_advanced(VRPN_INCLUDES)
mark_as_advanced(QUAT_INCLUDES)

find_library(QUAT_LIBRARIES NAMES quat PATHS /usr/lib /usr/local/lib)
find_library(VRPN_LIBRARIES NAMES vrpn PATHS /usr/lib /usr/local/lib)

if(QUAT_LIBRARIES AND VRPN_LIBRARIES)
link_directories(${QUAT_LIBRARIES})
link_directories(${VRPN_LIBRARIES})
endif(QUAT_LIBRARIES AND VRPN_LIBRARIES)

if(QUAT_LIBRARIES AND VRPN_LIBRARIES)
  add_definitions(-DHAVE_VRPN)
endif(QUAT_LIBRARIES AND VRPN_LIBRARIES)

mark_as_advanced(QUAT_LIBRARIES)
mark_as_advanced(VRPN_LIBRARIES)
