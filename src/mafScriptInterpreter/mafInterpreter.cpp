/* mafInterpreter.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 15:31:39 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 21:20:30 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 400
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <iostream>

#include <QtDebug>
#include <QtCore>
#include <QtGui>

#include "mafLog.h"

#include "mafScriptEditor.h"
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
#include "mafScriptEditorPython.h"
#endif

#include <mafTextEditorSyntaxHighlighterPython.h>
#include <mafInterpreter.h>
#include <mafInterpreterPreferencesWidget.h>

using namespace mafScriptInterpreter;

mafInterpreter::mafInterpreter(QWidget *parent) : mafTextEditor(parent)
{
    m_Interpreter = NULL;
    preferences = NULL;

    history_index = 0;
    history_dirty = false;

    this->setShowLineNumbers(false);
    this->setShowCurrentLine(false);
    this->setShowRevisions(false);
    
    promptFlag = false;

}

mafInterpreter::~mafInterpreter(void)
{
}

mafScriptEditor *mafInterpreter::interpreter(void)
{
    return m_Interpreter;
}

void mafInterpreter::keyPressEvent(QKeyEvent *event)
{
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
        }
        
        else if(cursor.columnNumber() > filter(m_Interpreter->prompt()).size())
            mafTextEditor::keyPressEvent(event);
        
        this->onKeyLeftPressed();

    } else if(event->key() == Qt::Key_Right) {

        if(cursor.columnNumber() < currentLine().size())
            mafTextEditor::keyPressEvent(event);

        this->onKeyRightPressed();

    } else
        mafTextEditor::keyPressEvent(event);
}

void mafInterpreter::mousePressEvent(QMouseEvent *event)
{
    cursor = this->textCursor();

    mafTextEditor::mousePressEvent(event);
}

void mafInterpreter::mouseReleaseEvent(QMouseEvent *event)
{
    mafTextEditor::mouseReleaseEvent(event);

    if(cursor.blockNumber() +1 != currentLineNumber() && cursor.columnNumber() <= filter(m_Interpreter->prompt()).size())
        this->setTextCursor(cursor);
}

void mafInterpreter::readSettings(void)
{
    QSettings settings;
    settings.beginGroup("interpreter");
    this->setFont(settings.value("font").value<QFont>());    
    this->setBackgroundColor(settings.value("backgroundcolor", Qt::darkGray).value<QColor>());
    this->setForegroundColor(settings.value("foregroundcolor", Qt::white).value<QColor>());
    settings.endGroup();
}

void mafInterpreter::writeSettings(void)
{
    QSettings settings;
    settings.beginGroup("interpreter");
    settings.setValue("font", this->font());    
    settings.setValue("backgroundcolor", this->backgroundColor());
    settings.setValue("foregroundcolor", this->foregroundColor());
    settings.endGroup();
}

void mafInterpreter::registerAsHandler(mafLog::Handler handler)
{
    mafLog::registerHandler(handler);
}

void mafInterpreter::registerInterpreter(mafScriptEditor *interpreter)
{
    m_Interpreter = interpreter;
    m_Interpreter->setVerbose(false);

    connect(m_Interpreter, SIGNAL(interpreted(const QString&, int *)), this, SLOT(output(const QString&, int *)));
    connect(this, SIGNAL(input(const QString&, int *)), m_Interpreter,    SLOT(interpret(const QString&, int *)));
    connect(this, SIGNAL( load(const QString&)),        m_Interpreter,    SLOT(     load(const QString&)));
    connect(this, SIGNAL( save(const QString&)),        m_Interpreter,    SLOT(     save(const QString&)));
    connect(this, SIGNAL(stopped(void)),                m_Interpreter,  SIGNAL(  stopped(void)));

    mafTextEditorSyntaxHighlighter *highlighter = NULL;

#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
    if(dynamic_cast<mafScriptEditorPython *>(m_Interpreter))
        highlighter = new mafTextEditorSyntaxHighlighterPython(this);
#endif

    Q_UNUSED(highlighter);

    this->appendPlainText(filter(m_Interpreter->prompt()));
}

mafInterpreterPreferencesWidget *mafInterpreter::preferencesWidget(QWidget *parent)
{
    if(!preferences)
        preferences = new mafInterpreterPreferencesWidget(this, parent);

    return preferences;
}

void mafInterpreter::onKeyUpPressed(void)
{
    if(history.size() == 0)
        return;

    if(history_index && history_index == (unsigned int)history.size()-1)
        return;

    if(history_index == 0 && !history_dirty) {
        QString line = currentLine();
        if(m_Interpreter)
            line.remove(filter(m_Interpreter->prompt()));
        
        history.push_front(line);
        history_dirty = true;
    }

    history_index++;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, filter(m_Interpreter->prompt()).size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(history.at(history_index));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);
}

void mafInterpreter::onKeyDownPressed(void)
{
    if(history_index == 0)
        return;

    history_index--;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, filter(m_Interpreter->prompt()).size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(history.at(history_index));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);

    if(history_index == 0 && history_dirty) {
        history.pop_front();
        history_dirty = false;
    }
}

void mafInterpreter::onKeyLeftPressed(void)
{
    // check cursor
}

void mafInterpreter::onKeyRightPressed(void)
{
    // check cursor
}

void mafInterpreter::onKeyEnterPressed(void)
{
    int stat;

    QString line = currentLine();

    if(m_Interpreter)
        line.remove(filter(m_Interpreter->prompt()));

    if (line == "bye" || line == "exit" || line == "quit")
        emit stopped();
    
    if(!line.isEmpty()) {
        if(history_index > 0 && history_dirty)
            history.removeFirst();

        history.push_front(line);
        history_index = 0;
        history_dirty = false;
    }
    
    if (line.startsWith(":load ")) {
        emit load(line.remove(":load "));
        
    } else if (line.startsWith(":save ")) {
        emit save(line.remove(":save "));
        
    } else if (line.startsWith(":emacs")) {
        std::cerr << "emacs bindings not supported in gui mode" << std::endl;
        
        emit input("", &stat);
        
    } else if (line.startsWith(":vi")) {
        std::cerr << "vi bindings not supported in gui mode" << std::endl;
        
        emit input("", &stat);
        
    } else if (line.startsWith(":help")) {
        std::cerr << "File manipulation:";
        std::cerr << " :load [file]        loads file and interprets its content" << std::endl;
        std::cerr << " :save [file]        saves interpreter history to file" << std::endl;
        std::cerr << "" << std::endl;
        
        emit input("", &stat);
        
    } else if (line.startsWith(":man ")) {
        
        emit input("", &stat);

    } else if(line.isEmpty()) {

        this->appendPlainText(filter(m_Interpreter->prompt()));

    } else {

        emit input(line, &stat);

    }
    
    promptFlag = true;
}

void mafInterpreter::onKeyBackspacePressed(void)
{

}

void mafInterpreter::output(const QString& result,  int *stat)
{
    Q_UNUSED(stat);

    QString text(result);

    if(!text.simplified().isEmpty())
        this->appendPlainText(filter(text));

    //this->appendPlainText(filter(m_Interpreter->prompt()));
    
    
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
}

QString mafInterpreter::filter(QString text)
{
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

bool mafInterpreter::eventFilter(QObject *object, QEvent *event)
{
    mafLogEvent *log = dynamic_cast<mafLogEvent *>(event);
    mafInterpreter *interpreter = dynamic_cast<mafInterpreter *>(object);

    if (log && interpreter) {
        int stat; interpreter->output(log->message(), &stat);
        return true;
    } else {
        if(promptFlag) {
            this->appendPlainText(filter(m_Interpreter->prompt()));
            promptFlag = false;
        }
        return QObject::eventFilter(object, event);
    }
}
