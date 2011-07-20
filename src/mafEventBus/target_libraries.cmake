#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(nanogear_libraries
  Nanogear
  NanogearIHTTPConnector
)

SET(foundation_libraries 
  qxmlrpc
  QtSOAP
  ${nanogear_libraries}
)


SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  )
