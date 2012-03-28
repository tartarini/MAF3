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

#ifndef MAFVIEWSCRIPTINTERPRETER_H
#define MAFVIEWSCRIPTINTERPRETER_H

#include <mafView.h>
 
#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

/**
 Class name: mafViewScriptInterpreter
 Base class for mafView representing a script console.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafViewScriptInterpreter : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewScriptInterpreter(const QString code_location = "");
    
    /// initialize components for the view.
    /*virtual*/ bool initialize();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreter();
};

} // namespace mafScriptInterpreter

#endif // MAFVIEWSCRIPTINTERPRETER_H
