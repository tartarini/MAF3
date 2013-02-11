/*
 *  mafViewScriptInterpreter.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNODEADVANCEDGRAPHICWIDGET_H
#define MAFNODEADVANCEDGRAPHICWIDGET_H

#include "mafGUIDefinitions.h"
#include <mafNodeGraphicWidget.h>

//class forwarding

namespace mafGUI {

class MAFGUISHARED_EXPORT mafNodeAdvancedGraphicWidget : public mafNodeGraphicWidget {
public:
        /// object constructor.
        mafNodeAdvancedGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Qt::WindowFlags wFlags = 0);
        
        /// retrieve vector of connectors
        const QVector<mafNodeConnectorGraphicWidget *> &connectorVector();
        
        /// add the widget to embed
        void setWidget(QWidget *widget);

        /// insert a connector with a label
        virtual void addConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorType t, mafNodeConnectorGraphicWidget::ConnectorAlignment align);

        /// serialization with a stream.
        virtual void serialize(QTextStream& out);

        /// restore serialized object
        virtual void deserialize(QTextStream& out, QMap<int, mafNodeAdvancedGraphicWidget*>& map);

        /// retrieve the Id
        virtual int getId() = 0;

private:
        QGridLayout  *innerGridLayout;
        QHBoxLayout *topLayout;
        QVBoxLayout *leftLayout;
        QVBoxLayout *rightLayout;
        QHBoxLayout *bottomLayout;

        QVector<mafNodeConnectorGraphicWidget *> m_ConnectorVector;
};


}

#endif // MAFNODEADVANCEDGRAPHICWIDGET_H
