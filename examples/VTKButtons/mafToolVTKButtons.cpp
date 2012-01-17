/*
 *  mafToolVTKButtons.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 13/01/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKButtons.h"
#include <mafSceneNodeVTK.h>

#include <vtkSmartPointer.h>
#include <vtkTIFFReader.h>
#include <vtkPNGReader.h>

#include <vtkButtonWidget.h>
#include <vtkEllipticalButtonSource.h>
#include <vtkTexturedButtonRepresentation.h>
//#include <vtkTexturedButtonRepresentation2D.h>

#include <vtkCommand.h>


#define VTK_CREATE(obj_type, obj_name) vtkSmartPointer<obj_type> obj_name = vtkSmartPointer<obj_type>::New()

//using namespace mafPluginVTK;

// Callback for the interaction
class vtkButtonCallback : public vtkCommand {
public:
    static vtkButtonCallback *New() { 
        return new vtkButtonCallback; 
    }

    virtual void Execute(vtkObject *caller, unsigned long, void*) {
        vtkButtonWidget *buttonWidget = reinterpret_cast<vtkButtonWidget*>(caller);
        vtkTexturedButtonRepresentation *rep = reinterpret_cast<vtkTexturedButtonRepresentation*>(buttonWidget->GetRepresentation());
        int state = rep->GetState();
        qDebug() << QString("state: %1").arg(state);
    }
};


mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location) {
    // Create an image for the button
    QString fname("./Images/beach.tif");
    fname = QDir::toNativeSeparators(fname);
    QByteArray ba = fname.toAscii();

    VTK_CREATE(vtkTIFFReader, image1);
    image1->SetFileName(ba.data());
    image1->SetOrientationType( 4 );
    image1->Update();

    // Create an image for the button
    QString fname2("./Images/fran_cut.png");
    fname2 = QDir::toNativeSeparators(fname2);
    ba = fname2.toAscii();

    VTK_CREATE(vtkPNGReader, image2);
    image2->SetFileName(ba.data());
    image2->Update();

//     VTK_CREATE(vtkTexturedButtonRepresentation2D, rep);
//     rep->SetNumberOfStates(2);
//     rep->SetButtonTexture(0,image1->GetOutput());
//     rep->SetButtonTexture(1,image2->GetOutput());
//     rep->SetPlaceFactor(1);
//     double bds[6]; 
//     bds[0] = 25; bds[1] = 65; bds[2] = 50; bds[3] = 200;
//     rep->PlaceWidget(bds);

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

    VTK_CREATE(vtkButtonCallback, myCallback);

    m_ButtonWidget = vtkButtonWidget::New();
    m_ButtonWidget->SetRepresentation(rep);
    m_ButtonWidget->AddObserver(vtkCommand::StateChangedEvent,myCallback);
}

mafToolVTKButtons::~mafToolVTKButtons() {
    m_ButtonWidget->Delete();
}

void mafToolVTKButtons::resetTool() {
    removeWidget(m_ButtonWidget);
}

void mafToolVTKButtons::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the widget.
    addWidget(m_ButtonWidget);
}
