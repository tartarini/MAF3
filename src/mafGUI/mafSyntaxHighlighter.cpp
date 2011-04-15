/*
 *  mafSyntaxHighlighter.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSyntaxHighlighter.h"

using namespace mafGUI;

mafSyntaxHighlighter::mafSyntaxHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent) {
    m_Lock = new QReadWriteLock(QReadWriteLock::Recursive);
}

mafSyntaxHighlighter::~mafSyntaxHighlighter() {
    delete m_Lock;
}

void mafSyntaxHighlighter::highlightBlock(const QString &text) {
    m_Lock->lockForWrite();
    
    foreach (const mafHighlightingRule &rule, m_HighlightingRules) {
        QRegExp expression(rule.m_Pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.m_Format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    //this code is applied when there is a comment
    
    if(m_CommentStartExpression.isEmpty()) {
        m_Lock->unlock();
        return;
    }
    
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
        setFormat(startIndex, commentLength, m_CommentFormat);
        startIndex = m_CommentStartExpression.indexIn(text, startIndex + commentLength);
    }
    m_Lock->unlock();
}

void mafSyntaxHighlighter::insertRule(const QString &name, mafHighlightingRule rule) {
    QWriteLocker locker(m_Lock);
    m_HighlightingRules.insert(name, rule);
}

void mafSyntaxHighlighter::insertRule(const QString &name, QRegExp pattern, QTextCharFormat format) {
    m_Lock->lockForWrite();
    mafHighlightingRule rule;
    rule.m_Pattern = pattern;
    rule.m_Format = format;
    m_Lock->unlock();
    insertRule(name, rule);
}

void mafSyntaxHighlighter::setMultilineRule(QRegExp startPattern, QRegExp endPattern, QTextCharFormat format) {
    m_Lock->lockForWrite();
    m_CommentStartExpression = startPattern;
    m_CommentEndExpression = endPattern;
    m_CommentFormat = format;
    m_Lock->unlock();
}


void mafSyntaxHighlighter::removeRule(const QString& name) {
    QWriteLocker locker(m_Lock);
    m_HighlightingRules.remove(name);
}
