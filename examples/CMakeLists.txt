#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2011 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#


#############################################################################################
### find project subdirs avoiding the svn directory and make the SUBDIR CMake command on them.
#############################################################################################

include(${MAF_USE_FILE_CONFIG})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/mafApplicationConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/mafApplicationConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_SUBDIRECTORY(qaCheck)
ADD_SUBDIRECTORY(RemoteCommunicationXMLRPC)
ADD_SUBDIRECTORY(SimpleApp)


if(BUILD_WRAP)
ADD_SUBDIRECTORY(mafPythonQtExample)
endif(BUILD_WRAP)

#ADD_SUBDIRECTORY(mafCTKSimpleApp)
#ADD_SUBDIRECTORY(mafCTKHostedApplication)
#ADD_SUBDIRECTORY(mapReduceExample)
ADD_SUBDIRECTORY(VTKButtons)
