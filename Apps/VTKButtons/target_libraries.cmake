#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  ${QXMLRPC_LIBRARY} 
  ${QTSOAP_LIBRARY}
  opencv_core
)

if(NOT ${BUILD_SHARED_LIBS})
  FIND_PACKAGE(zlib REQUIRED)
  SET(foundation_libraries 
  ${foundation_libraries} 
  ${ZLIB_LIBRARY}
)
endif(NOT ${BUILD_SHARED_LIBS})

SET(VTK_LIBRARIES
  vtkHybrid 
  vtkRendering 
  vtkVolumeRendering
  vtkGraphics 
  vtkImaging 
  vtkIO 
  vtkFiltering 
  vtkCommon
  QVTK
)

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  mafCore
  mafEventBus
  mafApplicationLogic
  mafResources
  mafSerialization
  mafGUI
  ${VTK_LIBRARIES}
  mafPluginVTK
  )

if(${BUILD_WRAP})
set( target_libraries 
     ${target_libraries} 
     mafScriptInterpreter
)
endif(${BUILD_WRAP})
