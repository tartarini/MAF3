/*
 *  mafView.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani - Daniele Giunchi on 20/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
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
class mafToolHandler;

/**
 Class name: mafView
 This is the base class for the MAF3 views.
 */
class MAFRESOURCESSHARED_EXPORT mafView : public mafResource {
    Q_OBJECT
    Q_PROPERTY(QVariant renderWidget READ renderWidget WRITE setRenderingWidget STORED false)
    Q_PROPERTY(QString configurationFile READ configurationFile WRITE setConfigurationFile)
    Q_PROPERTY(QVariantHash visualPipeHash READ visualPipeHash WRITE setVisualPipeHash)
    Q_PROPERTY(mafCore::mafHierarchyPointer sceneGraph READ sceneGraph STORED false)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafView(const QString code_location = "");

    /// Assign the layout configuration file to the view.
    void setConfigurationFile(const QString configFile);

    /// Retrieve the layout configuration file associated to the view.
    QString configurationFile() const;

    /// Fill the SceneGraph with the VMEs present in the hierarchy.
    void fillSceneGraph(mafCore::mafHierarchy *hierarchy);

    /// Return the SceneGraph.
    mafCore::mafHierarchyPointer sceneGraph() const;

    /// Remove a scene node from the ScenGraph.
    virtual void removeSceneNode(mafSceneNode *node);

    /// Select a SceneNode.
    virtual void selectSceneNode(mafSceneNode *node, bool select);

    /// Called to show/hide SceneNode.
    virtual void showSceneNode(mafSceneNode *node, bool show = true);

    /// Select this view.
    void select(bool select);

    /// Return true if view is selected.
    bool isSelected();

    /// Set rendering window used by the view.
    void setRenderingWidget(QVariant renWidget);

    /// Get the rendering widget used by the view to render its scene.
    QVariant renderWidget() const;

    /// return mafSceneNode containing vme.
    mafSceneNode *sceneNodeFromVme(mafObjectBase *vme);
    
    /// clear and delete the SceneGraphs.
    virtual void clearScene();

    ///Get the selected SceneNode.
    mafSceneNode *selectedSceneNode();

    /// update scene nodes information from VMEs.
    void updateSceneNodesInformation();

    /// Return the hash that contains the association between data type and related visual pipe to use for render it.
    QVariantHash visualPipeHash() const;
    
    /// Assign the hash that will contains the association between data type and related visual pipe to use for render it.
    void setVisualPipeHash(const QVariantHash hash);

protected:
    /// Object destructor.
    /* virtual */ ~mafView();

    /// Initialize the SceneGraph and fill it with the VME hierarchy.
    /**
    This method is called automatically by each mafView subclass at the end of the initialize() method
    after that each setup code for the view and rendering widget has been created.
    */
    void setupSceneGraph();

    /// Emit signal to inform about visual pipe of the current node.
    void notityVisualPipeSelected();

    /// factory method for creating SceneNode.
    virtual mafSceneNode *createSceneNode(mafVME *vme);

    QObject *m_RenderWidget; ///< Rendering widget for the view.
    bool m_Selected; ///< Flag for active view.
    mafSceneNode *m_SelectedNode; ///< Keep track of the selected SceneNode.
    
    mafResources::mafPipeVisual *m_PipeVisualSelection; ///< variable with the pipe for vme selection
    
    unsigned int m_VisibleObjects; ///< Number of visible objects in view.
    QString m_SceneNodeType; ///< Type of SceneNode to instantiate for the current view.
    mafToolHandler *m_ToolHandler; ///< Handler for tools in view.

private:
    QString m_LayoutConfigurationFile; ///< XML filename that represents the layout configuration file for compound view
    QVariantHash *m_VisualPipeHash; ///< Bind between dataType and Visual pipe.
    QHash<QString, QVariantHash> m_PipeParametersBindHash; ///< Hash containing the bind between visual pipe and its initialization parameter's hash to be assigned to the pipe when allocated.
    mafCore::mafHierarchyPointer m_Scenegraph; ///< SceneGraph
    QHash<mafVME *, mafSceneNode *> m_SceneNodeHash; ///< variable useful for rapid iteration between mafTreeItem


Q_SIGNALS:
    /// Notify selection of a sceneNode. 
    void pipeVisualSelectedSignal(mafCore::mafObjectBase *pipeVisual);
        
public Q_SLOTS:
    /// Wrap the new VME into a mafSceneNode.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Update view.
    virtual void updateView();

    /// Reset the visualization to show visible objects
    virtual void resetVisualization(double *bounds = NULL);

    /// Allows to bing visual pipe type with parameter's hash used to initialize its pipe algorithm parameters when allocated.
    void plugPipeParametersHashItem(QString pipe_type, QVariantHash hash);

private Q_SLOTS:

    /// Called when the scene node has been destroyed.
    void sceneNodeDestroy();

    /// Allow to keep track of the selected SceneNode.
    void vmeSelect(mafCore::mafObjectBase *vme);
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafHierarchyPointer mafView::sceneGraph() const {
    return m_Scenegraph;
}

inline QVariantHash mafView::visualPipeHash() const {
    return *m_VisualPipeHash;
}

inline void mafView::setVisualPipeHash(const QVariantHash hash) {
    if (m_VisualPipeHash == NULL) {
        m_VisualPipeHash = new QVariantHash();
    }
    m_VisualPipeHash->unite(hash);
}

inline void mafView::plugPipeParametersHashItem(QString pipe_type, QVariantHash hash) {
    m_PipeParametersBindHash.insert(pipe_type, hash);
}

inline void mafView::setConfigurationFile(const QString configFile) {
    m_LayoutConfigurationFile = configFile;
}

inline QString mafView::configurationFile() const {
    return m_LayoutConfigurationFile;
}

inline bool mafView::isSelected() {
    return m_Selected;
}

inline void mafView::setRenderingWidget(QVariant renWidget) {
    m_RenderWidget = renWidget.value<QObject*>();
}

inline QVariant mafView::renderWidget() const {
    QVariant v;
    v.setValue<QObject*>(m_RenderWidget);
    return v;
}

inline mafSceneNode * mafView::selectedSceneNode(){
    return m_SelectedNode;
}

} //namespace mafResources


#endif // MAFVIEW_H
