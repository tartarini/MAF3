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

using namespace mafPluginEventBusMonitor;

mafNodeCompoundGraphicWidget::mafNodeCompoundGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene) : mafNodeAdvancedGraphicWidget(contextMenu, parent, scene) {
}

void mafNodeCompoundGraphicWidget::setNodeToConnectorAndLabel(QString name, mafNodeConnectorGraphicWidget::ConnectorAlignment alignment, mafNodeConnectorGraphicWidget::ConnectorType type ) {
    m_Id = 1;
    addConnectorAndLabel(name, type, alignment);
}

void mafNodeCompoundGraphicWidget::setNodeBox(const QVector<QString> &strings, const QVector<mafNodeConnectorGraphicWidget::ConnectorAlignment> &alignments, const QVector<mafNodeConnectorGraphicWidget::ConnectorType> types) {
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
    
    //MyDialog::MyDialog(QWidget* parent, const char* name)	: QDialog(parent, name, true, WStyle_Customize | WStyle_DialogBorder | WStyle_Title | WStyle_SysMenu)

    /*
    QDialog *dialog1 = new QDialog(NULL, Qt::WindowType::FramelessWindowHint);
    dialog1->setLayout(layout2);
    */

    //dialog1->setAttribute(Qt::WA_DeleteOnClose);

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

void mafNodeCompoundGraphicWidget::setNodeFilter(QString left, QString right) {
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

void mafNodeCompoundGraphicWidget::setSource(QString label) {
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

void mafNodeCompoundGraphicWidget::setSink(QString label) {
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

int mafNodeCompoundGraphicWidget::getId() { 
    return m_Id;
}
