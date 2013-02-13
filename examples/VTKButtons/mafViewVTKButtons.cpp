/*
 *  mafViewVTKButtons.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 17/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include "mafViewVTKButtons.h"
#include "mafToolVTKButtons.h"
#include "mafToolVTKButtonsGroup.h"
#include "msvQVTKButtonsManager.h"
#include "mafEventBusManager.h"
#include <mafSceneNode.h>
#include <mafVME.h>
#include <mafTool.h>
#include <mafToolHandler.h>
#include <vtkAlgorithmOutput.h>
#include <mafDataSet.h>
#include <QObject>
#include <QFileDialog>
#include <QLineEdit>
#include <QXmlStreamReader>

using namespace mafPluginVTK;
using namespace mafCore;

mafViewVTKButtons::mafViewVTKButtons(const QString code_location) : mafViewVTK(code_location), m_Group(NULL) {
    m_UIFilename = "mafVTKButtons.ui";
}

mafViewVTKButtons::~mafViewVTKButtons() {
}

bool mafViewVTKButtons::initialize() {
    if (Superclass::initialize()) {
      msvQVTKButtonsManager::instance()->setRenderer(m_Renderer);
      return true;
    }
    return false;
}

mafResources::mafSceneNode *mafViewVTKButtons::createSceneNode(mafResources::mafVME *vme) {
  mafResources::mafSceneNode *sn = Superclass::createSceneNode(vme);
  
  if (sn != NULL) {
    mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, vme->dataSetCollection()->itemAtCurrentTime()->dataValue());
    //if(dataSet)
    {
      if(!m_Group)
      {
        //create the instance for selection pipe.
        m_Group = mafNEW(mafToolVTKButtonsGroup);
        m_Group->setFollowSelectedObjectVisibility(true);
        m_Group->setFollowSelectedObject(false);
        // set the scene node because the tool even if doesn't forward a vme, it needs to keep knowledge its scenenode.
        m_Group->setSceneNode(sn);
        m_Group->setInput(vme);
        m_ToolHandler->addTool(m_Group);
        mafDEL(m_Group);
        bool res = connect(sn, SIGNAL(modifiedObject()), m_Group, SLOT(updatePipe()));
      }
      //create the instance for selection pipe.
      mafToolVTKButtons *toolButton = mafNEW(mafToolVTKButtons);
      toolButton->setVisibility(false);
      toolButton->setFollowSelectedObjectVisibility(true);
      toolButton->setFollowSelectedObject(false);
      // set the scene node because the tool even if doesn't forward a vme, it needs to keep knowledge its scenenode.
      toolButton->setSceneNode(sn);
      toolButton->setInput(vme);
      m_ToolHandler->addTool(toolButton);
      mafDEL(toolButton);
      bool res = connect(sn, SIGNAL(modifiedObject()), toolButton, SLOT(updatePipe()));
    }
  }
  return sn;
}
void mafViewVTKButtons::showSceneNode(mafResources::mafSceneNode *node, bool show) {
  Superclass::showSceneNode(node, show);
  // Add/remove button to the group
  if(node != NULL){
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
      mafResources::mafTool *tool = tList->at(i);
      mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
      if (button && button->input() && button->input()->isEqual(node->vme())) {
        if(show) {
          m_Group->addButton(button);
          m_Group->setVisibility(true);
          m_Group->updatePipe();
        }
        else {
          m_Group->removeButton(button);
          m_Group->updatePipe();
        }
      }
    }
  }
}

void mafViewVTKButtons::removeSceneNode(mafResources::mafSceneNode *node) {
    //When the sceneNode is removed, remove also relative tool.
    if (node != NULL) {
        QList<mafToolVTKButtons *> buttonsToRemove;
        QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();

        //First: search for toolButtons inside m_ToolHandler
        for (int i = 0; i < tList->count(); i++) {
            mafResources::mafTool *tool = tList->at(i);
            mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
            if (button && button->input() && button->input()->isEqual(node->vme())) {
                buttonsToRemove.append(button);
            }
        }

        //Then: remove toolButtons from m_ToolHandler
        for (int i = 0; i < buttonsToRemove.count(); i++) {
            mafToolVTKButtons *button = buttonsToRemove.at(i);
            button->resetTool();
            m_ToolHandler->removeTool(button);
        }
    }
    Superclass::removeSceneNode(node);
}

void mafViewVTKButtons::clearScene() {
    QList<mafToolVTKButtons *> buttonsToRemove;
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();

    //First: search for toolButtons inside m_ToolHandler
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            buttonsToRemove.append(button);
        }
    }

    //Then: remove toolButtons from m_ToolHandler
    for (int i = 0; i < buttonsToRemove.count(); i++) {
        mafToolVTKButtons *button = buttonsToRemove.at(i);
        button->resetTool();
        m_ToolHandler->removeTool(button);
    }
    Superclass::clearScene();
}

void mafViewVTKButtons::on_showButtonsCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        mafToolVTKButtonsGroup *group = dynamic_cast<mafToolVTKButtonsGroup *>(tool);
        if(button) {
            //need to switch off setFollowSelectedObjectVisibility if showButtonsCheck is false
            button->setFollowSelectedObjectVisibility(state);
            if (state) {
                //when showButtons is set to ON, I have to show only buttons of visible SceneNodes
                if (button->sceneNode()) {
                    bool visibility = button->sceneNode()->property("visibility").toBool();
                    button->setVisibility(visibility);
                }
            } else {
                button->setVisibility(state);
            }
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_showLabelsCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setShowLabel(state);
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_flyToCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setFlyTo(state);
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_positionComboBox_activated(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setOnCenter(state);
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_browseFileButton_clicked() {
    //open dialog for selecting the name of the session
    QFileDialog::Options options;
    //    if (!native->isChecked())
    //        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString w = "*.xml";
    QString file;
    file = QFileDialog::getOpenFileName(NULL,
                              mafTr("Import Data"),
                              ".",
                              w,
                              &selectedFilter,
                              options);

    // set filename
    this->widget()->findChild<QLineEdit*>("filePathLineEdit")->setText(file);
    this->xmlParse(file);
}

void mafViewVTKButtons::on_animatePathButton_clicked() {
  msvQVTKButtonsManager::instance()->animate();
}

void mafViewVTKButtons::xmlParse(QString xmlFilePath) {
    msvQVTKButtonsManager::instance()->clearCameraBrakPoints();
    /* We'll parse the path xml */
    QFile* file = new QFile(xmlFilePath);
    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        // OG MESSAGE
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(file);

    //QList< QMap<QString,QString> > persons;
    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() &&
            !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            /* If it's named persons, we'll go to the next.*/
            if(xml.name() == "cameraBreakPoints") {
                continue;
            }
            /* If it's named person, we'll dig the information from there.*/
            if(xml.name() == "cameraBreakPoint") {
                this->xmlParseCamraBreakPoint(xml);
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        //LOG MESSAGAE
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    //this->addPersonsToUI(persons);
}

void mafViewVTKButtons::xmlParseCamraBreakPoint(QXmlStreamReader& xml) {


    //QMap<QString, QString> person;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "cameraBreakPoint") {
        //return NULL;
    }

    /* Next element...
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named cameraBreakPoint.
     */
    double *position = new double[3];
    double *focalPoint = new double[3];
    double *viewUp = new double[3];
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "cameraBreakPoint")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "position") {
              this->xmlParseCoordinate(xml,"position",position);
            }
            /* We've found surname. */
            if(xml.name() == "focalPoint") {
              this->xmlParseCoordinate(xml,"focalPoint",focalPoint);
            }
            /* We've found email. */
            if(xml.name() == "viewUp") {
              this->xmlParseCoordinate(xml,"viewUp",viewUp);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    msvQVTKButtonsManager::instance()->addCameraBreakPoint(position,focalPoint,viewUp);
    delete position;
    delete focalPoint;
    delete viewUp;

}

void mafViewVTKButtons::xmlParseCoordinate(QXmlStreamReader& xml, QString tagName, double xyz[3]) {

    //QMap<QString, QString> person;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "cameraBreakPoint") {
        //return NULL;
    }

    /* Next element...
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named tagName.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == tagName)) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found x. */
            if(xml.name() == "x") {
              xyz[0] = this->xmlReadDouble(xml);
            }
            /* We've found y. */
            if(xml.name() == "y") {
              xyz[1] = this->xmlReadDouble(xml);
            }
            /* We've found z. */
            if(xml.name() == "z") {
              xyz[2] = this->xmlReadDouble(xml);
            }

        }
        /* ...and next... */
        xml.readNext();
    }
}

double mafViewVTKButtons::xmlReadDouble(QXmlStreamReader& xml)
{
  /* We need a start element, like <foo> */
      if(xml.tokenType() != QXmlStreamReader::StartElement) {
          return 0;
      }
      /* Let's read the name... */
      //QString elementName = xml.name().toString();
      /* ...go to the next. */
      xml.readNext();
      /*
       * This elements needs to contain Characters so we know it's
       * actually data, if it's not we'll leave.
       */
      if(xml.tokenType() != QXmlStreamReader::Characters) {
          return 0;
      }
      /* Now we can add it to the map.*/
      return xml.text().toString().toDouble();
}
