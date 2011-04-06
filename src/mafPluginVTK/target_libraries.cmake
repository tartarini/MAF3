#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  qxmlrpc 
  QtSOAP
  vcl
  vnl
)

SET(VTK_LIBRARIES
  vtkHybrid 
  vtkRendering 
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
  ${VTK_LIBRARIES}
  mafCore
  mafEventBus
  mafResources
  )

