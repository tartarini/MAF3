# import VTK foundation library references

COMPILER = $$QMAKE_CC

!contains(COMPILER, gcc) {
    LIB_DIR = Release
    CONFIG(debug, debug|release) {
        LIB_DIR = Debug
    }
}

win32 {
    LIB_DIR = Release
    CONFIG(debug, debug|release) {
        LIB_DIR = Debug
    }
}

unix:LIB_CHECK = $$(MAF3_FOUNDATION_LIB_DIR)/vtk-5.6/lib/
win32:LIB_CHECK = $$(MAF3_FOUNDATION_LIB_DIR)\vtk-5.6\lib\\$$LIB_DIR\

# Foundation libraries dependencies
exists( $$LIB_CHECK ) {
 INCLUDEPATH += $$(MAF3_FOUNDATION_LIB_DIR)/vtk-5.6/include
 LIBS += -L$$(MAF3_FOUNDATION_LIB_DIR)/vtk-5.6/lib/$$LIB_DIR \
 -lMapReduceMPI \
 -lmpistubs \
 -lQVTK \
 -lvtkQtChart \
 -lvtkCharts \
 -lvtkCommon \
 -lvtkDICOMParser \
 -lvtkFiltering \
 -lvtkfreetype \
 -lvtkGenericFiltering \
 -lvtkGeovis \
 -lvtkGraphics \
 -lvtkHybrid \
 -lvtkIO \
 -lvtkImaging \
 -lvtkInfovis \
 -lvtkNetCDF \
 -lvtkRendering \
 -lvtkViews \
 -lvtkVolumeRendering \
 -lvtkWidgets \
 -lvtkalglib \
 -lvtkexoIIc \
 -lvtkexpat \
 -lvtkfreetype \
 -lvtkftgl \
 -lvtkjpeg \
 -lvtklibxml2 \
 -lvtkmetaio \
 -lvtkpng \
 -lvtkproj4 \
 -lvtksqlite \
 -lvtksys \
 -lvtktiff \
 -lvtkverdict \
 -lvtkzlib
}

win32:!exists( $$DESTDIR\*vtkCommon* ):QMAKE_PRE_LINK += copy \
        $$quote($$(MAF3_FOUNDATION_LIB_DIR)\vtk-5.6\lib\\$$LIB_DIR\*vtk*) \
        $$DESTDIR

unix:!exists( $$DESTDIR/*vtkCommon* ):QMAKE_PRE_LINK += cd $$(MAF3_FOUNDATION_LIB_DIR)/vtk-5.6/lib/$$LIB_DIR ;tar -cf zipped *;mv zipped $$DESTDIR/;cd $$DESTDIR/;tar -xvf zipped;rm zipped


