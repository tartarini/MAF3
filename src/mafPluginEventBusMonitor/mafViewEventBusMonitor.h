/*
 *  mafViewScriptInterpreter.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWEVENTBUSMONITOR_H
#define MAFVIEWEVENTBUSMONITOR_H

#include <mafView.h>
 
#include "mafPluginEventBusMonitor_global.h"
#include "mafPluginEventBusMonitorDefinitions.h"


//class forwarding
class mafDiagramScene;

namespace mafPluginEventBusMonitor {

/**
 Class name: mafViewEventBusMonitor
 Base class for mafView representing a script console.
*/
class MAFPLUGINEVENTBUSMONITORSHARED_EXPORT mafViewEventBusMonitor : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewEventBusMonitor(const QString code_location = "");
    
    /// initialize components for the view.
    /*virtual*/ bool initialize();

    /// generate diagram of event
    void generateEventBusDiagramConnections();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewEventBusMonitor();

private:
    mafDiagramScene *m_Scene;
};

} // namespace mafScriptInterpreter

#endif // MAFVIEWEVENTBUSMONITOR_H
