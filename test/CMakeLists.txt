#message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")

include(${MAF_USE_FILE_CONFIG})

ADD_SUBDIRECTORY(mafCoreTest)
ADD_SUBDIRECTORY(mafEventBusTest)
ADD_SUBDIRECTORY(mafQATest)


ADD_SUBDIRECTORY(mafTimeSpaceTest)
ADD_SUBDIRECTORY(mafSerializationTest)
ADD_SUBDIRECTORY(mafPluginTest)
ADD_SUBDIRECTORY(mafResourcesTest)
ADD_SUBDIRECTORY(mafPluginVTKTest)
ADD_SUBDIRECTORY(mafApplicationLogicTest)
ADD_SUBDIRECTORY(mafGUITest)

#message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")
