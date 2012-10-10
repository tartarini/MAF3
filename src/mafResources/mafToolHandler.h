/*
 *  mafToolHandler.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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

// mafResources module class forwarding
class mafTool;
class mafSceneNode;

/**
 Class name: mafToolHandler
 This is the class that is in charge to manage the MAF3 tools.
 To use this class, simply: 
 - Instantiate it.
 - Assign to it the graphic object which contains the layer in which the tools are visualized. 
 - Add the tools that you want to be handled and then when the active SceneNode is set, the handler update the visibility of the tools that have the followSceneNode flag active.
 */
class MAFRESOURCESSHARED_EXPORT mafToolHandler : public mafCore::mafObjectBase {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafToolHandler(const QString code_location = "");

    /// Add a tool to be handled
    void addTool(mafTool *tool);

    /// Return the list of added tools
    QList<mafTool *> *toolList();

    /// Remove the given tool from the list
    void removeTool(mafTool *tool);

    /// Remove all the tools from the handler.
    void removeAllTools();

    /// Assign the active SceneNode.
    virtual void setActiveSceneNode(mafSceneNode *node);

    /// Set the visibility of the tool
    void setVisibility(bool visible = true);

    /// Assign the graphic object containing the rendering layer in which visualize the tools.
    void setGraphicObject(QObject *graphicObject);

protected:
    /// Object destructor.
    /* virtual */ ~mafToolHandler();

private:
    QList<mafTool *> m_ToolList;  ///< List of added tools.
    mafSceneNode    *m_SceneNode; ///< Last active SceneNode.
    QObject         *m_GraphicObject;  ///< Graphic object containing the renderer associated to the tool layer.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QList<mafTool *> *mafToolHandler::toolList() {
    return &m_ToolList;
}

inline void mafToolHandler::setGraphicObject(QObject *graphicObject) {
    REQUIRE(graphicObject);
    m_GraphicObject = graphicObject;
}


} //namespace mafResources

#endif // MAFTOOLHANDLER_H
