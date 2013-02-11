/*
 *  mafViewEventBusMonitor.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewEventBusMonitor.h"
#include "mafQGraphicsViewDiagram.h"
#include "mafNodeCompoundGraphicWidget.h"

#include <QPaintEvent>
#include <QWidget>


using namespace mafEventBus;
using namespace mafResources;
using namespace mafGUI;
using namespace mafPluginEventBusMonitor;

#define LINE_SPACE 40

#include <QGraphicsView>
#include <QPaintEvent>

mafViewEventBusMonitor::mafViewEventBusMonitor(const QString code_location) : m_Scene(NULL), mafView(code_location) {
}

bool mafViewEventBusMonitor::initialize() {
    if(Superclass::initialize()) {
        //dw new: moved up
        //scene = new DiagramScene(itemMenu);
        QMenu *itemMenu = new QMenu;
        m_Scene = new mafDiagramScene(itemMenu, NULL);


        connect(m_Scene, SIGNAL(nodeItemInserted(mafNodeGraphicWidget *)),
            this, SLOT(itemInserted(mafNodeGraphicWidget *)));

        QHBoxLayout *layout = new QHBoxLayout;

        generateEventBusDiagramConnections();

        m_View = new mafQGraphicsViewDiagram(m_Scene);
        m_View->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        // add the canvas
        layout->addWidget(m_View);

        m_RenderWidget = new QWidget();
        ((QWidget *)m_RenderWidget)->setLayout(layout);

        m_RenderWidget->setObjectName("EventBus Monitor");
        setupSceneGraph();

        return true;
    }
    return false;
}

mafViewEventBusMonitor::~mafViewEventBusMonitor() {
    if(m_Scene) {
        delete m_Scene;
    }
}

void mafViewEventBusMonitor::generateEventBusDiagramConnections() {

    const mafEventBusConnectionInfoHash &infos = mafEventBusManager::instance()->connectionInfosDump();

    mafEventBusConnectionInfoHash::const_iterator i;
    unsigned int count = 1;
    for (i = infos.constBegin(); i != infos.constEnd(); ++i) {
        QString topic = i.key();
        mafEventBusConnectionInfo info = i.value();

        //analyze
        m_Scene->setMode(mafDiagramScene::InsertNode);
        mafNodeCompoundGraphicWidget *node1 = new mafNodeCompoundGraphicWidget(NULL,NULL,m_Scene);
        node1->setSource(topic);
        mafNodeCompoundGraphicWidget *node2 = new mafNodeCompoundGraphicWidget(NULL,NULL,m_Scene);
        node2->setNodeFilter(info.signalName,"");

        node1->setPos(QPointF(50,count*LINE_SPACE));
        node2->setPos(QPointF(450,count*LINE_SPACE));
        m_Scene->drawNode(node1);
        m_Scene->drawNode(node2);

        //second connector
        mafNodeConnectorGraphicWidget *node22 = node2->connectorVector().at(1);

        QVector<mafNodeCompoundGraphicWidget *>nodeSlots;

        int i = 0;
        int numberOfSlots = info.slotName.count();
        for(; i<numberOfSlots;++i) {
            mafNodeCompoundGraphicWidget *ns = new mafNodeCompoundGraphicWidget(NULL,NULL,m_Scene);
            ns->setSink(info.slotName.at(0));
            ns->setPos(QPointF(1050,count*LINE_SPACE));
            nodeSlots.push_back(ns);
            m_Scene->drawNode(ns);

            mafNodeConnectionGraphicWidget *ar  = new mafNodeConnectionGraphicWidget(node22, ns->connectorVector().at(0), NULL, m_Scene);
            m_Scene->drawArrow(ar);
            ++count;
        }


        m_Scene->setMode(mafDiagramScene::InsertItem);

        mafNodeConnectorGraphicWidget *node11 = node1->connectorVector().at(0);
        mafNodeConnectorGraphicWidget *node21 = node2->connectorVector().at(0);
        mafNodeConnectionGraphicWidget *signalArrow = new mafNodeConnectionGraphicWidget(node11, node21, NULL, m_Scene);

        m_Scene->drawArrow(signalArrow);

        count++;
    }

    int numberOfConnections = count;
    m_Scene->setSceneRect(QRectF(50, 0, 2000, (numberOfConnections+2) * 30 + 50 ));
    m_Scene->update();
}
