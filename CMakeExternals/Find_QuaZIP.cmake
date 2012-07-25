SET(QuaZIP_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/QuaZIP")
include(${QuaZIP_SOURCE_DIR}/FindQuaZip.cmake) #to change with FIND_PACKAGE(QuaZIP REQUIRED)

#additional include directory
LINK_DIRECTORIES(${MAF_EXTERNAL_BUILD_DIR}/Build/QuaZIP/quazip)


