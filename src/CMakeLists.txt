cmake_minimum_required(VERSION 2.8)
message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")

include(${MAF_USE_FILE_CONFIG})

SET(dirs ${dirs}
  mafCore
  mafEventBus
  mafQA
  mafTimeSpace
  mafSerialization
  mafResources
  #mafPluginVTK
  mafApplicationLogic
  mafGUI
  )

ADD_SUBDIRECTORY(mafCore)
ADD_SUBDIRECTORY(mafEventBus)
ADD_SUBDIRECTORY(mafTimeSpace)
ADD_SUBDIRECTORY(mafSerialization)
ADD_SUBDIRECTORY(mafResources)
ADD_SUBDIRECTORY(mafPluginVTK)
ADD_SUBDIRECTORY(mafApplicationLogic)
ADD_SUBDIRECTORY(mafGUI)

message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")