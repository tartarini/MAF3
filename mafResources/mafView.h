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
#include <mafContainerInterface.h>
#include <mafHierarchy.h>

namespace mafResources {

// Class forwarding list
class mafSceneNode;
class mafVisualPipe;

/**
 Class name: mafView
 This is the base class for the MAF3 views.
 */
class MAFRESOURCESSHARED_EXPORT mafView : public mafResource {
    Q_OBJECT
    Q_PROPERTY(QVariant renderWidget READ renderingWidget WRITE setRenderingWidget)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafView(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafView();

    /// Create the view
    virtual void create();

    /// Remove a scene node from the scengraph.
    virtual void removeSceneNode(mafSceneNode *node);

    /// Select a scene node.
    virtual void selectSceneNode(mafSceneNode *node, bool select);

    /// Called to show/hide scene node.
    virtual void showSceneNode(mafSceneNode *node, bool show = true, const mafString visualPipeType = "");

    /// Select this view.
    void select(bool select);

    /// Return true if view is selected.
    bool isSelected();

    /// Set default visual pipe for a type of data.
    void plugVisualPipe(mafString dataType, const mafString visualPipeType);

    /// Set rendering window used by the view.
    void setRenderingWidget(mafVariant renWidget);

    /// Get the rendering widget used by the view to render its scene.
    mafVariant renderingWidget() const;

private slots:
    /// Wrap the new VME into a mafSceneNode.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Called when the scene node has been destroyed.
    void sceneNodeDestroy();

  protected:
    mafCore::mafContainerInterface *m_RenderWidget; ///< Rendering widget for the view.
    mafString m_DefaultVisualPipe; ///< Name of the default visual pipe.
    mafCore::mafHierarchy *m_Scenegraph; ///< Scenegraph
    mafHash<mafString, mafString> m_VisualPipeHash; ///< Bind between dataType and Visual pipe.
    bool m_Selected; ///< Flag for active view.
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

inline void mafView::setRenderingWidget(mafVariant renWidget) {
    m_RenderWidget = renWidget.value<mafCore::mafContainerInterfacePointer>();
}

inline mafVariant mafView::renderingWidget() const {
    mafVariant v;
    v.setValue<mafCore::mafContainerInterfacePointer>(m_RenderWidget);
    return v;
}

} //namespace mafResources


#endif // MAFVIEW_H
