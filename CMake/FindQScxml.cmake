#
#  FindQScxml.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

SET(QSCXML_DIR "$ENV{MAF3_FOUNDATION_LIB_DIR}/qscxml/")

INCLUDE_DIRECTORIES("${QSCXML_DIR}/include/")
LINK_DIRECTORIES(${LINK_DIRECTORIES} "${QSCXML_DIR}/lib/")
SET(QSCXML_LIBRARY "qscxml")

SET(QSCXML_FOUND TRUE)
