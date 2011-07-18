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


namespace mafPluginVTK {

class mafAxes;

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

    /// Overloaded mouse press handler
    /*virtual*/ void mousePressEvent(QMouseEvent* event);

    /// Overloaded mouse release handler
    /*virtual*/ void mouseReleaseEvent(QMouseEvent* event);

    /// Overloaded mouse wheel handler
    /*virtual*/ void wheelEvent(QWheelEvent* event);

    /// Overloaded mouse move handler
    /*virtual*/ void mouseMoveEvent(QMouseEvent* event);

    /// Allows to show the axes representing the global reference system. This method has to be called after that the renderer has been added to the renderwindow.
    void showAxes(bool show = true);

signals:
    /// picked button pressed.
    void vmePickSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent * e);
    
    void mousePressSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    void mouseReleaseSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    void mouseMoveSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);

protected:
    /// Object destructor.
    /* virtual */ ~mafVTKWidget();
    
    // overloaded resize handler
    /*virtual*/ void resizeEvent(QResizeEvent* event);

private:
    /// Get key modifiers.
    void getModifiers(vtkRenderWindowInteractor* iren);

    void mousePress(vtkRenderWindowInteractor* iren, QEvent *e);
    void mouseRelease(vtkRenderWindowInteractor* iren, QEvent *e);
    void mouseMove(vtkRenderWindowInteractor* iren, QEvent *e);
    
    /// initialize connections
    void initializeConnections();
    
    unsigned long m_Modifiers;  ///< Optional modifiers for the button.

    mafAxes *m_Axes; ///< Tool representing a global reference system.
};

} // namespace mafPluginVTK

#endif // MAFVTKWIDGET_H
