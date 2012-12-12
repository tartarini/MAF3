#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  qxmlrpc 
  QtSOAP
  opencv_core
)

SET(CTK_LIBRARIES
  CTKDICOMCore
  CTKDICOMWidgets
  CTKCommandLineModulesBackendFunctionPointer
  CTKCommandLineModulesBackendLocalProcess
  CTKCommandLineModulesCore
  CTKCommandLineModulesFrontendQtGui
  CTKCommandLineModulesFrontendQtWebKit
  )

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  ${CTK_LIBRARIES}
  mafCore
  mafEventBus
  mafResources
  )

