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

SET(QtSOAP_DIR "$ENV{MAF3_FOUNDATION_LIB_DIR}/qtsoap/")
INCLUDE_DIRECTORIES("${QtSOAP_DIR}/include/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${QtSOAP_DIR}/lib/")
#debug or release
SET(QTSOAP_LIBRARY "QtSolutions_SOAP-2.7")
SET(QtSOAP_FOUND TRUE)    

