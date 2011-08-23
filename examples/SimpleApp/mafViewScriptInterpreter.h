#include <mafView.h>
 
class mafViewScriptInterpreter : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewScriptInterpreter(const QString code_location = "");
    
protected:
    /// Object destructor.
    /* virtual */ ~mafViewScriptInterpreter();
};
