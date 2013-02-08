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
 
#include "mafPluginEventBusMonitor_global.h"
#include "mafPluginEventBusMonitorDefinitions.h"

#include "mafNodeAdvancedGraphicWidget.h"

namespace mafPluginEventBusMonitor {

/**
 Class name: mafNodeCompoundGraphicWidget
 Class with several widgets created when instatiated in a scene.
*/    
    
class MAFPLUGINEVENTBUSMONITORSHARED_EXPORT mafNodeCompoundGraphicWidget : public mafNodeAdvancedGraphicWidget {
public:
        mafNodeCompoundGraphicWidget(QMenu *contextMenu = NULL, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setNodeToConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment = mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::ConnectorType type = mafNodeConnectorGraphicWidget::In );

        void setNodeBox(const QVector<QString> &strings, const QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> &alignments, const QVector<mafNodeConnectorGraphicWidget::ConnectorType> types);
        void setNodeFilter(QString left, QString right);
        void setSource(QString label);
        void setSink(QString label);

        /*virtual*/int getId();

private:
        unsigned int m_Id;
};

    
    
}

#endif // MAFNODECOMPOUNDGRAPHICWIDGET_H
