SET(foundation_libraries 
 ${PYTHON_LIBRARIES}
)

SET(target_libraries
  ${MAF_BASE_LIBRARIES}
  ${foundation_libraries}
  mafCore
  mafQA
  mafGUI
  mafScriptInterpreter
  )