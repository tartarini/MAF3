/*
 *  mafSyntaxHighlighterECMAScript.h
 *  mafScriptEditor
 *
 *  Created by Paolo Quadrani on 27/03/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSYNTAXHIGHLIGHTERECMASCRIPT_H
#define MAFSYNTAXHIGHLIGHTERECMASCRIPT_H

#include "mafScriptInterpreter_global.h"
#include <mafSyntaxHighlighter.h>

namespace mafScriptInterpreter {

/**
    Class Name: mafSyntaxHighlighterECMAScript
    Highlighter for ECMAScript.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafSyntaxHighlighterECMAScript : public mafGUI::mafSyntaxHighlighter {
public:
    ///Object Constructor. 
    mafSyntaxHighlighterECMAScript(QTextDocument *parent = 0);
};

} // end namespace

#endif // MAFSYNTAXHIGHLIGHTERECMASCRIPT_H
