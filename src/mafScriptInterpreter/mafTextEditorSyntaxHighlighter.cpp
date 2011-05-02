/* mafTextEditorSyntaxHighlighter.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Apr 15 00:32:02 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Apr 15 13:13:11 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 49
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 15-Apr-2009    Julien Wintz  
 *    Last-Updated: Wed Apr 15 13:01:27 2009 (+0200) #47 (Julien Wintz)
 *    Initial revision. Method basically behave the same, later
 *    revisions will distinguish them.
 * 
 */

#include <mafTextEditor.h>
#include <mafTextEditorSyntaxHighlighter.h>

using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafTextEditorSyntaxHighlighterPrivate
{
public:
    QRegExp         multiLineCommentStartExpression;
    QRegExp         multiLineCommentEndExpression;
    QTextCharFormat multiLineCommentFormat;

    QVector<mafTextEditorSyntaxHighlighter::mafTextEditorHighlightingRule> rules;
};

mafTextEditorSyntaxHighlighter::mafTextEditorSyntaxHighlighter(mafTextEditor *editor) : QSyntaxHighlighter(editor->document())
{
    d = new mafTextEditorSyntaxHighlighterPrivate;
}

mafTextEditorSyntaxHighlighter::~mafTextEditorSyntaxHighlighter(void)
{
    delete d;
}

void mafTextEditorSyntaxHighlighter::setupKeywords(QStringList patterns, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    foreach (const QString &pattern, patterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = format;
        d->rules << rule;
    }
}

void mafTextEditorSyntaxHighlighter::setupVariable(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupClass(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupFunction(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupQuotation(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupSingleLineComment(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupMultiLineComment(QRegExp startPattern, QRegExp endPattern, QTextCharFormat format)
{
    d->multiLineCommentStartExpression = QRegExp(startPattern);
    d->multiLineCommentEndExpression = QRegExp(endPattern);
    d->multiLineCommentFormat = format;
}

void mafTextEditorSyntaxHighlighter::setupDecimalNumber(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupFloatingPointNumber(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupOctalNumber(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

void mafTextEditorSyntaxHighlighter::setupHexNumber(QRegExp pattern, QTextCharFormat format)
{
    mafTextEditorHighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format  = format;
    d->rules << rule;
}

// /////////////////////////////////////////////////////////////////
// Here goes highlighting
// /////////////////////////////////////////////////////////////////

void mafTextEditorSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const mafTextEditorHighlightingRule& rule, d->rules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = d->multiLineCommentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = d->multiLineCommentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + d->multiLineCommentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, d->multiLineCommentFormat);
        startIndex = d->multiLineCommentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
