#
# VXL
#

SET(VXL_DIR "$ENV{MAF3_FOUNDATION_LIB_DIR}/vxl/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/core/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/v3p/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/vcl/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${VXL_DIR}/lib/")

SET(VXL_LIBRARY "netlib" "testlib" "v3p_netlib" "vcl" "vcsl" "vgl_algo" "vgl" "vnl_algo" "vnl")
SET(VXL_FOUND TRUE)    

