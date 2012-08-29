#
# FindGit
#

SET(gperftools_FOUND FALSE)
find_library(gperftools_LIBRARY 
              NAMES tcmalloc 
              PATHS /usr/local/lib /usr/lib)

get_filename_component(gperftools_LIBRARY_DIR ${gperftools_LIBRARY} PATH)

MARK_AS_ADVANCED(gperftools_FOUND)
 
IF(gperftools_LIBRARY)
  SET(gperftools_FOUND TRUE)
ENDIF(gperftools_LIBRARY)
