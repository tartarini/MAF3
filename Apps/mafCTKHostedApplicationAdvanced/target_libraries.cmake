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
  CTKPluginFramework
  org_commontk_dah_hostedapp
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
  CTKPluginFramework
  org_commontk_dah_hostedapp
  )

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  mafCore
  mafEventBus
  mafApplicationLogic
  mafResources
  mafGUI
  ${CTK_LIBRARIES}  
)

if(${BUILD_WRAP})
set( target_libraries 
     ${target_libraries} 
     mafScriptInterpreter
)
endif(${BUILD_WRAP})

