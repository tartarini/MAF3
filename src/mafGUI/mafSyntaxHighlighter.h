/*
 *  mafSyntaxHighlighter.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFSYNTAXHIGHLIGHTER_H
#define MAFSYNTAXHIGHLIGHTER_H

#include "mafGUIDefinitions.h"

namespace mafGUI {

// forward declaration
class mafTextDocument;

/**
 Class Name: mafSyntaxHighlighter
 Highlight the text inside a text editor or browser class.
 The format can be for:
   - keyword
   - class
   - singleLineComment
   - multiLineComment
   - quotation
   - function
   - integer
   - float
   - octal
   - hexadecimal
*/
class MAFGUISHARED_EXPORT mafSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    /// Object constructor.
    mafSyntaxHighlighter(QTextDocument *parent = 0);
    
    /// Object destructor.
    ~mafSyntaxHighlighter();

    /// Insert new rule
    void insertRule(const QString &name, mafHighlightingRule rule);

    /// Insert new rule from a pattern and a format
    void insertRule(const QString &name, QRegExp pattern, QTextCharFormat format);
    
    /// Insert a unique rule from a pattern, and endPattern and a format
    void setMultilineRule(QRegExp startPattern, QRegExp endPattern, QTextCharFormat format);

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

#endif // MAFSYNTAXHIGHLIGHTER_H
