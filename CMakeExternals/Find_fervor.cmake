  set(fervor_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/fervor")
  FIND_PACKAGE(fervor REQUIRED)
  include(${fervor_DIR}/fervorConfig.cmake)
  include(${fervor_USE_FILE})
  #additional include directory
  include_directories(${MAF_EXTERNAL_BUILD_DIR}/Source/fervor)
  link_directories(${MAF_EXTERNAL_BUILD_DIR}/Build/fervor/bin)
