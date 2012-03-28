/*
 *  mafViewScriptInterpreterECMAScript.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 27/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWSCRIPTINTERPRETERECMASCRIPT_H
#define MAFVIEWSCRIPTINTERPRETERECMASCRIPT_H

#include "mafViewScriptInterpreter.h"
 
#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

/**
 Class name: mafViewScriptInterpreterECMAScript
 mafView representing the ECMAScript console.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafViewScriptInterpreterECMAScript : public mafViewScriptInterpreter {
    Q_OBJECT
    mafSuperclassMacro(mafScriptInterpreter::mafViewScriptInterpreter);
    
public:
    /// Object constructor.
    mafViewScriptInterpreterECMAScript(const QString code_location = "");
    
    /// initialize components for the view.
    /*virtual*/ bool initialize();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreterECMAScript();
};

} // namespace mafScriptInterpreter

#endif // MAFVIEWSCRIPTINTERPRETERECMASCRIPT_H
