# Macro for change a cmake OPTION which depends from another option.
# Sample:
# OPTION(MAF_RESOURCES "Build mafResources module." ON)
# OPTION(MAF_PLUGINVTK "Build mafPluginVTK module." ON)
#
# if(MAF_PLUGINVTK)
#   SET( MAF_RESOURCES ON ) 
#   mafDependentOption(MAF_RESOURCES "Build mafResources module." ON MAF_PLUGINVTK ON)
# endif(MAF_PLUGINVTK)
#
# If mafPluginVTK is ON, mafResources must be ON

MACRO (mafDependentOption option doc default depends force)
  SET(${option}_AVAILABLE 1)

  # Test for each required variable.
  FOREACH(d ${depends})
    IF(NOT ${d})
      SET(${option}_AVAILABLE 0)
    ENDIF(NOT ${d})
  ENDFOREACH(d)
  IF(${option}_AVAILABLE)
    # The option is available.
    OPTION(${option} "${doc}" "${default}")

    # Show the cache value to the user.
    SET(${option} "${${option}}" CACHE BOOL "${doc}" FORCE)
  ELSE(${option}_AVAILABLE)
    # Option should not be available.  Hide the cache value.
    SET(${option} "${${option}}" CACHE INTERNAL "${doc}")

    # Force the option value to be that specified.
    SET(${option} ${force})
  ENDIF(${option}_AVAILABLE)
ENDMACRO()