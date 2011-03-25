#-----------------------------------------------------------------------------
# Get and build VTK
#

set(proj VTK)

if(NOT DEFINED VTK_DIR OR NOT DEFINED VTK_SOURCE_DIR)
#  message(STATUS "Adding project:${proj}")

  set(VTK_WRAP_TCL OFF)
  set(VTK_WRAP_PYTHON OFF)


  #if (MAF_USE_PYTHONQT)
  #  set(VTK_WRAP_PYTHON ON)
  #  list(APPEND VTK_DEPENDENCIES python)
  #endif()

  #set(VTK_PYTHON_ARGS)
  #if(MAF_USE_PYTHON OR MAF_USE_PYTHONQT)
  #  set(VTK_PYTHON_ARGS
  #    -DPYTHON_INCLUDE_DIR:PATH=${MAF_PYTHON_INCLUDE}
  #    -DPYTHON_LIBRARY:FILEPATH=${MAF_PYTHON_LIBRARY}
  #    )
  #endif()
  
  set(VTK_QT_ARGS)
  if(NOT APPLE)
    if(MAF_USE_QT)
      set(VTK_QT_ARGS
        -DDESIRED_QT_VERSION:STRING=4
        #-DVTK_USE_GUISUPPORT:BOOL=ON
        -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
        -DVTK_USE_QT:BOOL=ON
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        )
    endif()
  else()
    if(MAF_USE_QT)
      set(VTK_QT_ARGS
        -DVTK_USE_CARBON:BOOL=OFF
        -DVTK_USE_COCOA:BOOL=ON # Default to Cocoa, VTK/CMakeLists.txt will enable Carbon and disable cocoa if needed
        -DVTK_USE_X:BOOL=OFF
        #-DVTK_USE_RPATH:BOOL=ON
        -DDESIRED_QT_VERSION:STRING=4
        #-DVTK_USE_GUISUPPORT:BOOL=ON
        -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
        -DVTK_USE_QT:BOOL=ON
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
        )
    endif()
  endif()

  # Disable Tk when Python wrapping is enabled
  #if (MAF_USE_PYTHONQT)
  #  list(APPEND VTK_QT_ARGS -DVTK_USE_TK:BOOL=OFF)
  #endif()

  ExternalProject_Add(${proj}
    SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj}
    BINARY_DIR ${proj}-build
    GIT_REPOSITORY "${git_protocol}://vtk.org/VTK.git"
    #GIT_TAG "origin/Slicer-4.0"
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${ep_common_args}
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
      -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
      -DVTK_USE_PARALLEL:BOOL=OFF
      #-DVTK_DEBUG_LEAKS:BOOL=${MAF_USE_VTK_DEBUG_LEAKS}
      #-DVTK_WRAP_TCL:BOOL=${VTK_WRAP_TCL}
      #-DVTK_USE_RPATH:BOOL=ON
      -DDESIRED_QT_VERSION:STRING=4
      #-DVTK_USE_GUISUPPORT:BOOL=ON
      -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
      -DVTK_USE_QT:BOOL=ON
      -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      ${VTK_TCL_ARGS}
      -DVTK_WRAP_PYTHON:BOOL=${VTK_WRAP_PYTHON}
      -DVTK_INSTALL_PYTHON_USING_CMAKE:BOOL=ON
      ${VTK_PYTHON_ARGS}
      ${VTK_QT_ARGS}
      ${VTK_MAC_ARGS}
    INSTALL_COMMAND ""
    DEPENDS 
      ${VTK_DEPENDENCIES}
    )
  set(VTK_DIR ${CMAKE_BINARY_DIR}/${proj}-build)
  set(VTK_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})

else()
  # The project is provided using VTK_DIR and VTK_SOURCE_DIR, nevertheless since other 
  # project may depend on VTK, let's add an 'empty' one
  MAFMacroEmptyExternalProject(${proj} "${VTK_DEPENDENCIES}")
endif()
