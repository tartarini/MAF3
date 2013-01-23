/*
 *  mafToolVTKButtons.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 13/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
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
#include <vtkDataSet.h>

#include <mafVTKWidget.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

mafToolVTKButtons::mafToolVTKButtons(const QString code_location) : mafPluginVTK::mafToolVTK(code_location), isLoaded(false), m_Button(NULL){

}

msvQVTKButtons *mafToolVTKButtons::button() {
    if(m_Button == NULL) {
        m_Button = new msvQVTKButtons();
        m_Button->setShowButton(true);
        bool result = QObject::connect(m_Button, SIGNAL(showTooltip(QString)), this, SLOT(showTooltip(QString)));
        result = QObject::connect(m_Button, SIGNAL(hideTooltip()), this, SLOT(hideTooltip()));
    }
    return m_Button;
}

mafToolVTKButtons::~mafToolVTKButtons() {
    if(m_Button) {
        delete m_Button;
    }
}

void mafToolVTKButtons::resetTool() {
    removeWidget(button()->button());
}

void mafToolVTKButtons::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the widget.
    addWidget(button()->button());
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
    vtkTexturedButtonRepresentation2D *rep = static_cast<vtkTexturedButtonRepresentation2D *> (button()->button()->GetRepresentation());

    //Load image only the first time
    if (isLoaded == false) {
        QString iconType = vme->property("iconType").toString();
        if(iconType.compare("")!=0)
        {
            QString iconFileName = mafIconFromObjectType(iconType);
            button()->setIconFileName(iconFileName);

            isLoaded = true;
        }
    }

    int size[2]; size[0] = 16; size[1] = 16;
    rep->GetBalloon()->SetImageSize(size);

    if (showLabel()) {
        QString vmeName = vme->property("objectName").toString();
        button()->setLabel(vmeName);
    } else {
        QString emptyString;
        button()->setLabel(emptyString);
    }

    //modify position of the vtkButton 
    double bds[3];
    if (onCenter()) {
        newBounds->center(bds);
    } else {
        //on the corner of the bounding box of the VME.
        bds[0] = newBounds->xMin();
        bds[1] = newBounds->yMin(); 
        bds[2] = newBounds->zMin();
    }
    rep->PlaceWidget(bds, size);
    rep->Modified();
    button()->button()->SetRepresentation(rep);
    ///////////-------- BUTTON WIDGET -----------////////////

    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(this->graphicObject());

    button()->setCurrentRenderer(widget->renderer());
    double b6[6];
    newBounds->bounds(b6);
    button()->setBounds(b6);
    button()->setFlyTo(FlyTo());
    updatedGraphicObject();


    mafProxyInterface *pi =  vme->dataSetCollection()->itemAtCurrentTime()->dataValue();
    if(pi == NULL) {
        return;
    }

    QString dt = pi->externalDataType();
    char *m = dt.toAscii().data();

    mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, pi);
    if(dataSet && dt.contains("vtkAlgorithmOutput"))
    {
      vtkAlgorithm *producer = (*dataSet)->GetProducer();
      //
      if(producer == NULL) {
          return;
      }
      vtkDataObject *dataObject = producer->GetOutputDataObject(0);
      vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);

      button()->setData(vtkData);
    }

}

void mafToolVTKButtons::showTooltip(QString tooltip) {
    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }

    QString matrixString = vme->dataSetCollection()->itemAtCurrentTime()->poseMatrixString();
    QString text("<table border=\"0\"");
    text.append("<tr>");
    text.append("<td>");
    QImage preview = button()->getPreview(180,180);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    preview.save(&buffer, "PNG");

    text.append(QString("<img src=\"data:image/png;base64,%1\">").arg(QString(buffer.data().toBase64())));
    text.append("</td>");

    text.append("<td>");
    text.append("<b>Data type</b>: ");
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
    text.append("</td>");
    text.append("</tr>");
    text.append("</table>");
 
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

void mafToolVTKButtons::setVisibility(bool visible) {
    button()->setShowButton(visible);
    Superclass::setVisibility(visible);
    button()->update();
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
    return button()->flyTo();
}

void mafToolVTKButtons::setOnCenter(bool onCenter) {
    button()->setOnCenter(onCenter);
}

bool mafToolVTKButtons::onCenter() {
    return button()->onCenter();
}
