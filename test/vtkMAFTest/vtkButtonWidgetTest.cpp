/*
 *  vtkButtonWidgetTest.cpp
 *  vtkMAF
 *
 *  Created by Paolo Quadrani on 28/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>

#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>

#define VTK_DATA_DIR "C:\\Devel\\MAF3\\TestData\\vtkdata-5.8.0\\"

using namespace mafCore;

#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkAppendPolyData.h"
#include "vtkSphereSource.h"

#include "vtkButtonWidget.h"
#include "vtkTexturedButtonRepresentation.h"
#include "vtkTexturedButtonRepresentation2D.h"
#include "vtkProp3DButtonRepresentation.h"

#include "vtkEllipticalButtonSource.h"

#include "vtkInteractorEventRecorder.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkLookupTable.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTIFFReader.h"
#include "vtkPNGReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeTextureMapper2D.h"
#include "vtkVolume.h"
#include "vtkOutlineSource.h"
#include "vtkPlatonicSolidSource.h"
#include "vtkSmartPointer.h"
#include "vtkTestUtilities.h"

#define VTK_CREATE(type, name) \
   vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

 // This does the actual work: updates the vtkPlane implicit function.
 // This in turn causes the pipeline to update and clip the object.
 // Callback for the interaction
class vtkButtonCallback : public vtkCommand {
public:
    static vtkButtonCallback *New()
     { return new vtkButtonCallback; }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
     {
       vtkButtonWidget *buttonWidget = reinterpret_cast<vtkButtonWidget*>(caller);
       vtkTexturedButtonRepresentation *rep = reinterpret_cast<vtkTexturedButtonRepresentation*>(buttonWidget->GetRepresentation());
       int state = rep->GetState();
       cout << "State: " << state << "\n";
       this->Glyph->SetScaleFactor(0.05*(1+state));
     }
    vtkButtonCallback():Glyph(0) {}
    vtkGlyph3D *Glyph;
};


/**
 Class name: mafExporterVTKTest
 This class implements the test suite for mafImporterVTK.
 */
class vtkButtonWidgetTest: public QObject {
    Q_OBJECT
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }
    
    /// Test the import of VTK file created.
    void TestButtonWidget();
};


void vtkButtonWidgetTest::TestButtonWidget() {
   // Create an image for the button
   QString fname(VTK_DATA_DIR);
   fname.append("Data/beach.tif");
   
   fname = QDir::toNativeSeparators(fname);
   VTK_CREATE(vtkTIFFReader, image1);
   image1->SetFileName(fname.toAscii().data());
   image1->SetOrientationType( 4 );
   image1->Update();
 
   // Create an image for the button
   QString fname2(VTK_DATA_DIR);
   fname2.append("Data/fran_cut.png");

   fname2 = QDir::toNativeSeparators(fname2);
   VTK_CREATE(vtkPNGReader, image2);
   image2->SetFileName(fname2.toAscii().data());
   image2->Update();
 
   // Create a mace out of filters.
   //
   VTK_CREATE(vtkSphereSource, sphere);
   VTK_CREATE(vtkConeSource, cone);
   VTK_CREATE(vtkGlyph3D, glyph);
   glyph->SetInputConnection(sphere->GetOutputPort());
   glyph->SetSource(cone->GetOutput());
   glyph->SetVectorModeToUseNormal();
   glyph->SetScaleModeToScaleByVector();
   glyph->SetScaleFactor(0.25);
   glyph->Update();
 
   // Appending just makes things simpler to manage.
   VTK_CREATE(vtkAppendPolyData, apd);
   apd->AddInput(glyph->GetOutput());
   apd->AddInput(sphere->GetOutput());
 
   VTK_CREATE(vtkPolyDataMapper, maceMapper);
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
 
   // The SetInteractor method is how 3D widgets are associated with the render
   // window interactor. Internally, SetInteractor sets up a bunch of callbacks
   // using the Command/Observer mechanism (AddObserver()).
   VTK_CREATE(vtkButtonCallback, myCallback);
   myCallback->Glyph = glyph;
 
   ////////////////////////////////////////////////////////////////////////////////////////////
   VTK_CREATE(vtkEllipticalButtonSource, button);
   button->TwoSidedOn();
   button->SetCircumferentialResolution(24);
   button->SetShoulderResolution(24);
   button->SetTextureResolution(24);
 
   VTK_CREATE(vtkTexturedButtonRepresentation, rep);
   rep->SetNumberOfStates(2);
   rep->SetButtonTexture(0,image1->GetOutput());
   rep->SetButtonTexture(1,image2->GetOutput());
   rep->SetButtonGeometryConnection(button->GetOutputPort());
   rep->SetPlaceFactor(1);
   double bds[6];
   bds[0] = 0.6; bds[1] = 0.75; bds[2] = 0.6; bds[3] = 0.75; bds[4] = 0.6; bds[5] = 0.75;
   rep->PlaceWidget(bds);
   rep->FollowCameraOn();
 
   VTK_CREATE(vtkButtonWidget, buttonWidget);
   buttonWidget->SetInteractor(iren);
   buttonWidget->SetRepresentation(rep);
   buttonWidget->AddObserver(vtkCommand::StateChangedEvent,myCallback);
   ////////////////////////////////////////////////////////////////////////////////////////////
 
   // Another 3D button widget, this time use alternative PlaceWidget() method
   ////////////////////////////////////////////////////////////////////////////////////////////
   VTK_CREATE(vtkEllipticalButtonSource, button2);
   button2->TwoSidedOn();
   button2->SetCircumferentialResolution(24);
   button2->SetShoulderResolution(24);
   button2->SetTextureResolution(24);
   button2->SetWidth(0.65);
   button2->SetHeight(0.45);
   button2->SetTextureStyleToFitImage();
 
   VTK_CREATE(vtkTexturedButtonRepresentation, rep2);
   rep2->SetNumberOfStates(2);
   rep2->SetButtonTexture(0,image1->GetOutput());
   rep2->SetButtonTexture(1,image2->GetOutput());
   rep2->SetButtonGeometryConnection(button2->GetOutputPort());
   rep2->SetPlaceFactor(1);
   bds[0] = 0.0; bds[1] = 0.0; bds[2] = 0.65; bds[3] = 0.0; bds[4] = 0.0; bds[5] = 1;
   rep2->PlaceWidget(0.5, bds, bds+3);
   rep2->FollowCameraOff();
 
   VTK_CREATE(vtkButtonWidget, buttonWidget2);
   buttonWidget2->SetInteractor(iren);
   buttonWidget2->SetRepresentation(rep2);
   buttonWidget2->AddObserver(vtkCommand::StateChangedEvent,myCallback);
   ////////////////////////////////////////////////////////////////////////////////////////////
 
   // Okay now for the 2D version of the widget (in display space)
   ////////////////////////////////////////////////////////////////////////////////////////////
   VTK_CREATE(vtkTexturedButtonRepresentation2D, rep3);
   rep3->SetNumberOfStates(2);
   rep3->SetButtonTexture(0,image1->GetOutput());
   rep3->SetButtonTexture(1,image2->GetOutput());
   rep3->SetPlaceFactor(1);
   bds[0] = 25; bds[1] = 65; bds[2] = 50; bds[3] = 200;
   rep3->PlaceWidget(bds);
 
   VTK_CREATE(vtkButtonWidget, buttonWidget3);
   buttonWidget3->SetInteractor(iren);
   buttonWidget3->SetRepresentation(rep3);
   buttonWidget3->AddObserver(vtkCommand::StateChangedEvent,myCallback);
   ////////////////////////////////////////////////////////////////////////////////////////////

   // Okay now for the 2D version of the widget (in world space)
   ////////////////////////////////////////////////////////////////////////////////////////////
   VTK_CREATE(vtkTexturedButtonRepresentation2D, rep4);
   rep4->SetNumberOfStates(2);
   rep4->SetButtonTexture(0,image1->GetOutput());
   rep4->SetButtonTexture(1,image2->GetOutput());
   rep4->SetPlaceFactor(1);
   bds[0] = 0.75; bds[1] = 0; bds[2] = 0;
   int size[2]; size[0] = 25; size[1] = 45;
   rep4->PlaceWidget(bds,size);
 
   VTK_CREATE(vtkButtonWidget, buttonWidget4);
   buttonWidget4->SetInteractor(iren);
   buttonWidget4->SetRepresentation(rep4);
   buttonWidget4->AddObserver(vtkCommand::StateChangedEvent,myCallback);
   ////////////////////////////////////////////////////////////////////////////////////////////

   // Finally now a set of vtkProp3D's to define a vtkProp3DButtonRepresentation
   ////////////////////////////////////////////////////////////////////////////////////////////
   VTK_CREATE(vtkLookupTable,lut);
   lut->SetNumberOfColors(20);
   lut->Build();
   lut->SetTableValue(0, 1, 0, 0, 1);
   lut->SetTableValue(1, 0, 1, 0, 1);
   lut->SetTableValue(2, 1, 1, 0, 1);
   lut->SetTableValue(3, 0, 0, 1, 1);
   lut->SetTableValue(4, 1, 0, 1, 1);
   lut->SetTableValue(5, 0, 1, 1, 1);
   lut->SetTableValue(6, 0.0000, 1.0000, 0.4980, 1.0);
   lut->SetTableValue(7, 0.9020, 0.9020, 0.9804, 1.0);
   lut->SetTableValue(8, 0.9608, 1.0000, 0.9804, 1.0);
   lut->SetTableValue(9, 0.5600, 0.3700, 0.6000, 1.0);
   lut->SetTableValue(10, 0.1600, 0.1400, 0.1300, 1.0);
   lut->SetTableValue(11, 1.0000, 0.4980, 0.3137, 1.0);
   lut->SetTableValue(12, 1.0000, 0.7529, 0.7961, 1.0);
   lut->SetTableValue(13, 0.9804, 0.5020, 0.4471, 1.0);
   lut->SetTableValue(14, 0.3700, 0.1500, 0.0700, 1.0);
   lut->SetTableValue(15, 0.9300, 0.5700, 0.1300, 1.0);
   lut->SetTableValue(16, 1.0000, 0.8431, 0.0000, 1.0);
   lut->SetTableValue(17, 0.1333, 0.5451, 0.1333, 1.0);
   lut->SetTableValue(18, 0.2510, 0.8784, 0.8157, 1.0);
   lut->SetTableValue(19, 0.8667, 0.6275, 0.8667, 1.0);
   lut->SetTableRange(0, 19);
 
   VTK_CREATE(vtkPlatonicSolidSource,tet);
   tet->SetSolidTypeToTetrahedron();
   VTK_CREATE(vtkPolyDataMapper,tetMapper);
   tetMapper->SetInputConnection(tet->GetOutputPort());
   tetMapper->SetLookupTable(lut);
   tetMapper->SetScalarRange(0,19);
   VTK_CREATE(vtkActor,tetActor);
   tetActor->SetMapper(tetMapper);
 
   VTK_CREATE(vtkPlatonicSolidSource,cube);
   cube->SetSolidTypeToCube();
   VTK_CREATE(vtkPolyDataMapper,cubeMapper);
   cubeMapper->SetInputConnection(cube->GetOutputPort());
   cubeMapper->SetLookupTable(lut);
   cubeMapper->SetScalarRange(0,19);
   VTK_CREATE(vtkActor,cubeActor);
   cubeActor->SetMapper(cubeMapper);
 
   VTK_CREATE(vtkPlatonicSolidSource,oct);
   oct->SetSolidTypeToOctahedron();
   VTK_CREATE(vtkPolyDataMapper,octMapper);
   octMapper->SetInputConnection(oct->GetOutputPort());
   octMapper->SetLookupTable(lut);
   octMapper->SetScalarRange(0,19);
   VTK_CREATE(vtkActor,octActor);
   octActor->SetMapper(octMapper);
 
   VTK_CREATE(vtkPlatonicSolidSource,ico);
   ico->SetSolidTypeToIcosahedron();
   VTK_CREATE(vtkPolyDataMapper,icoMapper);
   icoMapper->SetInputConnection(ico->GetOutputPort());
   icoMapper->SetLookupTable(lut);
   icoMapper->SetScalarRange(0,19);
   VTK_CREATE(vtkActor,icoActor);
   icoActor->SetMapper(icoMapper);
 
   VTK_CREATE(vtkPlatonicSolidSource,dode);
   dode->SetSolidTypeToDodecahedron();
   VTK_CREATE(vtkPolyDataMapper,dodeMapper);
   dodeMapper->SetInputConnection(dode->GetOutputPort());
   dodeMapper->SetLookupTable(lut);
   dodeMapper->SetScalarRange(0,19);
   VTK_CREATE(vtkActor,dodeActor);
   dodeActor->SetMapper(dodeMapper);
 
   VTK_CREATE(vtkProp3DButtonRepresentation, rep5);
   rep5->SetNumberOfStates(5);
   rep5->SetButtonProp(0,tetActor);
   rep5->SetButtonProp(1,cubeActor);
   rep5->SetButtonProp(2,octActor);
   rep5->SetButtonProp(3,icoActor);
   rep5->SetButtonProp(4,dodeActor);
   rep5->SetPlaceFactor(1);
   bds[0] = 0.65; bds[1] = 0.75; bds[2] = -0.75; bds[3] = -0.65; bds[4] = 0.65; bds[5] = 0.75;
   rep5->PlaceWidget(bds);
   rep5->FollowCameraOn();
 
   VTK_CREATE(vtkButtonWidget, buttonWidget5);
   buttonWidget5->SetInteractor(iren);
   buttonWidget5->SetRepresentation(rep5);
   buttonWidget5->AddObserver(vtkCommand::StateChangedEvent,myCallback);
 
   // A volume rendered button!
   // Create the reader for the data
   QString fname3(VTK_DATA_DIR);
   fname3.append("Data/ironProt.vtk");

   fname3 = QDir::toNativeSeparators(fname3);
   VTK_CREATE(vtkStructuredPointsReader, reader);
   reader->SetFileName(fname3.toAscii().data());
   
   // Create transfer mapping scalar value to opacity
   VTK_CREATE(vtkPiecewiseFunction, opacityTransferFunction);
   opacityTransferFunction->AddPoint(20,0);
   opacityTransferFunction->AddPoint(255,1);
 
   // Create transfer mapping scalar value to color
   VTK_CREATE(vtkColorTransferFunction, colorTransferFunction);
   colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
   colorTransferFunction->AddRGBPoint(64.0, 1.0, 0.0, 0.0);
   colorTransferFunction->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
   colorTransferFunction->AddRGBPoint(192.0, 0.0, 1.0, 0.0);
   colorTransferFunction->AddRGBPoint(255.0, 0.0, 0.2, 0.0);
 
   // The property describes how the data will look
   VTK_CREATE(vtkVolumeProperty, volumeProperty);
   volumeProperty->SetColor(colorTransferFunction);
   volumeProperty->SetScalarOpacity(opacityTransferFunction);
   volumeProperty->ShadeOn();
   volumeProperty->SetInterpolationTypeToLinear();
 
   // The mapper / ray cast function know how to render the data
   VTK_CREATE(vtkVolumeTextureMapper2D, volumeMapper);
   volumeMapper->SetInputConnection(reader->GetOutputPort());
 
   // The volume holds the mapper and the property and
   // can be used to position/orient the volume
   VTK_CREATE(vtkVolume, volume);
   volume->SetMapper(volumeMapper);
   volume->SetProperty(volumeProperty);
 
   // Create transfer mapping scalar value to color
   VTK_CREATE(vtkColorTransferFunction, colorTransferFunction2);
   colorTransferFunction2->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
   colorTransferFunction2->AddRGBPoint(64.0, 0.0, 0.0, 1.0);
   colorTransferFunction2->AddRGBPoint(128.0, 1.0, 0.0, 1.0);
   colorTransferFunction2->AddRGBPoint(192.0, 0.5, 0.0, 0.5);
   colorTransferFunction2->AddRGBPoint(255.0, 0.2, 0.0, 0.2);
 
   // The property describes how the data will look
   VTK_CREATE(vtkVolumeProperty, volumeProperty2);
   volumeProperty2->SetColor(colorTransferFunction2);
   volumeProperty2->SetScalarOpacity(opacityTransferFunction);
   volumeProperty2->ShadeOn();
   volumeProperty2->SetInterpolationTypeToLinear();
 
   // The mapper / ray cast function know how to render the data
   VTK_CREATE(vtkVolumeTextureMapper2D, volumeMapper2);
   volumeMapper2->SetInputConnection(reader->GetOutputPort());
 
   VTK_CREATE(vtkVolume, volume2);
   volume2->SetMapper(volumeMapper2);
   volume2->SetProperty(volumeProperty2);
 
   VTK_CREATE(vtkProp3DButtonRepresentation, rep6);
   rep6->SetNumberOfStates(2);
   rep6->SetButtonProp(0,volume);
   rep6->SetButtonProp(1,volume2);
   rep6->SetPlaceFactor(1);
   bds[0] = -0.75; bds[1] = -0.35; bds[2] = 0.6; bds[3] = 1; bds[4] = -1; bds[5] = -0.6;
   rep6->PlaceWidget(bds);
   rep6->FollowCameraOn();
 
   VTK_CREATE(vtkButtonWidget, buttonWidget6);
   buttonWidget6->SetInteractor(iren);
   buttonWidget6->SetRepresentation(rep6);
   buttonWidget6->AddObserver(vtkCommand::StateChangedEvent,myCallback);
   ////////////////////////////////////////////////////////////////////////////////////////////
 
   // Outline is for debugging
   VTK_CREATE(vtkOutlineSource, outline);
   outline->SetBounds(bds);
 
   VTK_CREATE(vtkPolyDataMapper, outlineMapper);
   outlineMapper->SetInputConnection(outline->GetOutputPort());
 
   VTK_CREATE(vtkActor, outlineActor);
   outlineActor->SetMapper(outlineMapper);
 
   ren1->AddActor(maceActor);
 //  ren1->AddActor(outlineActor); //used for debugging
 
   // Add the actors to the renderer, set the background and size
   //
   ren1->SetBackground(0.1, 0.2, 0.4);
   renWin->SetSize(300, 300);
 
   // render the image
   //
   iren->Initialize();
   renWin->Render();

   buttonWidget->EnabledOn();
   buttonWidget2->EnabledOn();
   buttonWidget3->EnabledOn();
   buttonWidget4->EnabledOn();
   buttonWidget5->EnabledOn();
   buttonWidget6->EnabledOn();
 
   iren->Start();
}

MAF_REGISTER_TEST(vtkButtonWidgetTest);
#include "vtkButtonWidgetTest.moc"
