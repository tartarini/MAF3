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

// forward declarations
class mafImporterWidget;

namespace Ui {
    class mafOperationWidget;
}

// Class forwarding list
class mafObjectBase;

/// Base panel on which will be shown all MAF operation.
/**
    Class name: mafOperationWidget
    This class is the base panel for oll the MAF operations. This widget create the Ok/Cancel buttons to manage the start and stop operation execution.
    When a new operation has started, the mafOperationWidget is created and get the pointer to the mafOperation (algorithmic code) and to the UI loaded.
    It then connect the widgets contained into the UI with the slots presents inside the mafOperation and initialize the widgets with the operation's properties.
    When the operation is dismissed (with Ok or Cancel) it emits the signal operationDismissed to notify the mafGUIManager that the operation execution is
    terminated and the GUI of the main window has to be updated.
*/
class mafOperationWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString operationName READ operationName WRITE setOperationName)

public:
    /// Object constructor.
    mafOperationWidget(QWidget *parent = 0);

    /// Object destructor.
    ~mafOperationWidget();

    /// Assign the current operation
    void setOperation(mafCore::mafObjectBase *op);

    /// Assign the operation's name associated to the menu label.
    void setOperationName(QString name);

    /// Return the operation name.
    QString operationName() const;

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

protected:
    /// Method called when the user update the Application's locale.
    void changeEvent(QEvent *e);

private:
    Ui::mafOperationWidget *ui; ///< Pointer to the associated UI file
    mafCore::mafObjectBase *m_Operation;   ///< Pointer to the current running operation.
    QWidget *m_OperationGUI; ///< GUI associated to m_Operation.
    mafImporterWidget *m_ImporterWidget;
};

#endif // MAFOPERATIONWIDGET_H
