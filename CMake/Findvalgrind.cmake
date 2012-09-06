#
# FindGit
#

SET(valgrind_FOUND 0)

find_program(valgrind_EXECUTABLE 
              NAMES valgrind 
              PATHS /usr/local/bin /usr/bin ${valgrind_PATH})

#fill the directory
get_filename_component(valgrind_PATH ${valgrind_EXECUTABLE} PATH)

MARK_AS_ADVANCED(valgrind_FOUND)
 
IF(valgrind_EXECUTABLE)
  SET(valgrind_FOUND 1)
  SET(valgrind_ENABLE 1)
  message(STATUS "valgrind found.")
ENDIF(valgrind_EXECUTABLE)
