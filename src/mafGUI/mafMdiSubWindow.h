/*
 *  mafMdiSubWindow.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 13/01/11.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMDISUBWINDOW_H
#define MAFMDISUBWINDOW_H

#include "mafGUIDefinitions.h"

#include <QMdiSubWindow>
#include <QEvent>

namespace mafGUI {

/**
 Class name: mafMdiSubWindow
 This class represent a subclass of the QMdiSubWindow and allows to animate
 the show and close throught the animation of its geometry property.
*/
class MAFGUISHARED_EXPORT mafMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(QMdiSubWindow);

public:
    /// Object constructor.
    mafMdiSubWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public Q_SLOTS:
    /// set the visibility of the window.
    /*virtual*/ void setVisible(bool visible);

protected:
    /// This virtual function receives events to an object and should return true if the event e was recognized and processed.
    bool event(QEvent *event);

    QEvent *ce; ///<  this represents a qt event object.

private Q_SLOTS:
    /// represents the function called when the animation is terminated.
    void animationEnded();
};

} // namespace mafGUI

#endif // MAFMDISUBWINDOW_H
