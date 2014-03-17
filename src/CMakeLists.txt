#message (".......................... Entering ${CMAKE_CURRENT_LIST_FILE} ............................")

include(${MAF_USE_FILE_CONFIG})

ADD_SUBDIRECTORY(mafCore)
ADD_SUBDIRECTORY(mafEventBus)
ADD_SUBDIRECTORY(mafQA)

if(${MAF_TIME_SPACE})
  ADD_SUBDIRECTORY(mafTimeSpace)
endif(${MAF_TIME_SPACE})

if(${MAF_SERIALIZATION})
  ADD_SUBDIRECTORY(mafSerialization)
endif(${MAF_SERIALIZATION})

if(${MAF_RESOURCES})
  ADD_SUBDIRECTORY(mafResources)
endif(${MAF_RESOURCES})

if(${MAF_PLUGINVTK})
  ADD_SUBDIRECTORY(mafPluginVTK)
endif(${MAF_PLUGINVTK})

if(${VTK_MAF})
  ADD_SUBDIRECTORY(vtkMAF)
endif(${VTK_MAF})

if(${MAF_APPLICATION_LOGIC})
  ADD_SUBDIRECTORY(mafApplicationLogic)
endif(${MAF_APPLICATION_LOGIC})

if(${MAF_QTGUI})
  ADD_SUBDIRECTORY(mafGUI)
endif(${MAF_QTGUI})

if(${BUILD_WRAP} AND ${MAF_SCRIPT_INTERPRETER})
  ADD_SUBDIRECTORY(mafScriptInterpreter)
endif(${BUILD_WRAP} AND ${MAF_SCRIPT_INTERPRETER})

if(${MAF_PLUGINCTK})
  #ADD_SUBDIRECTORY(mafPluginCTK)
endif(${MAF_PLUGINCTK})

if(${MAF_PLUGIN_OUTOFCORE})
  ADD_SUBDIRECTORY(mafPluginOutOfCore)
endif(${MAF_PLUGIN_OUTOFCORE})

if(${MAF_PLUGIN_ZIP})
  ADD_SUBDIRECTORY(mafPluginZip)
endif(${MAF_PLUGIN_ZIP})

if(${MAF_PLUGIN_EVENTBUSMONITOR})
  ADD_SUBDIRECTORY(mafPluginEventBusMonitor)
endif(${MAF_PLUGIN_EVENTBUSMONITOR})
#ADD_SUBDIRECTORY(mafPluginGimias)


#message (".......................... Exiting ${CMAKE_CURRENT_LIST_FILE} ............................")
