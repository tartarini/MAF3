#include "mafViewScriptInterpreter.h"
 
class mafViewScriptInterpreterPython : public mafViewScriptInterpreter {
    Q_OBJECT
    mafSuperclassMacro(mafViewScriptInterpreter);
    
public:
    /// Object constructor.
    mafViewScriptInterpreterPython(const QString code_location = "");
    
    /// create components for the view.
    /*virtual*/ void create();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreterPython();
};
