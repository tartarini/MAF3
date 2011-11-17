#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  ${QXMLRPC_LIBRARY} 
  ${QTSOAP_LIBRARY}
)

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  ${CTK_LIBRARIES}
  mafCore
  mafEventBus
  mafApplicationLogic
  mafResources
  mafGUI
  mafScriptInterpreter
  mafPluginCTK
  )
