#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries 

)

if(NOT ${BUILD_SHARED_LIBS})
  FIND_PACKAGE(zlib REQUIRED)
  SET(foundation_libraries 
  ${foundation_libraries} 
  ${ZLIB_LIBRARY}
)
endif(NOT ${BUILD_SHARED_LIBS})

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  mafCore
  mafEventBus
  mafResources
  mafApplicationLogic
  mafQA
  )
