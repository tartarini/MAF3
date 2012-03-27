/*
 *  mafSyntaxHighlighterECMAScript.cpp
 *  mafScriptEditor
 *
 *  Created by Paolo Quadrani on 27/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSyntaxHighlighterECMAScript.h"

using namespace mafScriptInterpreter;

mafSyntaxHighlighterECMAScript::mafSyntaxHighlighterECMAScript(QTextDocument *parent) : mafGUI::mafSyntaxHighlighter(parent)
{
	// ECMAScript keywords
    QStringList keywords;
    keywords 
        << "\\bvar\\b"
        << "\\bfunction\\b"
        << "\\bif\\b"
        << "\\belse\\b"
        << "\\bfalse\\b"
        << "\\btrue\\b"
        << "\\btry\\b"
        << "\\bcatch\\b"
        << "\\bfinally\\b"
        << "\\bdo\\b"
        << "\\bwhile\\b"
        << "\\bfor\\b"
        << "\\breturn\\b"
        << "\\balert\\b"
        << "\\bnull\\b"
        << "\\bthis\\b"
        << "\\bnew\\b"
        << "\\btypeof\\b"
        << "\\bthrow\\b"
        << "\\bFunction\\b"
        << "\\bNaN\\b"
        << "\\bundefined\\b"
        << "\\bswitch\\b"
        << "\\bcase\\b"
        << "\\bbreak\\b"
        << "\\bdefault\\b"
        << "\\bin\\b"
        << "\\bwith\\b"
        << "\\bcontinue\\b"
        << "\\bdelete\\b"
        << "\\binstanceof\\b"
        << "\\beval\\b"
        << "\\bdocument\\b"
        << "\\bwrite\\b";
    
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
    format.setForeground(Qt::red);
    insertRule("quotation",QRegExp("(\".*\")|('.*')"), format);
    
    //Single-line comment
    format.setForeground(Qt::darkGreen);
     insertRule("singleLineComment",QRegExp("//[^\n]*"), format);
    
    //Multi-line comment
     format.setForeground(Qt::darkGreen);
//     setMultilineRule(QRegExp("(/*)"), QRegExp("(*/)"),  format);
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
    format.setForeground(Qt::gray);
    insertRule("hexadecimal",QRegExp("\\b0[xX][0-9a-fA-F]+([Uu]([Ll]|LL|ll)?|([Ll]|LL|ll)[Uu]?)?\\b"), format);
}
