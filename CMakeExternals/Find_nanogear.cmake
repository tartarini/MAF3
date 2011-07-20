  SET(nanogear_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/nanogear")
  FIND_PACKAGE(nanogear REQUIRED)
  include(${nanogear_DIR}/nanogearConfig.cmake)
  INCLUDE(${nanogear_USE_FILE})
