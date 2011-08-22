/* tstMainWindow.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:19:37 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Apr 14 10:31:21 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 6
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef TSTMAINWINDOW_H
#define TSTMAINWINDOW_H

#include <QMainWindow>
#include <mafInterpreterEditor.h>

class tstMainWindowPrivate;

class tstMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     tstMainWindow(QWidget *parent = 0);
    ~tstMainWindow(void);

    mafScriptInterpreter::mafInterpreterEditor *interpreter(void);
    
    /// syncronize all the splitters
    void synchronizeSplitters(QObject * sender);

public slots:
    void showPreferences(void);

protected:
    void closeEvent(QCloseEvent *event);

private:
    tstMainWindowPrivate *d;
};

#endif // TSTMAINWINDOW_H
