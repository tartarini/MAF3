#ifndef MAFSYNTAXHIGHLIGHTERMAFLOGGER_H
#define MAFSYNTAXHIGHLIGHTERMAFLOGGER_H

#include "mafSyntaxHighlighter.h"

namespace mafGUI {
	
// forward declaration
class mafTextDocument;

class MAFGUISHARED_EXPORT mafSyntaxHighlighterMAFLogger : public mafSyntaxHighlighter {
public:
     mafSyntaxHighlighterMAFLogger(QTextDocument *parent = 0);
};

} // end namespace

#endif //MAFSYNTAXHIGHLIGHTERMAFLOGGER_H
