/*
 *  mafVTKWidget.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/10/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKWIDGET_H
#define MAFVTKWIDGET_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafContainer.h>
#include <vtkActor.h>
#include <QVTKWidget.h>
#include <qwidget.h>



namespace mafPluginVTK {

/**
Class name: mafVTKWidget
This class overloads VKT mouse events and forward them to EventBus.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKWidget : public QVTKWidget {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(QVTKWidget);
public:
    /// Object constructor.
    mafVTKWidget(QWidget* parent = NULL, Qt::WFlags f = 0);

    /// Object destructor.
    ~mafVTKWidget();

    /// Overloaded mouse press handler
    /*virtual*/ void mousePressEvent(QMouseEvent* event);

    /// Overloaded mouse release handler
    /*virtual*/ void mouseReleaseEvent(QMouseEvent* event);

    /// Overloaded mouse wheel handler
    /*virtual*/ void wheelEvent(QWheelEvent* event);

    /// Overloaded mouse move handler
    /*virtual*/ void mouseMoveEvent(QMouseEvent* event);

signals:
    /// picked button pressed.
    void vmePickSignal(double *pos, unsigned long modifiers, mafCore::mafContainerInterface *interface, QEvent * e);

private:
    /// Get key modifiers.
    void getModifiers(vtkRenderWindowInteractor* iren);

    /// Check if VME has been picked
    void vmePickCheck(vtkRenderWindowInteractor* iren, QEvent *e);

    mafCore::mafContainer<vtkActor> *m_Actor; ///<  Container of the actor.

    unsigned long m_Modifiers;  ///< Optional modifiers for the button.
};

} // namespace mafPluginVTK

#endif // MAFVTKWIDGET_H
