#include "mafSyntaxHighlighterPython.h"

using namespace mafScriptInterpreter;

mafSyntaxHighlighterPython::mafSyntaxHighlighterPython(QTextDocument *parent) : mafGUI::mafSyntaxHighlighter(parent)
{
	// Python keywords
    QStringList keywords;
    keywords 
        << "\\bpython\\b"
        << "\\bimport\\b"
        << "\\bfrom\\b"
        << "\\bas\\b"
        << "\\bFalse\\b"
        << "\\bNone\\b"
        << "\\bTrue\\b"
        << "\\b__name__\\b"
        << "\\b__debug__\\b"
        << "\\bdef\\b"
        << "\\bclass\\b"
        << "\\breturn\\b"
        << "\\band\\b"
        << "\\bassert\\b"
        << "\\bbreak\\b"
        << "\\bcontinue\\b"
        << "\\bdel\\b"
        << "\\belif\\b"
        << "\\belse\\b"
        << "\\bexcept\\b"
        << "\\bexec\\b"
        << "\\bfinally\\b"
        << "\\bfor\\b"
        << "\\bglobal\\b"
        << "\\bif\\b"
        << "\\bin\\b"
        << "\\bis\\b"
        << "\\blambda\\b"
        << "\\bnot\\b"
        << "\\bor\\b"
        << "\\bpass\\b"
        << "\\bprint\\b"
        << "\\braise\\b"
        << "\\btry\\b"
        << "\\bwhile\\b"
        << "\\byield\\b"
        << "\\bArithmeticError\\b"
        << "\\bAssertionError\\b"
        << "\\bAttributeError\\b"
        << "\\bEnvironmentError\\b"
        << "\\bEOFError\\b"
        << "\\bException\\b"
        << "\\bFloatingPointError\\b"
        << "\\bImportError\\b"
        << "\\bIndentationError\\b"
        << "\\bIndexError\\b"
        << "\\bIOError\\b"
        << "\\bKeyboardInterrupt\\b"
        << "\\bKeyError\\b"
        << "\\bLookupError\\b"
        << "\\bMemoryError\\b"
        << "\\bNameError\\b"
        << "\\bNotImplementedError\\b"
        << "\\bOSError\\b"
        << "\\bOverflowError\\b"
        << "\\bReferenceError\\b"
        << "\\bRuntimeError\\b"
        << "\\bStandardError\\b"
        << "\\bStopIteration\\b"
        << "\\bSyntaxError\\b"
        << "\\bSystemError\\b"
        << "\\bSystemExit\\b"
        << "\\bTabError\\b"
        << "\\bTypeError\\b"
        << "\\bUnboundLocalError\\b"
        << "\\bUnicodeDecodeError\\b"
        << "\\bUnicodeEncodeError\\b"
        << "\\bUnicodeError\\b"
        << "\\bUnicodeTranslateError\\b"
        << "\\bValueError\\b"
        << "\\bWindowsError\\b"
        << "\\bZeroDivisionError\\b"
        << "\\bWarning\\b"
        << "\\bUserWarning\\b"
        << "\\bDeprecationWarning\\b"
        << "\\bPendingDeprecationWarning\\b"
        << "\\bSyntaxWarning\\b"
        << "\\bOverflowWarning\\b"
        << "\\bRuntimeWarning\\b"
        << "\\bFutureWarning\\b"		
        
        << "\\b__import__\\b"
        << "\\babs\\b"
        << "\\bapply\\b"
        << "\\bbasestring\\b"
        << "\\bbool\\b"
        << "\\bbuffer\\b"
        << "\\bcallable\\b"
        << "\\bchr\\b"
        << "\\bclassmethod\\b"
        << "\\bcmp\\b"
        << "\\bcoerce\\b"
        << "\\bcompile\\b"
        << "\\bcomplex\\b"
        << "\\bdelattr\\b"
        << "\\bdict\\b"
        << "\\bdir\\b"
        << "\\bdivmod\\b"
        << "\\benumerate\\b"
        << "\\beval\\b"
        << "\\bexecfile\\b"
        << "\\bfile\\b"
        << "\\bfilter\\b"
        << "\\bfloat\\b"
        << "\\bgetattr\\b"
        << "\\bglobals\\b"
        << "\\bhasattr\\b"
        << "\\bhash\\b"
        << "\\bhex\\b"
        << "\\bid\\b"
        << "\\binput\\b"
        << "\\bint\\b"
        << "\\bintern\\b"
        << "\\bisinstance\\b"
        << "\\bissubclass\\b"
        << "\\biter\\b"
        << "\\blen\\b"
        << "\\blist\\b"
        << "\\blocals\\b"
        << "\\blong\\b"
        << "\\bmap\\b"
        << "\\bmax\\b"
        << "\\bmin\\b"
        << "\\bobject\\b"
        << "\\boct\\b"
        << "\\bopen\\b"
        << "\\bord\\b"
        << "\\bpow\\b"
        << "\\bproperty\\b"
        << "\\brange\\b"
        << "\\braw_input\\b"
        << "\\breduce\\b"
        << "\\breload\\b"
        << "\\brepr\\b"
        << "\\bround\\b"
        << "\\bsetattr\\b"
        << "\\bslice\\b"
        << "\\bstaticmethod\\b"
        << "\\bstr\\b"
        << "\\bsum\\b"
        << "\\bsuper\\b"
        << "\\btuple\\b"
        << "\\btype\\b"
        << "\\bunichr\\b"
        << "\\bunicode\\b"
        << "\\bvars\\b"
        << "\\bxrange\\b"
        << "\\bzip\\b";
    
    QTextCharFormat format;
    format.setForeground(Qt::darkBlue);
    format.setFontWeight(QFont::Bold);
        
    int count = 0;
    Q_FOREACH(const QString &pattern, keywords) {
        insertRule(QString::number(count++), QRegExp(pattern), format);
    }

    //Classes
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkMagenta);
    insertRule("class",QRegExp("\\b(maf[A-Za-z]+)|(self)\\b"), format);
    
    //Functions
    format.setFontItalic(true);
    format.setForeground(Qt::blue);
    insertRule("function",QRegExp("\\b[A-Za-z0-9_]+(?=\\()"), format);
    
    //Quotation
    format.setForeground(Qt::darkGreen);
    insertRule("quotation",QRegExp("(\".*\")|('.*')"), format);
    
    //Singleline comment
    format.setForeground(Qt::red);
    insertRule("singleLineComment",QRegExp("#[^\n]*"), format);
    
    //Multiline comment
    format.setForeground(Qt::red);
    setMultilineRule(QRegExp("(\"\"\")"), QRegExp("(\"\"\")"),  format);
    
    //Integer
    format.setForeground(Qt::darkYellow);
    insertRule("integer",QRegExp("\\b([1-9][0-9]*|0)([Uu]([Ll]|LL|ll)?|([Ll]|LL|ll)[Uu]?)?\\b"), format);
    

    //Floating point
    format.setForeground(Qt::darkBlue);
    insertRule("float", QRegExp("\\b([0-9]+[Ee][-]?[0-9]+|([0-9]*\\.[0-9]+|[0-9]+\\.)([Ee][-]?[0-9]+)?)[fFlL]?\\b"), format);
    

    //Octal number
    format.setForeground(Qt::cyan);
    insertRule("octal",QRegExp("\\b0[0-7]+([Uu]([Ll]|LL|ll)?|([Ll]|LL|ll)[Uu]?)?\\b"), format);
    
    //Hex number
    format.setForeground(Qt::green);
    insertRule("hexadecimal",QRegExp("\\b0[xX][0-9a-fA-F]+([Uu]([Ll]|LL|ll)?|([Ll]|LL|ll)[Uu]?)?\\b"), format);
}
