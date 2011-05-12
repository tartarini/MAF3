/*
 *  mafView.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEW_H
#define MAFVIEW_H

// Includes list
#include "mafResource.h"
#include <mafProxyInterface.h>
#include <mafHierarchy.h>

namespace mafResources {

// Class forwarding list
class mafSceneNode;
class mafPipeVisual;

/**
 Class name: mafView
 This is the base class for the MAF3 views.
 */
class MAFRESOURCESSHARED_EXPORT mafView : public mafResource {
    Q_OBJECT
    Q_PROPERTY(QVariant renderWidget READ renderingWidget WRITE setRenderingWidget)
    Q_PROPERTY(QVariant hierarchy READ hierarchy )

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafView(const QString code_location = "");

    /// Create the view
    virtual void create();

    /// Remove a scene node from the scengraph.
    virtual void removeSceneNode(mafSceneNode *node);

    /// Select a scene node.
    void selectSceneNode(mafSceneNode *node, bool select);

    /// Called to show/hide scene node.
    virtual void showSceneNode(mafSceneNode *node, bool show = true, const QString visualPipeType = "");

    /// Select this view.
    void select(bool select);

    /// Return true if view is selected.
    bool isSelected();

    /// Set default visual pipe for a type of data.
    void plugVisualPipe(QString dataType, const QString visualPipeType);

    /// Set rendering window used by the view.
    void setRenderingWidget(QVariant renWidget);

    /// Get the rendering widget used by the view to render its scene.
    QVariant renderingWidget() const;

    /// Return sceneNode hierarchy.
    QVariant hierarchy() const;

    /// return mafSceneNode containing vme.
    mafSceneNode *sceneNodeFromVme(mafObjectBase *vme);
    
    /// clear and delete the scenegraphs.
    void clearScene();
    
public slots:
    /// Wrap the new VME into a mafSceneNode.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Update view.
    virtual void updateView();

private slots:

    /// Called by reparent of items in tree.
    void sceneNodeReparent(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent);

    /// Called when the scene node has been destroyed.
    void sceneNodeDestroy();

    /// Allow to keep track of the selected SceneNode.
    void vmeSelect(mafCore::mafObjectBase *vme);

protected:
    /// Object destructor.
    /* virtual */ ~mafView();



    QObject *m_RenderWidget; ///< Rendering widget for the view.
    QString m_DefaultVisualPipe; ///< Name of the default visual pipe.
    mafCore::mafHierarchyPointer m_Scenegraph; ///< Scenegraph
    QHash<QString, QString> m_VisualPipeHash; ///< Bind between dataType and Visual pipe.
    bool m_Selected; ///< Flag for active view.
    mafSceneNode *m_SelectedNode; ///< Keep track of the selected SceneNode.
    QList<mafSceneNode *> m_SceneNodeList; ///< variable useful for rapid iteration between mafTreeItem
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafView::select(bool select) {
    m_Selected = select;
}

inline bool mafView::isSelected() {
    return m_Selected;
}

inline void mafView::setRenderingWidget(QVariant renWidget) {
    m_RenderWidget = renWidget.value<QObject*>();
}

inline QVariant mafView::renderingWidget() const {
    QVariant v;
    v.setValue<QObject*>(m_RenderWidget);
    return v;
}

inline QVariant mafView::hierarchy() const {
    QVariant v;
    v.setValue<mafCore::mafHierarchyPointer>(m_Scenegraph);
    return v;
}

} //namespace mafResources


#endif // MAFVIEW_H
