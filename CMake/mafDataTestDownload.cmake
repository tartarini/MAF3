MACRO (mafDataTestDownload)
  set(GIT_REPOSITORY "git://github.com/openmaf/MAF3Data.git")

  if(NOT EXISTS ${MAF_DATA_DIR})
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" clone ${GIT_REPOSITORY} ${MAF_DATA_DIR}
    OUTPUT_VARIABLE _out
    ERROR_VARIABLE _err
    RESULT_VARIABLE _res)
  else(NOT EXISTS ${MAF_DATA_DIR})
    execute_process(
    WORKING_DIRECTORY ${MAF_DATA_DIR}
    COMMAND "${GIT_EXECUTABLE}" pull 
    OUTPUT_VARIABLE _out
    ERROR_VARIABLE _err
    RESULT_VARIABLE _res)
  endif(NOT EXISTS ${MAF_DATA_DIR})
  

  message(STATUS "out: ${_out}")
  message(STATUS "err: ${_err}")
  message(STATUS "res: ${_res}")

ENDMACRO()
