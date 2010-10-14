MACRO(mafMacroInitProject test)

  # Extract current directory name to use as project name
  file(GLOB CUR_FILE "CMakeLists.txt")
  get_filename_component(CUR_ABSOLUTE_DIR ${CUR_FILE} PATH)
  get_filename_component(DIR_NAME ${CUR_ABSOLUTE_DIR} NAME)
  PROJECT(${DIR_NAME})

  FILE(GLOB include_file_list "${PROJECT_SOURCE_DIR}/*.h")
  FILE(GLOB implementation_file_list "${PROJECT_SOURCE_DIR}/*.cpp")
  FILE(GLOB templete_file_list1 "${PROJECT_SOURCE_DIR}/*.txx")
  FILE(GLOB templete_file_list2 "${PROJECT_SOURCE_DIR}/*.tpp")
  # Set your list of sources here.
  SET(PROJECT_SRCS
	${implementation_file_list}
	${include_file_list}
	${templete_file_list1}
	${templete_file_list2}
  )

  set(MY_MOC_CXX )
  if(${test})
    INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
    foreach(FILE_NAME_ABS ${implementation_file_list})
      get_filename_component(FILE_NAME ${FILE_NAME_ABS} NAME_WE)
      set(MOC_FILE "${FILE_NAME}.moc")
      QT4_GENERATE_MOC(${FILE_NAME_ABS} ${MOC_FILE})
      LIST(APPEND MY_MOC_CXX "${PROJECT_BINARY_DIR}/${MOC_FILE}")
    endforeach()
  else(${test})
    QT4_WRAP_CPP(MY_MOC_CXX ${include_file_list})
  endif(${test})

  # List libraries that are needed by this project.
  mafMacroGetTargetLibraries(dependency_libraries)
  SET(PROJECT_LIBS ${dependency_libraries})

ENDMACRO()
