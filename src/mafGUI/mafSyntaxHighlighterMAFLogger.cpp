#include "mafSyntaxHighlighterMAFLogger.h"

using namespace mafGUI;

mafSyntaxHighlighterMAFLogger::mafSyntaxHighlighterMAFLogger(QTextDocument *parent) : mafSyntaxHighlighter(parent) {
        
    QTextCharFormat format;
    format.setForeground(Qt::darkGreen);
    format.setFontWeight(QFont::Bold);
    QRegExp patternDebug("\\bDebug\\b");
    insertRule("debug", patternDebug, format);

    format.setForeground(Qt::darkYellow);
    format.setFontWeight(QFont::Bold);
    QRegExp patternWarning("\\bWarning\\b");
    insertRule("warning", patternWarning, format);

    format.setForeground(Qt::red);
    format.setFontWeight(QFont::Bold);
    QRegExp patternFatal("\\bFatal\\b");
    insertRule("fatal", patternFatal, format);

    format.setForeground(Qt::red);
    format.setFontWeight(QFont::Bold);
    QRegExp patternCritical("\\bCritical\\b");
    insertRule("critical", patternCritical, format);
}
