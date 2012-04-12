#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 
  qxmlrpc
  QtSOAP
)


SET(target_libraries
  if(MINGW)
    ${foundation_libraries}
    ${MAF_BASE_LIBRARIES}    
   else()
    ${MAF_BASE_LIBRARIES}
    ${foundation_libraries}   
   endif(MINGW)  
)
