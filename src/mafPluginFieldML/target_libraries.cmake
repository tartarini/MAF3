#
# See CMake/mafMacroGetTargetLibraries.cmake
# 
# This file should list the libraries required to build the current MAF module.
# 

SET(foundation_libraries
  ${QXMLRPC_LIBRARY} 
  ${QTSOAP_LIBRARY}
)

if(NOT ${BUILD_SHARED_LIBS})
  FIND_PACKAGE(zlib REQUIRED)
  SET(foundation_libraries 
  ${foundation_libraries} 
  ${ZLIB_LIBRARY}
)
endif(NOT ${BUILD_SHARED_LIBS})

set(FIELDML_LIB
  xml2
  fieldml_api
  fieldml_io_api
)

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  ${FIELDML_LIB}
  mafCore
  mafEventBus
  mafResources
  )
