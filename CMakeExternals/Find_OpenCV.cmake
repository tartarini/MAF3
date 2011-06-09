SET(OpenCV_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/OpenCV")
FIND_PACKAGE(OpenCV REQUIRED)
include(${OpenCV_DIR}/OpenCVConfig.cmake)
