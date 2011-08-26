#ifndef MAFSYNTAXHIGHLIGHTERPYTHON_H
#define MAFSYNTAXHIGHLIGHTERPYTHON_H

#include "mafScriptInterpreter_global.h"
#include <mafSyntaxHighlighter.h>

namespace mafScriptInterpreter {
	
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafSyntaxHighlighterPython : public mafGUI::mafSyntaxHighlighter {
public:
     mafSyntaxHighlighterPython(QTextDocument *parent = 0);
};

} // end namespace

#endif //MAFSYNTAXHIGHLIGHTERPYTHON_H
