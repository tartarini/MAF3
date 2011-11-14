/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"


mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafResources::mafViewCompound(code_location) {
	setConfigurationFile("OrthoSlice.xml");
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
}

bool mafViewOrthoSlice::initialize() {
    if (Superclass::initialize()) {
        QList<mafResources::mafView *> *views = viewList();
        mafResources::mafView *v;
        QString name;
        Q_FOREACH(v, *views) {
            name = v->objectName();
            if (name.contains("Parallel", Qt::CaseSensitive)) {
                QVariant rw = v->renderWidget();
                QObject *obj = rw.value<QObject *>();
                obj->setProperty("parallelCameraMode", QVariant(true));
            }
        }
        return true;
    }
    return false;
}
