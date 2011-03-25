#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  ${QXMLRPC_LIBRARY} 
  ${QTSOAP_LIBRARY}
  ${VXL_LIBRARY}
)

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  ${VTK_LIBRARY}
  mafCore
  mafEventBus
  mafResources
  )

