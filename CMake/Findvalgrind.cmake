#
# FindGit
#

SET(valgrind_FOUND FALSE)
find_program(valgrind_LIBRARY 
              NAMES valgrind 
              PATHS /usr/local/bin /usr/bin)

get_filename_component(valgrind_LIBRARY_DIR ${valgrind_LIBRARY} PATH)

MARK_AS_ADVANCED(valgrind_FOUND)
 
IF(valgrind_LIBRARY)
  SET(valgrind_FOUND TRUE)
  message(STATUS "valgrind found.")
ENDIF(valgrind_LIBRARY)
