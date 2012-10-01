/*
 *  mafMdiSubWindow.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 13/01/11.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMdiSubWindow.h"
#include <QPropertyAnimation>

using namespace mafGUI;

mafMdiSubWindow::mafMdiSubWindow(QWidget *parent, Qt::WindowFlags flags) : QMdiSubWindow(parent, flags) {
}

void mafMdiSubWindow::setVisible(bool visible) {
    QRect startRect = this->geometry();
    QRect endRect = startRect;

    if(visible) {
        // show
        startRect.setWidth(0);
        startRect.setHeight(0);
    } else {
        // hide
        endRect.setWidth(0);
        endRect.setHeight(0);
    }

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(200);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    Superclass::setVisible(visible);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void mafMdiSubWindow::animationEnded() {
    Superclass::event(ce);
}

bool mafMdiSubWindow::event(QEvent *event) {
    if(event->type() == QEvent::Close) {
        QRect startRect = this->geometry();
        QRect endRect = startRect;
        endRect.setWidth(0);
        endRect.setHeight(0);

        QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(startRect);
        animation->setEndValue(endRect);
        animation->setEasingCurve(QEasingCurve::InOutQuad);

        bool res = connect(animation, SIGNAL(finished()), this, SLOT(animationEnded()));
        ce = event;
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        return res;
    } else {
        return Superclass::event(event);
    }
}
