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

#include <QPaintEvent>
#include <QWidget>


using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginEventBusMonitor;

#define LINE_SPACE 40

#include <QGraphicsView>
#include <QPaintEvent>

MyFasterGraphicView::MyFasterGraphicView(QGraphicsScene* scene): QGraphicsView(scene) {}
void MyFasterGraphicView::paintEvent ( QPaintEvent * event) {
        QRect r = event->region().boundingRect();
        QRect r1 = scene()->itemsBoundingRect().toRect();

        QPaintEvent *newEvent=new QPaintEvent(r1);
        QGraphicsView::paintEvent(newEvent);
        delete newEvent;
}


class mafNodeAdvancedGraphicWidget : public mafNodeGraphicWidget {
public:
        QGridLayout  *innerGridLayout;
        QHBoxLayout *topLayout;
        QVBoxLayout *leftLayout;
        QVBoxLayout *rightLayout;
        QHBoxLayout *bottomLayout;

        const QVector<mafNodeConnectorGraphicWidget *> &connectorVector() {
            return m_ConnectorVector;
        }


        void setWidget(QWidget *widget) {
                //does this work for all possible wiget types
                mafNodeGraphicWidget::setWidget(widget);
                widget->setObjectName("mafNodeAdvancedGraphicWidget");
        }


        mafNodeAdvancedGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Qt::WindowFlags wFlags = 0) : mafNodeGraphicWidget(contextMenu, parent, scene, wFlags) {
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


                outterFrame->setObjectName("mafNodeAdvancedGraphicWidget");
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

                mafNodeConnectorGraphicWidget *connector = new mafNodeConnectorGraphicWidget(this, scene(), l, t, align, false);
                m_ConnectorVector.push_back(connector);
                addConnector(connector);
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
                                mafNodeAdvancedGraphicWidget* oi = static_cast<mafNodeAdvancedGraphicWidget*>(oc->parentItem());
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

        virtual void deserialize(QTextStream& out, QMap<int, mafNodeAdvancedGraphicWidget*>& map) {
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

private:
        QVector<mafNodeConnectorGraphicWidget *> m_ConnectorVector;
};

//"mafNodeAdvancedGraphicWidget: dialog, single in, only one connection"
class mafNodeCompoundGraphicWidget : public mafNodeAdvancedGraphicWidget {
public:
        mafNodeCompoundGraphicWidget(QMenu *contextMenu = NULL, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0) : mafNodeAdvancedGraphicWidget(contextMenu, parent, scene) {
        }

        void setNodeToConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment = mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::ConnectorType type = mafNodeConnectorGraphicWidget::In ) {
            m_Id = 1;
            addConnectorAndLabel(name, type, alignment);
        }

        void setNodeBox(const QVector<QString> &strings, const QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> &alignments, const QVector<mafNodeConnectorGraphicWidget::ConnectorType> types) {
                //QFormLayout *layout2 = new QFormLayout;
                /*
                QLabel* out0 = new QLabel("out0");
                out0->setAlignment(Qt::AlignRight);
                QLabel* in0 = new QLabel("in0");

                innerGridLayout->addWidget(in0,0,0);
                innerGridLayout->addWidget(out0,0,1);
                QLabel* in1 = new QLabel("in1");
                innerGridLayout->addWidget(in1,1,0,1,2);
                QLabel* in2 = new QLabel("in2");
                innerGridLayout->addWidget(in2,2,0,1,2);
                QLabel* inout0 = new QLabel("inOut0");
                inout0->setAlignment(Qt::AlignRight);
                innerGridLayout->addWidget(inout0,3,0,1,2);

                QSpinBox* spinBox = new QSpinBox();
                spinBox->setMinimum(0);
                spinBox->setMaximum(9);
                innerGridLayout->addWidget(spinBox,4,0,1,2);
                */

                int numberOfItems = strings.count();
                int index = 0;
                for(; index < numberOfItems; ++index) {
                    setNodeToConnectorAndLabel(strings.at(index), alignments.at(index), types.at(index));
                }

                /*QSpinBox* spinBox = new QSpinBox();
                spinBox->setMinimum(0);
                spinBox->setMaximum(9);
                leftLayout->addWidget(spinBox);
                addConnector(new mafNodeConnectorGraphicWidget(this, scene, spinBox, mafNodeConnectorGraphicWidget::InOut));*/

                //QDialog *dialog1 = new QDialog();
                //dw from: http://lists.trolltech.com/qt-interest/2002-05/thread00837-0.html
                //MyDialog::MyDialog(QWidget* parent, const char* name)	: QDialog(parent, name, true, WStyle_Customize | WStyle_DialogBorder | WStyle_Title | WStyle_SysMenu)

                /*
                QDialog *dialog1 = new QDialog(NULL, Qt::WindowType::FramelessWindowHint);
                dialog1->setLayout(layout2);
                */

                //dialog1->setAttribute(Qt::WA_DeleteOnClose);

                //dw new3
                //dialog1->setStyle(new QStyle::());

                //dialog1->setSizeGripEnabled(false);

                //dialog1->setFixedSize(dialog1->size());
                //dialog1->setSizePolicy(QSizePolicy::Fixed);
                //layout2->setSizeConstraint(QLayout::SetFixedSize);

                //dialog1->layout()->setSizeConstraint( QLayout::SetFixedSize ) ;
                //dialog1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


                /*
                this->setWidget(dialog1);
                setWindowFlags(dialog1->windowFlags() | Qt::Tool);
                */
                /*
                addConnector(new mafNodeGraphicWidget(this, scene, out0, mafNodeGraphicWidget::Out, mafNodeGraphicWidget::Right));
                addConnector(new mafNodeGraphicWidget(this, scene, in0, mafNodeGraphicWidget::In));
                addConnector(new mafNodeGraphicWidget(this, scene, in1, mafNodeGraphicWidget::In));
                addConnector(new mafNodeGraphicWidget(this, scene, in2, mafNodeGraphicWidget::In));
                addConnector(new mafNodeGraphicWidget(this, scene, inout0, mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Right));
                addConnector(new mafNodeGraphicWidget(this, scene, spinBox, mafNodeGraphicWidget::InOut));
                */
                m_Id = 5;
        }

        void setNodeFilter(QString left, QString right) {
            QVector<QString> strings;
            strings << left << right;

            QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> alignments;
            alignments << mafNodeConnectorGraphicWidget::Left << mafNodeConnectorGraphicWidget::Right;

            QVector<mafNodeConnectorGraphicWidget::ConnectorType> types;
            types << mafNodeConnectorGraphicWidget::InOut << mafNodeConnectorGraphicWidget::InOut;


            setNodeBox(strings, alignments, types);

            /*
            //QFormLayout *layout = new QFormLayout;
            QLabel* inout0 = new QLabel("inout0");
            QLabel* inout1 = new QLabel("inout1");
            inout1->setAlignment(Qt::AlignRight);
            //layout->addRow(inout0, inout1);
            //QGroupBox *dialog = new QGroupBox();
            //dialog->setLayout(layout);

            innerGridLayout->addWidget(inout0,0,0);
            innerGridLayout->addWidget(inout1,0,1);
            */

            /*
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            this->setWidget(dialog);

            setWindowFlags(dialog->windowFlags() | Qt::Tool | Qt::WindowType::FramelessWindowHint);
            */

            /*
            addConnector(new mafNodeGraphicWidget(this, scene, inout0, mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Left, false));
            addConnector(new mafNodeGraphicWidget(this, scene, inout1, mafNodeGraphicWidget::InOut, mafNodeGraphicWidget::Right, false));
            */
            m_Id = 6;
        }

        void setSource(QString label) {
            setNodeToConnectorAndLabel(label, mafNodeConnectorGraphicWidget::Right, mafNodeConnectorGraphicWidget::Out);

            /*
            //QFormLayout *layout = new QFormLayout;
            QLabel* out0 = new QLabel("out0");
            out0->setAlignment(Qt::AlignRight);
            */
            /*
            layout->addRow(out0);
            QDialog *dialog = new QDialog(NULL);
            dialog->setWindowTitle("ExampleNode7: frame with title, 1 x out, multi connection");
            //QToolBox *dialog = new QToolBox();
            dialog->setWindowTitle("xxx7");
            dialog->setLayout(layout);
            //dialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            //dialog->setFixedSize(dialog->sizeHint());
            //layout->setSizeConstraint(QLayout::SetFixedSize);
            this->mNoResize = false;
            */

            //dialog->setAttribute(Qt::WA_DeleteOnClose);
            //this->setWidget(dialog);

            /*
            innerGridLayout->addWidget(out0);
            addConnector(new mafNodeGraphicWidget(this, scene, out0, mafNodeGraphicWidget::Out, mafNodeGraphicWidget::Right, false));
            */
            m_Id = 7;
        }

        void setSink(QString label) {
            setNodeToConnectorAndLabel(label, mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::In);

            /*
            //QFormLayout *layout = new QFormLayout;
            QLabel* out0 = new QLabel("out0");
            out0->setAlignment(Qt::AlignRight);
            */
            /*
            layout->addRow(out0);
            QDialog *dialog = new QDialog(NULL);
            dialog->setWindowTitle("ExampleNode7: frame with title, 1 x out, multi connection");
            //QToolBox *dialog = new QToolBox();
            dialog->setWindowTitle("xxx7");
            dialog->setLayout(layout);
            //dialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            //dialog->setFixedSize(dialog->sizeHint());
            //layout->setSizeConstraint(QLayout::SetFixedSize);
            this->mNoResize = false;
            */

            //dialog->setAttribute(Qt::WA_DeleteOnClose);
            //this->setWidget(dialog);

            /*
            innerGridLayout->addWidget(out0);
            addConnector(new mafNodeGraphicWidget(this, scene, out0, mafNodeGraphicWidget::Out, mafNodeGraphicWidget::Right, false));
            */
            m_Id = 7;
        }

        int getId() { return m_Id; }
private:
        unsigned int m_Id;
};

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

        m_View = new MyFasterGraphicView(m_Scene);
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
        node2->setPos(QPointF(550,count*LINE_SPACE));

        if(info.slotName.count()) {
            mafNodeCompoundGraphicWidget *node3 = new mafNodeCompoundGraphicWidget(NULL,NULL,m_Scene);
            node3->setSink(info.slotName.at(0));
            node3->setPos(QPointF(1250,count*LINE_SPACE));
            m_Scene->drawNode(node3);
        }

        m_Scene->drawNode(node1);
        m_Scene->drawNode(node2);

        m_Scene->setMode(mafDiagramScene::InsertItem);

        mafNodeConnectorGraphicWidget *node11 = node1->connectorVector().at(0);
        mafNodeConnectorGraphicWidget *node21 = node2->connectorVector().at(0);
        mafNodeConnectionGraphicWidget *tmpArrow = new mafNodeConnectionGraphicWidget(node11, node21, NULL, m_Scene);

        m_Scene->drawArrow(tmpArrow);

        count++;
    }

    int numberOfConnections = count;
    m_Scene->setSceneRect(QRectF(50, 0, 2000, (numberOfConnections+2) * 30 + 50 ));
    m_Scene->update();
}
