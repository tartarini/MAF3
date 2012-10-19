/*
 *  mafToolVTKButtonsInterface.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 13/01/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKButtonsInterface.h"
#include <mafSceneNodeVTK.h>
#include <QImage>
#include <QDir>
#include <mafVME.h>
#include <mafPoint.h>

#include <msvQVTKButtonsGroup.h>

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

mafToolVTKButtonsInterface::mafToolVTKButtonsInterface(const QString code_location) : mafPluginVTK::mafToolVTK(code_location), isLoaded(false), m_Element(NULL){

}

mafToolVTKButtonsInterface::~mafToolVTKButtonsInterface() {
}

void mafToolVTKButtonsInterface::showTooltip(QString tooltip) {
    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }

    QString matrixString = vme->dataSetCollection()->itemAtCurrentTime()->poseMatrixString();
    QString text("<b>Group<b>");

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString , text));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.showTooltip", mafEventBus::mafEventTypeLocal, &argList);
}

void mafToolVTKButtonsInterface::hideTooltip() {
    mafEventBus::mafEventArgumentsList argList;
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.gui.hideTooltip", mafEventBus::mafEventTypeLocal, &argList);
}

void mafToolVTKButtonsInterface::selectVME() {
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

void mafToolVTKButtonsInterface::setVisibility(bool visible) {
    element()->setShowButton(visible);
    Superclass::setVisibility(visible);
    element()->update();
}
void mafToolVTKButtonsInterface::setShowLabel(bool show) {
    element()->setShowLabel(show);
}

bool mafToolVTKButtonsInterface::showLabel() {
    return element()->showLabel();
}
