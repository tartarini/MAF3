  #just replace ExternalLibrary with the right one
  SET(ExternalLibrary_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/ExternalLibrary")
  FIND_PACKAGE(ExternalLibrary REQUIRED)
  include(${ExternalLibrary_DIR}/ExternalLibraryConfig.cmake)
  INCLUDE(${ExternalLibrary_USE_FILE})
  #additional include directory
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Source/ExternalLibrary/include")
