/*
 *  mafInterpreterConsole.cpp
 *  mafGui
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 10/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEditorPython.h"

#include <mafSyntaxHighlighterPython.h>
#include <mafInterpreterConsole.h>


using namespace mafScriptInterpreter;
using namespace mafGUI;

mafInterpreterConsole::mafInterpreterConsole(QWidget *parent) : mafTextEditor(parent) {
    m_Interpreter = NULL;
    m_HistoryIndex = 0;
    m_HistoryDirty = false;

    this->setShowLineNumbers(false);
    this->setShowCurrentLine(false);
    this->setShowRevisions(false);
    
}

mafInterpreterConsole::~mafInterpreterConsole(void) {
}

mafScriptEditor *mafInterpreterConsole::interpreter(void) {
    return m_Interpreter;
}

void mafInterpreterConsole::keyPressEvent(QKeyEvent *event) {
    QTextCursor cursor = textCursor();

    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        this->onKeyEnterPressed();
    } else if(event->key() == Qt::Key_Backspace) {
        if(cursor.columnNumber() > filter(m_Interpreter->prompt()).size())
            mafTextEditor::keyPressEvent(event);
        
        this->onKeyBackspacePressed();
    } else if(event->key() == Qt::Key_Up) {
        this->onKeyUpPressed();
    } else if(event->key() == Qt::Key_Down) {
        this->onKeyDownPressed();
    } else if(event->key() == Qt::Key_Left) {
        if(event->modifiers() & Qt::ControlModifier) {
            cursor.movePosition(QTextCursor::StartOfLine);
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, filter(m_Interpreter->prompt()).size());

            this->setTextCursor(cursor);
        } else if(cursor.columnNumber() > filter(m_Interpreter->prompt()).size())
            mafTextEditor::keyPressEvent(event);
        this->onKeyLeftPressed();
    } else if(event->key() == Qt::Key_Right) {
        if(cursor.columnNumber() < currentLine().size()) {
            mafTextEditor::keyPressEvent(event);
        }
        this->onKeyRightPressed();
    } else {
        mafTextEditor::keyPressEvent(event);
    }
}

void mafInterpreterConsole::mousePressEvent(QMouseEvent *event) {
    m_Cursor = this->textCursor();
    mafTextEditor::mousePressEvent(event);
}

void mafInterpreterConsole::mouseReleaseEvent(QMouseEvent *event) {
    mafTextEditor::mouseReleaseEvent(event);

    if(m_Cursor.blockNumber() +1 != currentLineNumber() && m_Cursor.columnNumber() <= filter(m_Interpreter->prompt()).size())
        this->setTextCursor(m_Cursor);
}

void mafInterpreterConsole::readSettings(void) {
    QSettings settings;
    settings.beginGroup("interpreter");
    this->setFont(settings.value("font").value<QFont>());    
    this->setBackgroundColor(settings.value("backgroundcolor", Qt::white).value<QColor>());
    this->setForegroundColor(settings.value("foregroundcolor", Qt::black).value<QColor>());
    settings.endGroup();
}

void mafInterpreterConsole::writeSettings(void) {
    QSettings settings;
    settings.beginGroup("interpreter");
    settings.setValue("font", this->font());    
    settings.setValue("backgroundcolor", this->backgroundColor());
    settings.setValue("foregroundcolor", this->foregroundColor());
    settings.endGroup();
}

void mafInterpreterConsole::registerInterpreter(mafScriptEditor *interpreter) {
    m_Interpreter = interpreter;
    m_Interpreter->setVerbose(false);

    connect(this, SIGNAL(input(const QString&, int *)), m_Interpreter,    SLOT(interpret(const QString&, int *)));
    connect(this, SIGNAL( load(const QString&)),        m_Interpreter,    SLOT(     load(const QString&)));
    connect(this, SIGNAL( save(const QString&)),        m_Interpreter,    SLOT(     save(const QString&)));
    connect(this, SIGNAL(stopped(void)),                m_Interpreter,  SIGNAL(  stopped(void)));

    mafSyntaxHighlighter *highlighter = NULL;

    if(dynamic_cast<mafScriptEditorPython *>(m_Interpreter)) {
        highlighter = new mafSyntaxHighlighterPython(this->document());
    }

    Q_UNUSED(highlighter);

    this->appendPlainText(filter(m_Interpreter->prompt()));
}

void mafInterpreterConsole::onKeyUpPressed(void) {
    if(m_History.size() == 0) {
        return;
    }

    if(m_HistoryIndex && m_HistoryIndex == (unsigned int)m_History.size()-1) {
        return;
    }

    if(m_HistoryIndex == 0 && !m_HistoryDirty) {
        QString line = currentLine();
        if(m_Interpreter)
            line.remove(filter(m_Interpreter->prompt()));
        
        m_History.push_front(line);
        m_HistoryDirty = true;
    }

    m_HistoryIndex++;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, filter(m_Interpreter->prompt()).size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(m_History.at(m_HistoryIndex));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);
}

void mafInterpreterConsole::onKeyDownPressed(void) {
    if(m_HistoryIndex == 0) {
        return;
    }

    m_HistoryIndex--;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, filter(m_Interpreter->prompt()).size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(m_History.at(m_HistoryIndex));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);

    if(m_HistoryIndex == 0 && m_HistoryDirty) {
        m_History.pop_front();
        m_HistoryDirty = false;
    }
}

void mafInterpreterConsole::onKeyLeftPressed(void) {
    // check cursor
}

void mafInterpreterConsole::onKeyRightPressed(void) {
    // check cursor
}

void mafInterpreterConsole::onKeyEnterPressed(void) {
    int stat;

    QString line = currentLine();

    if(m_Interpreter) {
        line.remove(filter(m_Interpreter->prompt()));
    }

    if (line == "bye" || line == "exit" || line == "quit") {
        emit stopped();
    }
    
    if(!line.isEmpty()) {
        if(m_HistoryIndex > 0 && m_HistoryDirty)
            m_History.removeFirst();

        m_History.push_front(line);
        m_HistoryIndex = 0;
        m_HistoryDirty = false;
    }
    
    if (line.startsWith(":load ")) {
        emit load(line.remove(":load "));
        
    } else if (line.startsWith(":save ")) {
        emit save(line.remove(":save "));
    } else if (line.startsWith(":emacs")) {
        qWarning() << "emacs bindings not supported in gui mode";
        emit input("", &stat);
    } else if (line.startsWith(":vi")) {
        qWarning() << "vi bindings not supported in gui mode";
        emit input("", &stat);
    } else if (line.startsWith(":help")) {
        qWarning() << "File manipulation:";
        qWarning() << " :load [file]        loads file and interprets its content";
        qWarning() << " :save [file]        saves interpreter history to file";
        emit input("", &stat);
    } else if (line.startsWith(":man ")) {
        emit input("", &stat);
    } else if(line.isEmpty()) {
        this->appendPlainText(filter(m_Interpreter->prompt()));
    } else {
        QString result = emit input(line, &stat);
        if(!result.isEmpty()) {
            output(result, &stat);
        }
    }
}

void mafInterpreterConsole::onKeyBackspacePressed(void) {

}

void mafInterpreterConsole::output(const QString& result,  int *stat) {
    Q_UNUSED(stat);

    QString text(result);

    if(!text.simplified().isEmpty())
        this->appendPlainText(filter(text));

    this->appendPlainText(filter(m_Interpreter->prompt()));
    
    
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
}

QString mafInterpreterConsole::filter(QString text) {
    return text
        .remove(MAF_COLOR_FG_BLACK)
        .remove(MAF_COLOR_FG_RED)
        .remove(MAF_COLOR_FG_GREEN)
        .remove(MAF_COLOR_FG_YELLOW)
        .remove(MAF_COLOR_FG_BLUE)
        .remove(MAF_COLOR_FG_MAGENTA)
        .remove(MAF_COLOR_FG_CYAN)
        .remove(MAF_COLOR_FG_WHITE)
        
        .remove(MAF_COLOR_FG_LTBLACK)
        .remove(MAF_COLOR_FG_LTRED)
        .remove(MAF_COLOR_FG_LTGREEN)
        .remove(MAF_COLOR_FG_LTYELLOW)
        .remove(MAF_COLOR_FG_LTBLUE)
        .remove(MAF_COLOR_FG_LTMAGENTA)
        .remove(MAF_COLOR_FG_LTCYAN)
        .remove(MAF_COLOR_FG_LTWHITE)
        
        .remove(MAF_COLOR_BG_BLACK)
        .remove(MAF_COLOR_BG_RED)
        .remove(MAF_COLOR_BG_GREEN)
        .remove(MAF_COLOR_BG_YELLOW)
        .remove(MAF_COLOR_BG_BLUE)
        .remove(MAF_COLOR_BG_MAGENTA)
        .remove(MAF_COLOR_BG_CYAN)
        .remove(MAF_COLOR_BG_WHITE)

        .remove(MAF_COLOR_FG_BD)
        .remove(MAF_COLOR_FG_UL)
        .remove(MAF_NO_COLOR);
}
