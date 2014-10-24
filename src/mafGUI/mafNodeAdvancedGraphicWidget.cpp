/*
 *  mafNodeAdvancedGraphicWidget.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNodeAdvancedGraphicWidget.h"
#include <QVBoxLayout>

using namespace mafGUI;

mafNodeAdvancedGraphicWidget::mafNodeAdvancedGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene, Qt::WindowFlags wFlags) : mafNodeGraphicWidget(contextMenu, parent, scene, wFlags) {
                                
    QFrame *outterFrame = new QFrame;
    outterFrame->setObjectName("outterFrame");

    QVBoxLayout* outterLayout = new QVBoxLayout;
    outterLayout->setMargin(0);

    QFrame *innerFrame = new QFrame();
    innerFrame->setObjectName("innerFrame");

    innerGridLayout = new QGridLayout;
    topLayout = new QHBoxLayout();
    QFrame *topFrame = new QFrame;
    topFrame->setLayout(topLayout);
    innerGridLayout->addWidget(topFrame, 0, 0, 1, 2/*, Qt::AlignCenter*/);
    leftLayout = new QVBoxLayout();
    QFrame *leftFrame = new QFrame;
    leftFrame->setLayout(leftLayout);
    innerGridLayout->addWidget(leftFrame, 1, 0, 1, 1/*, Qt::AlignLeft*/);
    rightLayout = new QVBoxLayout();
    QFrame *rightFrame = new QFrame;
    rightFrame->setLayout(rightLayout);
    innerGridLayout->addWidget(rightFrame, 1, 1, 1, 1/*, Qt::AlignRight*/);
    bottomLayout = new QHBoxLayout();
    QFrame *bottomFrame = new QFrame;
    bottomFrame->setLayout(bottomLayout);
    innerGridLayout->addWidget(bottomFrame, 2, 0, 1, 2/*, Qt::AlignCenter*/);

    topLayout->setMargin(0);
    leftLayout->setMargin(0);
    rightLayout->setMargin(0);
    bottomLayout->setMargin(0);

    innerGridLayout->setMargin(0);

    innerFrame->setLayout(innerGridLayout);
    outterLayout->addWidget(innerFrame);

    outterFrame->setLayout(outterLayout);
    setWidget(outterFrame);

    outterFrame->setObjectName("mafNodeAdvancedGraphicWidget");
}


void mafNodeAdvancedGraphicWidget::addConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorType t, mafNodeConnectorGraphicWidget::ConnectorAlignment align) {
    int cols = innerGridLayout->columnCount();
    int rows = innerGridLayout->rowCount();

    QLabel* l = new QLabel(name);
    if (align == mafNodeConnectorGraphicWidget::Left) {
        l->setObjectName("leftLabel");
        l->setAlignment(Qt::AlignLeft);
        if (leftLayout->count() > 1) {
            leftLayout->addStretch();
        }
        leftLayout->addWidget(l);
        //leftLayout->addStretch();
    } else if (align == mafNodeConnectorGraphicWidget::Right) {
        l->setObjectName("rightLabel");
        l->setAlignment(Qt::AlignRight);
        if (leftLayout->count() > 1) {
            rightLayout->addStretch();
        }
        rightLayout->addWidget(l);
        //rightLayout->addStretch();
    } else if (align == mafNodeConnectorGraphicWidget::Top) {
        l->setObjectName("topLabel");
        l->setAlignment(Qt::AlignCenter);
        if (leftLayout->count() > 1) {
            topLayout->addStretch();
        }
        topLayout->addWidget(l);
        //topLayout->addStretch();
    } else if (align == mafNodeConnectorGraphicWidget::Bottom) {
        l->setObjectName("bottomLabel");
        l->setAlignment(Qt::AlignCenter);
        if (leftLayout->count() > 1) {
            bottomLayout->addStretch();
        }
        bottomLayout->addWidget(l);
        //bottomLayout->addStretch();
    }

    mafNodeConnectorGraphicWidget *connector = new mafNodeConnectorGraphicWidget(this, scene(), l, t, align, false);
    m_ConnectorVector.push_back(connector);
    addConnector(connector);
}

void mafNodeAdvancedGraphicWidget::serialize(QTextStream& out) {
    out << "typeId=" << QString::number(getId()) << "\n";
    out << "uid=" << QString::number((size_t) this) << ", x=" << QString::number(pos().x()) << ", y=" << QString::number(pos().y()) << "\n";
    foreach(mafNodeConnectorGraphicWidget* con, connectors()) {
        foreach(mafNodeConnectionGraphicWidget* c, con->arrows) {
            mafNodeConnectorGraphicWidget* oc = NULL;
            if (c->startConnector() == con) {
                oc = c->endConnector();
            } else {
                oc = c->startConnector();
            }
            mafNodeAdvancedGraphicWidget* oi = static_cast<mafNodeAdvancedGraphicWidget*>(oc->parentItem());
            if (this == oi && this->connectors().indexOf(con) < oi->connectors().indexOf(oc)) {
                continue;
            }
            if (c->startConnector() == con) {
                out << "startNodeConnector=" << QString::number(this->connectors().indexOf(con));
                out << ", endNodeUid=" << QString::number((size_t) oi);
                out << ", endNodeConnector=" << QString::number(oi->connectors().indexOf(oc));
            } else {
                out << "endNodeConnector=" << QString::number(this->connectors().indexOf(con));
                out << ", startNodeUid=" << QString::number((size_t) oi);
                                        out << ", startNodeConnector=" << QString::number(oi->connectors().indexOf(oc));
            }
            out << "\n";
        }
    }
    out << "\n";
}

void mafNodeAdvancedGraphicWidget::deserialize(QTextStream& out, QMap<int, mafNodeAdvancedGraphicWidget*>& map) {
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
        if (map.contains(oid)) {
            mafNodeGraphicWidget* other = map[oid];
            mafNodeConnectorGraphicWidget* thisCon = this->connectors()[c];
            mafNodeConnectorGraphicWidget* oCon = other->connectors()[oc];
            mafNodeConnectionGraphicWidget* newCon = NULL;
            if (isStart) {
                newCon = new mafNodeConnectionGraphicWidget(thisCon, oCon, NULL, this->scene());
            }  else {
                newCon = new mafNodeConnectionGraphicWidget(oCon, thisCon, NULL, this->scene());
            }
            thisCon->addConnection(newCon);
            oCon->addConnection(newCon);
            newCon->setZValue(-1000.0);
        }
    }
}

const QVector<mafNodeConnectorGraphicWidget *> &mafNodeAdvancedGraphicWidget::connectorVector() {
    return m_ConnectorVector;
}


void mafNodeAdvancedGraphicWidget::setWidget(QWidget *widget) {
    mafNodeGraphicWidget::setWidget(widget);
    widget->setObjectName("mafNodeAdvancedGraphicWidget");
}
