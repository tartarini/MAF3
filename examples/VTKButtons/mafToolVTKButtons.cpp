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
#include <QImage>

#include <vtkSmartPointer.h>
#include <vtkQImageToImageSource.h>

#include <vtkButtonWidget.h>
#include <vtkEllipticalButtonSource.h>
#include <vtkTexturedButtonRepresentation.h>

#include <vtkCommand.h>

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
    QImage image1;
    bool loaded = image1.load(":/images/spe.png");

    VTK_CREATE(vtkQImageToImageSource, imageToVTK1);
    imageToVTK1->SetQImage(&image1);
    imageToVTK1->Update();

    // Create an image for the button
    QImage image2;
    loaded = image2.load(":/images/fran_cut.png");

    VTK_CREATE(vtkQImageToImageSource, imageToVTK2);
    imageToVTK2->SetQImage(&image2);
    imageToVTK2->Update();

    VTK_CREATE(vtkEllipticalButtonSource, button);
    button->TwoSidedOn();
    button->SetCircumferentialResolution(24);
    button->SetShoulderResolution(24);
    button->SetTextureResolution(24);

    VTK_CREATE(vtkTexturedButtonRepresentation, rep);
    rep->SetNumberOfStates(2);
    rep->SetButtonTexture(0,imageToVTK2->GetOutput());
    rep->SetButtonTexture(1,imageToVTK1->GetOutput());
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
