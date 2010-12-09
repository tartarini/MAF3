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
    mafVTKWidget();

    // overloaded mouse press handler
    /*virtual*/ void mousePressEvent(QMouseEvent* event);
    /*virtual*/ void mouseReleaseEvent(QMouseEvent* event);
    /*virtual*/ void wheelEvent(QWheelEvent* event);
    /*virtual*/ void mouseMoveEvent(QMouseEvent* event);

private:
    /// Get key modifiers.
    void getModifiers(vtkRenderWindowInteractor* iren);

    /// Check if VME has been picked
    void vmePickCheck(vtkRenderWindowInteractor* iren);

    unsigned long m_Modifiers;  ///< Optional modifiers for the button.
};

} // namespace mafPluginVTK

#endif // MAFVTKWIDGET_H
