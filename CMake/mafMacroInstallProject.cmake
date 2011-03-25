#
#  mafMacroInstallProject.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroInstallProject)
  # AWFUL CODE!!!!!! 
  # to be removed as soon as git repositories for foundation libraries comes !!!!!
  # 
  FOREACH(foundation_library ${foundation_libraries})
    if(${foundation_library} MATCHES "qxmlrpc")
      mafCopySharedLibrariesInInstallDir("${QXMLRPC_DIR}lib")
    endif(${foundation_library} MATCHES "qxmlrpc")
    
    if(${foundation_library} MATCHES "QtSolutions_SOAP-2.7")
      mafCopySharedLibrariesInInstallDir("${QtSOAP_DIR}lib")
    endif(${foundation_library} MATCHES "QtSolutions_SOAP-2.7")
    
    if(${foundation_library} MATCHES "qscxml")
      mafCopySharedLibrariesInInstallDir("${QSCXML_DIR}lib")
    endif(${foundation_library} MATCHES "qscxml")

  ENDFOREACH()

ENDMACRO()
