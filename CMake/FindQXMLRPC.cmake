#
#  FindQXMLRPC.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

SET(QXMLRPC_DIR "$ENV{MAF3_FOUNDATION_LIB_DIR}/qxmlrpc/")
INCLUDE_DIRECTORIES("${QXMLRPC_DIR}/include/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${QXMLRPC_DIR}/lib/")

SET(QXMLRPC_LIBRARY "qxmlrpc")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
     if(WIN32)
      set(extension "_d")
    else(WIN32)
      set(extension "_debug")
    endif(WIN32)

    set(QXMLRPC_LIBRARY ${QXMLRPC_LIBRARY}${extension})
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

#message(${QXMLRPC_LIBRARY})


SET(QXMLRPC_FOUND TRUE)    

