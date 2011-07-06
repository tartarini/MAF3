/*
 *  mafTextEditorWidget.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 14/01/11.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTextEditWidget.h"
#include "mafTextHighlighter.h"
#include <QHBoxLayout>

using namespace mafGUI;

mafTextEditWidget::mafTextEditWidget(QWidget *parent): QWidget(parent), m_TextEditor(NULL), m_Highlighter(NULL) {
    initialize();
}

mafTextEditWidget::mafTextEditWidget(QSyntaxHighlighter *highlighter, QWidget *parent): QWidget(parent), m_TextEditor(NULL), m_Highlighter(highlighter) {
   initialize();
}

void mafTextEditWidget::initialize() {
    QVBoxLayout *hboxLayout = new QVBoxLayout( this );
    hboxLayout->setMargin(0);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    m_TextEditor = new QTextEdit(this);
    m_TextEditor->setFont(font);
    
    bool res = connect(this, SIGNAL(updateText(const QString)), m_TextEditor, SLOT(append(const QString)));
    
    
    if(m_Highlighter) {
        m_Highlighter->setDocument(m_TextEditor->document());
    }

    //sample test
    //m_TextEditor->append("vme Path New! Don't hate us, matrix users. It turns out that the driver reported in millimeters, rather than the VRPN stated standard of meters. One of the users noticed this, so I've changed version 2.27 to be compliant to the standard. The good news is that it now matches all of the other trackers. The bad news is that you'll need to change your code if you relied on it being in mm.New! Version 7.27 switched to the new InterSense libraries. Previous versions used the old ones. The new code that was submitted is not backwards compatible with the old libraries. So you can (and must) upgrade your Intersense libraries for the new version");
    hboxLayout->addWidget(m_TextEditor);
}

mafTextEditWidget::~mafTextEditWidget() {
}

void mafTextEditWidget::clear() {
    m_TextEditor->clear();
}

void mafTextEditWidget::find(QString text){
     mafTextHighlighter *textHighlighter = qobject_cast<mafTextHighlighter *>(m_Highlighter);

    if (!text.isEmpty()) {
        QTextCharFormat format;
        format.setBackground(Qt::yellow);
        format.setFontWeight(QFont::Bold);
        QString pattern = "\\b" + text + "\\b";
        QRegExp patternDebug(pattern, Qt::CaseInsensitive);

        textHighlighter->insertFormat("Find", format);
        textHighlighter->insertRule("Find", patternDebug, textHighlighter->format("Find"));
        
        textHighlighter->rehighlight();
    } else {
        textHighlighter->removeRule("Find");
        textHighlighter->removeFormat("Find");
    }
}

void mafTextEditWidget::append(const QString text) {
    emit updateText(text);
}

void mafTextEditWidget::setHighlighter(QSyntaxHighlighter *highlighter) {
    REQUIRE(highlighter);

    m_Highlighter = highlighter;
    m_Highlighter->setDocument(m_TextEditor->document());
}

void mafTextEditWidget::enableEditing(bool enable) {
    m_TextEditor->setReadOnly(!enable);
}

void mafTextEditWidget::loadDictionaryFromCSV(const QString &csvFile) {
    ///read from a csv file
}
