#
# FindGit
#

SET(valgrind_FOUND 0)
find_program(valgrind_LIBRARY 
              NAMES valgrind 
              PATHS /usr/local/bin /usr/bin ${valgrind_LIBRARY})

get_filename_component(valgrind_LIBRARY_DIR ${valgrind_LIBRARY} PATH)

MARK_AS_ADVANCED(valgrind_FOUND)
 
IF(valgrind_LIBRARY)
  SET(valgrind_FOUND 1)
  SET(valgrind_ENABLE 1)
  message(STATUS "valgrind found.")
ENDIF(valgrind_LIBRARY)
