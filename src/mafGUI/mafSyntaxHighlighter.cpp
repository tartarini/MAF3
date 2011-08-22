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
    initialize();
}

mafSyntaxHighlighter::~mafSyntaxHighlighter() {
    delete m_Lock;
}

void mafSyntaxHighlighter::initialize() {
    //initialize several Formats
    QTextCharFormat format;
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

    m_CommentStartExpression = QRegExp("/\\*");
    m_CommentEndExpression = QRegExp("\\*/");
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
        setFormat(startIndex, commentLength, m_Formats["multiLineComment"]);
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
    insertFormat("multiLineComment", format);
    m_Lock->unlock();
}

void mafSyntaxHighlighter::removeRule(const QString& name) {
    QWriteLocker locker(m_Lock);
    m_HighlightingRules.remove(name);
}

void mafSyntaxHighlighter::insertFormat(const QString &name, QTextCharFormat format) {
    QWriteLocker locker(m_Lock);
    m_Formats.insert(name, format);
}

void mafSyntaxHighlighter::removeFormat(const QString& name) {
    QWriteLocker locker(m_Lock);
    m_Formats.remove(name);
}

const QTextCharFormat &mafSyntaxHighlighter::format(const QString &name) {
    QReadLocker locker(m_Lock);
    return m_Formats[name.toAscii()];
}
