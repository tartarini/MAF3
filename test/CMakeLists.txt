#message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")

include(${MAF_USE_FILE_CONFIG})

ADD_SUBDIRECTORY(mafCoreTest)
ADD_SUBDIRECTORY(mafEventBusTest)
ADD_SUBDIRECTORY(mafQATest)
ADD_SUBDIRECTORY(mafModuleTest)

if(${MAF_TIME_SPACE})
  ADD_SUBDIRECTORY(mafTimeSpaceTest)
endif(${MAF_TIME_SPACE})

if(${MAF_SERIALIZATION})
  ADD_SUBDIRECTORY(mafSerializationTest)
endif(${MAF_SERIALIZATION})

if(${MAF_RESOURCES})
  ADD_SUBDIRECTORY(mafPluginTest)
  ADD_SUBDIRECTORY(mafResourcesTest)
endif(${MAF_RESOURCES})

if(${MAF_PLUGINVTK})
  ADD_SUBDIRECTORY(mafPluginVTKTest)
endif(${MAF_PLUGINVTK})

if(${VTK_MAF})
  ADD_SUBDIRECTORY(vtkMAFTest)
endif(${VTK_MAF})

if(${MAF_APPLICATION_LOGIC})
  ADD_SUBDIRECTORY(mafApplicationLogicTest)
endif(${MAF_APPLICATION_LOGIC})

if(${MAF_QTGUI})
  ADD_SUBDIRECTORY(mafGUITest)
endif(${MAF_QTGUI})

if(${MAF_SCRIPT_INTERPRETER})
  ADD_SUBDIRECTORY(mafScriptInterpreterTest)
endif(${MAF_SCRIPT_INTERPRETER})








#message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")
