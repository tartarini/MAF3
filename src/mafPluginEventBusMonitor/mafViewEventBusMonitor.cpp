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
#include <mafDiagramScene.h>

#include <QWidget>

using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginEventBusMonitor;

class ExampleBaseNode : public mafNodeGraphicWidget {
public:
        QGridLayout  *innerGridLayout;
        QHBoxLayout *topLayout;
        QVBoxLayout *leftLayout;
        QVBoxLayout *rightLayout;
        QHBoxLayout *bottomLayout;


        void setWidget(QWidget *widget) {
                //does this work for all possible wiget types
                mafNodeGraphicWidget::setWidget(widget);
                widget->setObjectName("ExampleBaseNode");
        }


        ExampleBaseNode(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Qt::WindowFlags wFlags = 0) : mafNodeGraphicWidget(contextMenu, parent, scene, wFlags) {
                                //dw new
                QFrame *outterFrame = new QFrame;
                //QWidget *outterFrame = new QWidget;
                outterFrame->setObjectName("outterFrame");
                //QFormLayout *formlayout = new QFormLayout;

                QVBoxLayout* outterLayout = new QVBoxLayout;
                outterLayout->setMargin(0);


                //QGroupBox *innerFrame = new QGroupBox(outterFrame/*"Contact Details"*/);
                //QFrame *innerFrame = new QFrame(outterFrame/*"Contact Details"*/);
                //QFrame *innerFrame = new QFrame(outterFrame);
                //QWidget *innerFrame = new QWidget(outterFrame);
                QFrame *innerFrame = new QFrame();
                innerFrame->setObjectName("innerFrame");

                //QLineEdit *numberEdit = new QLineEdit;
                //QFormLayout *layout = new QFormLayout;
                //QGridLayout  *innerGridLayout = new QGridLayout;
                innerGridLayout = new QGridLayout;
                topLayout = new QHBoxLayout();
                //innerGridLayout->addLayout(topLayout, 0, 0, 1, 2/*, Qt::AlignCenter*/);
                QFrame *topFrame = new QFrame;
                topFrame->setLayout(topLayout);
                innerGridLayout->addWidget(topFrame, 0, 0, 1, 2/*, Qt::AlignCenter*/);
                leftLayout = new QVBoxLayout();
                //innerGridLayout->addLayout(leftLayout, 1, 0, 1, 1/*, Qt::AlignLeft*/);
                QFrame *leftFrame = new QFrame;
                leftFrame->setLayout(leftLayout);
                innerGridLayout->addWidget(leftFrame, 1, 0, 1, 1/*, Qt::AlignLeft*/);
                rightLayout = new QVBoxLayout();
                //innerGridLayout->addLayout(rightLayout, 1, 1, 1, 1/*, Qt::AlignRight*/);
                QFrame *rightFrame = new QFrame;
                rightFrame->setLayout(rightLayout);
                innerGridLayout->addWidget(rightFrame, 1, 1, 1, 1/*, Qt::AlignRight*/);
                bottomLayout = new QHBoxLayout();
                //innerGridLayout->addLayout(bottomLayout, 2, 0, 1, 2/*, Qt::AlignCenter*/);
                QFrame *bottomFrame = new QFrame;
                bottomFrame->setLayout(bottomLayout);
                innerGridLayout->addWidget(bottomFrame, 2, 0, 1, 2/*, Qt::AlignCenter*/);

                topLayout->setMargin(0);
                leftLayout->setMargin(0);
                rightLayout->setMargin(0);
                bottomLayout->setMargin(0);


                //dw newwww
                innerGridLayout->setMargin(0);
                /*
                innerGridLayout->addItem(new QSpacerItem(0,0),0,0);
                innerGridLayout->addItem(new QSpacerItem(0,0),0,1);
                innerGridLayout->addItem(new QSpacerItem(0,0),1,0);
                innerGridLayout->addItem(new QSpacerItem(0,0),1,1);
                */


                /*
                addConnectorAndLabel("b1", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Bottom);
                addConnectorAndLabel("righttest1", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Right);
                addConnectorAndLabel("righttest2", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Right);
                addConnectorAndLabel("b2", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Bottom);
                addConnectorAndLabel("righttest3", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Right);
                addConnectorAndLabel("lefttest1", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Left);
                addConnectorAndLabel("lefttest2", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Left);
                addConnectorAndLabel("lefttest3", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Left);
                addConnectorAndLabel("b3", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Bottom);
                addConnectorAndLabel("t1", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Top);
                addConnectorAndLabel("t2", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Top);
                addConnectorAndLabel("lefttest4", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Left);
                addConnectorAndLabel("b4", mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Bottom);
                */


                //formlayout->addWidget(innerFrame);
                //setWidget(innerFrame);
                //outterLayout->addWidget(innerFrame);

                //outterLayout->addLayout(innerGridLayout);
                innerFrame->setLayout(innerGridLayout);
                outterLayout->addWidget(innerFrame);

                //innerFrame->setLayout(innerGridLayout);
                outterFrame->setLayout(outterLayout);
                setWidget(outterFrame);


                outterFrame->setObjectName("ExampleBaseNode");
        }



        virtual void addConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorType t, mafNodeConnectorGraphicWidget::ConnectorAlignment align) {
                int cols = innerGridLayout->columnCount();
                int rows = innerGridLayout->rowCount();

                QLabel* l = new QLabel(name);
                if (align == mafNodeConnectorGraphicWidget::Left) {
                        l->setObjectName("leftLabel");
                        l->setAlignment(Qt::AlignLeft);
                        if (leftLayout->count() > 1) leftLayout->addStretch();
                        leftLayout->addWidget(l);
                        //leftLayout->addStretch();
                }
                else if (align == mafNodeConnectorGraphicWidget::Right) {
                        l->setObjectName("rightLabel");
                        l->setAlignment(Qt::AlignRight);
                        if (leftLayout->count() > 1) rightLayout->addStretch();
                        rightLayout->addWidget(l);
                        //rightLayout->addStretch();
                }
                else if (align == mafNodeConnectorGraphicWidget::Top) {
                        l->setObjectName("topLabel");
                        l->setAlignment(Qt::AlignCenter);
                        if (leftLayout->count() > 1) topLayout->addStretch();
                        topLayout->addWidget(l);
                        //topLayout->addStretch();
                } else if (align == mafNodeConnectorGraphicWidget::Bottom) {
                        l->setObjectName("bottomLabel");
                        l->setAlignment(Qt::AlignCenter);
                        if (leftLayout->count() > 1) bottomLayout->addStretch();
                        bottomLayout->addWidget(l);
                        //bottomLayout->addStretch();
                }

                //innerGridLayout->upd

                addConnector(new mafNodeConnectorGraphicWidget(this, scene(), l, t, align, false));
        }


        virtual void serialize(QTextStream& out) {
                //QString res;
                //node
                out << "typeId=" << QString::number(getId()) << "\n";
                out << "uid=" << QString::number((size_t) this) << ", x=" << QString::number(pos().x()) << ", y=" << QString::number(pos().y()) << "\n";
                //connections that start on this node
                foreach(mafNodeConnectorGraphicWidget* con, connectors) {
                        foreach(mafNodeConnectionGraphicWidget* c, con->arrows) {
                                mafNodeConnectorGraphicWidget* oc = NULL;
                                if (c->startConnector() == con) {
                                        oc = c->endConnector();
                                }
                                else {
                                        oc = c->startConnector();
                                }
                                ExampleBaseNode* oi = static_cast<ExampleBaseNode*>(oc->parentItem());
                                //otherwise self-connections on this node (but not when on same connector) would be written twice, so do nothing in one of the two cases
                                if (this == oi && this->connectors.indexOf(con) < oi->connectors.indexOf(oc)) {
                                        continue;
                                }
                                if (c->startConnector() == con) {
                                        out << "startNodeConnector=" << QString::number(this->connectors.indexOf(con));
                                        out << ", endNodeUid=" << QString::number((size_t) oi);
                                        out << ", endNodeConnector=" << QString::number(oi->connectors.indexOf(oc));
                                }
                                else {
                                        out << "endNodeConnector=" << QString::number(this->connectors.indexOf(con));
                                        out << ", startNodeUid=" << QString::number((size_t) oi);
                                        out << ", startNodeConnector=" << QString::number(oi->connectors.indexOf(oc));
                                }
                                out << "\n";
                        }
                }
                out << "\n";
        }

        virtual void deserialize(QTextStream& out, QMap<int, ExampleBaseNode*>& map) {
                //typeId is read by caller
                QString  l;
                if ((l=out.readLine()) != "") {
                        QStringList list = l.split(",");
                        int id = list[0].split("=")[1].toInt();
                        int x = list[1].split("=")[1].toInt();
                        int y = list[2].split("=")[1].toInt();
                        this->setPos(x, y);
                        map[id] = this;
                }
                while ((l=out.readLine()) != "") {
                        bool isStart = l.startsWith("startNodeConnector=");
                        QStringList list = l.split(",");
                        int c = list[0].split("=")[1].toInt();
                        int oid = list[1].split("=")[1].toInt();
                        int oc = list[2].split("=")[1].toInt();
                        //only the second node creates the connection
                        if (map.contains(oid)) {
                                mafNodeGraphicWidget* other = map[oid];
                                mafNodeConnectorGraphicWidget* thisCon = this->connectors[c];
                                mafNodeConnectorGraphicWidget* oCon = other->connectors[oc];
                                mafNodeConnectionGraphicWidget* newCon = NULL;
                                if (isStart) {
                                        newCon = new mafNodeConnectionGraphicWidget(thisCon, oCon, NULL, this->scene());
                                }
                                else {
                                        newCon = new mafNodeConnectionGraphicWidget(oCon, thisCon, NULL, this->scene());
                                }
                                //FIXME: should not be job of user
                                thisCon->addConnection(newCon);
                                oCon->addConnection(newCon);
                                newCon->setZValue(-1000.0);
                        }
                }
        }

        virtual int getId() = 0;
};

//"ExampleNode1: dialog, single in, only one connection"
class ExampleNode1 : public ExampleBaseNode {
public:
        ExampleNode1(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment = mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::ConnectorType type = mafNodeConnectorGraphicWidget::In ,QMenu *contextMenu = NULL, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0) : ExampleBaseNode(contextMenu, parent, scene) {
                /*
                QLabel* in0 = new QLabel("in0");
                this->innerGridLayout->addWidget(in0);

                //dialog->setAttribute(Qt::WA_DeleteOnClose);
                //this->setWidget(dialog);

                addConnector(new mafNodeGraphicWidget(this, scene, in0, mafNodeGraphicWidget::In, mafNodeGraphicWidget::Left, true));
                */
                //FIXME:
                addConnectorAndLabel(name, type, alignment);
        }
        int getId() { return 1; }
};


mafViewEventBusMonitor::mafViewEventBusMonitor(const QString code_location) : m_Scene(NULL), mafView(code_location) {
}

bool mafViewEventBusMonitor::initialize() {
    if(Superclass::initialize()) {
        //dw new: moved up
        //scene = new DiagramScene(itemMenu);
        QMenu *itemMenu = new QMenu;
        m_Scene = new mafDiagramScene(itemMenu, NULL);
        m_Scene->setSceneRect(QRectF(0, 0, 500, 500));

        //dw
        connect(m_Scene, SIGNAL(nodeItemInserted(mafNodeGraphicWidget *)),
            this, SLOT(itemInserted(mafNodeGraphicWidget *)));

        QHBoxLayout *layout = new QHBoxLayout;

        QGraphicsView *view = new QGraphicsView(m_Scene);

        view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        // add the canvas
        layout->addWidget(view);

        m_RenderWidget = new QWidget();
        ((QWidget *)m_RenderWidget)->setLayout(layout);

        m_RenderWidget->setObjectName("EventBus Monitor");
        setupSceneGraph();

        generateEventBusDiagramConnections();

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
        ExampleNode1 *node1 = new ExampleNode1(topic, mafNodeConnectorGraphicWidget::Right, mafNodeConnectorGraphicWidget::In, NULL,NULL,m_Scene);
        ExampleNode1 *node2 = new ExampleNode1("cribbio", mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::InOut, NULL,NULL,m_Scene);

        node1->setPos(QPointF(-250,count*30));
        node2->setPos(QPointF(550,count*30));

        m_Scene->drawNode(node1);
        m_Scene->drawNode(node2);

        m_Scene->setMode(mafDiagramScene::InsertItem);
        count++;
    }

    int numberOfConnections = count;
    m_Scene->setSceneRect(QRectF(0, 0, 500, (numberOfConnections+2) * 30 ));
    m_Scene->update();
}
