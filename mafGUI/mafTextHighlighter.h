/*
 *  mafTextHighlighter.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFTEXTHIGHLIGHTER_H
#define MAFTEXTHIGHLIGHTER_H

#include "mafGUIDefinitions.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

namespace mafGUI {

/**
 Class Name: mafTextHighlighter
 Highlight the text inside a text editor or browser class.

*/
class MAFGUISHARED_EXPORT mafTextHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    mafTextHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

} // end namespace

#endif // MAFTEXTHIGHLIGHTER_H
