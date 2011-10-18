/*
 *  mafViewScriptInterpreterPython.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewScriptInterpreter.h"
 
#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

/**
 Class name: mafViewScriptInterpreterPython
 mafView representing the Python console.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafViewScriptInterpreterPython : public mafViewScriptInterpreter {
    Q_OBJECT
    mafSuperclassMacro(mafScriptInterpreter::mafViewScriptInterpreter);
    
public:
    /// Object constructor.
    mafViewScriptInterpreterPython(const QString code_location = "");
    
    /// create components for the view.
    /*virtual*/ void create();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreterPython();
};

} // namespace mafScriptInterpreter
