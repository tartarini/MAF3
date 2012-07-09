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

#include <msvQVTKButtons.h>

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

mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location)/*, m_ShowLabel(true), m_FlyTo(true), m_OnCenter(false)*/ {

}

msvQVTKButtons *mafToolVTKButtons::button() {
    if(m_Button == NULL) {
        m_Button = new msvQVTKButtons();
        QObject::connect(this, SIGNAL(hideTooltipSignal), m_Button, SIGNAL(hideTooltip));
    }
    return m_Button;
}

mafToolVTKButtons::~mafToolVTKButtons() {
    if(m_Button) {
        delete m_Button;
    }
}

void mafToolVTKButtons::resetTool() {
    removeWidget(m_Button->button());
}

void mafToolVTKButtons::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the widget.
    addWidget(m_Button->button());
}

void mafToolVTKButtons::updatePipe(double t) {
    /*mafVME *vme = input();
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
    updatedGraphicObject();*/
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

     QStringList list = matrixString.split(" ");
     int numElement = list.count();
     int i = 0;

     text.append("<b>Pose Matrix</b>:");
     text.append("<table border=\"0.2\">");
     for ( ; i < numElement; i++ ) {
         text.append("<tr>");
         text.append("<td>" + list[i] +"</td>");
         i++;
         text.append("<td>" + list[i] +"</td>");
         i++;
         text.append("<td>" + list[i] +"</td>");
         i++;
         text.append("<td>" + list[i] +"</td>");
         text.append("</tr>");
     }
     text.append("</table>");

    mafBounds *bounds = vme->dataSetCollection()->itemAtCurrentTime()->bounds();
    text.append("<b>Bounds: (min - max)</b>:");
    text.append("<table border=\"0.2\">");
    text.append("<tr>");
    text.append("<td>" + QString::number(bounds->xMin()) +"</td>");
    text.append("<td>" + QString::number(bounds->xMax()) +"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td>" + QString::number(bounds->yMin()) +"</td>");
    text.append("<td>" + QString::number(bounds->yMax()) +"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td>" + QString::number(bounds->zMin()) +"</td>");
    text.append("<td>" + QString::number(bounds->zMax()) +"</td>");
    text.append("</tr>");
    text.append("</table>");
 
    button()->setToolTip(text);
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString , text));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.showTooltip", mafEventBus::mafEventTypeLocal, &argList);
}

void mafToolVTKButtons::hideTooltip() {
    mafEventBus::mafEventArgumentsList argList;
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.hideTooltip", mafEventBus::mafEventTypeLocal, &argList);
}

void mafToolVTKButtons::selectVME() {
    //select the VME associated to the button pressed
    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }
    mafCore::mafObjectBase *objBase = vme;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase*, objBase));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
}

void mafToolVTKButtons::setShowButton(bool show) {
    button()->setShowButton(show);
}

bool mafToolVTKButtons::showButton() {
    return button()->showButton();
}

void mafToolVTKButtons::setShowLabel(bool show) {
    button()->setShowLabel(show);
}

bool mafToolVTKButtons::showLabel() {
    return button()->showLabel();
}

void mafToolVTKButtons::setFlyTo(bool active) {
    button()->setFlyTo(active);
}

bool mafToolVTKButtons::FlyTo() {
    return button()->FlyTo();
}

void mafToolVTKButtons::setOnCenter(bool onCenter) {
    button()->setOnCenter(onCenter);
}

bool mafToolVTKButtons::OnCenter() {
    return button()->OnCenter();
}
