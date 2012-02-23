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
#include <mafVME.h>

#include "mafAnimateVTK.h"

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkQImageToImageSource.h>
#include <mafDataSet.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>

#include <vtkButtonWidget.h>
#include <vtkEllipticalButtonSource.h>
#include <vtkTexturedButtonRepresentation.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkBalloonRepresentation.h>

#include <vtkCommand.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

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

        mafVTKWidget *widget = qobject_cast<mafPluginVTK::mafVTKWidget *>(this->graphicObject);
        mafAnimateVTK *animateCamera = mafNEW(mafPluginVTK::mafAnimateVTK);
        if (flyTo) {
            animateCamera->flyTo(widget, bounds, 200);
        } else {
            widget->renderer()->ResetCamera(bounds);
        }
        
        mafDEL(animateCamera);
    }

    void setBounds(double b[6]) {
        for (int i = 0; i <6; i++) {
            bounds[i] = b[i];
        }
    }

    void setFlyTo(bool fly) {
        flyTo = fly;
    }

    vtkButtonCallback():graphicObject(0), flyTo(true) {}
    QObject *graphicObject;
    double bounds[6];
    bool flyTo;
};

mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location), m_ShowLabel(true), m_FlyTo(true), m_OnCenter(false) {
    // Create an image for the button
    QImage image2;
    image2.load(":/images/fran_cut.png");

    VTK_CREATE(vtkQImageToImageSource, imageToVTK2);
    imageToVTK2->SetQImage(&image2);
    imageToVTK2->Update();

    VTK_CREATE(vtkTexturedButtonRepresentation2D, rep);
    rep->SetNumberOfStates(1);
    rep->SetButtonTexture(0, imageToVTK2->GetOutput());
    rep->SetPlaceFactor(1);

    myCallback = vtkButtonCallback::New();

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

void mafToolVTKButtons::updatePipe(double t) {
    mafVME *vme = input();
    QString vmeName = vme->property("objectName").toString();
    mafDataSet *data = dataSetForInput(0, t);
    if(data == NULL) {
        return;
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    if (dataSet == NULL) {
        resetTool();
    } else {
        vtkAlgorithm *producer = (*dataSet)->GetProducer();
        vtkDataObject *dataObject = producer->GetOutputDataObject(0);
        vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);

        double b[6];
        vtkData->GetBounds(b);
        vtkTexturedButtonRepresentation2D *rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*>(m_ButtonWidget->GetRepresentation());

        if (m_ShowLabel) {
            //Add a label to the button and change its text property
            rep->GetBalloon()->SetBalloonText(vmeName.toAscii());
            vtkTextProperty *textProp = rep->GetBalloon()->GetTextProperty();
            textProp->SetFontSize(12);
            //textProp->SetColor(0.9,0.9,0.9);

            //Set label position
            rep->GetBalloon()->SetBalloonLayoutToImageLeft();

            //This method allows to set opacity of the background od the label
            rep->GetBalloon()->GetFrameProperty()->SetOpacity(0.65);
        } else {
            rep->GetBalloon()->SetBalloonText("");
        }

        //modify position of the vtkButton 
        double bds[6];
        if (m_OnCenter) {
            //on the center of the bounding box of the VME.
            bds[0] = (b[0] + b[1]) / 2;
            bds[1] = (b[2] + b[3]) / 2;
            bds[2] = (b[4] + b[5]) / 2;
        } else {
            //on the corner of the bounding box of the VME.
            bds[0] = b[0]; bds[1] = b[2]; bds[2] = b[4];
        }

        int size[2]; size[0] = 25; size[1] = 45;

        rep->PlaceWidget(bds,size);
        m_ButtonWidget->SetRepresentation(rep);

        myCallback->graphicObject = this->m_GraphicObject;
        myCallback->setBounds(b);
        myCallback->setFlyTo(m_FlyTo);
    }
    updatedGraphicObject();
}

