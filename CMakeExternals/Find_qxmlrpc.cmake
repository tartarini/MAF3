  SET(qxmlrpc_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/qxmlrpc")
  FIND_PACKAGE(qxmlrpc REQUIRED)
  include(${qxmlrpc_DIR}/qxmlrpcConfig.cmake)
  INCLUDE(${qxmlrpc_USE_FILE})
  #additional include directory
  INCLUDE_DIRECTORIES(${MAF_EXTERNAL_BUILD_DIR}/Source/qxmlrpc/xmlrpc)
