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
class mafVME;
class mafVisualPipe;

/**
 Class name: mafView
 This is the base class for the MAF3 views.
 */

class MAFRESOURCESSHARED_EXPORT mafView : public mafResource {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafView(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafView();

    /// Create the view
    virtual void create();

    /// Add a VME to the scenegraph.
    void addVME(mafVME *vme);

    /// Remove a VME from the scengraph.
    virtual void removeVME(mafVME *vme);

    /// Select a VME.
    void selectVME(mafVME *vme, bool select);

    /// Called to show/hide VME.
    virtual void showVME(mafVME *vme, bool show = true, const mafString visualPipeType = "");

    /// Select this view.
    void select(bool select);

    /// Return true if view is selected.
    bool isSelected();

    /// Set default visual pipe for a type of data.
    void plugVisualPipe(mafString dataType, const mafString visualPipeType);

    /// Set rendering window used by the view.
    void setRenderingWindow(mafCore::mafContainerInterface *renWindow);

  protected:
    mafCore::mafContainerInterface *m_RenWindow; ///< Rendering window for the view.
    mafString m_DefaultVisualPipe; ///< Name of the default visual pipe.
    mafCore::mafHierarchy *m_Scenegraph; ///< Scenegraph
    mafMap<mafString, mafString> m_CustomVisualPipeMap; ///< Bind between dataType and Visual pipe.
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

inline void mafView::setRenderingWindow(mafCore::mafContainerInterface *renWindow) {
    m_RenWindow = renWindow;
}

} //namespace mafResources


#endif // MAFVIEW_H
