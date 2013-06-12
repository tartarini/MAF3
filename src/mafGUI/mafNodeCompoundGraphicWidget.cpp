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

#include "mafNodeCompoundGraphicWidget.h"

using namespace mafGUI;

mafNodeCompoundGraphicWidget::mafNodeCompoundGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene) : mafNodeAdvancedGraphicWidget(contextMenu, parent, scene) {
}

void mafNodeCompoundGraphicWidget::setNodeToConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment, mafNodeConnectorGraphicWidget::ConnectorType type ) {
    m_Id = 1;
    addConnectorAndLabel(name, type, alignment);
}

void mafNodeCompoundGraphicWidget::setNodeBox(const QVector<QString> &strings, const QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> &alignments, const QVector<mafNodeConnectorGraphicWidget::ConnectorType> types) {

    int numberOfItems = strings.count();
    int index = 0;
    for(; index < numberOfItems; ++index) {
        setNodeToConnectorAndLabel(strings.at(index), alignments.at(index), types.at(index));
    }


    m_Id = 5;
}

void mafNodeCompoundGraphicWidget::setNodeFilter(QString left, QString right) {
    QVector<QString> strings;
    strings << left << right;

    QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> alignments;
    alignments << mafNodeConnectorGraphicWidget::Left << mafNodeConnectorGraphicWidget::Right;

    QVector<mafNodeConnectorGraphicWidget::ConnectorType> types;
    types << mafNodeConnectorGraphicWidget::InOut << mafNodeConnectorGraphicWidget::InOut;

    setNodeBox(strings, alignments, types);

    m_Id = 6;
}

void mafNodeCompoundGraphicWidget::setSource(QString label) {
    setNodeToConnectorAndLabel(label, mafNodeConnectorGraphicWidget::Right, mafNodeConnectorGraphicWidget::Out);


    m_Id = 7;
}

void mafNodeCompoundGraphicWidget::setSink(QString label) {
    setNodeToConnectorAndLabel(label, mafNodeConnectorGraphicWidget::Left, mafNodeConnectorGraphicWidget::In);

    /* //sample
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

int mafNodeCompoundGraphicWidget::getId() { 
    return m_Id;
}
