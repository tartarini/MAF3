/*
 *  mafScriptEngine.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 18/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafObject.h>

// Include list
#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

class mafScriptEditor;

/**
 Class name: mafScriptEngine
 Singleton Class that will manage the execution of the scripts associated with the MAF object passed during its registration.
 Defines topic for object registration, un-registration and trigger execution.
 - maf.local.script.register
 - maf.local.script.unregister
 - maf.local.script.execute
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEngine : public mafCore::mafObjectBase {
    Q_OBJECT
    mafSuperclassMacro(mafCore::mafObjectBase);
	
Q_SIGNALS:
	/// Signal emitted to register the given object.
    void registerObjectToManagerSignal(mafCore::mafObject *obj);
	
	/// Signal emitted to unregister the given object.
    void unregisterObjectFromManagerSignal(mafCore::mafObject *obj);
	
	/// Signal emitted to request the execution of a script associated to the given object.
    bool executeScriptSignal(mafCore::mafObject *obj);

public Q_SLOTS:
	/// Register the given object.
    void registerObjectToManager(mafCore::mafObject *objectWithScript);
	
	/// Unregister the given object.
    void unregisterObjectFromManager(mafCore::mafObject *objectWithScript);
	
	/// Request the execution of a script associated to the given object.
    bool executeScript(mafCore::mafObject *objectWithScript = NULL);

    /// Register the given object.
    bool isObjectRegistered(mafCore::mafObject *objectToCheck);
	
public:
    /// Return an instance of the engine
    static mafScriptEngine *instance();
    
    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

protected:
    /// Object destructor.
    /* virtual */ ~mafScriptEngine();
	
private:
    /// Object constructor.
    mafScriptEngine(const QString code_location = "");

    QList<mafCore::mafObject *> m_ObjectsWithScript; ///< List of objects that have a script attached.
    mafScriptEditor *m_Interpreter; ///< Script interpreter that will execute the script.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafScriptEngine::isObjectRegistered(mafCore::mafObject *objectToCheck) {
    return m_ObjectsWithScript.contains(objectToCheck);
}


} // namespace mafScriptInterpreter
