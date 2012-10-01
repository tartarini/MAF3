/*
 *  vtkMAFVolumeSlicerTest.cpp
 *  vtkMAF
 *
 *  Created by Paolo Quadrani on 11/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>

#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>

using namespace mafCore;

#include "vtkMAFVolumeSlicer.h"

#include "vtkDataSetReader.h"

#include "vtkSmartPointer.h"
#include "vtkActor.h"

#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

/**
 Class name: vtkMAFVolumeSlicerTest
 This class implements the test suite for vtkMAFVolumeSlicer.
 */
class vtkMAFVolumeSlicerTest: public QObject {
    Q_OBJECT
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }
    
    /// Test the slicing of VTK StructuredPoint volume.
    void TestVolumeSlicerSP();
};


void vtkMAFVolumeSlicerTest::TestVolumeSlicerSP() {
   // Create an image for the button
   QString fname(MAF_DATA_DIR);
   fname.append("/VTK/RectGrid2.vtk");
   fname = QDir::toNativeSeparators(fname);
   QByteArray ba = fname.toAscii();

   VTK_CREATE(vtkDataSetReader, reader);
   VTK_CREATE(vtkMAFVolumeSlicer, slicer);

   slicer->SetInputConnection(reader->GetOutputPort());

   double origin[3] = {0,0,0};
   slicer->SetPlaneOrigin(origin);

   double x_vector[3] = {1., 0., 0.};
   double y_vector[3] = {0., 1., 0.};

   slicer->SetPlaneAxisX(x_vector);
   slicer->SetPlaneAxisY(y_vector);

   slicer->Update();

/*   VTK_CREATE(vtkPolyDataMapper, maceMapper);
   maceMapper->SetInputConnection(apd->GetOutputPort());
 
   VTK_CREATE(vtkActor, maceActor);
   maceActor->SetMapper(maceMapper);
   maceActor->VisibilityOn();
 
   // Create the RenderWindow, Renderer and both Actors
   //
   VTK_CREATE(vtkRenderer, ren1);
   VTK_CREATE(vtkRenderWindow, renWin);
   renWin->AddRenderer(ren1);
 
   VTK_CREATE(vtkRenderWindowInteractor, iren);
   iren->SetRenderWindow(renWin);

   // Outline is for debugging
   VTK_CREATE(vtkOutlineSource, outline);
   outline->SetBounds(bds);
 
   VTK_CREATE(vtkPolyDataMapper, outlineMapper);
   outlineMapper->SetInputConnection(outline->GetOutputPort());
 
   VTK_CREATE(vtkActor, outlineActor);
   outlineActor->SetMapper(outlineMapper);
 
   ren1->AddActor(maceActor);

   // Add the actors to the renderer, set the background and size
   //
   ren1->SetBackground(0.1, 0.2, 0.4);
   renWin->SetSize(300, 300);
 
   // render the image
   //
   iren->Initialize();
   renWin->Render();
 
   iren->Start();*/
}

MAF_REGISTER_TEST(vtkMAFVolumeSlicerTest);
#include "vtkMAFVolumeSlicerTest.moc"
