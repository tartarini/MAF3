#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
)

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
  mafQA
  vtkMAF
  ${VTK_LIBRARIES}
  )
