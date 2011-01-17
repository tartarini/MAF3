#ifndef MAFTEXTHIGHLIGHTER_H
#define MAFTEXTHIGHLIGHTER_H

#include "mafGUIDefinitions.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class mafTextHighlighter : public QSyntaxHighlighter
{
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

#endif // MAFTEXTHIGHLIGHTER_H
