  SET(FieldML-API_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/FieldML-API")

  #additional include directory
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Source/FieldML-API/core/src")
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Source/FieldML-API/io/src")
  
  #link directories
  LINK_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Build/FieldML-API/core/")
  LINK_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Build/FieldML-API/io")