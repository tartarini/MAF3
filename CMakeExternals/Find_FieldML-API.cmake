  SET(FieldML-API_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/FieldML-API")
  FIND_PACKAGE(FieldML-API REQUIRED)
  include(${FieldML-API_DIR}/FieldML-APIConfig.cmake)
  INCLUDE(${FieldML-API_USE_FILE})
  #additional include directory
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Source/FieldML-API/core/src")
