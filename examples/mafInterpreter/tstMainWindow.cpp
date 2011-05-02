/* tstMainWindow.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:19:56 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Aug  5 11:16:46 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 21
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <QtGui>

#include "tstMainWindow.h"

#include <mafInterpreter.h>
#include <mafInterpreterPreferencesWidget.h>

class mafSplitter : public QSplitter
{
    
public:
    mafSplitter(QWidget * parent);
    void adjustPosition(int p);
    
protected:
    void paintEvent ( QPaintEvent * pe );
};

mafSplitter::mafSplitter(QWidget * parent) : QSplitter( parent )
{
    setOpaqueResize(true);
}

void mafSplitter::paintEvent (QPaintEvent *)
{
    ((tstMainWindow*)parent()->parent())->synchronizeSplitters(this);
}

void mafSplitter::adjustPosition(int p)
{
    moveSplitter(p,1);
    refresh();
}

using namespace mafScriptInterpreter;

class tstMainWindowPrivate
{
public:
    QMenu *fileMenu;

    QAction *preferencesAction;

    mafInterpreter *interpreter;
    mafSplitter *m_HorizontalSplitterTop; ///< horizontal splitter in top section 
    mafSplitter *m_HorizontalSplitterBottom; ///< horizontal splitter in bottom section
    QTextEdit *m_LeftTopPanel; ///< left/top panel
    QTextEdit *m_RightTopPanel; ///< right/top panel
    QTextEdit *m_LeftBottomPanel; ///< left/bottom panel
    QTextEdit *m_RightBottomPanel; ///< right/bottom panel

public:
    tstMainWindow *q;
};

tstMainWindow::tstMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new tstMainWindowPrivate;

    QSplitter * mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Vertical);
    mainSplitter->setOpaqueResize(true);
    
    d->m_HorizontalSplitterTop = new mafSplitter(mainSplitter);
    //d->m_LeftTopPanel = new QTextEdit(d->m_HorizontalSplitterTop);
    //d->m_LeftTopPanel->setText("here... <b>command console</b> (progress for python console and scripts)");
    /// intepreter creation
    d->interpreter = new mafInterpreter(d->m_HorizontalSplitterTop);
    d->interpreter->readSettings();
    
    d->m_RightTopPanel = new QTextEdit(d->m_HorizontalSplitterTop);
    d->m_RightTopPanel->setText("here... <b>workflow execution</b> (progress on highlighted and autocomplete text editor, or other solution...");
    
    d->m_HorizontalSplitterBottom = new mafSplitter(mainSplitter);
    d->m_LeftBottomPanel = new QTextEdit(d->m_HorizontalSplitterBottom);
    d->m_LeftBottomPanel->setText("here... <b>status monitor</b> (which services are available");
    d->m_RightBottomPanel = new QTextEdit(d->m_HorizontalSplitterBottom);
    d->m_RightBottomPanel->setText("here... <b>log monitor</b> (need to create session files or record in db which can be requested");
    setCentralWidget(mainSplitter);

    d->preferencesAction = new QAction("Preferences", this);
    d->preferencesAction->setShortcut(Qt::ControlModifier + Qt::Key_Comma);
    connect(d->preferencesAction, SIGNAL(triggered()), this, SLOT(showPreferences()));

    d->fileMenu = this->menuBar()->addMenu("File");
    d->fileMenu->addAction(d->preferencesAction);

    d->q = this;

    this->setWindowTitle("Hypermodel Monitor");

}

tstMainWindow::~tstMainWindow(void)
{
    delete d->interpreter;
    delete d;
}

mafInterpreter *tstMainWindow::interpreter(void)
{
    return d->interpreter;
}

void tstMainWindow::showPreferences(void)
{
    mafInterpreterPreferencesWidget *widget = d->interpreter->preferencesWidget(this);
    widget->setWindowFlags(Qt::Sheet);
    widget->show();
}

void tstMainWindow::closeEvent(QCloseEvent *event)
{
    d->interpreter->writeSettings();
    event->accept();
}

void tstMainWindow::synchronizeSplitters(QObject * sender)
{
    if(d->m_HorizontalSplitterTop->sizes()==d->m_HorizontalSplitterBottom->sizes() || !d->m_HorizontalSplitterTop->isVisible() || !d->m_HorizontalSplitterBottom->isVisible())
        return;
    
    mafSplitter* s = (mafSplitter*)sender;
    if (s==d->m_HorizontalSplitterTop) {
        d->m_HorizontalSplitterBottom->adjustPosition(d->m_HorizontalSplitterTop->sizes()[0]);
    }
    else if(s==d->m_HorizontalSplitterBottom) {
        d->m_HorizontalSplitterTop->adjustPosition(d->m_HorizontalSplitterBottom->sizes()[0]);
    };
    
}
