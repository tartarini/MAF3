############################################################
# MAF Dependencies Inclusions
############################################################
include(${MAF_USE_FILE_CONFIG})
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_qxmlrpc.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_QtSOAP.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_OpenCV.cmake)
#include(${MAF_SOURCE_DIR}/CMakeExternals/Find_VTK.cmake)
include(${MAF_SOURCE_DIR}/CMakeExternals/Find_CTK.cmake)


############################################################
# CTK target libraries 
############################################################
#> Extract all library names starting with org_mydomain_
macro(GetMyTargetLibraries all_target_libraries varname)
  set(re_ctkplugin "^org_maf_[a-zA-Z0-9_]+$")
  set(_tmp_list)
  list(APPEND _tmp_list ${all_target_libraries})
  ctkMacroListFilter(_tmp_list re_ctkplugin OUTPUT_VARIABLE ${varname})
endmacro()
#>

#> Create a list of available plug-ins and make them known to the build system
set(plugins
  org.maf.dah.exampleapp:ON
)

ctkMacroSetupPlugins(${plugins})
