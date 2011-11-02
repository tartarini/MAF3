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

/**
 Class name: mafView
 This is the base class for the MAF3 views.
 */
class MAFRESOURCESSHARED_EXPORT mafView : public mafResource {
    Q_OBJECT
    Q_PROPERTY(QVariant renderWidget READ renderWidget WRITE setRenderingWidget)
    Q_PROPERTY(QString configurationFile READ configurationFile WRITE setConfigurationFile)
    Q_PROPERTY(QVariant hierarchy READ hierarchy STORED false)

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

    /// Set default visual pipe for a type of data.
    void plugVisualPipeBindingHash(QHash<QString , QString> *hash);

    /// Set rendering window used by the view.
    void setRenderingWidget(QVariant renWidget);

    /// Get the rendering widget used by the view to render its scene.
    QVariant renderWidget() const;

    /// Return SceneNode hierarchy.
    QVariant hierarchy() const;

    /// return mafSceneNode containing vme.
    mafSceneNode *sceneNodeFromVme(mafObjectBase *vme);
    
    /// clear and delete the SceneGraphs.
    void clearScene();

    ///Get the selected SceneNode.
    mafSceneNode *selectedSceneNode();

    /// Set name of the view (usually is the name that appears on the menu.
    void setViewName(QString view_name);
    
    /// update scene nodes information from VMEs.
    void updateSceneNodesInformation();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafView();
    
    /// factory method for creating SceneNode.
    virtual mafSceneNode *createSceneNode(mafVME *vme);

    /// Initialize the SceneGraph and fill it with the VME hierarchy.
    /**
    This method is called automatically by each mafView subclass at the end of the initialize() method
    after that each setup code for the view and rendering widget has been created.
    */
    void setupSceneGraph();

    /// Emit signal to inform about visual pipe of the current node.
    void notityVisualPipeSelected();
    
    QString m_ViewName; ///< Name associated to the specific view (usually name that appears in the view menu).
    QObject *m_RenderWidget; ///< Rendering widget for the view.
    mafCore::mafHierarchyPointer m_Scenegraph; ///< SceneGraph
    QHash<QString, QString> *m_VisualPipeHash; ///< Bind between dataType and Visual pipe.
    bool m_Selected; ///< Flag for active view.
    mafSceneNode *m_SelectedNode; ///< Keep track of the selected SceneNode.
    QList<mafSceneNode *> m_SceneNodeList; ///< variable useful for rapid iteration between mafTreeItem
    
    mafResources::mafPipeVisual *m_PipeVisualSelection; ///< variable with the pipe for vme selection
    
    unsigned int m_VisibleObjects;

private:
    QString m_LayoutConfigurationFile; ///< XML filename that represents the layout configuration file for compound view

Q_SIGNALS:

    /// Notify selection of a sceneNode. 
    void pipeVisualSelectedSignal(mafCore::mafObjectBase *pipeVisual);
        
public Q_SLOTS:
    /// Wrap the new VME into a mafSceneNode.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Update view.
    virtual void updateView();

    /// Get name of the view.
    QString viewName() const;

private Q_SLOTS:

    /// Called when the scene node has been destroyed.
    void sceneNodeDestroy();

    /// Allow to keep track of the selected SceneNode.
    void vmeSelect(mafCore::mafObjectBase *vme);
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

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

inline QVariant mafView::hierarchy() const {
    QVariant v;
    v.setValue<mafCore::mafHierarchyPointer>(m_Scenegraph);
    return v;
}

inline mafSceneNode * mafView::selectedSceneNode(){
    return m_SelectedNode;
}

inline void  mafView::setViewName(QString view_name){
    m_ViewName = view_name ;
}

inline QString mafView::viewName() const{
    return m_ViewName;
}

} //namespace mafResources


#endif // MAFVIEW_H
