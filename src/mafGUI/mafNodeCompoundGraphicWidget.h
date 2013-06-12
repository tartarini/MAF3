/*
 *  mafNodeCompoundGraphicWidget.h
 *  
 *
 *  Created by Daniele Giunchi on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNODECOMPOUNDGRAPHICWIDGET_H
#define MAFNODECOMPOUNDGRAPHICWIDGET_H
 
#include "mafGUIDefinitions.h"
#include "mafNodeAdvancedGraphicWidget.h"

namespace mafGUI {

/**
 Class name: mafNodeCompoundGraphicWidget
 Class with several widgets created when instatiated in a scene.
*/    
class MAFGUISHARED_EXPORT mafNodeCompoundGraphicWidget : public mafNodeAdvancedGraphicWidget {
public:
        /// object constructor.
        mafNodeCompoundGraphicWidget(QMenu *contextMenu = NULL, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        /// add to the scene a node connector and a label.
        void setNodeToConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment = mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::ConnectorType type = mafNodeConnectorGraphicWidget::In );

        /// add to the scene a box of connectors with labels, alignments of each of them
        void setNodeBox(const QVector<QString> &strings, const QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> &alignments, const QVector<mafNodeConnectorGraphicWidget::ConnectorType> types);

        /// set a node which is a filter, with a input and output connector and two labels
        void setNodeFilter(QString left, QString right);

        /// set a source node, with a connector and a string, the connector is right-aligned
        void setSource(QString label);

        /// set a sink node, with a connector and a string, the connector is left-aligned
        void setSink(QString label);

        /// return Id
        /*virtual*/int getId();

private:
        unsigned int m_Id;
};

    
    
}

#endif // MAFNODECOMPOUNDGRAPHICWIDGET_H
