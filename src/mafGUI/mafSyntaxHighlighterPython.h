#ifndef MAFSYNTAXHIGHLIGHTERPYTHON_H
#define MAFSYNTAXHIGHLIGHTERPYTHON_H

#include "mafSyntaxHighlighter.h"

namespace mafGUI {
	
// forward declaration
class mafTextDocument;

class mafSyntaxHighlighterPython : public mafSyntaxHighlighter {
public:
     mafSyntaxHighlighterPython(QTextDocument *parent = 0);
};

} // end namespace

#endif //MAFSYNTAXHIGHLIGHTERPYTHON_H
