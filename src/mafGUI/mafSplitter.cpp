/*
 *  mafSplitter.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 25/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSplitter.h"

mafSplitter::mafSplitter(Qt::Orientation orientation, QWidget * parent) : QSplitter(orientation, parent ) {
    setOpaqueResize(true);
}

mafSplitter::~mafSplitter() {
    int v = 0;
    int n = m_Views.count();
    for (; v < n; ++v) {
        mafDEL(m_Views[v]);
    }
}

void mafSplitter::paintEvent (QPaintEvent *) {
    Q_EMIT synchronizeSplitterSignal(this);
}

void mafSplitter::adjustPosition(int p) {
    moveSplitter(p,1);
    refresh();
}

void mafSplitter::setViews(QList<mafCore::mafObjectBase*> &vlist) {
    int v = 0;
    int n = m_Views.count();

    if (m_Views.count() != 0) {
        // remove the old render widget from the splitter
        QObjectList c = this->children();
        Q_FOREACH(QObject *obj, c) {
            if (obj) {
                obj->setParent(NULL);
                delete obj;
            }
        }
        for (; v < n; ++v) {
            mafDEL(m_Views[v]);
        }
    }
    // Add the view list to the m_Views
    v = 0;
    n = vlist.count();
    for (; v < n; ++v) {
        addView(vlist[v]);
    }
}

void mafSplitter::addView(mafCore::mafObjectBase *view) {
    REQUIRE(view);

    view->retain();
    view->initialize();
    view->setParent(this);
    m_Views.append(view);
    QVariant r = view->property("renderWidget");
    QObject *obj = r.value<QObject *>();
    if (obj) {
        this->addWidget((QWidget *)obj);
    } else {
        qCritical() << mafTr("Object named \"%1\" doesn't contains a widget").arg(view->objectName());
    }
}
