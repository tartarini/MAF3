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

mafSplitter::mafSplitter(QWidget * parent) : QSplitter( parent ) {
    setOpaqueResize(true);
}

void mafSplitter::paintEvent (QPaintEvent *) {
    Q_EMIT synchronizeSplitterSignal(this);
}

void mafSplitter::adjustPosition(int p) {
    moveSplitter(p,1);
    refresh();
}
