#
#  mafFindVXL.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


SET(VXL_DIR "${MafFoundationLibDir}/vxl/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/core/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/v3p/")
INCLUDE_DIRECTORIES("${VXL_DIR}/include/vcl/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${VXL_DIR}/lib/")

SET(VXL_LIBRARY "netlib" "testlib" "v3p_netlib" "vcl" "vcsl" "vgl_algo" "vgl" "vnl_algo" "vnl")
SET(VXL_FOUND TRUE)    

