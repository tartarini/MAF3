#
#  FindQtSOAP.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

SET(QtSOAP_DIR "${MafFoundationLibDir}/qtsoap/")
INCLUDE_DIRECTORIES("${QtSOAP_DIR}/include/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${QtSOAP_DIR}/lib/")

SET(QTSOAP_LIBRARY "QtSolutions_SOAP-2.7")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
     if(WIN32)
      set(extension "_d")
    else(WIN32)
      set(extension "_debug")
    endif(WIN32)

    set(QTSOAP_LIBRARY ${QTSOAP_LIBRARY}${extension})
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

#message(${QTSOAP_LIBRARY})

SET(QtSOAP_FOUND TRUE)
