  SET(fervor_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/fervor")
  FIND_PACKAGE(fervor REQUIRED)
  include(${fervor_DIR}/fervorConfig.cmake)
  INCLUDE(${fervor_USE_FILE})
  #additional include directory
  INCLUDE_DIRECTORIES(${MAF_EXTERNAL_BUILD_DIR}/Source/fervor/xmlrpc)
