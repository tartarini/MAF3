/*
 *  mafViewScriptInterpreter.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafView.h>
 
#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

/**
 Class name: mafScriptEditorSynchronizer
 Class with the capability of synchronizing editors when using multiple threads.
 It is a singleton.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafViewScriptInterpreter : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewScriptInterpreter(const QString code_location = "");
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreter();
};

} // namespace mafScriptInterpreter
