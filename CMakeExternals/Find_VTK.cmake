# need ${VTK_DIR} that is be set by ExternalProject_Add for vtk
SET(VTK_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/VTK")
FIND_PACKAGE(VTK REQUIRED)
#this set is because, inside VTK 5, nobody fills it
SET(VTK_CMAKE_DIR "${MAF_EXTERNAL_BUILD_DIR}/Source/VTK/CMake")
INCLUDE(${VTK_USE_FILE})
