#ifndef MAFSYNTAXHIGHLIGHTERPYTHON_H
#define MAFSYNTAXHIGHLIGHTERPYTHON_H

#include "mafScriptInterpreter_global.h"
#include <mafSyntaxHighlighter.h>

namespace mafScriptInterpreter {

/**
    Class Name: mafSyntaxHighlighterPython
    Highlighter for Python.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafSyntaxHighlighterPython : public mafGUI::mafSyntaxHighlighter {
public:
    ///Object Constructor. 
    mafSyntaxHighlighterPython(QTextDocument *parent = 0);
};

} // end namespace

#endif //MAFSYNTAXHIGHLIGHTERPYTHON_H
