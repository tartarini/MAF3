#
# See CMake/ctkMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current CTK libraries
# 

SET(foundation_libraries 
  ${QXMLRPC_LIBRARY} 
  ${QTSOAP_LIBRARY}
)

SET(CTK_LIBRARIES
  CTKDICOMCore
  CTKDICOMWidgets
  )

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  mafCore
  mafEventBus
  mafApplicationLogic
  mafResources
  mafGUI
  mafScriptInterpreter
  ${CTK_LIBRARIES}
  
  )
