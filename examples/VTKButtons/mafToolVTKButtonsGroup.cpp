/*
 *  mafToolVTKButtonsGroup.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 13/01/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKButtonsGroup.h"
#include <mafSceneNodeVTK.h>
#include <QImage>
#include <QDir>
#include <mafVME.h>
#include <mafPoint.h>

#include <msvQVTKButtonsInterface.h>
#include <msvQVTKButtonsGroup.h>
#include <msvQVTKButtonsManager.h>

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
#include <vtkSliderWidget.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

mafToolVTKButtonsGroup::mafToolVTKButtonsGroup(const QString code_location) : mafToolVTKButtonsInterface(code_location){

}

msvQVTKButtonsInterface *mafToolVTKButtonsGroup::element() {
  if(m_Element== NULL) {
    m_Element = m_Element = msvQVTKButtonsManager::instance()->createGroup();
    m_Element->setShowButton(true);
    bool result = QObject::connect(m_Element, SIGNAL(showTooltip(QString)), this, SLOT(showTooltip(QString)));
    result = QObject::connect(m_Element, SIGNAL(hideTooltip()), this, SLOT(hideTooltip()));

  }
  return m_Element;
}

mafToolVTKButtonsGroup::~mafToolVTKButtonsGroup() {
    if(m_Element) {
        delete m_Element;
    }
}

void mafToolVTKButtonsGroup::resetTool() {
    removeWidget(static_cast<msvQVTKButtonsGroup*>(element())->slider());
}

void mafToolVTKButtonsGroup::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the widget.
    addWidget(static_cast<msvQVTKButtonsGroup*>(element())->slider());
}

void mafToolVTKButtonsGroup::addButton(mafToolVTKButtonsInterface *button) {
  static_cast<msvQVTKButtonsGroup*>(element())->addElement(button->element());
}

void mafToolVTKButtonsGroup::removeButton(mafToolVTKButtonsInterface *button) {
  static_cast<msvQVTKButtonsGroup*>(element())->removeElement(button->element());
}

void mafToolVTKButtonsGroup::updatePipe(double t) {
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
  
  QString iconType = vme->property("iconType").toString();
  if (iconType != "") {
    QString iconFileName = mafIconFromObjectType(iconType);
    element()->setIconFileName(iconFileName);
  }

  int size[2]; size[0] = 16; size[1] = 16;


  if (showLabel()) {
    QString vmeName = vme->property("objectName").toString();
    element()->setLabel(vmeName);
  } else {
    QString emptyString;
    element()->setLabel(emptyString);
  }
  ///////////-------- BUTTON WIDGET -----------////////////

  mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(this->graphicObject());

  static_cast<msvQVTKButtonsGroup*>(element())->setCurrentRenderer(widget->renderer());

}

// void mafToolVTKButtonsInterface::showTooltip(QString tooltip) {
//   mafVME *vme = input();
//   if(vme == NULL) {
//     return;
//   }
// 
//   QString matrixString = vme->dataSetCollection()->itemAtCurrentTime()->poseMatrixString();
//   QString text("<b>Group<b>");
// 
//   mafEventBus::mafEventArgumentsList argList;
//   argList.append(mafEventArgument(QString , text));
//   mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.showTooltip", mafEventBus::mafEventTypeLocal, &argList);
// }
// 
// void mafToolVTKButtonsInterface::hideTooltip() {
//   mafEventBus::mafEventArgumentsList argList;
//   mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.hideTooltip", mafEventBus::mafEventTypeLocal, &argList);
// }
// 
// void mafToolVTKButtonsInterface::selectVME() {
//   //select the VME associated to the button pressed
//   mafVME *vme = input();
//   if(vme == NULL) {
//     return;
//   }
//   mafCore::mafObjectBase *objBase = vme;
//   mafEventArgumentsList argList;
//   argList.append(mafEventArgument(mafCore::mafObjectBase*, objBase));
//   mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
// }
// 
// void mafToolVTKButtonsInterface::setVisibility(bool visible) {
//   element()->setShowButton(visible);
//   Superclass::setVisibility(visible);
//   element()->update();
// }
// void mafToolVTKButtonsInterface::setShowLabel(bool show) {
//   element()->setShowLabel(show);
// }
// 
// bool mafToolVTKButtonsInterface::showLabel() {
//   return element()->showLabel();
// }
