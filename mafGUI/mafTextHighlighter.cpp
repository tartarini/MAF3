/*
 *  mafTextHighlighter.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTextHighlighter.h"

using namespace mafGUI;

mafTextHighlighter::mafTextHighlighter(QTextDocument *parent):mafSyntaxHighlighter(parent) {
    initialize();
}

void mafTextHighlighter::initialize() {
    //initialize several Formats
    mafTextCharFormat format;
    format.setForeground(Qt::darkGreen);
    format.setFontWeight(QFont::Bold);
    m_Formats.insert("keywords", format);

    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkMagenta);
    m_Formats.insert("class", format);

    format.setForeground(Qt::red);
    m_Formats.insert("singleLineComment", format);

    format.setForeground(Qt::red);
    m_Formats.insert("multiLineComment", format);

    format.setForeground(Qt::darkGreen);
    m_Formats.insert("quotation", format);

    format.setFontItalic(true);
    format.setForeground(Qt::blue);
    m_Formats.insert("function", format);


    /*mafHighlightingRule rule;
    mafStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b";
    int count = 0;
    foreach (const QString &pattern, keywordPatterns) {
        rule.m_Pattern = mafRegExp(pattern);
        rule.m_Format = m_Formats["keywords"];
        m_HighlightingRules.insert(mafString::number(count++),rule);
    }


    rule.m_Pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.m_Format = m_ClassFormat;
    m_HighlightingRules.insert("class",rule);


    rule.m_Pattern = QRegExp("//[^\n]*");
    rule.m_Format = m_SingleLineCommentFormat;
    m_HighlightingRules.insert("silgeComment", rule);

    rule.m_Pattern = mafRegExp("\".*\"");
    rule.m_Format = m_QuotationFormat;
    m_HighlightingRules.insert("quotation",rule);

    rule.m_Pattern = mafRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.m_Format = m_FunctionFormat;
    m_HighlightingRules.insert("function",rule);*/

    m_CommentStartExpression = mafRegExp("/\\*");
    m_CommentEndExpression = mafRegExp("\\*/");
}

void mafTextHighlighter::highlightBlock(const mafString &text) {
    foreach (const mafHighlightingRule &rule, m_HighlightingRules) {
        mafRegExp expression(rule.m_Pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.m_Format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    //this code is applied when there is a comment
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = m_CommentStartExpression.indexIn(text);

    while (startIndex >= 0) {

        int endIndex = m_CommentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + m_CommentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, m_Formats["multiLineComment"]);
        startIndex = m_CommentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

void mafTextHighlighter::insertRule(const mafString &name, mafHighlightingRule rule) {
    m_HighlightingRules.insert(name, rule);
}

void mafTextHighlighter::insertRule(const mafString &name, mafRegExp pattern, mafTextCharFormat format) {
    mafHighlightingRule rule;
    rule.m_Pattern = pattern;
    rule.m_Format = format;
    insertRule(name, rule);
}

void mafTextHighlighter::removeRule(const mafString& name) {
    m_HighlightingRules.remove(name);
}

void mafTextHighlighter::insertFormat(const mafString &name, mafTextCharFormat format) {
    m_Formats.insert(name, format);
}

void mafTextHighlighter::removeFormat(const mafString& name) {
    m_Formats.remove(name);
}

const mafTextCharFormat &mafTextHighlighter::format(const mafString &name) {
    return m_Formats[name.toAscii()];
}
