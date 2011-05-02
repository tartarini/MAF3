/* mafTextEditorSyntaxHighlighter.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Apr 15 00:28:50 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Apr 15 13:12:48 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 21
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFTEXTEDITORSYNTAXHIGHLIGHTER_H
#define MAFTEXTEDITORSYNTAXHIGHLIGHTER_H

#include "mafScriptInterpreter_global.h"
#include <QSyntaxHighlighter>

namespace mafScriptInterpreter {

class mafTextEditor;
class mafTextEditorSyntaxHighlighterPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafTextEditorSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
        
public:
     mafTextEditorSyntaxHighlighter(mafTextEditor *editor = 0);
    ~mafTextEditorSyntaxHighlighter(void);

    struct mafTextEditorHighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };

protected:
    void setupKeywords            (QStringList patterns, QTextCharFormat format);
    void setupVariable            (QRegExp pattern, QTextCharFormat format);
    void setupClass               (QRegExp pattern, QTextCharFormat format);
    void setupFunction            (QRegExp pattern, QTextCharFormat format);
    void setupQuotation           (QRegExp pattern, QTextCharFormat format);
    void setupSingleLineComment   (QRegExp pattern, QTextCharFormat format);
    void setupMultiLineComment    (QRegExp startPattern, QRegExp endPattern, QTextCharFormat format);
    void setupDecimalNumber       (QRegExp pattern, QTextCharFormat format);
    void setupFloatingPointNumber (QRegExp pattern, QTextCharFormat format);
    void setupOctalNumber         (QRegExp pattern, QTextCharFormat format);
    void setupHexNumber           (QRegExp pattern, QTextCharFormat format);

protected:
    void highlightBlock(const QString &text);

private:
    mafTextEditorSyntaxHighlighterPrivate *d;
};
    
} // end namespace

#endif
