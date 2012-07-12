PROJECT(org_maf_dah_exampleapp)

SET(PLUGIN_export_directive "org_maf_example_dicomapp_EXPORT")

SET(PLUGIN_SRCS
  mafCTKExampleDicomAppLogic_p.h
  mafCTKExampleDicomAppPlugin_p.h
  mafCTKExampleDicomAppLogic.cpp
  mafCTKExampleDicomAppPlugin.cpp
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  mafCTKExampleDicomAppLogic_p.h
  mafCTKExampleDicomAppPlugin_p.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
  mafCTKExampleDicomAppWidget.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries}
)

# here link the target libraries for maf
TARGET_LINK_LIBRARIES(${PROJECT_NAME} mafCore mafEventBus mafApplicationLogic mafResources mafGUI)
