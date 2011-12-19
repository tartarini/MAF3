/*
 *  mafToolHandler.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLHANDLER_H
#define MAFTOOLHANDLER_H

// Includes list
#include <mafObjectBase.h>
#include "mafResourcesDefinitions.h"


namespace mafResources {

// Class forwarding
class mafTool;


/**
 Class name: mafToolHandler
 This is the class that is in charge to manage the MAF tools.
 */
class MAFRESOURCESSHARED_EXPORT mafToolHandler : public mafCore::mafObjectBase {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafToolHandler(const QString code_location = "");

    /// Add a tool to be handled
    void addTool(mafTool *tool, bool follow_selection = true);

    /// Return the list of added tools
    QList<mafTool *> *toolList();

    /// Remove the given tool from the list
    void removeTool(mafTool *tool);

    /// Remove all the tools from the handler.
    void removeAllTools();

protected:
    /// Object destructor.
    /* virtual */ ~mafToolHandler();

private:
    QList<mafTool *> m_ToolList; ///< List of added tools.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QList<mafTool *> *mafToolHandler::toolList() {
    return &m_ToolList;
}

} //namespace mafResources

#endif // MAFTOOLHANDLER_H
