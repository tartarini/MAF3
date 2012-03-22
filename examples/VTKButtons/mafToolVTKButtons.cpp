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

    vtkButtonCallback(): graphicObject(0), flyTo(true) {}
    QObject *graphicObject;
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
       
        if ( highlightState == vtkButtonRepresentation::HighlightHovering && previousHighlightState == vtkButtonRepresentation::HighlightNormal ) {
            //show tooltip (not if previous state was selecting
            toolButton->showTooltip();        
        } else if ( highlightState == vtkButtonRepresentation::HighlightNormal) {
            //hide tooltip
            toolButton->hideTooltip();
        } 
        previousHighlightState = highlightState;
    }

    vtkButtonHighLightCallback():toolButton(NULL), previousHighlightState(0) {}
    mafToolVTKButtons *toolButton;
    int previousHighlightState;
        
};

mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location), m_ShowLabel(true), m_FlyTo(true), m_OnCenter(false) {
    bool loaded = false;
    VTK_CREATE(vtkTexturedButtonRepresentation2D, rep);
    rep->SetNumberOfStates(1);

    buttonCallback = vtkButtonCallback::New();
    
    highlightCallback = vtkButtonHighLightCallback::New();
    highlightCallback->toolButton = this;

    m_ButtonWidget = vtkButtonWidget::New();
    m_ButtonWidget->SetRepresentation(rep);
    m_ButtonWidget->AddObserver(vtkCommand::StateChangedEvent,buttonCallback);
    rep->AddObserver(vtkCommand::HighlightEvent,highlightCallback);
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

   
    buttonCallback->graphicObject = this->m_GraphicObject;
    buttonCallback->setBounds(newBounds);
    buttonCallback->setFlyTo(m_FlyTo);
    updatedGraphicObject();
}

void mafToolVTKButtons::showTooltip() {
    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }

    QString matrixString = vme->dataSetCollection()->itemAtCurrentTime()->poseMatrixString();
    QString text("<b>Data type</b>: ");
    text.append(vme->dataSetCollection()->itemAtCurrentTime()->externalDataType());
    text.append("<br>");
    text.append("<b>Pose Matrix</b>: <br>");

    QStringList list = matrixString.split(" ");
    int numElement = list.count();
    int n = 0;
    int i = 0;
    for ( ; i < numElement; i++ ) {
        n++;
        text.append(list[i]);
        text.append(" ");
        if (n == 4 ) {
            text.append("<br>");
            n = 0;
        }
    }


    mafBounds *bounds = vme->dataSetCollection()->itemAtCurrentTime()->bounds();
    text.append("<b>Xmin</b>: ");
    text.append(QString::number(bounds->xMin()));
    text.append(" <b>Xmax</b>: ");
    text.append(QString::number(bounds->xMax()));
    text.append("<br>");

    text.append("<b>Ymin</b>: ");
    text.append(QString::number(bounds->yMin()));
    text.append(" <b>Ymax</b>: ");
    text.append(QString::number(bounds->yMax()));
    text.append("<br>");

    text.append("<b>Zmin</b>: ");
    text.append(QString::number(bounds->zMin()));
    text.append(" <b>Zmax</b>: ");
    text.append(QString::number(bounds->zMax()));
 

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString , text));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.showTooltip", mafEventBus::mafEventTypeLocal, &argList);
}

void mafToolVTKButtons::hideTooltip() {
    mafEventBus::mafEventArgumentsList argList;
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.hideTooltip", mafEventBus::mafEventTypeLocal, &argList);
}
