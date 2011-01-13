#ifndef MAFMDISUBWINDOW_H
#define MAFMDISUBWINDOW_H

#include "mafGUIDefinitions.h"

#include <QMdiSubWindow>
#include <QEvent>

namespace mafGUI {

/**
 Class name: mafLogic
 This class represent a subclass of the QMdiSubWindow and allows to animate
 the show and close throught the animation of its geometry property.
*/
class MAFGUISHARED_EXPORT mafMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(QMdiSubWindow);

public:
    /// Object constructor
    mafMdiSubWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
    /*virtual*/ void setVisible(bool visible);

protected:
    bool event(QEvent *event);
    QEvent *ce;

private slots:
    void animationEnded();
};

} // namespace mafGUI

#endif // MAFMDISUBWINDOW_H
