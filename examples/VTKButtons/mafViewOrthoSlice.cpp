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
#include <mafSceneNode.h>

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
        extractViews(m_RenderWidget);
        setupSceneGraph();
        return m_RenderWidget != NULL;
    }
    return false;
}

void mafViewOrthoSlice::extractViews(QObject *root) {
    QObjectList c = root->children();
    Q_FOREACH(QObject *obj, c) {
        if (obj->children().count() > 0) {
            extractViews(obj);
        }
        mafView *v = qobject_cast<mafResources::mafView *>(obj);
        if (v) {
            m_ViewList.append(v);
        }
    }
}

void mafViewOrthoSlice::showSceneNode(mafResources::mafSceneNode *node, bool show) {
    mafResources::mafView *subView;
    Q_FOREACH(subView, m_ViewList) {
        subView->showSceneNode(subView->sceneNodeFromVme((mafCore::mafObjectBase *)node->vme()), show);
    }
}

mafResources::mafSceneNode *mafViewOrthoSlice::createSceneNode(mafResources::mafVME *vme) {
    mafResources::mafView *subView;
    Q_FOREACH(subView, m_ViewList) {
        subView->createSceneNode(vme);
    }
    return Superclass::createSceneNode(vme);
}

void mafViewOrthoSlice::selectSceneNode(mafResources::mafSceneNode *node, bool select) {
    mafResources::mafView *subView;
    Q_FOREACH(subView, m_ViewList) {
        if (node) {
            subView->selectSceneNode(subView->sceneNodeFromVme((mafCore::mafObjectBase *)node->vme()), select);
        } else {
            // reset the selected node for the sub-view.
            subView->selectSceneNode(NULL, select);
        }
    }
    Superclass::selectSceneNode(node, select);
}

void mafViewOrthoSlice::removeSceneNode(mafResources::mafSceneNode *node) {
    mafResources::mafView *subView;
    Q_FOREACH(subView, m_ViewList) {
        subView->removeSceneNode(subView->sceneNodeFromVme((mafCore::mafObjectBase *)node->vme()));
    }
    Superclass::removeSceneNode(node);
}

void mafViewOrthoSlice::updateView() {
    mafResources::mafView *subView;
    Q_FOREACH(subView, m_ViewList) {
        subView->updateView();
    }
}
