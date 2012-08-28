MACRO (mafDataTestDownload)
  set(GIT_REPOSITORY "git://github.com/b3c/maf3data.git")

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" clone ${GIT_REPOSITORY} ${MAF_DATA_DIR}
    OUTPUT_VARIABLE _out
    ERROR_VARIABLE _err
    RESULT_VARIABLE _res)

  message(STATUS "out: ${_out}")
  message(STATUS "err: ${_err}")
  message(STATUS "res: ${_res}")

ENDMACRO()
