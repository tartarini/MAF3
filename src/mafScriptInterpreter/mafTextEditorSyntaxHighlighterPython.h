/* mafTextEditorSyntaxHighlighterPython.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Apr 15 12:37:51 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Apr 15 12:38:54 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 2
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFTEXTEDITORSYNTAXHIGHLIGHTERPYTHON_H
#define MAFTEXTEDITORSYNTAXHIGHLIGHTERPYTHON_

#include "mafScriptInterpreter_global.h"
#include <mafTextEditorSyntaxHighlighter.h>

namespace mafScriptInterpreter {

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafTextEditorSyntaxHighlighterPython : public mafTextEditorSyntaxHighlighter
{
public:
     mafTextEditorSyntaxHighlighterPython(mafTextEditor *editor = 0);
};

} //end namespace 

#endif
