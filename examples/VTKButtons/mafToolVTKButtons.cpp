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
#include <QDir>
#include <mafVME.h>
#include <mafPoint.h>

#include "mafAnimateVTK.h"

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkQImageToImageSource.h>
#include <mafDataSet.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>

#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkBalloonRepresentation.h>
#include <vtkCommand.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;


// Callback respondign to vtkCommand::StateChangedEvent
class vtkButtonCallback : public vtkCommand {
public:
    static vtkButtonCallback *New() { 
        return new vtkButtonCallback; 
    }

    virtual void Execute(vtkObject *caller, unsigned long, void*) {
        mafVTKWidget *widget = qobject_cast<mafPluginVTK::mafVTKWidget *>(this->graphicObject);
        mafAnimateVTK *animateCamera = mafNEW(mafPluginVTK::mafAnimateVTK);
        //on zooming, don't show tooltip
        highligthButtonRepresentation->SetVisibility(0);
        if (flyTo) {
            animateCamera->flyTo(widget, bounds, 200);
        } else {
            widget->renderer()->ResetCamera(bounds);
        }
        mafDEL(animateCamera);
    }

    void setBounds(mafBounds *b) {
        bounds[0] = b->xMin(); 
        bounds[1] = b->xMax();
        bounds[2] = b->yMin();
        bounds[3] = b->yMax();
        bounds[4] = b->zMin();
        bounds[5] = b->zMax();
    }

    void setFlyTo(bool fly) {
        flyTo = fly;
    }

    vtkButtonCallback():graphicObject(0), highligthButtonRepresentation(NULL), flyTo(true) {}
    QObject *graphicObject;
    vtkTexturedButtonRepresentation2D *highligthButtonRepresentation;
    double bounds[6];
    bool flyTo;
};

// Callback respondign to vtkCommand::HighlightEvent
class vtkButtonHighLightCallback : public vtkCommand {
public:
    static vtkButtonHighLightCallback *New() { 
        return new vtkButtonHighLightCallback; 
    }

    virtual void Execute(vtkObject *caller, unsigned long, void*) {
        vtkTexturedButtonRepresentation2D *rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*>(caller);
        int highlightState = rep->GetHighlightState();
        
        if ( highlightState == vtkButtonRepresentation::HighlightHovering ) {
            highligthButtonRepresentation->SetVisibility(1);
        } else if ( highlightState == vtkButtonRepresentation::HighlightNormal) {
            highligthButtonRepresentation->SetVisibility(0);
        }

        mafVTKWidget *widget = qobject_cast<mafPluginVTK::mafVTKWidget *>(this->graphicObject);
        vtkRenderer *ren = widget->renderer();

        double pos[3];
        ren->SetWorldPoint(bounds[0], bounds[2], bounds[4], 1.0);
        ren->WorldToDisplay();
        ren->GetDisplayPoint(pos);

        //Set position of the Tooltip under the button
        double position[6];
        position[0] = pos[0];
        position[1] = pos[0];
        position[2] = pos[1]-35;
        position[3] = pos[1];
        position[4] = 0.0;
        position[5] = 0.0;
        highligthButtonRepresentation->PlaceWidget(position);

    }

    void setBounds(mafBounds *b) {
        bounds[0] = b->xMin(); 
        bounds[1] = b->xMax();
        bounds[2] = b->yMin();
        bounds[3] = b->yMax();
        bounds[4] = b->zMin();
        bounds[5] = b->zMax();
    }

    vtkButtonHighLightCallback():graphicObject(0), highligthButtonRepresentation(NULL){}
    QObject *graphicObject;
    vtkTexturedButtonRepresentation2D *highligthButtonRepresentation;
    double bounds[6];
    
};

mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location), m_ShowLabel(true), m_FlyTo(true), m_OnCenter(false) {
    bool loaded = false;
    VTK_CREATE(vtkTexturedButtonRepresentation2D, rep);
    rep->SetNumberOfStates(1);

    VTK_CREATE(vtkTexturedButtonRepresentation2D, repTooltip);
    repTooltip->SetNumberOfStates(1);

    // Uncomment this code to use a standard circle icon 
    // Create an image for the button
//     QImage image;
//     
//     QString pathStr = QDir::currentPath();
//     loaded = image.load(pathStr + "/" + "buttonIcon.png");
// 
//     if (!loaded) {
//         image.load(":/images/buttonIcon.png");
//     }
// 
//     VTK_CREATE(vtkQImageToImageSource, imageToVTK2);
//     imageToVTK2->SetQImage(&image);
//     imageToVTK2->Update();


    //rep->SetButtonTexture(0, imageToVTK2->GetOutput());
    buttonCallback = vtkButtonCallback::New();
    
    buttonCallback->highligthButtonRepresentation = repTooltip;
    highlightCallback = vtkButtonHighLightCallback::New();
    highlightCallback->highligthButtonRepresentation = repTooltip;

    m_ButtonWidget = vtkButtonWidget::New();
    m_ButtonWidget->SetRepresentation(rep);
    m_ButtonWidget->AddObserver(vtkCommand::StateChangedEvent,buttonCallback);

    m_TooltipWidget = vtkButtonWidget::New();
    m_TooltipWidget->SetRepresentation(repTooltip);
    repTooltip->SetVisibility(0);
    
    rep->AddObserver(vtkCommand::HighlightEvent,highlightCallback);
}

mafToolVTKButtons::~mafToolVTKButtons() {
    m_ButtonWidget->Delete();
    m_TooltipWidget->Delete();
}

void mafToolVTKButtons::resetTool() {
    removeWidget(m_ButtonWidget);
    removeWidget(m_TooltipWidget);
}

void mafToolVTKButtons::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the widget.
    addWidget(m_ButtonWidget);
    addWidget(m_TooltipWidget);
}

void mafToolVTKButtons::updatePipe(double t) {
    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }
    
    //Get absolute pose matrix from mafVMEManager
    mafObjectBase *obj = vme;
    mafMatrixPointer absMatrix = NULL;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);

    //Transform bounds with absMatrix
    double b[6];
    vme->bounds(b, t);
    mafBounds *newBounds = new mafBounds(b);
    if (absMatrix) {
        newBounds->transformBounds(absMatrix);
        delete absMatrix;
        absMatrix = NULL;
    }

    ///////////-------- BUTTON WIDGET -----------////////////
    vtkTexturedButtonRepresentation2D *rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*>(m_ButtonWidget->GetRepresentation());

    //Load image only the first time
    if (m_IconFileName.isEmpty()) {
        QImage image;
        QString iconType = vme->property("iconType").toString();
        m_IconFileName = mafIconFromObjectType(iconType);
        image.load(m_IconFileName);
        VTK_CREATE(vtkQImageToImageSource, imageToVTK);
        imageToVTK->SetQImage(&image);
        imageToVTK->Update();
        rep->SetButtonTexture(0, imageToVTK->GetOutput());
    }

    int size[2]; size[0] = 16; size[1] = 16;
    rep->GetBalloon()->SetImageSize(size);

    if (m_ShowLabel) {
        //Add a label to the button and change its text property
        QString vmeName = vme->property("objectName").toString();
        rep->GetBalloon()->SetBalloonText(vmeName.toAscii());
        vtkTextProperty *textProp = rep->GetBalloon()->GetTextProperty();
        rep->GetBalloon()->SetPadding(2);
        textProp->SetFontSize(13);
        textProp->BoldOff();
        //textProp->SetColor(0.9,0.9,0.9);

        //Set label position
        rep->GetBalloon()->SetBalloonLayoutToImageLeft();

        //This method allows to set the label's background opacity
        rep->GetBalloon()->GetFrameProperty()->SetOpacity(0.65);
    } else {
        rep->GetBalloon()->SetBalloonText("");
    }

    //modify position of the vtkButton 
    double bds[3];
    if (m_OnCenter) {
        newBounds->center(bds);
    } else {
        //on the corner of the bounding box of the VME.
        bds[0] = newBounds->xMin();
        bds[1] = newBounds->yMin(); 
        bds[2] = newBounds->zMin();
    }
    rep->PlaceWidget(bds, size);
    rep->Modified();
    m_ButtonWidget->SetRepresentation(rep);
    ///////////-------- BUTTON WIDGET -----------////////////

    ///////////-------- TOOLTIP WIDGET -----------////////////
    vtkTexturedButtonRepresentation2D *repTooltip = reinterpret_cast<vtkTexturedButtonRepresentation2D*>(m_TooltipWidget->GetRepresentation());
    //Add text to the tooltip
    QString tooltipString("Data type: ");
    tooltipString.append(vme->dataSetCollection()->itemAtCurrentTime()->externalDataType());
    tooltipString.append("\nPose matrix: ");
    tooltipString.append(vme->dataSetCollection()->itemAtCurrentTime()->poseMatrixString());
    repTooltip->GetBalloon()->SetBalloonText(tooltipString.toAscii());
    vtkTextProperty *textPropTooltip = repTooltip->GetBalloon()->GetTextProperty();
    repTooltip->GetBalloon()->SetPadding(2);
    textPropTooltip->SetFontSize(10);
    textPropTooltip->BoldOff();
    //textProp->SetColor(0.9,0.9,0.9);

    //Set label position
    repTooltip->GetBalloon()->SetBalloonLayoutToImageLeft();

    //Set the label's background property
    //repTooltip->GetBalloon()->GetFrameProperty()->SetColor(0.77, 0.88, 1.0);
    repTooltip->GetBalloon()->GetFrameProperty()->SetOpacity(0.65);

//     //Set position on the Tooltip under the button
//     bds[1] = bds[1] - 0.2;
//     repTooltip->PlaceWidget(bds, size);

    repTooltip->Modified();
    m_TooltipWidget->SetRepresentation(repTooltip);
    ///////////-------- TOOLTIP WIDGET -----------////////////

    highlightCallback->graphicObject = this->m_GraphicObject;
    highlightCallback->setBounds(newBounds);
    
    buttonCallback->graphicObject = this->m_GraphicObject;
    buttonCallback->setBounds(newBounds);
    buttonCallback->setFlyTo(m_FlyTo);
    updatedGraphicObject();
}


