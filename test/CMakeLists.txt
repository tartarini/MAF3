#message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")

include(${MAF_USE_FILE_CONFIG})

ADD_SUBDIRECTORY(mafCoreTest)
ADD_SUBDIRECTORY(mafEventBusTest)
ADD_SUBDIRECTORY(mafQATest)

if(${MAF_TIME_SPACE_TEST})
  ADD_SUBDIRECTORY(mafTimeSpaceTest)
endif(${MAF_TIME_SPACE_TEST})

if(${MAF_SERIALIZATION_TEST})
  ADD_SUBDIRECTORY(mafSerializationTest)
endif(${MAF_SERIALIZATION_TEST})

if(${MAF_RESOURCES_TEST})
  ADD_SUBDIRECTORY(mafPluginTest)
  ADD_SUBDIRECTORY(mafResourcesTest)
endif(${MAF_RESOURCES_TEST})

if(${MAF_PLUGINVTK_TEST})
  ADD_SUBDIRECTORY(mafPluginVTKTest)
endif(${MAF_PLUGINVTK_TEST})

if(${MAF_APPLICATION_LOGIC_TEST})
  ADD_SUBDIRECTORY(mafApplicationLogicTest)
endif(${MAF_APPLICATION_LOGIC_TEST})

if(${MAF_QTGUI_TEST})
  ADD_SUBDIRECTORY(mafGUITest)
endif(${MAF_QTGUI_TEST})

#message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")
