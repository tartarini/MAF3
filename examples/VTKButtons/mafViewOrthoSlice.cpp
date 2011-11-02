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

#include <mafEventBusManager.h>

using namespace mafEventBus;

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafResources::mafView(code_location) {
	setConfigurationFile("OrthoSlice.xml");
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
}

bool mafViewOrthoSlice::initialize() {
    if (Superclass::initialize()) {
        QString cf(configurationFile());
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(QString, cf));
        QGenericReturnArgument ret_val = mafEventReturnArgument(QObject *, m_RenderWidget);
        mafEventBusManager::instance()->notifyEvent("maf.local.gui.compoundWidgetConfigure", mafEventTypeLocal, &argList, &ret_val);
        if (m_RenderWidget == NULL) {
            qCritical() << mafTr("Problem loading layout configuration file %1").arg(cf);
        }
        setupSceneGraph();
        return m_RenderWidget != NULL;
    }
    return false;
}
