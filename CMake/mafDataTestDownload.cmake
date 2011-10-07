MACRO (mafDataTestDownload)
  set(_SvnCredPart)
  set(_SvnUserPart)
  set(_SvnPassPart)

  set(SVN_REPOSITORY "http://svn.scsolutions.it/maf3data")

  execute_process(
    COMMAND "${Subversion_SVN_EXECUTABLE}" --non-interactive
    ${_SvnCredPart} ${_SvnUserPart} ${_SvnPassPart}
    checkout ${SVN_REPOSITORY} ${MAF_DATA_DIR}
    OUTPUT_VARIABLE _out
    ERROR_VARIABLE _err
    RESULT_VARIABLE _res)

  message(STATUS "out: ${_out}")
  message(STATUS "err: ${_err}")
  message(STATUS "res: ${_res}")

ENDMACRO()
