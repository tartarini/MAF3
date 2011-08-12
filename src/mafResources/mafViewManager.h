/*
 *  mafViewManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWMANAGER_H
#define MAFVIEWMANAGER_H

#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafView;
class mafResource;
class mafVME;
class mafPipeVisual;

/**
Class name: mafViewManager
This class provides the manager class for MAF3 views. The topics are:
- maf.local.resources.view.customizeVisualization allows to plug visual pipes associated with VME for a given view type.
- maf.local.resources.view.create allows to create a new View.
- maf.local.resources.view.created notify all the observers that a new View has been created.
- maf.local.resources.view.destroy allows to destroy a given View.
- maf.local.resources.view.select allows to select the active View on which show/hide VMEs.
- maf.local.resources.view.selected allows to retrieve the current selected View.
- maf.local.resources.view.sceneNodeReparent allows to reparent scene nodes.
- maf.local.resources.view.sceneNodeShow allow to show/hide a given mafSceneNode in the selected View.
- maf.local.resources.view.noneViews emitted when all views are destroyed.
- maf.local.resources.view.clearViews
- maf.local.resources.view.fillViews
- maf.local.resources.view.pipeVisualSelected allows to retrieve the current selected pipe visual.

*/
class MAFRESOURCESSHARED_EXPORT mafViewManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafViewManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

signals:
    /// Return the selected view
    mafCore::mafObjectBase * selectedViewSignal();

    /// Signal which allow to keep track of the selected view.
    void selectViewSignal(mafCore::mafObjectBase *view);

    /// Signal which allow to reparent scene nodes of the tree.
    void sceneNodeReparentSignal(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent);

    /// Signal which allow to show/hide the given node passed as argument.
    void sceneNodeShowSignal(mafCore::mafObjectBase *node, bool show = true);

    /// Signal which allow to remove the view from the manager.
    void destroyViewSignal(mafCore::mafObjectBase *view);

    /// Allow to create a new mafView given the type name as string. Return the created view on success.
    void createViewSignal(QString view_type);

    /// Notify the view creation to all observers.
    void viewCreatedSignal(mafCore::mafObjectBase *view);

    /// Notify the clear of all the scene-graphs.
    void clearViewsSignal();
    
    /// Notify the fill of all the scenegraphs.
    void fillViewsSignal();

    /// Notify the destruction on all views
    void noneViewsSignal();

    /// Signal associated to the 
    void customPipeVisualForVMEInViewSignal(QString view_type, QString data_type, QString pipe_type);

public slots:
    /// Create a memento class used to save the status of opened views. The caller has to delete the allocated memory he asked.
    /** This method allows to save the status of opened views when logic
    emits the signal maf.local.logic.settings.view.store.
    The manager save the number and types of opened views, their position and size.
    Then each view save its settings depending on the view itself.*/
    mafCore::mafMemento *createMemento() const;

    /// Allows to rebuild a previous saved object's state.
    /**
    This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization purposes.*/
    void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

private slots:
    /// Allow to keep track of the selected view.
    mafCore::mafObjectBase *selectedView();

    /// Allow to keep track of the selected view.
    void selectView(mafCore::mafObjectBase *view);
    
    /// Allow to show/hide the given scene node passed as argument.
    void sceneNodeShow(mafCore::mafObjectBase *node, bool show = true);

    /// Remove the view from the manager.
    void destroyView(mafCore::mafObjectBase *view);

    /// Allow to create a new mafView given the type name as string. Return the pointer to the created view on success.
    void createView(QString view_type);

    /// Method called when a view has been destroyed (by someone else).
    void viewDestroyed();
    
    /// Clear the scene-graphs of each view. 
    void clearViews();

    /// fill the scenegraphs of each view. 
    void fillViews();

    /// Allows to register the information of bindings between data type and visual pipe for the given view type.
    void customPipeVisualForVMEInView(QString view_type, QString data_type, QString pipe_type);

    /// Called by reparent of items in tree.
    void sceneNodeReparent(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent);

protected:
    /// Object destructor
    /*virtual*/ ~mafViewManager();

private:
    /// Object constructor.
    mafViewManager(const QString code_location = "");

    /// create IDs and connect signals with slots for View create, destroy, select and VME show.
    void initializeConnections();

    /// Add the created view to the view list.
    void addViewToCreatedList(mafView *v);

    /// Remove the given view from the created list
    void removeView(mafView *view);

    /// Delete all created views
    void destroyAllViews();

    /// Fill sceneGraph with sceneNodes.
    void fillSceneGraph(mafView *v, mafCore::mafHierarchy *hierarchy);
    
    /// Clear the scene-graph for view in the argument.
    void clearView(mafCore::mafObjectBase *view);

    mafResourceList m_CreatedViewList; ///< List of created views.
    mafView *m_SelectedView; ///< Keep trak of the current selected view.
    QHash<QString, QHash<QString, QString>* > m_VisualizationBindHash; ///< Hash containing the view type as key and the hash of binds between data type and visual pipe to use for rendering.
};

} // namespace mafResources

#endif // MAFVIEWMANAGER_H
