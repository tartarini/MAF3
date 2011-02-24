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
class MAFGUISHARED_EXPORT mafTextHighlighter : public mafSyntaxHighlighter {
    Q_OBJECT
public:
    /// Object constructor.
    mafTextHighlighter(QTextDocument *parent = 0);

    /// Insert new rule
    void insertRule(const mafString &name, mafHighlightingRule rule);

    /// Insert new rule from a pattern and a format
    void insertRule(const mafString &name, mafRegExp pattern, mafTextCharFormat format);

    /// Remove rule
    void removeRule(const mafString &name);

    /// Insert new rule
    void insertFormat(const mafString &name, mafTextCharFormat format);
    /// Remove rule
    void removeFormat(const mafString &name);

    /// retrieve format from name
    const mafTextCharFormat &format(const mafString &name);

protected:
    /// Set the text which will be highlighted
    /*virtual*/ void highlightBlock(const mafString &text);

private:
    /// initialize several patterns
    void initialize();

    mafMap<mafString, mafHighlightingRule> m_HighlightingRules; ///< container with the entire list of rules
    mafMap<mafString, mafTextCharFormat> m_Formats; ///< container with the entire list of rules

    //sample for multiline comment
    mafRegExp m_CommentStartExpression; ///< variable which set the pattern for starting a comment
    mafRegExp m_CommentEndExpression; ///< variable which set the pattern for ending a comment
};

} // end namespace

#endif // MAFTEXTHIGHLIGHTER_H
