#
#  FindVTK.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

get_target_property(TARGET_LOC ${PROJECT_NAME} LOCATION)
get_filename_component(TARGET_ABSOLUTE_DIR ${TARGET_LOC} PATH)
message("******************** ${TARGET_ABSOLUTE_DIR}  ${CONFIGURATION}")

SET(VTK_DIR "${MafFoundationLibDir}/vtk-5.6/")
INCLUDE_DIRECTORIES("${VTK_DIR}/include/")

if(WIN32)
    SET(link_dir "${VTK_DIR}lib/${CMAKE_BUILD_TYPE}/")
else(WIN32)
    SET(link_dir "${VTK_DIR}lib")
endif(WIN32)

LINK_DIRECTORIES(${LINK_DIRECTORIES} ${link_dir})

SET( VTK_LIBRARY )
SET(VTK_LIBRARY_TEMP "MapReduceMPI"
 "mpistubs"
 "QVTK"
 "vtkQtChart"
 "vtkCharts"
 "vtkCommon"
 "vtkDICOMParser"
 "vtkFiltering"
 "vtkfreetype"
 "vtkGenericFiltering"
 "vtkGeovis"
 "vtkGraphics"
 "vtkHybrid"
 "vtkIO"
 "vtkImaging"
 "vtkInfovis"
 "vtkNetCDF"
 "vtkRendering"
 "vtkViews"
 "vtkVolumeRendering"
 "vtkWidgets"
 "vtkalglib"
 "vtkexoIIc"
 "vtkexpat"
 "vtkfreetype"
 "vtkftgl"
 "vtkjpeg"
 "vtklibxml2"
 "vtkmetaio"
 "vtkpng"
 "vtkproj4"
 "vtksqlite"
 "vtksys"
 "vtktiff"
 "vtkverdict"
 "vtkzlib" )
 
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    IF(WIN32)
      if(MSVC)
      FOREACH(target_library ${VTK_LIBRARY_TEMP})
         LIST(APPEND VTK_LIBRARY "${target_library}_d")
      ENDFOREACH(target_library ${VTK_LIBRARY_TEMP})
      else(MSVC)
      FOREACH(target_library ${VTK_LIBRARY_TEMP})
         LIST(APPEND VTK_LIBRARY "${target_library}")
      ENDFOREACH(target_library ${VTK_LIBRARY_TEMP})
      endif(MSVC)
    ELSE(WIN32)
      SET(VTK_LIBRARY ${VTK_LIBRARY_TEMP})
    ENDIF(WIN32)
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(VTK_LIBRARY ${VTK_LIBRARY_TEMP})
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

SET(VTK_FOUND TRUE)

