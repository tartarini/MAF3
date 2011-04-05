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

namespace mafGUI {

// forward declaration
class mafTextDocument;

/**
 Class Name: mafTextHighlighter
 Highlight the text inside a text editor or browser class.
 The format can be:
   - KeywordFormat
   - ClassFormat
   - SingleLineCommentFormat
   - MultiLineCommentFormat
   - QuotationFormat
   - FunctionFormat
*/
class MAFGUISHARED_EXPORT mafTextHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    /// Object constructor.
    mafTextHighlighter(QTextDocument *parent = 0);
    
    /// Object destructor.
    ~mafTextHighlighter();

    /// Insert new rule
    void insertRule(const QString &name, mafHighlightingRule rule);

    /// Insert new rule from a pattern and a format
    void insertRule(const QString &name, QRegExp pattern, QTextCharFormat format);

    /// Remove rule
    void removeRule(const QString &name);

    /// Insert new rule
    void insertFormat(const QString &name, QTextCharFormat format);
    /// Remove rule
    void removeFormat(const QString &name);

    /// retrieve format from name
    const QTextCharFormat &format(const QString &name);

protected:
    /// Set the text which will be highlighted
    /*virtual*/ void highlightBlock(const QString &text);

private:
    /// initialize several patterns
    void initialize();

    mutable QReadWriteLock *m_Lock; ///< Lock variable for thread safe access to object.

    QMap<QString, mafHighlightingRule> m_HighlightingRules; ///< container with the entire list of rules
    QMap<QString, QTextCharFormat> m_Formats; ///< container with the entire list of rules

    //sample for multiline comment
    QRegExp m_CommentStartExpression; ///< variable which set the pattern for starting a comment
    QRegExp m_CommentEndExpression; ///< variable which set the pattern for ending a comment
};

} // end namespace

#endif // MAFTEXTHIGHLIGHTER_H
