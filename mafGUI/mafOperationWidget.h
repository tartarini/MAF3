/*
 *  mafOperationWidget.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 08/02/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONWIDGET_H
#define MAFOPERATIONWIDGET_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QWidget>

namespace Ui {
    class mafOperationWidget;
}

// Class forwarding list

/**
    Class name: mafOperationWidget
*/
class mafOperationWidget : public QWidget {
    Q_OBJECT
public:
    /// Object constructor.
    mafOperationWidget(QWidget *parent = 0);
    /// Object destructor.
    ~mafOperationWidget();

    /// Assign the current operation
    void setOperation(QObject *op);

    /// Assign the operation's name
    void setOperationName(mafString name);

    /// Assign the operation's GUI
    void setOperationGUI(QWidget *gui);

signals:
    /// Signal to alert the observet that the operation GUI has been dismissed.
    void operationDismissed();

public slots:
    /// Execute the operation
    void execute();

    /// Cancel the operation.
    void cancel();

    /// Update the operation's property.
    void setParameter(QVariant var);

protected:
    void changeEvent(QEvent *e);

private:
    void connectOperationSlotsByName(QObject *slot_object, QObject *signal_object);

    Ui::mafOperationWidget *ui; ///< Pointer to the associated UI file
    QObject *m_Operation;   ///< Pointer to the current running operation.
    QWidget *m_OperationGUI; ///< GUI associated to m_Operation.
};

#endif // MAFOPERATIONWIDGET_H
